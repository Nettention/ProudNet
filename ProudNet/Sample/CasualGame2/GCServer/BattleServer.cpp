#include "StdAfx.h"
#include "../GCCommon/Vars.h"
#include "../GCCommon/battle_common.cpp"
#include "../gccommon/battle_proxy.cpp"
#include "../gccommon/battle_stub.cpp"
#include "Battleserver.h"
#include "CasualGamer.h"
#include "GCServerDlg.h"
#include "resource.h"

int64_t BanTooOldAllowanceInterval = 60 * 3 * 1000;

CGameRoom_S::CGameRoom_S()
{
	m_p2pGroupID = HostID_None;
	m_loadingModeStartedTimeMs = 0;
	m_clientPlayLoadingTimeoutMs = 60 * 3 * 1000; // 3 minutes
}

/// Select room master from room member except gamer who pointed by gamerGuid.
/// gamerGuid가 가리키는 게이머를 제외한 나머지 room member 중에서 방장을 선발합니다.
void CGameRoom_S::ChangeMasterGamerOtherThan(Guid gamerGuid)
{
	m_info.m_masterHeroGuid = Guid(); // Remove anyway  // 일단 제거
	m_info.m_masterHeroName = L"";

	// 찾은 다음 갱신
	for each(JoinedGamers::value_type i in m_joinedGamers)
	{
		CBattleClient_S* rc = i.second;
		if (rc->GetGamerGuid() != gamerGuid)
		{
			m_info.m_masterHeroGuid = rc->GetSelectedHeroGuid();
			m_info.m_masterHeroName = rc->GetSelectedHeroName().GetString();
			return;
		}
	}
}

/// Are all room members clicked ready in game room?
/// 게임방 내의 모든 방 멤버들의 READY 상태인지 검사합니다.
bool CGameRoom_S::IsEveryHeroPlayReady()
{
	if (m_joinedGamers.Count == 0)
		return false;

	for each(JoinedGamers::value_type i in m_joinedGamers)
	{
		CBattleClient_S* rc = i.second;
		if (m_info.m_masterHeroGuid != rc->GetSelectedHeroGuid() && rc->m_info.m_gamingState != GamingState_Ready)
			return false;
	}

	return true;
}

/// Has all gamers finished data loading process?
/// 모든 게이머가 플레이를 위한 데이터 로딩 과정이 끝났는지 검사합니다.
bool CGameRoom_S::IsEveryGamerLoadFinished()
{
	for each(JoinedGamers::value_type i in m_joinedGamers)
	{
		CBattleClient_S* rc = i.second;
		if (rc->m_info.m_gamingState == GamingState_Loading)
			return false;
	}

	return true;
}

CBattleServer::CBattleServer(void) : m_AccumTimeForCreateBullet(0)
{
	m_lastGuiUpdateTimeMs = 0;
}

CBattleServer::~CBattleServer(void)
{
	try
	{
		m_netServer->Stop();
	}
	catch (Proud::Exception e)
	{
		LogError(LoadString(IDS_STRING110) + L" : Stop Battle server");
	}

	// 상속 클래스에서 먼저 파괴해야 합니다. 왜냐하면 이들의 콜백이 이 클래스의 멤버를 접근하기 때문입니다.
	// Destory it firstly at inherit class. Because their callback accesses member of this class.
	m_dbClient.Free();
	m_farmClient.Free();
	m_netServer.Free();
}

/// Start listening of battle server.
/// 배틀 서버의 listening을 시작합니다.
void CBattleServer::Start()
{
	SetTitle(GetDescription().GetString());

	m_lobbyServerHostID = HostID_None;

	// Starts battle server
	// Battle server를 시작합니다.
	m_netServer.Attach(CNetServer::Create());
	m_netServer->SetEventSink(this);
	m_netServer->AttachProxy(&m_s2cProxy);
	m_netServer->AttachStub((BattleC2C::Stub*)this);
	m_netServer->AttachStub((BattleC2S::Stub*)this);

	// NOTE: Dynamic assgin to battle server port. Because it may run several processes.
	// NOTE: Battle server port는 동적 할당을 하도록 합니다. 여러 프로세스를 작동시키는 경우도 있기 때문입니다.
	CStartServerParameter p1;
	p1.m_protocolVersion = CSettings::GetSharedPtr()->GetBattleVersion();
	p1.m_timerCallbackContext = NULL;
	// Ontick has to use OnTick which callBack form FarmClient.
	// Ontick은 FarmClient 에서 callBack되어져 오는 OnTick을 사용합니다.
	//p1.m_timerCallbackInterval = 1;//millisecond

	//m_timer.Attach(CMilisecTimer::New(MilisecTimerType_QPC));

	// Try to connect farm server
	// Farm Server에 연결을 시도합니다..
	SetStatusText(LoadString(IDS_STRING111));

	m_farmClient.Attach(new CFarmClient);

	CFarmClientConnectParameter p2;
	p2.m_delegate = this;
	p2.m_farmName = CFarmSettings::GetSharedPtr()->m_FarmName;
	p2.m_farmClientName = m_lobbyName + String(L"- Battle Server") + Win32Guid::NewGuid().ToString().GetString();
	p2.m_farmClientType = ServerMode_Battle;
	m_farmClient->Init(p2, this, GetDescription());

	// Start DB cache client
	// DB cache client를 시작합니다.
	m_dbClient.Attach(CDbCacheClient2::New());
	CDbCacheClient2ConnectParameter p3;
	p3.m_delegate = this;
	p3.m_serverAddr = L"localhost";
	p3.m_serverPort = CFarmSettings::GetSharedPtr()->GetDbServerPort();
	p3.m_authenticationKey = CFarmSettings::GetSharedPtr()->m_dbAuthKey;


	//m_netServer->Start(p1); // 예전 코드

	try
	{
		m_netServer->Start(p1);
	}
	catch (Exception& e)
	{
		printf("Server Start Error: %s\n", e.what());
	}

	CFastArray<String> localAddresses;
	CNetUtil::GetLocalIPAddresses(localAddresses);
	m_serverAddr.m_addr = localAddresses[0];

	CFastArray<AddrPort> output;
	m_netServer->GetTcpListenerLocalAddrs(output);
	m_serverAddr.m_port = output[0].m_port;

	m_dbClient->Connect(p3);

	m_farmClient->Connect();
}

/// Processing every specific time
/// 일정 시간마다의 프로세싱
void CBattleServer::OnTick(void* context)
{
	m_farmClient->FarmClientFrameMove();

	CriticalSectionLock lock(m_cs, true);

	RemoveTooOldPendedJoinees();

	for each (BattleGameRooms::value_type i in m_gameRooms)
	{
		CGameRoom_S* room = i.second;
		FrameMove_GameRoom(room);
	}

	// 일정 시간마다 서버 GUI에 표시 내용을 갱신합니다.
	// (주의: 실제 만드는 서버는 GUI를 가질 수 없기에 이런 식으로 구현하면 곤란합니다. 그냥 참고 바람.)
	// Renew indicated detail at server GUI with ervery specific time.
	// (Note: Actually server cannot have GUI so do not realize it. Please use for only reference)

	if (GetPreciseCurrentTimeMs() - m_lastGuiUpdateTimeMs > 1000)
	{
		m_lastGuiUpdateTimeMs = GetPreciseCurrentTimeMs();

		int userCount = m_remoteClients.Count;

		String txt;
		txt.Format(L"%s : %d", LoadString(IDS_STRING118), userCount);
		SetStatusText(txt.GetString());
	}

	FrameMove();

}

