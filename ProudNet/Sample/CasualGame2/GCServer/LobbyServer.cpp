#include "StdAfx.h"
#include ".\Lobbyserver.h"
#include "../GCCommon/Vars.h"
#include "GCServerDlg.h"
#include "../GCCommon/Lobby_common.cpp"
#include "../GCCommon/Lobby_proxy.cpp"
#include "../GCCommon/Lobby_stub.cpp"
#include "CasualGamer.h"

int64_t RequestCreateGameRoomMinimumInterval = 1000;

LPCWSTR SampleRoomNames[] =
{
	L"fighting!",
	L"Have a good time!",
	L"Good job!",
	L"Together!",
	L"Let's play together!",
	L"Ya~~ho!",
	L"Have a game of play",
	L"Come here~!",
	L"I'm winner",
	L"Play! Play! Play!",
	L"Fighters pride",
	L"Lovely game",
	L"Come on"
};

/// Create character name at random. It is test purpose.
/// 랜덤으로 캐릭터 이름을 만듭니다. 테스트를 위함.
LPCWSTR GetRandomRoomName()
{
	int index = CRandom::StaticGetInt() % (sizeof(SampleRoomNames) / sizeof(SampleRoomNames[0]));
	return SampleRoomNames[index];
}

CRemoteLobbyClient_S::CRemoteLobbyClient_S()
{

	m_lastRequestTimeOfNewGameRoomMs = 0;
}

/// Gets guid of this gamer.
/// 이 게이머의 guid를 얻습니다.
Proud::Guid CRemoteLobbyClient_S::GetGamerGuid()
{
	if (m_dbLoadedGamerData)
		return m_dbLoadedGamerData->GetRootUUID();
	else
		return Guid();
}

/// Gives used player character information at last time, if it is loaded gamer.
/// 로딩한 게이머라면 이 메서드는 마지막에 사용한 플레이어 캐릭터의 정보를 줍니다.
Proud::Guid CRemoteLobbyClient_S::GetSelectedHeroGuid()
{
	if (!m_dbLoadedGamerData)
		return Guid();

	return CCasualGamer::GetSelectedHeroGuid(m_dbLoadedGamerData);
}

/// Gives used player character information at last time, if it is loaded gamer.
/// 로딩한 게이머라면 이 메서드는 마지막에 사용한 플레이어 캐릭터의 정보를 줍니다.
String CRemoteLobbyClient_S::GetSelectedHeroName()
{
	if (!m_dbLoadedGamerData)
		return String();

	return CCasualGamer::GetSelectedHeroName(m_dbLoadedGamerData);
}
///////////////////////////Member function of CRemoteLobbyClient_S..
///////////////////////////CRemoteLobbyClient_S의 멤버함수들..

CLobbyServer::CLobbyServer(void)
{
	m_lastGuiUpdateTime = 0;

	m_diffCustomArg = 10;
}

CLobbyServer::~CLobbyServer(void)
{
	try
	{
		m_netServer->Stop();
	}
	catch (Proud::Exception e)
	{
		LogError(LoadString(IDS_STRING110) + L" : Stop Entry server");
	}
	// Destory it first at inherit class. Because their callback accesses member of this class.
	// 상속 클래스에서 먼저 파괴해야 합니다. 왜냐하면 이들의 콜백이 이 클래스의 멤버를 접근하기 때문입니다.
	m_dbClient.Free();
	m_farmClient.Free();
	m_netServer.Free();
}

