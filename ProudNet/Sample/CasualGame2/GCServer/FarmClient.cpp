#include "StdAfx.h"
#include "FarmClient.h"
#include "../GCCommon/Farm_common.cpp"
#include "../GCCommon/Farm_Proxy.cpp"
#include "../GCCommon/Farm_Stub.cpp"

#include "GCServerDlg.h"

int64_t BroadcastFarmClientInfoInterval = 1000;

CFarmClient::CFarmClient(void)
{
	m_authed = false;
	m_connectStartedTime = 0;
	m_broadcastFarmClientInfoCoolTime = 0;
	m_StatusServerHostID=HostID_None;
	m_p2pGroupID = HostID_None;
}

CFarmClient::~CFarmClient(void)
{
	m_lanClient.Free();
}

void CFarmClient::Init( CFarmClientConnectParameter& param, CServerBase* pOwner, String &ServerName)
{
	m_owner = pOwner;
	m_ownerServerName = ServerName;
	m_dg = param.m_delegate;
	m_farmName = param.m_farmName;
	m_farmClientName = param.m_farmClientName;
	m_farmClientType = param.m_farmClientType;

	CDbLogParameter dbparam;
	dbparam.m_dbmsConnectionString = (LPCWSTR)CFarmSettings::GetSharedPtr()->m_logdbConnectionString;
	dbparam.m_loggerName = m_farmClientName;
	m_logWriteDelegate.Attach(new CTestLogWriterDelegate());
	m_DbLogWriter.Attach(CDbLogWriter::New(dbparam, m_logWriteDelegate));

	m_authed = false;
	m_connectStartedTime = 0;

	m_lanClient.Attach(CNetClient::Create());
	m_lanClient->SetEventSink(this);
	m_lanClient->AttachProxy(&m_c2sProxy);
	m_lanClient->AttachStub((FarmS2C::Stub*)this);
	m_lanClient->AttachProxy(&m_c2cProxy);
	m_lanClient->AttachStub((FarmC2C::Stub*)this);
}

/// Starts connecting attempt to farm server.
/// Farm 서버에 연결 시도를 시작합니다.
void CFarmClient::Connect()
{
	CNetConnectionParam p1;
	p1.m_protocolVersion = CFarmSettings::GetSharedPtr()->GetFarmVersion();
	p1.m_serverIP = L"localhost";
	p1.m_serverPort = CFarmSettings::GetSharedPtr()->GetFarmServerPort();
	p1.m_timerCallbackIntervalMs = 1000;

	// m_lanClient의 이벤트 처리를 user worker thread pool에서 처리하기 위해. 
	// 그리고 FrameMove를 호출 할 필요가 없게 하기 위해.
	p1.m_userWorkerThreadModel = ThreadModel_MultiThreaded;

	ErrorInfoPtr outerror = ErrorInfoPtr(new ErrorInfo);
	m_lanClient->Connect(p1, outerror);
}

/// Processing that calls every specific time
/// 일정 시간마다 호출해야 하는 프로세싱
void CFarmClient::FarmClientFrameMove()
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);
	// m_lanClient->FrameMove();lanclient is not calling framemove. Thread pooling.
	//m_lanClient->FrameMove();lanclient는 framemove를 호출하지 않습니다.스레드 풀링.

	// If it takes too long for server connection process of CFarmClient then process authentication failed.
	// CFarmClient의 서버 연결 과정이 너무 오래 걸리면 인증 실패 처리를 하도록 합니다.
	if (m_authed == false && m_connectStartedTime && GetPreciseCurrentTimeMs() - m_connectStartedTime > 30)
	{
		m_dg->OnJoinFarmServerComplete(ErrorInfo::From(ErrorType_ConnectServerTimeout, HostID_Server, L"Connect to Farm Server timed out!"));
		m_lanClient->Disconnect();
		m_connectStartedTime = 0;
	}

	if (m_broadcastFarmClientInfoCoolTime < GetPreciseCurrentTimeMs() && m_p2pGroupID != HostID_None)
	{
		m_broadcastFarmClientInfoCoolTime = BroadcastFarmClientInfoInterval + GetPreciseCurrentTimeMs();

		RefreshFarmClientInfo();
		m_c2cProxy.NotifyFarmClientInfo(m_p2pGroupID, RmiContext::ReliableSend, m_info);
	}
}

void CFarmClient::OnJoinServerComplete( ErrorInfo *info, const ByteArray &replyFromServer )
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);
	if (info->m_errorType != ErrorType_Ok)
	{
		// Failed to connect server. Pass failure notice to farm server.
		// 서버 연결이 실패했습니다. 실패했습니다라는 notify를 farm server에게 토스합니다.
		m_dg->OnJoinFarmServerComplete(info);
	}
	else
	{
		// Succeed to connect farm server. Now this process start authentication as normal server.
		// Farm 서버로의 연결이 성공했습니다. 이제 이 프로세스는 정상적인 서버임을 인증함을 시도합니다.
		m_c2sProxy.RequestFarmLogon(HostID_Server, RmiContext::ReliableSend, m_farmName, m_farmClientName, m_farmClientType);

	}
}