/// IFarmClientDelegate 구현
/// Realize IFarmClientDelegate
void CBattleServer::OnJoinFarmServerComplete(ErrorInfo* info)
{
	if (info->m_errorType == ErrorType_Ok)
	{
		// Succeed to connect with server.
		// 서버와의 연결이 성공했습니다.
		CriticalSectionLock lock(m_ownerDlg->m_cs, true);
		// Working normally
		// 정상 작동 중
		CStringW loadString;
		loadString.LoadStringW(IDS_STRING102);
		SetStatusText(loadString);
	}
	else
	{
		// Close process because it failed to connect with farm server.
		// Farm 서버와의 연결이 실패했으므로 프로세스를 종료합니다..
		String txt;
		txt.Format(L"%s : %s", LoadString(IDS_STRING115), info->ToString());
		LogError(txt.GetString());
		m_disposed = true;
	}
}

/// Close server process targets to this gamer because disconnected with farm servr.
/// Farm 서버와의 연결이 끊어졌으므로 이 게이머 대상 서버 프로세스도 종료합니다.
void CBattleServer::OnLeaveFarmServer(ErrorType reason)
{
	m_disposed = true;

}

/// Game room creation process by request from lobby server
/// 로비 서버 요청에 의해 게임방을 생성하는 과정
CasualResult CBattleServer::CreateGameRoomByLobbyServer(CGameRoomParameter& inoutParameter)
{
	if (inoutParameter.m_guid == Guid())
	{
		// Room guid must be defined before calling this function!
		throw Exception(LoadString(IDS_STRING112));
	}

	// Parameter suitability check
	// 파라메터 적합성 체크
	if (inoutParameter.m_maxGamerCount < 1 || inoutParameter.m_maxGamerCount >= 50)
	{
		return Casual_BadGamerCount;
	}
	if (inoutParameter.m_masterHeroGuid == Guid())
	{
		return Casual_BadMasterHeroGuid;
	}

	CriticalSectionLock lock(m_cs, true);

	/* Execute game room creation request that arrived lobby server.
	NOTE: If you need to load data file (e.g: map data) for game room at here, you should do loading process before you do critical section lock.
	However it is better to stay that data in the moemry except first time because of performace.. */

	/* 로비 서버에 도착한 게임방 생성 요청을 실행합니다.
	주의: 만약 여기에서 게임방에 필요한 데이터 파일(예: 맵 데이터)를 로딩해야 하는 경우라면,
	위 critical section lock을 하기 전에 로딩 과정을 여기서 수행해야 할 것입니다.
	하지만 가능합니다면 초기 1회를 제외하고 로딩한 데이터를 메모리에 상주하는 것이 성능상 좋습니다. */
	CGameRoomPtr_S room(new CGameRoom_S);
	room->m_info = inoutParameter;

	CJoinAllowedGamerPtr newJp(new CJoinAllowedGamer);
	newJp->m_addedTimeMs = GetPreciseCurrentTimeMs();
	newJp->m_joinParameter.From(room->m_info, inoutParameter.m_masterHeroGuid);

	room->m_joinAllowedHeroGuidToObjMap.Add(inoutParameter.m_masterHeroGuid, newJp);
	room->m_info.m_serverAddr = GetServerAddr();

	ATLTRACE("생성된 게임방의 Guid = %s\n", room->m_info.m_guid.ToString().GetString());
	m_gameRooms.Add(room->m_info.m_guid, room);

	room->m_info.m_mode = RoomMode_Waiting;

	// Makes return value.
	// 리턴값을 만들어 줍니다.
	inoutParameter = room->m_info;

	return Casual_Ok;
}
CasualResult CBattleServer::JoinGameRoomByLobbyServer(const CJoinGameRoomParameter& joinParam, CGameRoomParameter& outJoinAllowedGameRoomInfo)
{
	if (joinParam.m_guid == Guid())
		throw Exception(LoadString(IDS_STRING113));

	CriticalSectionLock lock(m_cs, true);

	CGameRoomPtr_S room = GetGameRoomByGuid(joinParam.m_guid);
	if (!room)
	{
		return Casual_NoGameRoom;
	}

	if (!IsGameRoomJoinAllowed(room, joinParam))
		return Casual_NotAllowedHero;

	ASSERT(joinParam.m_joinRequestedHeroGuid != Guid());

	// Gives entering permission. It does not mean entering room right now, only give permission for entering.
	// Actual entering process is occuring when game client logon battle server.
	// 진입 허가를 내줍니다. 당장 게이머가 방에 들어가는건 아니고, 진입을 허락만 할 뿐입니다.
	// 실제 진입 절차는 게임 클라가 배틀 서버에 로그온할 때입니다.
	CJoinAllowedGamerPtr newJp(new CJoinAllowedGamer);
	newJp->m_addedTimeMs = GetPreciseCurrentTimeMs();
	newJp->m_joinParameter = joinParam;
	room->m_joinAllowedHeroGuidToObjMap.Add(joinParam.m_joinRequestedHeroGuid, newJp);

	// Make return value
	// 리턴값을 만들어 줍니다.
	outJoinAllowedGameRoomInfo = room->m_info;

	return Casual_Ok;
}
/// Handler that synchronize summarize state of other game server
/// 타 게임 서버의 개괄 상황을 동기화받는 handler
void CBattleServer::OnNotifyFarmClientInfo(const CFarmClientInfo& info)
{
	// This battle server is subordinating lobby server then save HostID of specific lobby server.
	// 이 배틀 서버가 종속할 로비 서버이면 해당 로비 서버의 HostID를 저장합니다.
	if (info.m_farmClientType == ServerMode_Lobby
		&& info.m_farmClientName.CompareNoCase(m_lobbyName) == 0)
	{
		m_lobbyServerHostID = info.m_farmClientHostID;
	}
}

int CBattleServer::GetGamerCount()
{
	CriticalSectionLock lock(m_cs, true);
	return m_remoteClients.Count;
}

Proud::NamedAddrPort CBattleServer::GetServerAddr()
{
	return m_serverAddr;
}

/// Returns lobby name that related with this farm client( game server )
/// 이 Farm 클라이언트( 즉 게임 서버 )가 연관된 로비 이름을 리턴합니다.
Proud::String CBattleServer::GetRelatedLobbyName()
{
	CriticalSectionLock lock(m_cs, true); // If it does not this, sometimes server is shutting down because extra thread change it!
	return m_lobbyName.GetString();
}


/// When battle client connet to server
/// 배틀 클라가 서버에 접속하는 순간 콜백됩니다.
void CBattleServer::OnClientJoin(CNetClientInfo *clientInfo)
{
	CriticalSectionLock lock(m_cs, true);

	// Create new client object.
	// 새 클라 객체를 생성합니다.
	CBattleClientPtr_S rc(new CBattleClient_S(this));
	rc->m_HostID = clientInfo->m_HostID;
	m_remoteClients.Add(rc->m_HostID, rc);
}