/// Starts listening
/// listening 시작
void CLobbyServer::Start()
{
	SetTitle(GetDescription().GetString());

	// Starts lobby server
	// Lobby server를 시작합니다.
	m_netServer.Attach(CNetServer::Create());
	m_netServer->SetEventSink(this);
	m_netServer->AttachProxy(&m_s2cProxy);
	m_netServer->AttachStub(this);

	// NOTE: Dynamic assgin to lobby server port. Because it may run several processes.
	// NOTE: lobby server port는 동적 할당을 하도록 합니다. 여러 프로세스를 작동시키는 경우도 있기 때문입니다.
	CStartServerParameter p1;
	p1.m_protocolVersion = CSettings::GetSharedPtr()->GetLobbyVersion();
	p1.m_timerCallbackContext = NULL;
	// Ontick은 FarmClient 에서 callBack되어져 오는 OnTick을 사용합니다.
	//p1.m_timerCallbackInterval = 1000;


	// Try to connect with farm server.
	// farm server에 연결을 시도합니다.
	SetStatusText(LoadString(IDS_STRING111));

	m_farmClient.Attach(new CFarmClient);

	CFarmClientConnectParameter p2;
	p2.m_delegate = this;
	p2.m_farmName = (LPCWSTR)CFarmSettings::GetSharedPtr()->m_FarmName;
	p2.m_farmClientName = m_lobbyName;
	p2.m_farmClientType = ServerMode_Lobby;
	m_farmClient->Init(p2, this, GetDescription());

	// Starts DB cache client.
	// DB cache client를 시작합니다.
	m_dbClient.Attach(CDbCacheClient2::New());
	CDbCacheClient2ConnectParameter p3;
	p3.m_delegate = this;
	p3.m_serverAddr = L"localhost";
	p3.m_serverPort = CFarmSettings::GetSharedPtr()->GetDbServerPort();
	p3.m_authenticationKey = (LPCWSTR)CFarmSettings::GetSharedPtr()->m_dbAuthKey;


	//m_netServer->Start(p1); // 예전 코드

	ErrorInfoPtr err;
	m_netServer->Start(p1, err);
	if (err)
	{
		printf("Server Start Error: %s\n", StringW2A(ErrorInfo::TypeToString_Kor(err->m_errorType)));
	}

	CFastArray<String> localAddresses;
	CNetUtil::GetLocalIPAddresses(localAddresses);
	m_serverAddr.m_addr = localAddresses[0];

	CFastArray<AddrPort> output;
	m_netServer->GetTcpListenerLocalAddrs(output);
	m_serverAddr.m_port = output[0].m_port;

	m_farmClient->Connect();
	m_dbClient->Connect(p3);

}

/// Processing every specific time
/// 일정 시간마다의 프로세싱
void CLobbyServer::OnTick(void* context)
{
	m_farmClient->FarmClientFrameMove();

	// Renew indicated detail at server GUI with ervery specific time.
	// (Note: Actually server cannot have GUI so do not realize it. Please use for only reference)
	// 일정 시간마다 서버 GUI에 표시 내용을 갱신합니다.
	// (주의: 실제 만드는 서버는 GUI를 가질 수 없기에 이런 식으로 구현하면 곤란합니다. 그냥 참고 바람.)

	if (GetPreciseCurrentTimeMs() - m_lastGuiUpdateTime > 1000)
	{
		m_lastGuiUpdateTime = GetPreciseCurrentTimeMs();

		int userCount = m_remoteClients.Count;

		String txt;
		txt.Format(L"%s : %d", LoadString(IDS_STRING118), userCount);
		SetStatusText(txt.GetString());
	}
}

void CLobbyServer::OnJoinFarmServerComplete(ErrorInfo* info)
{
	if (info->m_errorType == ErrorType_Ok)
	{
		// Succeed to connect with server.
		// 서버와의 연결이 성공했습니다.
		CriticalSectionLock lock(m_ownerDlg->m_cs, true);
		CStringW loadString;
		loadString.LoadStringW(IDS_STRING102);
		SetStatusText(loadString);
	}
	else
	{
		// Close process because it failed to connect with farm server.
		// Farm 서버와의 연결이 실패했으므로 프로세스를 종료합니다.
		String txt;
		txt.Format(L"%s : %s", LoadString(IDS_STRING115), info->ToString().GetString());
		LogError(txt.GetString());
		m_disposed = true;

	}
}

void CLobbyServer::OnLeaveFarmServer(ErrorType reason)
{
	// Close server process targets to this gamer because disconnected with farm servr.
	// Farm 서버와의 연결이 끊어졌으므로 이 게이머 대상 서버 프로세스도 종료합니다.
	m_disposed = true;

}