void CFarmClient::OnLeaveServer( ErrorInfo *errorInfo )
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);
	m_dg->OnLeaveFarmServer(errorInfo->m_errorType);
}

void CFarmClient::OnP2PMemberJoin( HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField )
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);
	m_p2pGroupID =  groupHostID;


	// If it is status server then notify about itself to members.
	// 자신이 status 서버일 때 멤버들에게 자신을 알립니다.
	if(m_farmClientType == ServerMode_Status)
	{
		//ATLTRACE("%d Send NotifyStatusServer packet to Host \n",memberHostID);
		//ATLTRACE("%d Host 에게 NotifyStatusServer패킷 송신 \n",memberHostID);
		m_c2cProxy.NotifyStatusServer(memberHostID,RmiContext::ReliableSend);
	}
}

void CFarmClient::RefreshFarmClientInfo()
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);
	m_info.m_farmClientType = m_farmClientType;
	m_info.m_farmClientName = m_farmClientName;
	m_info.m_relatedLobbyName = m_dg->GetRelatedLobbyName();
	m_info.m_gamerCount = m_dg->GetGamerCount();
	m_info.m_serverAddr = m_dg->GetServerAddr();
	m_info.m_farmClientHostID = m_lanClient->GetLocalHostID();
}

void CFarmClient::OnP2PMemberLeave( HostID memberHostID, HostID groupHostID, int memberCount )
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);
	// Remove exited P2P member (info object of farm client).
	// 나간 P2P member, 즉 farm client의 info 객체를 제거합니다.
	m_remoteFarmClientInfoMap.Remove(memberHostID);
}

DEFRMI_FarmS2C_NotifyFarmLogonFailed(CFarmClient)
{
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING102);
	m_dg->OnJoinFarmServerComplete(ErrorInfo::From(reason, HostID_Server, loadString.GetString()));
	// Leaves log at DbLog Table.
	// DbLog Table에 Log를 남긴다.
	m_DbLogWriter->WriteLine(loadString.GetString());

	return true;
}

DEFRMI_FarmS2C_NotifyFarmLogonSuccess(CFarmClient)
{
	m_authed = true;
	m_dg->OnJoinFarmServerComplete(ErrorInfo::From(ErrorType_Ok, HostID_Server, L""));

	// Leaves log at DbLog Table.
	// Example of adding new field
	// Warning!! Adds TsetField at DbLog table first.
	// DbLog Table에 Log를 남긴다.
	// 새 필드 추가 예시
	// 주의!! DbLog 테이블에 TestField를 미리 추가해놔야 합니다.
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING116);
	CProperty newnode;
	String TestField = L"TestField";
	CVariant TestValue = 3;
	newnode.Add(TestField, TestValue);
	m_DbLogWriter->WriteLine( loadString.GetString(), &newnode);

	return true;
}

DEFRMI_FarmC2C_NotifyFarmClientInfo(CFarmClient)
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);
	CFarmClientInfoPtr pInfo(new CFarmClientInfo);
	*pInfo = info;

	// If it is existed then renew it. If none, adds one.
	// 만약 이미 존재하면 그것을 갱신합니다. 없으면 추가.
	m_remoteFarmClientInfoMap.Remove(remote);
	m_remoteFarmClientInfoMap.Add(remote, pInfo);
	m_dg->OnNotifyFarmClientInfo(info);

	return true;
}

DEFRMI_FarmC2C_RequestCreateGameRoom(CFarmClient)
{
	CGameRoomParameter grparam = gameRoomParam;

	// Creates guid before request room creation. And user function has to use that.
	// If you create guid at user function, guid list will be twisted.
	// 방 생성 요청 전에 방 guid를 먼저 만들어야 합니다. 그리고 유저 함수는 그것을 사용해야 합니다.
	// 유저 함수에서 guid를 만들게 하면 guid 목록이 꼬이게 됩니다.
	grparam.m_guid = Win32Guid::NewGuid();

    // Save HostId of lobby server which request room creation. 
    // Save lobby server ID use for indentify owner of room at battle server.
    // 방 생성을 요청했던 로비 서버의 HostID를 여기에 저장합니다. 
    // 저장된 로비 서버 ID는 배틀 서버에서 방의 소유주를 식별하는데 씁니다.
	grparam.m_creatorLobbyServerHostID = remote;

	// Once it creates room then notify requested server about information of created room.
	// 방 생성을 한 후 생성된 방에 대한 정보를 요청했던 로비 서버에게 notify합니다.
	CasualResult result = m_dg->CreateGameRoomByLobbyServer(grparam);

	if (result == Casual_Ok)
	{
		grparam.m_serverAddr = m_dg->GetServerAddr();
		ASSERT(m_dg->GetServerAddr().m_addr != L"");
	}
	ATLTRACE("NotifyCreateGameRoomResult\n");
	m_c2cProxy.NotifyCreateGameRoomResult(remote, RmiContext::ReliableSend,
	                                      requestedLobbyClientID, result, grparam);

	return true;
}