// When battle client disconnect from server
// 배틀 클라이언트가 서버와 접속을 끊는 순간 콜백됩니다.
void CBattleServer::OnClientLeave(CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment)
{
	CriticalSectionLock lock(m_cs, true);
	CBattleClientPtr_S rc = GetRemoteClientByHostID(clientInfo->m_HostID);
	if (rc == NULL)
		return;

	// Process removal at game room.
	// 게임방에서도 제거를 처리합니다.
	CGameRoomPtr_S room = rc->m_joinedGameRoom;
	if (room != NULL)
	{
		// If exiting gamer is room master then change other gamer to room master.
		// 나가는 게이머가 방장이면 다른 게이머를 방장으로 교체합니다.
		if (room->m_info.m_masterHeroGuid == rc->GetSelectedHeroGuid())
		{
			room->ChangeMasterGamerOtherThan(rc->GetGamerGuid());
		}

		// Disconnect game room object
		// 게임방 객체와의 연결을 끊습니다.
		room->m_joinedGamers.Remove(rc->m_HostID);
		room->m_info.m_gamerCount--;


		// Notify removal of gamer list to other client.
		// 타 클라에게 게이머 목록 제거를 notify합니다.
		for each (CGameRoom_S::JoinedGamers::value_type i in room->m_joinedGamers)
		{
			CBattleClient_S* otherRc = i.second;
			m_s2cProxy.HeroSlot_Disappear(otherRc->m_HostID, RmiContext::ReliableSend,
				rc->m_info.m_guid);

			// Notify changing state of game room
			// 게임방 상태 변경도 notify합니다.
			m_s2cProxy.NotifyGameRoomInfo(otherRc->m_HostID, RmiContext::ReliableSend, room->m_info);
		}

		// Notice to lobby server
		// Notice start game room to lobby server.
		// 로비 서버에 notify합니다.
		// 로비 서버에 게임방 개시를 notify합니다.
		if (room->m_joinedGamers.Count == 0)
			m_farmClient->m_c2cProxy.GameRoom_Disappear(room->m_info.m_creatorLobbyServerHostID, RmiContext::ReliableSend, room->m_info.m_guid);
		else
			m_farmClient->m_c2cProxy.GameRoom_ShowState(room->m_info.m_creatorLobbyServerHostID, RmiContext::ReliableSend, room->m_info);

		// If number of game room member become 0 then destroy it.
		// 게임방 멤버수가 0이 되면 게임방 파괴.
		if (room->m_joinedGamers.Count == 0)
			m_gameRooms.Remove(room->m_info.m_guid);
	}


	// Notify logout at Status server.
	// Status서버에도 로그아웃되었음 을 알립니다.
	if (errorInfo->m_errorType != ErrorType_Ok && comment.GetCount() <= 0)
	{
		ATLTRACE("Client has dropped.");
		//ATLTRACE("Client가 튕겼습니다.");
	}
	const BYTE* DisConnectMessage = comment.GetData();
	if (DisConnectMessage == NULL || *DisConnectMessage == DisConnectLogout)
	{
		ATLTRACE("Client has dropped. comment.GetData()is NULL.");
		//ATLTRACE("Client가 튕겼습니다. comment.GetData()가 NULL입니다.");
	}

	// DB unload 처리도 합니다.
	// Also does DB unload process
	if (rc->m_dbLoadedGamerData)
	{
		m_farmClient->m_c2cProxy.UserLogOut(m_farmClient->m_StatusServerHostID, RmiContext::ReliableSend, rc->m_dbLoadedGamerData->GetRootUUID());

		m_dbClient->UnloadDataBySessionGuid(rc->m_dbLoadedGamerData->GetSessionGuid(), ByteArray());
		rc->m_dbLoadedGamerData = CLoadedData2Ptr();
	}

	// Finishing data
	// 데이터 마무리
	m_remoteClients.Remove(clientInfo->m_HostID);

	//// Destroy client object
	// 클라이언트 객체 파괴
	if (rc && rc->m_hero && room != NULL)
	{
		for each(CBattleClient_S* otherRC in rc->m_hero->m_viewers)
		{
			otherRC->m_tangibles.erase(rc->m_hero);
			m_s2cProxy.RemoteHero_Disappear(otherRC->m_HostID, RmiContext::ReliableSend, rc->m_HostID);
		}
		rc->m_hero->m_viewers.Clear();

		m_netServer->DestroyP2PGroup(rc->m_hero->m_viewersGroupID);

		room->m_gameObjects.Remove(rc->m_hero);
		delete rc->m_hero;
		rc->m_hero = NULL;
	}

	// 클라가 보고 있던 개체들의 소유주와의 관계도 끊어야 합니다. 안그러면 댕글링 혹은 누수.
	for each(CSynchEntity* t in rc->m_tangibles)
	{
		t->m_viewers.Remove(rc);
	}

	m_remoteClients.RemoveKey(clientInfo->m_HostID);
}

