#include "StdAfx.h"
#include ".\farmserver.h"
#include "../GCCommon/Vars.h"
#include "GCServerDlg.h"
#include "resource.h"

CFarmServer::CFarmServer(void)
{
}

CFarmServer::~CFarmServer(void)
{
	// Destory it firstly at inherit class. Because their callback accesses member of this class.
	// 상속 클래스에서 먼저 파괴해야 합니다. 왜냐하면 이들의 콜백이 이 클래스의 멤버를 접근하기 때문입니다.
	m_lanServer.Free();
	m_dbServer.Free();
}

/// Start listening of farm server.
/// Farm 서버의 listening을 시작합니다.
void CFarmServer::Start()
{
	m_farmName = (LPCWSTR)CFarmSettings::GetSharedPtr()->m_FarmName;
	// "Ready to start server..."
	// "서버 시작을 준비중입니다..."
	CStringW startServerString;
	startServerString.LoadStringW(IDS_STRING106);
	SetStatusText(startServerString);
	SetTitle(GetDescription().GetString());


	// Starts DB cache server too.
	// DB cache server도 시작합니다.
	m_dbServer.Attach(CDbCacheServer2::New());

	// Even you delete character, it does not removed instantly. It is possible to recover later by user.
	// 캐릭터를 지우더라도 바로 제거되지는 않습니다. 아래 설정에 따라 추후 사용자가 복원 가능합니다.
	//m_dbServer->SetAutoObliterateDuration(COleDateTimeSpan(3,0,0,0));
	m_dbServer->SetUnloadedDataHibernateDurationMs(3000);

	CDbCacheServer2StartParameter p2;
	p2.m_authenticationKey = (LPCWSTR)CFarmSettings::GetSharedPtr()->m_dbAuthKey;
	p2.m_DbmsConnectionString = (LPCWSTR)CFarmSettings::GetSharedPtr()->m_dbmsConnectionString;
	p2.m_delegate = this;
	p2.m_tcpPort = CFarmSettings::GetSharedPtr()->GetDbServerPort();

	m_gamerTable.m_rootTableName = L"Gamer";
	m_gamerTable.m_childTableNames.Add(L"Hero");
	m_gamerTable.m_childTableNames.Add(L"Item");
	m_gamerTable.m_childTableNames.Add(L"Friend");

	p2.m_tableNames.Add(m_gamerTable);

#ifdef _DEBUG
	m_dbServer->SetDefaultTimeoutTimeSec(6000);
#endif

	try
	{
		m_dbServer->Start(p2);
	}
	catch (Proud::AdoException &e)
	{
		MessageBox(NULL, L"You must create the DB schema of Casual Game 2 first.", L"Error", MB_OK);
		// "Exception occur during starts DB cache server: %s"
		// "DB cache server 시작 중 예외 발생: %s"
		CStringW loadString;
		loadString.LoadStringW(IDS_STRING107);
		String txt;
		txt.Format(loadString, StringA2W(e.what()).GetString());
		LogError(txt.GetString());
		m_disposed = true;

		return;
	}

	m_p2pGroupID = HostID_None;

	// Starts farm server.
	// farm server를 시작합니다.
	m_lanServer.Attach(CNetServer::Create());
	m_lanServer->SetEventSink(this);
	m_lanServer->AttachProxy(&m_s2cProxy);
	m_lanServer->AttachStub(this);

	// It is better to accept even one message is big between server communication.
	// Because it is LAN situation and often communication traffic is big.

	// 서버간 통신에서는 1개의 메시지 크기가 매우 커도 허락하게 하는 것이 좋습니다.
	// LAN 환경이기 때문인데다 통신량이 많은 경우가 흔하기 때문입니다.
	m_lanServer->SetMessageMaxLength(1024 * 1024, 1024 * 1024); // 최대 1GB 크기의 메시지를 보내는 것을 허용

	CStartServerParameter p1;
	p1.m_protocolVersion = CFarmSettings::GetSharedPtr()->GetFarmVersion();
	p1.m_tcpPorts.Add(CFarmSettings::GetSharedPtr()->GetFarmServerPort());
	ErrorInfoPtr errout = ErrorInfoPtr(new ErrorInfo);
#ifdef _DEBUG
	m_lanServer->SetDefaultTimeoutTimeMs(300 * 1000);
#endif
	m_lanServer->Start(p1, errout);

	// Excute entry server, lobby server, battle server program.
	// entry server, lobby server, battle server 프로그램을 실행시킵니다.
	m_ownerDlg->AddServer(L"-entry");
	m_ownerDlg->AddServer(L"-status");
	m_ownerDlg->AddServer(L"-lobby L1");
	m_ownerDlg->AddServer(L"-lobby L2");
	m_ownerDlg->AddServer(L"-battle L1");
	m_ownerDlg->AddServer(L"-battle L1");
	m_ownerDlg->AddServer(L"-battle L2");
	m_ownerDlg->AddServer(L"-battle L2");

	// "Working normally"
	// "정상 작동 중"
	CStringW logString;
	logString.LoadStringW(IDS_STRING102);
	SetStatusText(logString);


	// Creates Thread for FrameMove.
	// FrameMove를 위한 Thread를 생성합니다.
}