// Send response about room creation request to farm client owner.
// 방 생성 요청에 대한 응답을 farm client owner에게 던집니다.
void CLobbyServer::OnCreateGameRoomResult(HostID requestedLobbyClientID, CasualResult result, const CGameRoomParameter& createdGameRoomParam)
{
	CriticalSectionLock lock(m_cs, true);

	CRemoteLobbyClientPtr rc = GetAuthedRemoteClientByHostID(requestedLobbyClientID);
	if (!rc)
	{
		ATLTRACE("%s rc = NULL", __FUNCTION__);
		NotifyUnauthedAccess(requestedLobbyClientID);
		return;
	}

	// Once it succeed to create room then notice success signal to client who request room creation and add to list.
	// (Does not do adding game room to other client at lobby. This signal will come from battle server.)
	// 방 생성이 성공했으면 목록에 추가 & 방 생성 요청을 한 클라에게 성공 신호를 notify합니다.
	// (로비의 타 클라들에게 게임방 추가는 별도로 하지 않습니다. 이건 배틀 서버에서 별도로 신호가 올 것입니다.)
	if (result == Casual_Ok)
	{
		// Notify success signal to client who request room creation.
		// 방 생성 요청을 한 클라에게 성공 신호를 notify합니다.
		m_s2cProxy.NotifyCreateRoomSuccess(rc->m_hostID, RmiContext::ReliableSend, createdGameRoomParam);
	}
	else
		m_s2cProxy.NotifyCreateRoomFailed(rc->m_hostID, RmiContext::ReliableSend, result);
}
void CLobbyServer::OnJoinGameRoomResult(HostID requestedLobbyClientID, CasualResult result, const CGameRoomParameter& gameRoomParam)
{
	CriticalSectionLock lock(m_cs, true);

	CRemoteLobbyClientPtr rc = GetAuthedRemoteClientByHostID(requestedLobbyClientID);
	if (!rc)
	{
		NotifyUnauthedAccess(requestedLobbyClientID);
		return;
	}

	// Notify game room entery result to game client.
	// 게임방 진입 결과를 게임 클라에 notify합니다.
	if (result == Casual_Ok)
	{
		// Notify success signal to client who request room entery.
		// 방 진입 요청을 한 클라에게 성공 신호를 notify합니다.
		m_s2cProxy.NotifyJoinRoomSuccess(rc->m_hostID, RmiContext::ReliableSend, gameRoomParam);
	}
	else
		m_s2cProxy.NotifyJoinRoomFailed(rc->m_hostID, RmiContext::ReliableSend, result);
}

int CLobbyServer::GetGamerCount()
{
	CriticalSectionLock lock(m_cs, true);
	return m_remoteClients.Count;
}
Proud::NamedAddrPort CLobbyServer::GetServerAddr()
{
	return m_serverAddr;
}
/////////////IFarmClientDelegate!!!!!

void CLobbyServer::OnClientJoin(CNetClientInfo *clientInfo)
{
	CriticalSectionLock lock(m_cs, true);

	// 새 클라 객체를 생성합니다.
	CRemoteLobbyClientPtr rc(new CRemoteLobbyClient_S);
	rc->m_hostID = clientInfo->m_HostID;
	m_remoteClients.Add(rc->m_hostID, rc);
}

void CLobbyServer::OnClientLeave(CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment)
{
	CriticalSectionLock lock(m_cs, true);
	CRemoteLobbyClientPtr rc = GetRemoteClientByHostID(clientInfo->m_HostID);
	if (!rc)
		return;

	// Notify gamer list removal to other client.
	// 타 클라에게 게이머 목록 제거를 notify합니다.
	for (RemoteLobbyClients::iterator i = m_remoteClients.begin();
		i != m_remoteClients.end(); i++)
	{
		CRemoteLobbyClientPtr otherRc = i->second;
		if (otherRc != rc && otherRc->m_info.m_guid != Guid())
		{
			m_s2cProxy.HeroSlot_Disappear(otherRc->m_hostID, RmiContext::ReliableSend,
				rc->m_info.m_guid);
		}
	}

	// Does DB unload process.
	// DB unload 처리도 합니다.
	if (rc->m_dbLoadedGamerData)
	{
		m_farmClient->m_c2cProxy.UserLogOut(m_farmClient->m_StatusServerHostID, RmiContext::ReliableSend, rc->m_dbLoadedGamerData->GetRootUUID());

		m_dbClient->UnloadDataBySessionGuid(rc->m_dbLoadedGamerData->GetSessionGuid(), ByteArray());
		rc->m_dbLoadedGamerData = CLoadedData2Ptr();
	}

	// Finishing data
	// 데이터 마무리
	m_remoteClients.Remove(clientInfo->m_HostID);
}
///////////////////////INetServerEvent!!!!!


/// Communication error of unauthenticated gamer
/// 미인증 게이머의 통신 오류
void CLobbyServer::NotifyUnauthedAccess(HostID clientID)
{
	m_s2cProxy.NotifyUnauthedAccess(clientID, RmiContext::ReliableSend);
}

CRemoteLobbyClientPtr CLobbyServer::GetRemoteClientByGamerGuid(Guid gamerGuid)
{
	CriticalSectionLock lock(m_cs, true);
	for (RemoteLobbyClients::iterator i = m_remoteClients.begin(); i != m_remoteClients.end(); i++)
	{
		CRemoteLobbyClientPtr rc = i->second;
		if (rc->m_dbLoadedGamerData && rc->m_dbLoadedGamerData->GetRootUUID() == gamerGuid)
			return rc;
	}

	return CRemoteLobbyClientPtr();
}