void CBattleServer::OnExclusiveLoadDataComplete(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	HostID remote = (HostID)args.m_tag;
	if (args.m_items.GetCount() < 1)
	{
		return;
	}

	// Are there remote object?
	// remote 객체가 있는지?
	CBattleClientPtr_S rc = GetRemoteClientByHostID(remote);
	if (rc == NULL)
	{
		return;
	}

	ErrorType result = args.m_items[0].m_reason;
	// 요청이 실패한 경우
	if (result != ErrorType_Ok)
	{
		m_s2cProxy.NotifyNextLogonFailed(remote, RmiContext::ReliableSend, result, Casual_Ok);
	}
	// 요청이 성공한 경우
	else
	{
		Guid selectedHeroGuid;
		CPropNodePtr selectedHero;
		// Check gamer data has entered correctly
		// 게이머 데이터가 잘 들어갔는가 체크
		selectedHeroGuid = args.m_items[0].m_loadedData->GetRootNode()->Fields[L"SelectedHeroGuid"];
		selectedHero = args.m_items[0].m_loadedData->GetNode(selectedHeroGuid);
		if (selectedHeroGuid == Guid() || selectedHero == NULL)
		{
			CStringW txt = LoadString(IDS_STRING114);
			m_s2cProxy.ShowError(rc->m_HostID, RmiContext::ReliableSend, txt.GetString());
			m_dbClient->UnloadDataBySessionGuid(args.m_items[0].m_sessionGuid);
			return;
		}

		Guid roomGuid;
		roomGuid = rc->m_joinIntendedGameRoomGuid;
		rc->m_joinIntendedGameRoomGuid = Guid();

		// Check avaliable of entering room
		// 입장할 방의 존재 확인
		CGameRoomPtr_S room = GetGameRoomByGuid(roomGuid);
		if (!room)
		{
			m_s2cProxy.NotifyNextLogonFailed(rc->m_HostID, RmiContext::ReliableSend, ErrorType_Unexpected, Casual_NoGameRoom);
			m_dbClient->UnloadDataBySessionGuid(args.m_items[0].m_sessionGuid);
			return;
		}

		// Check entering permission by room master
		// 입장 허가가 방장에 의해 났는가 확인
		CJoinAllowedGamerPtr jp;
		if (!room->m_joinAllowedHeroGuidToObjMap.TryGetValue(CCasualGamer::GetSelectedHeroGuid(args.m_items[0].m_loadedData), jp))
		{
			m_s2cProxy.NotifyNextLogonFailed(rc->m_HostID, RmiContext::ReliableSend, ErrorType_Unexpected, Casual_NotAllowedHero);
			m_dbClient->UnloadDataBySessionGuid(args.m_items[0].m_sessionGuid);
			return;
		}

		if (!IsGameRoomJoinAllowed(room, jp->m_joinParameter))
		{
			m_s2cProxy.NotifyNextLogonFailed(rc->m_HostID, RmiContext::ReliableSend, ErrorType_Unexpected, Casual_NotAllowedHero);
			m_dbClient->UnloadDataBySessionGuid(args.m_items[0].m_sessionGuid);
			return;
		}

		if (room->m_joinedGamers.Count == 0 && room->m_info.m_masterHeroGuid != CCasualGamer::GetSelectedHeroGuid(args.m_items[0].m_loadedData))
		{
			m_s2cProxy.NotifyNextLogonFailed(rc->m_HostID, RmiContext::ReliableSend, ErrorType_Unexpected, Casual_NotAllowedHero);
			m_dbClient->UnloadDataBySessionGuid(args.m_items[0].m_sessionGuid);
			return;
		}

		// If room has started game then dine it (TODO: If game allowes join users between game play, please modify it.)
		// 방이 게임을 이미 시작한 상태라면 거부 (TODO: 중도 난입 가능한 게임이라면 여기를 수정하셔도 됩니다.)
		if (room->m_info.m_mode != RoomMode_Waiting)
		{
			m_s2cProxy.NotifyNextLogonFailed(rc->m_HostID, RmiContext::ReliableSend, ErrorType_Unexpected, Casual_NotAllowedHero);
			m_dbClient->UnloadDataBySessionGuid(args.m_items[0].m_sessionGuid);
			return;
		}

		// Entery permission. Renew at Remote object.
		// 진입 허가 줍니다. Remote 객체에 갱신합니다.
		rc->m_dbLoadedGamerData = args.m_items[0].m_loadedData;

		// Hero information structure of rc.
		// rc의 hero정보도 줍니다.
		String tempname = CVariant(selectedHero->Fields[L"Name"]);
		CStringW name = tempname.GetString();
		rc->m_info.m_name = name;
		rc->m_info.m_guid = selectedHeroGuid;
		rc->m_info.m_type = selectedHero->Fields[L"HeroType"];
		rc->m_info.m_score = selectedHero->Fields[L"Sample_Score"];

		room->m_joinedGamers.Add(rc->m_HostID, rc);
		room->m_info.m_gamerCount++;
		rc->m_joinedGameRoom = room;
		rc->m_info.m_gamingState = GamingState_NotReady;

		room->m_joinAllowedHeroGuidToObjMap.Remove(CCasualGamer::GetSelectedHeroGuid(args.m_items[0].m_loadedData));

		ASSERT(room->m_info.m_serverAddr.m_addr != L"");
		ASSERT(room->m_info.m_creatorLobbyServerHostID != HostID_None);

		// Notify game room creation to lobby server.
		// 로비 서버에 게임방 개시를 notify합니다.
		if (room->m_joinedGamers.Count == 1)
			m_farmClient->m_c2cProxy.GameRoom_Appear(room->m_info.m_creatorLobbyServerHostID, RmiContext::ReliableSend, room->m_info);
		else
			m_farmClient->m_c2cProxy.GameRoom_ShowState(room->m_info.m_creatorLobbyServerHostID, RmiContext::ReliableSend, room->m_info);

		// P2P group
		// P2P 그룹
		if (room->m_joinedGamers.Count == 1)
			room->m_p2pGroupID = m_netServer->CreateP2PGroup(&rc->m_HostID, 1);
		else
			m_netServer->JoinP2PGroup(rc->m_HostID, room->m_p2pGroupID);

		// Room master
		//방장
		if (room->m_joinedGamers.Count == 1)
		{
			room->m_info.m_masterHeroGuid = CCasualGamer::GetSelectedHeroGuid(args.m_items[0].m_loadedData);
			room->m_info.m_masterHeroName = CCasualGamer::GetSelectedHeroName(args.m_items[0].m_loadedData).GetString();
		}

		m_s2cProxy.NotifyGameRoomInfo(room->m_p2pGroupID, RmiContext::ReliableSend, room->m_info);

		m_s2cProxy.NotifyNextLogonSuccess(rc->m_HostID, RmiContext::ReliableSend, args.m_items[0].m_loadedData->GetRootNode()->GetUUID(), rc->m_info);

		// Notify old member list to new member and new member appearance to old member.
		// 방의 새 멤버에게 구 멤버 리스트를, 구 멤버들에게는 새 멤버 등장을 notify합니다.
		for (CGameRoom_S::JoinedGamers::iterator it = room->m_joinedGamers.begin(); it != room->m_joinedGamers.end(); ++it)
		{
			CBattleClient_S* otherRc = it->second;
			if (otherRc != rc)
			{
				m_s2cProxy.HeroSlot_Appear(otherRc->m_HostID, RmiContext::ReliableSend, rc->m_info, rc->m_HostID);
				m_s2cProxy.HeroSlot_Appear(rc->m_HostID, RmiContext::ReliableSend, otherRc->m_info, otherRc->m_HostID);
			}
		}
		m_s2cProxy.HeroSlot_Appear(rc->m_HostID, RmiContext::ReliableSend, rc->m_info, rc->m_HostID);
	}
}

CBattleClientPtr_S CBattleServer::GetRemoteClientByDbSessionGuid(Guid dbSessionGuid)
{
	for (RemoteBattleClients::iterator i = m_remoteClients.begin();
		i != m_remoteClients.end(); i++)
	{
		CBattleClientPtr_S rc = i->second;
		if (rc->m_dbLoadedGamerData && rc->m_dbLoadedGamerData->sessionGuid == dbSessionGuid)
			return rc;
	}

	return CBattleClientPtr_S();
}

void CBattleServer::OnDataUnloadRequested(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	for (intptr_t i = 0; i < args.m_items.GetCount(); ++i)
	{
		CBattleClientPtr_S rc = GetRemoteClientByDbSessionGuid(args.m_items[i].m_sessionGuid);
		if (rc)
		{
			// Give exclusive rights.
			//독점권을 넘겨 줍니다.
			if (!m_dbClient->UnloadDataBySessionGuid(args.m_items[i].m_sessionGuid, ByteArray()))
			{
				// Exclusive rights does not pass correctly.
				// 독점권이 제대로 넘어가지 않습니다.
				CStringW logString;
				logString.LoadStringW(IDS_STRING103);
				LogError(logString);
			}

			rc->m_dbLoadedGamerData = CLoadedData2Ptr();

			// Close connection of current client because new client has joined.
			// 새로온 Client가 들어왔으므로 기존 Client의 연결을 종료합니다.
			if (args.m_items[i].m_message.FindByValue(NewEntryClient) != -1)
			{
				m_netServer->CloseConnection(rc->m_HostID);
			}
		}
	}
}