/// Find out farm client object that based on HostID value.
/// HostID 값을 근거로 Farm 클라이언트 객체를 찾습니다.
CRemoteFarmClientPtr CFarmServer::GetClientByHostID(HostID hostID)
{
	CriticalSectionLock lock(m_cs, true);
	CRemoteFarmClientPtr output;
	if (!m_remoteClients.TryGetValue(hostID, output))
		return CRemoteFarmClientPtr();
	else
		return output;
}

void CFarmServer::OnClientJoin(CNetClientInfo *clientInfo)
{
	CriticalSectionLock lock(m_cs, true);

	// Creates new object that opposite client then adds to collection.
	// 클라이언트에 대응하는 새 객체를 만들어서 콜렉션에 추가해놓습니다.
	CRemoteFarmClientPtr rc = GetClientByHostID(clientInfo->m_HostID);
	if (!rc)
	{
		CRemoteFarmClientPtr newObj(new CRemoteFarmClient);
		newObj->m_hostID = clientInfo->m_HostID;
		m_remoteClients[clientInfo->m_HostID] = newObj;
		rc = newObj;
	}

	// Join to P2P group that bind all new joined farm client.
	// 새로 접속한 farm client를 모두 묶는 P2P 그룹에 합류시킵니다.
	if (m_p2pGroupID != HostID_None)
	{
		m_lanServer->JoinP2PGroup(rc->m_hostID, m_p2pGroupID);
	}
	else
		m_p2pGroupID = m_lanServer->CreateP2PGroup(&rc->m_hostID, 1);
}

void CFarmServer::OnClientLeave(CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment)
{
	CriticalSectionLock lock(m_cs, true);

	// Remove opposite object.
	// 대응하는 객체를 제거합니다.
	CRemoteFarmClientPtr rc = GetClientByHostID(clientInfo->m_HostID);
	if (rc)
	{
		m_remoteClients.Remove(clientInfo->m_HostID);
	}
}

String CFarmServer::GetDescription()
{
	return L"Farm Server";
}

DEFRMI_FarmC2S_RequestFarmLogon(CFarmServer)
{
	CriticalSectionLock lock(m_cs, true);

	// Find out opposite object.
	// 대응하는 객체를 찾습니다.
	CRemoteFarmClientPtr rc = GetClientByHostID(remote);
	if (!rc)
	{
		m_s2cProxy.NotifyFarmLogonFailed(remote, RmiContext::ReliableSend, ErrorType_Unexpected);
		return true;
	}

	// If already authenticated or logon Farm client with same name then dine it.
	// 이미 인증됐거나 같은 이름의 Farm client가 로그온을 하는 경우 거절합니다.
	if (rc->m_authed || rc->m_farmClientName == FarmClientName)
	{
		m_s2cProxy.NotifyFarmLogonFailed(remote, RmiContext::ReliableSend, ErrorType_AlreadyConnected);
		return true;
	}

	if (FarmName != m_farmName)
	{
		m_s2cProxy.NotifyFarmLogonFailed(remote, RmiContext::ReliableSend, ErrorType_PermissionDenied);
		return true;
	}

	// Success
	// 성공
	rc->m_authed = true;
	rc->m_farmClientName = FarmClientName;

	m_s2cProxy.NotifyFarmLogonSuccess(remote, RmiContext::ReliableSend);


	return true;

}

CRemoteFarmClient::CRemoteFarmClient()
{
	m_authed = false;
}



/************************************* Error Processing *************************************/
/************************************* 오류 처리 *************************************/
void CFarmServer::OnError(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogError(txt.GetString());
}

void CFarmServer::OnWarning(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}


void CFarmServer::OnInformation(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}

void CFarmServer::OnException(const Proud::Exception &e)
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()).GetString(), __FUNCTIONW__);
	LogError(txt.GetString());
}