CRemoteLobbyClientPtr CLobbyServer::GetAuthedRemoteClientByHostID(HostID clientID)
{
	CRemoteLobbyClientPtr rc = GetRemoteClientByHostID(clientID);
	if (rc && rc->m_dbLoadedGamerData)
	{
		return rc;
	}
	return CRemoteLobbyClientPtr();
}



CRemoteLobbyClientPtr CLobbyServer::GetRemoteClientBySessionGuid(Guid requestSessionGuid)
{
	for (RemoteLobbyClients::iterator i = m_remoteClients.begin();
		i != m_remoteClients.end(); i++)
	{
		CRemoteLobbyClientPtr rc = i->second;
		if (rc->m_dbLoadedGamerData != NULL && rc->m_dbLoadedGamerData->GetSessionGuid() == requestSessionGuid)
			return rc;
	}

	return CRemoteLobbyClientPtr();
}


/** Find out battle server which running lightly.
However, only find out battle server which related with this lobby server.*/
/** 가장 가볍게 작동중인 배틀 서버를 찾습니다.
단, 이 로비 서버와 연관된 배틀 서버만 찾아야 합니다.*/
CFarmClientInfoPtr CLobbyServer::GetLightestBattleServer()
{
	CriticalSectionLock lock(m_ownerDlg->m_cs, true);

	CFarmClientInfoPtr choice;
	int choiceCount = INT_MAX;
	CFarmClientInfoPtr fc;
	for (FarmClientInfoMap::iterator i = m_farmClient->m_remoteFarmClientInfoMap.begin();
		i != m_farmClient->m_remoteFarmClientInfoMap.end(); i++)
	{
		fc = i->second;
		if (choiceCount > fc->m_gamerCount &&
			fc->m_farmClientType == ServerMode_Battle &&  // If farm client is battle server     // Farm 클라가 배틀 서버이며
			m_lobbyName.CompareNoCase(fc->m_relatedLobbyName) == 0) // If relation between battle server and lobby server is the same     // 배틀 서버의 관계 로비 서버가 동일하면
		{
			choiceCount = fc->m_gamerCount;
			choice = fc;
		}
	}

	return choice;

}

CRemoteLobbyClientPtr CLobbyServer::GetRemoteClientByHostID(HostID clientID)
{
	CRemoteLobbyClientPtr output;
	if (m_remoteClients.TryGetValue(clientID, output))
		return output;
	else
		return CRemoteLobbyClientPtr();
}

void CLobbyServer::GetAuthedClientList(CFastArray<HostID> &output)
{
	output.Clear();
	for (RemoteLobbyClients::iterator i = m_remoteClients.begin(); i != m_remoteClients.end(); i++)
	{
		CRemoteLobbyClientPtr rc = i->second;
		if (rc->m_dbLoadedGamerData)
			output.Add(rc->m_hostID);
	}
}

void CLobbyServer::OnGameRoomAppear(const CGameRoomParameter& roomInfo)
{
	CriticalSectionLock lock(m_cs, true);

	// Adds to list
	// 목록에 추가
	CLobbyGameRoomPtr_S room(new CLobbyGameRoom_S);
	room->m_info = roomInfo;
	ASSERT(room->m_info.m_serverAddr.m_addr != L"");
	m_gameRooms.Add(room->m_info.m_guid, room);

	// Synchronize adding list to all lobby clients
	// 모든 로비 클라에게 목록 추가 동기화
	CFastArray<HostID> sendTarget;
	GetAuthedClientList(sendTarget);
	//ATLTRACE("%s: Number of transmission target=%d\n",__FUNCTION__,sendTarget.Count);
	//ATLTRACE("%s: 송출 대상 갯수=%d\n",__FUNCTION__,sendTarget.Count);
	m_s2cProxy.GameRoom_Appear(sendTarget.GetData(), sendTarget.Count, RmiContext::ReliableSend,
		room->m_info);
}

void CLobbyServer::OnGameRoomShowState(const CGameRoomParameter& info)
{
	CriticalSectionLock lock(m_cs, true);

	// Renew list
	// 목록 갱신
	CLobbyGameRoomPtr_S room = GetGameRoomByGuid(info.m_guid);
	if (room)
	{
		room->m_info = info;

		// Synchronize renewal of list to all lobby client
		// 모든 로비 클라에게 목록 갱신 동기화
		CFastArray<HostID> sendTarget;
		GetAuthedClientList(sendTarget);
		m_s2cProxy.GameRoom_ShowState(sendTarget.GetData(), sendTarget.Count, RmiContext::ReliableSend,
			room->m_info);
	}
}