DEFRMI_FarmC2C_RequestJoinGameRoom(CFarmClient)
{
	// Once it creates room then notify to lobby server which requested about information of created room.
	// 방 생성을 한 후 생성된 방에 대한 정보를 요청했던 로비 서버에게 notify합니다.
	CGameRoomParameter gameRoomInfo;
	CasualResult result = m_dg->JoinGameRoomByLobbyServer(param, gameRoomInfo);

	if (result == Casual_Ok)
	{
		gameRoomInfo.m_serverAddr = m_dg->GetServerAddr();
		ASSERT(m_dg->GetServerAddr().m_addr != L"");
	}
	m_c2cProxy.NotifyJoinGameRoomResult(remote, RmiContext::ReliableSend,
	                                    requestedLobbyClientID, result, gameRoomInfo);

	return true;
}

DEFRMI_FarmC2C_NotifyCreateGameRoomResult(CFarmClient)
{
	// Pass response about room creation request to farm client owner.
	// 방 생성 요청에 대한 응답을 farm client owner에게 던집니다.
	m_dg->OnCreateGameRoomResult(requestedLobbyClientID, result, createdGameRoomParam);

	return true;
}

DEFRMI_FarmC2C_NotifyJoinGameRoomResult(CFarmClient)
{
	// Pass response about room creation request to farm client owner.
	// 방 생성 요청에 대한 응답을 farm client owner에게 던집니다.
	m_dg->OnJoinGameRoomResult(requestedLobbyClientID, result, createdGameRoomParam);

	return true;
}

DEFRMI_FarmC2C_GameRoom_Appear(CFarmClient)
{
	m_dg->OnGameRoomAppear(roomInfo);
	return true;
}

DEFRMI_FarmC2C_GameRoom_ShowState(CFarmClient)
{
	m_dg->OnGameRoomShowState(roomInfo);
	return true;
}


DEFRMI_FarmC2C_GameRoom_Disappear(CFarmClient)
{
	m_dg->OnGameRoomDisappear(roomGuid);
	return true;
}

DEFRMI_FarmC2C_NotifyStatusServer(CFarmClient)
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);
	
	//ATLTRACE("%d Receive DEFRMI_FarmC2C_NotifyStatusServer from HostID %s, ServerType %d \n", m_lanClient->GetLocalHostID() , (LPCSTR)CW2A(m_ownerServerName), m_owner->m_serverMode);
	//ATLTRACE("%d HostID %s에서 DEFRMI_FarmC2C_NotifyStatusServer를 받음, ServerType %d \n", m_lanClient->GetLocalHostID() , (LPCSTR)CW2A(m_ownerServerName), m_owner->m_serverMode);
	m_StatusServerHostID =remote;
	return true;
}

DEFRMI_FarmC2C_NotifyCreatedCredential(CFarmClient)
{
	m_dg->NotifyCreatedCredential(credential, UserHostID);

	return true;
}

DEFRMI_FarmC2C_NotifyCheckCredentialSuccess(CFarmClient)
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);

	m_dg->NotifyCheckCredentialSuccess(gamerGuid, userHostID);
	return true;
}

DEFRMI_FarmC2C_NotifyCheckCredentialFail(CFarmClient)
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);

	m_dg->NotifyCheckCredentialFail(userHostID);
	return true;
}

DEFRMI_FarmC2C_RequestCreateCredential(CFarmClient)
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);

	return m_dg->RequestCreateCredential(remote, gamerUUID, UserHostID);
}

DEFRMI_FarmC2C_RequestCheckCredential(CFarmClient)
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);

	return m_dg->RequestCheckCredential(remote, credential, userHostID, ServerName);
}

DEFRMI_FarmC2C_UserLogOut(CFarmClient)
{
	CriticalSectionLock lock(*m_dg->GetCritSec(), true);

	return m_dg->UserLogOut(remote, gamerUUID);
}

/************************************* Error Processing *************************************/
/************************************* 오류 처리 *************************************/
void CTestLogWriterDelegate::OnLogWriterException(Proud::AdoException &Err)
{
	ATLTRACE("%s", Err.what());
}

void CFarmClient::OnError(ErrorInfo *errorInfo) 
{
	if(m_owner == NULL)
		return;

	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	m_owner->LogError(txt.GetString());
}

void CFarmClient::OnWarning(ErrorInfo *errorInfo)
{
	if(m_owner == NULL)
		return;

	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	m_owner->LogEvent(txt.GetString());
}


void CFarmClient::OnInformation(ErrorInfo *errorInfo)
{
	if(m_owner == NULL)
		return;

	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	m_owner->LogEvent(txt.GetString());
}

void CFarmClient::OnException(const Proud::Exception &e) 
{
	if(m_owner == NULL)
		return;

	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()).GetString(), __FUNCTIONW__);
	m_owner->LogError(txt.GetString());
}

void CFarmClient::OnTick( void* context ) 
{
	if(m_dg)
		m_dg->OnTick(context);
}