/* Coming callback when it request type (At this point, it does UnilateralUpdateData() so it cannot execute...)
  When you do requestUpdateData() then callback comes to here.*/
/*request형일때 오는 콜백(여기서는  UnilateralUpdateData()했기 때문에 실행되진 않습니다...)
  requestUpdateData()를 할시에는 여기로 콜백이 옵니다.*/
void CBattleServer::OnUpdateDataFailed(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	// Failed UpdateData. Reason:%s Comments:%s
	// UpdateData 실패. 이유:%s 코멘트:%s
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING104);
	String text;
	text.Format(loadString, ErrorInfo::TypeToString(args.m_items[0].m_reason), args.m_items[0].m_comment.GetString());
	LogError(text.GetString());
}

/* Coming callback when it request type (At this point, it does UnilateralUpdateData() so it cannot execute...)
  When you do requestUpdateData() then callback comes to here.*/
/*request형일때 오는 콜백(여기서는  UnilateralUpdateData()했기 때문에 실행되진 않습니다...)
  requestUpdateData()를 할시에는 여기로 콜백이 옵니다.*/
void CBattleServer::OnUpdateDataSuccess(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	CBattleClientPtr_S rc = GetRemoteClientBySessionGuid(args.m_items[0].m_loadedData->GetRootUUID());
	rc->m_dbLoadedGamerData = args.m_items[0].m_loadedData;
}

void CBattleServer::OnAccessError(CCallbackArgs& args)
{
	String comment;
	comment = L"AccessError : ";
	comment = comment + args.m_items[0].m_comment;
	LogError(comment.GetString());
}

void CBattleServer::NotifyUnauthedAccess(HostID clientID)
{
	m_s2cProxy.NotifyUnauthedAccess(clientID, RmiContext::ReliableSend);
}

/// Get client object with receiving HostID of client but get only for logged on object.
/// 클라이언트의 HostID를 입력받아 클라이언트 객체를 얻되 로그온한 객체에 한해서만 얻습니다.
CBattleClientPtr_S CBattleServer::GetAuthedRemoteClientByHostID(HostID clientID)
{
	CBattleClientPtr_S rc = GetRemoteClientByHostID(clientID);
	if (rc && rc->m_dbLoadedGamerData)
	{
		return rc;
	}
	return CBattleClientPtr_S();
}

/// Find out client object with loading result waiting ticket that issued during battle client requst logon to server.
/// 배틀 클라이언트가 서버에 로그온 요청을 하는 동안 발급받았던 로딩 결과 대기 번호표를 입력해서, 클라이언트 객체를 찾습니다.
CBattleClientPtr_S CBattleServer::GetRemoteClientBySessionGuid(Guid requestGuid)
{
	for (RemoteBattleClients::iterator i = m_remoteClients.begin();
		i != m_remoteClients.end(); i++)
	{
		CBattleClientPtr_S rc = i->second;
		if (rc->m_dbLoadedGamerData != NULL && rc->m_dbLoadedGamerData->GetSessionGuid() == requestGuid)
			return rc;
	}

	return CBattleClientPtr_S();
}

CGameRoomPtr_S CBattleServer::GetGameRoomByGuid(Guid roomGuid)
{
	CGameRoomPtr_S output;
	if (m_gameRooms.TryGetValue(roomGuid, output))
		return output;
	else
		return CGameRoomPtr_S();
}

CBattleClientPtr_S CBattleServer::GetRemoteClientByHostID(HostID clientID)
{
	CBattleClientPtr_S output;
	if (m_remoteClients.TryGetValue(clientID, output))
		return output;
	else
		return CBattleClientPtr_S();
}

/// If client reserved to enter lobby server is not coming for a long time then remove it from reserve information.
/// 로비 서버에 의해 게임방 들어가기 예약이 되어있던 클라이언트가, 정작 너무 오랫동안 들어오지 않으면 예약 정보에서 제거합니다.
void CBattleServer::RemoveTooOldPendedJoinees()
{
	// If it stays too long items in CBattleGameRoom::m_joinAllowedGamerGuids then automatically remove it
	// CBattleGameRoom::m_joinAllowedGamerGuids에 있는 것들은 너무 오래되면 자동 제거
	for (BattleGameRooms::iterator i = m_gameRooms.begin(); i != m_gameRooms.end(); i++)
	{
		CGameRoomPtr_S room = i->second;
		CFastArray<Guid> deleteList;
		for (CGameRoom_S::JoinAllowedHeroGuidToObjMap::iterator j = room->m_joinAllowedHeroGuidToObjMap.begin();
			j != room->m_joinAllowedHeroGuidToObjMap.end(); j++)
		{
			CJoinAllowedGamerPtr jp = j->second;
			if (GetPreciseCurrentTimeMs() - jp->m_addedTimeMs > BanTooOldAllowanceInterval)
			{
				deleteList.Add(j->first);
			}
		}

		for (int j = 0; j < deleteList.Count; j++)
		{
			room->m_joinAllowedHeroGuidToObjMap.Remove(deleteList[j]);
		}
	}
}

/// Check enter permission of game client that request entering game room.
/// 게임방 진입을 요청하는 게임 클라이언트의 진입 허가를 체크합니다.
bool CBattleServer::IsGameRoomJoinAllowed(CGameRoomPtr_S room, const CJoinGameRoomParameter &joinParam)
{
	// TODO: 추가로 방 진입 조건을 수정하고자 합니다.면 여기를 손대자.
	// TODO: Modify here, if you want to modify room enterance condition.

	if ((int)room->m_joinedGamers.Count >= room->m_info.m_maxGamerCount)
		return false;
	if (room->m_info.m_mode != RoomMode_Waiting)
		return false;

	return true;
}

void CBattleServer::FrameMove_GameRoom(CGameRoom_S* gameRoom)
{
	switch (gameRoom->m_info.m_mode)
	{
	case RoomMode_Loading:
		FrameMove_GameRoom_LoadingMode(gameRoom);
		break;
	case RoomMode_Playing:
		FrameMove_GameRoom_PlayingMode(gameRoom);
		break;
	}
}

void CBattleServer::PurgeLoadIncompleteGamers(CGameRoom_S* gameRoom)
{
	for each(CGameRoom_S::JoinedGamers::value_type i in gameRoom->m_joinedGamers)
	{
		CBattleClient_S* rc = i.second;
		if (rc->m_info.m_gamingState == GamingState_Loading)
		{
			m_netServer->CloseConnection(rc->m_HostID);
		}
	}
}

void CBattleServer::FrameMove_GameRoom_LoadingMode(CGameRoom_S* gameRoom)
{
	// All gamer has finished loading then change to play mode.
	// 모든 게이머가 로딩 완료를 했으면 플레이 모드로 전환합니다.
	if (gameRoom->IsEveryGamerLoadFinished())
	{
		GameRoom_GotoPlayMode(gameRoom);

	}
	else if (GetPreciseCurrentTimeMs() - gameRoom->m_loadingModeStartedTimeMs > gameRoom->m_clientPlayLoadingTimeoutMs)
	{
		// If it goes a couple of minutes then change play mode forcibly. Kick out unfinished gamer.
		// 몇 분이 지났으면 강제로 플레이 모드로 전환 합니다. 이때 로딩 미완료 게이머는 추방합니다.
		PurgeLoadIncompleteGamers(gameRoom);
		GameRoom_GotoPlayMode(gameRoom);
	}
}