void CLobbyServer::OnGameRoomDisappear(Guid roomGuid)
{
	CriticalSectionLock lock(m_cs, true);

	// Remove from list and list synchronization to all lobby client
	// 목록에서 제거 및 모든 로비 클라에게 목록 동기화
	CLobbyGameRoomPtr_S room = GetGameRoomByGuid(roomGuid);
	if (room)
	{
		CFastArray<HostID> sendTarget;
		GetAuthedClientList(sendTarget);
		m_s2cProxy.GameRoom_Disappear(sendTarget.GetData(), sendTarget.Count, RmiContext::ReliableSend,
			roomGuid);

		m_gameRooms.Remove(roomGuid);
	}
}

CLobbyGameRoomPtr_S CLobbyServer::GetGameRoomByGuid(Guid roomGuid)
{
	CLobbyGameRoomPtr_S output;
	if (m_gameRooms.TryGetValue(roomGuid, output))
		return output;
	else
		return CLobbyGameRoomPtr_S();
}

/// Gets guid of game room and gets battle server object that manage game room.
/// 게임방 guid를 입력받아, 그 게임방을 주관하고 있는 배틀 서버 객체를 얻습니다.
CFarmClientInfoPtr CLobbyServer::GetBattleServerByGameRoom(Guid gameRoomGuid)
{
	CLobbyGameRoomPtr_S room = GetGameRoomByGuid(gameRoomGuid);
	if (!room)
		return CFarmClientInfoPtr();
	NamedAddrPort battleServerAddr = room->m_info.m_serverAddr;

	// Find out same battle server object from farm client list.
	// Farm 클라 목록으로부터 동일한 배틀 서버 객체를 찾습니다.
	for (FarmClientInfoMap::iterator i = m_farmClient->m_remoteFarmClientInfoMap.begin();
		i != m_farmClient->m_remoteFarmClientInfoMap.end(); i++)
	{
		CFarmClientInfoPtr fc = i->second;
		if (fc->m_serverAddr == battleServerAddr)
			return fc;
	}

	return CFarmClientInfoPtr();
}

void CLobbyServer::OnExclusiveLoadDataComplete(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);
	ATLTRACE("DEFRMI_LobbyC2S_RequestNextLogon--> CLobbyServer::OnExclusiveLoadDataSuccess(CCallbackArgs& args)\n");

	HostID remote = (HostID)args.m_tag;
	if (args.m_items.GetCount() < 1)
	{
		return;
	}

	CRemoteLobbyClientPtr rc = GetRemoteClientByHostID(remote);
	if (rc == NULL)
	{
		ATLTRACE("%s rc=NULL", __FUNCTION__);
		return;
	}

	ErrorType result = args.m_items[0].m_reason;
	// 요청이 실패한 경우
	if (result != ErrorType_Ok)
	{
		ATLTRACE("%s\n", __FUNCTION__);
		m_s2cProxy.NotifyNextLogonFailed(rc->m_hostID, RmiContext::ReliableSend, result);
	}
	// 요청이 성공한 경우
	else
	{
		CPropNodePtr gamer = args.m_items[0].m_loadedData->GetRootNode();
		if ((gamer->Fields[L"SelectedHeroGuid"]) == Guid() || (gamer->GetChild()) == NULL)
		{
			LogError(L"Data Load Error  ");
			m_s2cProxy.ShowError(rc->m_hostID, RmiContext::ReliableSend, L"Loaded Data has a Problem");
			m_dbClient->UnloadDataBySessionGuid(args.m_items[0].m_sessionGuid);
			return;
		}
		else
		{
			Guid SelectedHeroGuid = gamer->Fields[L"SelectedHeroGuid"];
			CPropNodePtr selectedHero = args.m_items[0].m_loadedData->GetNode(SelectedHeroGuid);
			String Name = CVariant(selectedHero->Fields[L"Name"]);
			rc->m_info.m_name = Name.GetString();;

			rc->m_info.m_guid = gamer->GetChild()->GetUUID();
			rc->m_info.m_type = gamer->GetChild()->Fields[L"HeroType"];
			rc->m_info.m_score = gamer->GetChild()->Fields[L"Sample_Score"];
		}

		// Data renewal
		//데이터 갱신
		rc->m_dbLoadedGamerData = args.m_items[0].m_loadedData;
		ASSERT(rc->m_dbLoadedGamerData->sessionGuid != Guid());
		ASSERT(args.m_items[0].m_rootUUID != Guid());
		ASSERT(m_farmClient->m_StatusServerHostID != HostID_None);
		ATLTRACE("Host %d", m_farmClient->m_StatusServerHostID);

		m_s2cProxy.NotifyNextLogonSuccess(rc->m_hostID, RmiContext::ReliableSend, rc->m_dbLoadedGamerData->RootUUID, rc->m_info);

		ATLTRACE("%s\n END", __FUNCTION__);
	}
}


void CLobbyServer::NotifyCheckCredentialFail(HostID userHostID)
{
	ATLTRACE("%s\n", __FUNCTION__);
	m_s2cProxy.NotifyNextLogonFailed(userHostID, RmiContext::ReliableSend, ErrorType_Unexpected);
	return;
}

void CLobbyServer::NotifyCheckCredentialSuccess(const Guid &gamerGuid, HostID userHostID)
{
	CRemoteLobbyClientPtr rc = GetRemoteClientByHostID(userHostID);

	if (!rc)
		return;

	// Starts gamer information loading (include account authentication) to DB cache server by logon attempt of Lobby client.
	// CustomField Setting that indicate it as Lobby client
	// Lobby client의 로그온 시도에 따라 DB cache server로 게이머 정보 로딩(계정 인증 포함)을 시작합니다.
	// Lobby Client임을 표시하기 위한 CustomField Setting
	ByteArray message;
	message.Add(NewLobbyClient);

	m_dbClient->RequestExclusiveLoadDataByGuid(L"Gamer", gamerGuid, userHostID, message);
}


// Realize handling routine when disappearing exclusive loading
//독점 로딩 증발시에 대한 handling 루틴도 구현
void CLobbyServer::OnDataUnloadRequested(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	for (intptr_t i = 0; i < args.m_items.GetCount(); ++i)
	{
		CRemoteLobbyClientPtr rc = GetRemoteClientBySessionGuid(args.m_items[i].m_sessionGuid);
		if (rc)
		{
			if (rc->m_dbLoadedGamerData)
			{
				if (!m_dbClient->UnloadDataBySessionGuid(args.m_items[i].m_sessionGuid, ByteArray()))
				{
					CStringW loadString;
					loadString.LoadStringW(IDS_STRING103);
					LogError(loadString);
				}

				rc->m_dbLoadedGamerData = CLoadedData2Ptr();
			}

			// Clost connection of current client because new Client has joined in.
			// 새로온 Client가 들어왔으므로 기존 Client의 연결을 종료합니다.
			if (args.m_items[i].m_message.FindByValue(NewEntryClient) != -1)
			{
				m_netServer->CloseConnection(rc->m_hostID);
			}
		}
	}
}



void CLobbyServer::OnUpdateDataFailed(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	// Faild UpdateData. Reason:%s Comments:%s
	// UpdateData 실패. 이유:%s 코멘트:%s
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING104);
	String text;
	text.Format(loadString, ErrorInfo::TypeToString(args.m_items[0].m_reason), args.m_items[0].m_comment.GetString());
	LogError(text.GetString());

}

void CLobbyServer::OnUpdateDataSuccess(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);
	if (m_diffCustomArg != args.m_tag)
	{
		// Ticket does not match. Setup is wrong or server error.
		// 대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다.
		CStringW loadString;
		loadString.LoadStringW(IDS_STRING109);
		LogError(loadString);
	}
	CRemoteLobbyClientPtr rc = GetRemoteClientByGamerGuid(args.m_items[0].m_loadedData->GetRootUUID());
	rc->m_dbLoadedGamerData = args.m_items[0].m_loadedData; // Data update    // 데이터 최신화


}


void CLobbyServer::OnAccessError(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);
	String comment;
	comment = L"AccessError : ";
	comment = comment + args.m_items[0].m_comment;
	LogError(comment.GetString());
}