void CBattleServer::GameRoom_GotoPlayMode(CGameRoom_S* gameRoom)
{
	if (gameRoom->m_info.m_mode != RoomMode_Playing)
	{
		gameRoom->m_info.m_mode = RoomMode_Playing;

		// 방 멤버들에게 변화를 notify합니다.
		for each(CGameRoom_S::JoinedGamers::value_type i in gameRoom->m_joinedGamers)
		{
			CBattleClient_S* otherRC = i.second;
			otherRC->m_info.m_gamingState = GamingState_Playing;
			m_s2cProxy.HeroSlot_ShowState(gameRoom->m_p2pGroupID, RmiContext::ReliableSend, otherRC->m_info);
		}

		// Notice to lobby server.
		// 로비 서버에도 notify합니다.
		m_farmClient->m_c2cProxy.GameRoom_ShowState(gameRoom->m_info.m_creatorLobbyServerHostID, RmiContext::ReliableSend, gameRoom->m_info);

		// Order play starting to room member.
		// room member들에게 플레이 시작을 지시합니다.
		m_s2cProxy.GotoPlayMode(gameRoom->m_p2pGroupID, RmiContext::ReliableSend);

	}
}

void CBattleServer::BeforeRmiInvocation(LPCWSTR RMIName)
{
	//ATLTRACE("RMI: %s\n",CW2A(RMIName));
}

void CBattleServer::GameRoom_GotoWaitingMode(CGameRoomPtr_S gameRoom)
{
	if (gameRoom->m_info.m_mode != RoomMode_Waiting)
	{
		// Change game room mode to standby mode forcibly. 
		// 게임방 모드를 대기 모드로 강제 전환합니다.
		gameRoom->m_info.m_mode = RoomMode_Waiting;

		for (CGameRoom_S::JoinedGamers::iterator i = gameRoom->m_joinedGamers.begin();
			i != gameRoom->m_joinedGamers.end(); i++)
		{
			CBattleClient_S* otherRC = i->second;
			otherRC->m_info.m_gamingState = GamingState_NotReady;
			m_s2cProxy.HeroSlot_ShowState(gameRoom->m_p2pGroupID, RmiContext::ReliableSend, otherRC->m_info);
		}

		// Notice to lobby server.
		// 로비 서버에도 notify합니다.
		m_farmClient->m_c2cProxy.GameRoom_ShowState(gameRoom->m_info.m_creatorLobbyServerHostID, RmiContext::ReliableSend, gameRoom->m_info);

		// Order forced move to waiting room mode at room members
		// room member들에게 대기실 모드로 강제 이동을 지시합니다.
		m_s2cProxy.GotoWaitingMode(gameRoom->m_p2pGroupID, RmiContext::ReliableSend);
	}
}


/************************************* Error processing *************************************/
/************************************* 오류 처리 *************************************/
void CBattleServer::OnError(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}

void CBattleServer::OnWarning(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}


void CBattleServer::OnInformation(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}

void CBattleServer::OnException(const Proud::Exception &e)
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()).GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}