// Send it to server, if data file loading for lobby client has completed.
// Server does actual communication that lobby related for client who sent it.
// 로비 클라용 데이터 파일 로딩이 완료됐으면 서버에 이걸 보냅니다.
// 서버는 이걸 보내온 클라에 대해서만 로비 관련 실제 통신을 수행합니다.
DEFRMI_LobbyC2S_NotifyChannelFormReady(CLobbyServer)
{
	CriticalSectionLock lock(m_cs, true);

	CRemoteLobbyClientPtr rc = GetAuthedRemoteClientByHostID(remote);
	if (!rc)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Send all information to client for showing lobby main form.
	// Waiting room list, player character information of local client, etc
	// 로비 메인 폼을 보여주기 위한 모든 정보를 클라에게 전송합니다.
	// 대기실 리스트, 로컬 클라의 플레이어 캐릭터 정보 등
	for (RemoteLobbyClients::iterator i = m_remoteClients.begin(); i != m_remoteClients.end(); i++)
	{
		CRemoteLobbyClientPtr otherRc = i->second;
		if (otherRc != rc && otherRc->m_info.m_guid != Guid())
		{
			// Send information about old gamer to new gamer.
			// 새 게이머에게는 기존 게이머에 대한 정보를 보내 줍니다.
			m_s2cProxy.HeroSlot_Appear(remote, RmiContext::ReliableSend,
				otherRc->m_info);

			// Send information about new gamer to old gamer.
			// 기존 게이머들에게는 새로 들어온 게이머에 대한 정보를 보내 줍니다.
			m_s2cProxy.HeroSlot_Appear(otherRc->m_hostID, RmiContext::ReliableSend,
				rc->m_info);
		}
	}
	// Send information of new gamer itself.
	// 새 게이머 자신의 정보도 송달.
	m_s2cProxy.HeroSlot_Appear(remote, RmiContext::ReliableSend,
		rc->m_info);

	// Send current room list to new gamer.
	// 새 게이머에게 기존 방 리스트를 보내 줍니다.
	for (LobbyGameRooms_S::iterator i = m_gameRooms.begin(); i != m_gameRooms.end(); i++)
	{
		CLobbyGameRoomPtr_S gr = i->second;
		m_s2cProxy.GameRoom_Appear(remote, RmiContext::ReliableSend, gr->m_info);
	}

	// Send player character state of new gamer.
	// 새 게이머의 플레이어 캐릭터 상태를 보내 줍니다.
	m_s2cProxy.LocalHeroSlot_Appear(remote, RmiContext::ReliableSend,
		rc->m_info);

	return true;
}


DEFRMI_LobbyC2S_RequestNextLogon(CLobbyServer)
{

	CriticalSectionLock lock(m_cs, true);

	// Find out client object which did not authenticate yet.
	// 아직 인증하지 않은 클라 객체를 찾습니다.
	CRemoteLobbyClientPtr rc = GetRemoteClientByHostID(remote);

	if (!rc || rc->m_hostID == HostID_None)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	if (credential == Guid())
	{
		// Wrong parameter!
		// 잘못된 파라메터다!
		ATLTRACE("%s\n", __FUNCTION__);
		m_s2cProxy.NotifyNextLogonFailed(remote, RmiContext::ReliableSend, ErrorType_Unexpected);
		return true;
	}

	m_farmClient->m_c2cProxy.RequestCheckCredential(m_farmClient->m_StatusServerHostID, RmiContext::ReliableSend, credential, rc->m_hostID, GetDescription());

	return true;
}


//// Whole chatting in 1 lobby channel
//// 1개 로비 채널 내에서의 전체 채팅
DEFRMI_LobbyC2S_Chat(CLobbyServer)
{
	// Send chatting message to all clients in waiting room.
	// 채팅 메시지를 대기실의 모든 클라에게 쏩니다.
	CriticalSectionLock lock(m_cs, true);
	CFastArray<HostID> sendTarget;

	CRemoteLobbyClientPtr rc = GetAuthedRemoteClientByHostID(remote);
	if (!rc)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	GetAuthedClientList(sendTarget);

	m_s2cProxy.ShowChat(sendTarget.GetData(), sendTarget.Count, RmiContext::ReliableSend,
		rc->m_info.m_guid, chatText);

	return true;
}