DEFRMI_BattleC2S_RequestNextLogon(CBattleServer)
{
	CriticalSectionLock lock(m_cs, true);

	// Find out client who did not authentication yet.
	// 아직 인증하지 않은 클라 객체를 찾습니다.
	CBattleClientPtr_S rc = GetRemoteClientByHostID(remote);
	if (rc == NULL || rc->m_dbLoadedGamerData != NULL)
	{
		ATLTRACE("%s rc = NULL", __FUNCTION__);
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Empty guid is dine
	// 빈 guid는 거부
	if (credential == Guid())
	{
		ATLTRACE("%s Gamerguid == Guid()", __FUNCTION__);
		m_s2cProxy.NotifyNextLogonFailed(remote, RmiContext::ReliableSend, ErrorType_Unexpected, Casual_Unexpected);
		return true;
	}

	// Even it logged on request confirmation to state server.
	// 스탯 서버에게, 로그온을 해도 확인을 요청합니다.
	m_farmClient->m_c2cProxy.RequestCheckCredential(m_farmClient->m_StatusServerHostID, RmiContext::ReliableSend, credential, remote, GetDescription());

	// Keep value which uses when it succeed to logon.
	// 로그온 성공시 사용될 값을 여기에 미리 보관 합니다.
	rc->m_joinIntendedGameRoomGuid = roomGuid;

	ATLTRACE("%s\n", __FUNCTION__);

	return true;

}


// It calls when it try to next logon but credential does not match.
// next logon을 하려고 했으나 credential이 안 맞을 때 호출됩니다.
void CBattleServer::NotifyCheckCredentialFail(HostID userHostID)
{
	// Find out client who did not authentication yet.
	// 아직 인증하지 않은 클라 객체를 찾습니다..
	CBattleClientPtr_S rc = GetRemoteClientByHostID(userHostID);
	if (!rc || rc->m_HostID == HostID_None)
	{
		return;
	}

	ATLTRACE("%s: Credential authentication failed\n", __FUNCTION__);
	//ATLTRACE("%s: Credential 인증 실패\n",__FUNCTION__); 
	m_remoteClients.Remove(userHostID);
	m_s2cProxy.NotifyNextLogonFailed(userHostID, RmiContext::ReliableSend, ErrorType_Unexpected, Casual_CheckCredentialFail);
}

void CBattleServer::NotifyCheckCredentialSuccess(const Guid &gamerGuid, HostID userHostID)
{
	CBattleClientPtr_S rc = GetRemoteClientByHostID(userHostID);
	if (rc == NULL)
		return;

	// Starts gamer information loading (include account authentication) to DB cache server by logon attemption of battle client.
	// Even it finished loading, it still has step called game room entering process.
	// battle client의 로그온 시도에 따라 DB cache server로 게이머 정보 로딩(계정 인증 포함)을 시작합니다.
	// 로딩이 끝나더라도 게임방 진입 처리라는 관문이 남아있습니다.

	// message Setting to indicate it as new Battle Client
	// 새로운 Battle Client임을 표시하기위한 message Setting
	ByteArray message;
	message.Add(NewBattleClient);

	m_dbClient->RequestExclusiveLoadDataByGuid(L"Gamer", gamerGuid, userHostID, message);
}


// In game waiting room, if gamer is not room master then notify game play is 'READY' to server. 
// It designed request-confirm type to correct synchronization.
// 게임방 대기실에서, 게이머가 방장이 아닌 경우 게임 플레이가 '준비됐음을' 
// 서버에 notify합니다.
// 동기화를 정확히 하기 위해 request-confirm 형태로 설계되었습니다.
DEFRMI_BattleC2S_RequestToggleBattleReady(CBattleServer)
{
	CriticalSectionLock lock(m_cs, true);

	CBattleClientPtr_S rc = GetAuthedRemoteClientByHostID(remote);
	if (!rc)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// 방 객체를 찾습니다.
	CGameRoomPtr_S room = rc->m_joinedGameRoom;
	if (!room)
	{
		return true;
	}

	// If room is not waiting mode then ignore it
	// 방이 대기실 모드가 아니면 무시
	if (room->m_info.m_mode != RoomMode_Waiting)
		return true;

	// If it is room master then ignore it
	// 방장인 경우 무시
	if (room->m_info.m_masterHeroGuid == rc->GetSelectedHeroGuid())
		return true;

	// Save ready check status and notify states.
	// 레디 체크 여부를 저장하고 상태를 notify합니다..
	if (rc->m_info.m_gamingState != GamingState_Ready &&
		rc->m_info.m_gamingState != GamingState_NotReady)
		return true;

	if (rc->m_info.m_gamingState == GamingState_Ready)
		rc->m_info.m_gamingState = GamingState_NotReady;
	else
		rc->m_info.m_gamingState = GamingState_Ready;

	// Notify slot state to gamers.
	// 게이머들에게 슬롯 상태를 notify합니다..
	m_s2cProxy.HeroSlot_ShowState(room->m_p2pGroupID, RmiContext::ReliableSend,
		rc->m_info);

	return true;
}

// Sends game play request from waiting mode.
// Room master only do this reqeust.
// 대기모드인 게임방의 플레이를 시작 요청을 합니다.
// 방장만 가능한 요청입니다.
DEFRMI_BattleC2S_RequestStartPlayMode(CBattleServer)
{
	CriticalSectionLock lock(m_cs, true);

	CBattleClientPtr_S rc = GetAuthedRemoteClientByHostID(remote);
	if (!rc)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Find out room object.
	// 방 객체를 찾습니다..
	CGameRoomPtr_S room = rc->m_joinedGameRoom;
	if (!room)
	{
		m_s2cProxy.NotifyStartPlayModeFailed(remote, RmiContext::ReliableSend);
		return true;
	}

	// If it is not waiting mode then ignore it
	// 방이 대기실 모드가 아니면 무시
	if (room->m_info.m_mode != RoomMode_Waiting)
	{
		m_s2cProxy.NotifyStartPlayModeFailed(remote, RmiContext::ReliableSend);
		return true;
	}

	// If it is not room master then ignore it
	// 방장이 아닌 경우 무시
	if (room->m_info.m_masterHeroGuid != rc->GetSelectedHeroGuid())
	{
		m_s2cProxy.NotifyStartPlayModeFailed(remote, RmiContext::ReliableSend);
		return true;
	}

	// Check all room members are ready or not
	// 모든 room member들이 레디 상태인가 체크
	if (!room->IsEveryHeroPlayReady())
	{
		m_s2cProxy.NotifyStartPlayModeFailed(remote, RmiContext::ReliableSend);
		return true;
	}

	// Change mode then notice to lobby server.
	// 모드를 전환 후 로비 서버에 notify합니다..
	room->m_info.m_mode = RoomMode_Loading;
	room->m_loadingModeStartedTimeMs = GetPreciseCurrentTimeMs();

	for (CGameRoom_S::JoinedGamers::iterator i = room->m_joinedGamers.begin(); i != room->m_joinedGamers.end(); i++)
	{
		CBattleClient_S* memberRC = i->second;
		memberRC->m_info.m_gamingState = GamingState_Loading;
	}

	// Order starts loading to all gamers.
	// 모든 게이머에게 로딩 시작을 지시합니다..
	m_s2cProxy.GotoLoadingMode(room->m_p2pGroupID, RmiContext::ReliableSend);


	// 로비 서버에 게임방 상태 변환을 notify합니다..
	// Notify state changes of game room to lobby server.
	m_farmClient->m_c2cProxy.GameRoom_ShowState(room->m_info.m_creatorLobbyServerHostID, RmiContext::ReliableSend, room->m_info);

	return true;
}

// When it finished loading for play mode then send it to server.
// 플레이 모드를 위한 로딩이 완료되면 서버에 이걸 보내야 합니다..
DEFRMI_BattleC2S_NotifyLoadFinished(CBattleServer)
{
	CriticalSectionLock lock(m_cs, true);

	CBattleClientPtr_S rc = GetAuthedRemoteClientByHostID(remote);
	if (!rc)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Find out room object.
	// 방 객체를 찾습니다.
	CGameRoomPtr_S room = rc->m_joinedGameRoom;
	if (!room)
	{
		return true;
	}

	// If room is not waiting mode then ignore it
	// 방이 대기실 모드가 아니면 무시
	if (room->m_info.m_mode != RoomMode_Loading)
		return true;

	// Mark it as finished loading
	// <<All gamer's loading has finished then deciding of game start is handling at CBattleServer.FrameMove. >>
	// 로딩 완료가 됐음을 마킹합니다.
	// <<모든 게이머의 로딩이 완료되면 게임 시작을 하는 판단은 CBattleServer.FrameMove에서 합니다. >>
	if (rc->m_info.m_gamingState == GamingState_Loading)
		rc->m_info.m_gamingState = GamingState_PlayWaiting;

	return true;
}


// Request 'Return to waiting room!' to server.
// 대기실로 돌아가라!'를 서버에 요청합니다.
DEFRMI_BattleC2S_LeaveBattleRoom(CBattleServer)
{
	CriticalSectionLock lock(m_cs, true);

	CBattleClientPtr_S rc = GetAuthedRemoteClientByHostID(remote);
	if (!rc)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Find room object.
	// 방 객체를 찾습니다.
	CGameRoomPtr_S room = rc->m_joinedGameRoom;
	if (!room)
	{
		return true;
	}

	// If room is not play mode then ignore it
	// 방이 플레이 모드가 아니면 무시
	if (room->m_info.m_mode != RoomMode_Playing)
		return true;

	// Leaves mission clear log to DB.
	// Mission Clear. Room : %s
	// DB에 미션 클리어 로그를 남긴다.
	// Mission이 클리어되었습니다. Room : %s
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING105);
	String strLog;
	strLog.Format(loadString, room->m_info.m_name);
	m_farmClient->DBLogWrite(strLog);

	GameRoom_GotoWaitingMode(room);

	return true;
}

DEFRMI_BattleC2S_LocalHero_Move(CBattleServer)
{
	CriticalSectionLock lock(m_cs, true);

	CBattleClient_S* rc = GetAuthedRemoteClientByHostID(remote);
	if (!rc)
		return true;

	/* Update state of character but do not broadcast.
	Broadcast process with P2P. ProudNet can do P2P communication just after P2P grouping request this type is safe(Refer: ProudNet Introduction Document). */
	/* 캐릭의 상태를 업데이트합니다.. 하지만, 브로드캐스트는 하지 않습니다..
	브로드캐스트는 P2P로 진행되기 때문입니다. ProudNet은 P2P group 맺기 요청 직후
	즉시 P2P 통신이 가능하므로 (참고: ProudNet 소개 문서) 이런 방식이 안전합니다. */
	if (rc->m_hero)
	{
		rc->m_hero->m_yaw = yaw;
		rc->m_hero->m_position = position;
		rc->m_hero->m_velocity = velocity;
	}

	return true;
}


// Report message that sends to server when it ready connecting attemption with server and game play at client then set first starting position.
// heroName: Character name
// position: Position
// direction: Direction

// 클라에서, 서버와의 연결 시도 및 게임 플레이 준비가 되었고, 최초의 시작 위치를 설정한 후
// 서버에 보내는 신고 메시지
// heroName: 캐릭터 이름
// position: 위치
// direction: 방향
DEFRMI_BattleC2S_RequestLocalHeroSpawn(CBattleServer)
{
	CriticalSectionLock lock(m_cs, true);

	CBattleClientPtr_S newRC = GetRemoteClientByHostID(remote);

	// If state is 'Client does not exist' or 'Already noticed character creation' then ignore it.
	// 만약 '그런 클라 없음' 혹은 '이미 캐릭 생성을 notify받았음' 상태이면 무시합니다..
	if (newRC == NULL || newRC->m_hero != NULL)
		return true;

	// 방에도 들어가 있어야
	CGameRoom_S* gameRoom = newRC->m_joinedGameRoom;
	if (gameRoom == NULL)
		return true;

	CPropNodePtr hero = newRC->GetSelectedHero();
	if (hero)
	{
		LONGLONG score = hero->Fields[L"Sample_Score"];
		m_s2cProxy.NotifyHeroScore(newRC->m_HostID, RmiContext::ReliableSend, score);
	}

	// Client is ready for everything so create character object for synchronizing.
	// 클라이언트가 모든게 준비가 됐으므로, 상태를 동기화할 캐릭 객체를 생성합니다..
	CGameObject_S* newHero(new CGameObject_S);
	gameRoom->m_gameObjects.Add(newHero);
	newRC->m_hero = newHero;

	newHero->m_type = GameObjectType_Hero;
	newHero->m_position = position;
	newHero->m_yaw = yaw;
	newHero->m_name = heroName;

	// Create P2P group of viewer who watching new character.
	// Make like this because it includes at P2P group member itself.
	// 신규 캐릭을 가시하는 viewer의 P2P group을 만듭니다.
	// 반드시 자기 자신은 P2P group member에 포함되어 있으므로, 이렇게 만듭니다.
	newHero->m_viewersGroupID = m_netServer->CreateP2PGroup(&newRC->m_HostID, 1, ByteArray());
	newHero->m_viewers.Add(newRC);
	newRC->m_tangibles.Add(newHero);

	assert(newHero->m_viewersGroupID != HostID_None);

	// Server is included P2P GroupMember. Server will receive it and using for visible area filtering.
	// Server도 P2P GoupMember에 포함합니다.. 서버는 이를 받아서 가시영역 필터링에 쓸 것이니까요...
	m_netServer->JoinP2PGroup(HostID_Server, newHero->m_viewersGroupID, ByteArray());

	for (Proud::Position i = m_remoteClients.GetStartPosition(); i;)
	{
		CBattleClientPtr_S oldRC = m_remoteClients.GetNextValue(i);
		if (newRC != oldRC)
		{
			// Add other client who can watching new character to viewers list then notify appearance of new character to other client.
			// And adds to 'P2P group which meaning viewers of new character'.
			// 신규 캐릭을 가시할 수 있는 타 클라를 viewers 리스트에 추가하고, 타 클라에게 신규 캐릭 출현 notify를 합니다.
			// 그리고 '신규 캐릭의 viewers를 의미하는 P2P group'에도 추가합니다.
			if (IsHeroVisibleToClient(newRC, oldRC))
			{
				newHero->m_viewers.Add(oldRC);
				oldRC->m_tangibles.Add(newHero);

				m_netServer->JoinP2PGroup(oldRC->m_HostID, newHero->m_viewersGroupID, ByteArray());

				m_s2cProxy.RemoteHero_Appear(oldRC->m_HostID, RmiContext::ReliableSend,
					newRC->m_HostID,
					newHero->m_name, newHero->m_position, newHero->m_velocity, newHero->m_yaw);
			}

			// Adds new user to viewers list of other character for watching other user's character by new user then notify appearance of other character to new client.
			// And adds to 'P2P group which meaning viewers of new character'.
			// 신규 유저가 가시하는 타 유저들의 캐릭에 대해, 타 캐릭의 viewers 리스트에 신규 유저를 추가하고,
			// 신규 클라에게 타 캐릭의 출현 notify를 합니다. 그리고 '타 캐릭의 viewers를 의미하는 P2P group'에도 추가합니다.
			if (IsHeroVisibleToClient(oldRC, newRC))
			{
				oldRC->m_hero->m_viewers.Add(newRC);
				newRC->m_tangibles.Add(oldRC->m_hero);

				assert(newHero->m_viewersGroupID != HostID_None);
				m_netServer->JoinP2PGroup(newRC->m_HostID, oldRC->m_hero->m_viewersGroupID, ByteArray());
				m_s2cProxy.RemoteHero_Appear(newRC->m_HostID, RmiContext::ReliableSend,
					oldRC->m_HostID,
					oldRC->m_hero->m_name, oldRC->m_hero->m_position, oldRC->m_hero->m_velocity, oldRC->m_hero->m_yaw);
			}
		}
	}

	// Respond to client about P2P group that called viewers of local character.
	// 로컬 캐릭의 viewers를 지칭하는 P2P group이 뭔지를 클라에게 회답합니다.
	m_s2cProxy.NotifyLocalHeroViewersGroupID(remote, RmiContext::ReliableSend, newHero->m_viewersGroupID);

	return true;
}



DEFRMI_BattleC2S_RequestBulletSpawn(CBattleServer)
{
	CriticalSectionLock lock(m_cs, true);

	return true;
}
// Check that viewer client is watching character of heroOwner.
// heroOwner의 캐릭터를 viewer 클라이언트가 가시하는지 체크합니다.
bool CBattleServer::IsHeroVisibleToClient(CBattleClient_S* heroOwner, CBattleClient_S* viewer)
{
	// If it does not have hero even one side then consider it as invisible.
	// 한쪽이라도 hero가 없으면 가시 불능으로 간주합니다.
	if (!heroOwner->m_hero || !viewer->m_hero)
		return false;

	// It considers always visible itself.
	// 자기 자신의 캐릭은 항상 가시로 간주합니다.
	if (heroOwner == viewer)
		return true;

	// If it stays in specific area then decide it as visible.
	// 일정 반경 안에 있으면 가시합니다.
	return D3DXVec3Length(&(heroOwner->m_hero->m_position - viewer->m_hero->m_position)) < SynchAreaRange;
}

// Notify client move to lobby server during game.
//게임중 클라가 로비서버로 이동했음을 notify 받습니다.
DEFRMI_BattleC2S_NotifyGotoLobbyServer(CBattleServer)
{
	return true;
}

void CBattleServer::FrameMove_GameRoom_PlayingMode(CGameRoom_S* gameRoom)
{
	// 각 클라에 대한 처리를 합니다.
	for each(CGameRoom_S::JoinedGamers::value_type i in gameRoom->m_joinedGamers)
	{
		CBattleClient_S* rc = i.second;
		if (rc != NULL)
			rc->FrameMove();
	}
}