DEFRMI_LobbyC2S_RequestCreateGameRoom(CLobbyServer)
{
	CriticalSectionLock lock(m_cs, true);

	CGameRoomParameter grparam = gameRoomParam;

	ATLTRACE("%s\n", __FUNCTION__);
	CRemoteLobbyClientPtr rc = GetAuthedRemoteClientByHostID(remote);
	if (!rc)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Too many room creation request is filtering from server.
	// It prevents some of server down attempt from hacked client.
	// 너무 잦은 횟수의 방 생성 요청은 서버에서 미리 걸러버립니다.
	// 이게 있어야 해킹 클라에서 서버를 다운시키기 위한 짓을 일부 방지합니다.
	if (GetPreciseCurrentTimeMs() - rc->m_lastRequestTimeOfNewGameRoomMs < RequestCreateGameRoomMinimumInterval)
	{
		m_s2cProxy.NotifyCreateRoomFailed(remote, RmiContext::ReliableSend, Casual_TooShortRequest);
		return true;
	}

	// Check suitability of parameter.
	// 파라메터의 합리성 체크를 합니다.
	if (grparam.m_maxGamerCount < 1)
	{
		m_s2cProxy.NotifyCreateRoomFailed(remote, RmiContext::ReliableSend, Casual_BadGamerCount);
		return true;
	}

	// Modify room title, if you require.
	// 필요시 방 제목을 수정합니다.
	if (grparam.m_name.CompareNoCase(NoRoomNameText) == 0)
	{
		grparam.m_name = GetRandomRoomName();
	}

	grparam.m_masterHeroGuid = rc->GetSelectedHeroGuid();
	grparam.m_masterHeroName = rc->GetSelectedHeroName().GetString();
	ASSERT(grparam.m_masterHeroGuid != Guid());

	// Find out battle server with light load.
	// 가장 가벼운 부하를 가진 battle 서버를 찾습니다.
	CFarmClientInfoPtr lightestBattleServer = GetLightestBattleServer();
	if (!lightestBattleServer)
	{
		CStringW loadString = LoadStringW(IDS_STRING117);
		// If it has not got any related battle server then make it as exception.
		// 연계된 battle server가 전혀 없는 경우도 있으므로 예외 처리를 합니다.
		m_s2cProxy.ShowError(remote, RmiContext::ReliableSend, loadString.GetString());
		return true;
	}

	ASSERT(lightestBattleServer->m_farmClientHostID != HostID_None);
	// Request room creation to found battle server.
	// 찾은 battle server에게 방 생성을 요청합니다.
	Guid requestGuid = Win32Guid::NewGuid();
	m_farmClient->m_c2cProxy.RequestCreateGameRoom(
		lightestBattleServer->m_farmClientHostID, RmiContext::ReliableSend, // Pointing Battle server for receiving target     // 수신 대상으로 Battle server를 가리킴
		rc->m_hostID, grparam);

	rc->m_lastRequestTimeOfNewGameRoomMs = GetPreciseCurrentTimeMs();

	return true;
}

DEFRMI_LobbyC2S_RequestJoinGameRoom(CLobbyServer)
{
	CriticalSectionLock lock(m_cs, true);

	CRemoteLobbyClientPtr rc = GetAuthedRemoteClientByHostID(remote);
	if (!rc)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	CJoinGameRoomParameter param2 = param;
	param2.m_joinRequestedHeroGuid = rc->GetSelectedHeroGuid();

	// Check whether opposited room is cached or not.
	// 대응하는 방이 로비 서버에 캐쉬되어 있는지 체크합니다.
	CLobbyGameRoomPtr_S room = GetGameRoomByGuid(param2.m_guid);
	if (!room)
	{
		m_s2cProxy.NotifyJoinRoomFailed(remote, RmiContext::ReliableSend, Casual_NoGameRoom);
		return true;
	}

	// Find out battle server that manage found room.
	// 찾은 방을 관리하는 battle 서버를 찾습니다.
	CFarmClientInfoPtr battleServer = GetBattleServerByGameRoom(room->m_info.m_guid);
	if (!battleServer)
	{
		// Notify entry failure becasue it does not have related battle server.
		// 연계된 battle server가 없으므로 진입 실패를 notify합니다.
		m_s2cProxy.NotifyJoinRoomFailed(remote, RmiContext::ReliableSend, Casual_NoGameRoom);
		return true;
	}

	// Reqeust room entry to found battle server.
	// 찾은 battle server에게 방 진입을 요청합니다.
	Guid requestGuid = Win32Guid::NewGuid();
	m_farmClient->m_c2cProxy.RequestJoinGameRoom(
		battleServer->m_farmClientHostID, RmiContext::ReliableSend, // Point Battle server as receiving target    // 수신 대상으로 Battle server를 가리킴
		rc->m_hostID, param2);

	return true;
}

/************************************* Error Processing *************************************/
/************************************* 오류 처리 *************************************/
void CLobbyServer::OnError(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogError(txt.GetString());
}

void CLobbyServer::OnWarning(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}


void CLobbyServer::OnInformation(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}

void CLobbyServer::OnException(const Proud::Exception &e)
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()).GetString(), __FUNCTIONW__);
	LogError(txt.GetString());
}
