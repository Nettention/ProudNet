#include "StdAfx.h"
#include "../GCCommon/Vars.h"
#include "../GCCommon/Entry_common.cpp"
#include "../gccommon/entry_proxy.cpp"
#include "../gccommon/entry_stub.cpp"
#include ".\entryserver.h"
#include "GCServerDlg.h"



CEntryServer::CEntryServer(void)
{
	m_lastGuiUpdateTime = 0;
	m_diffCustomArg = 10;
}

CEntryServer::~CEntryServer(void)
{
	try
	{
		m_netServer->Stop();
	}
	catch (Proud::Exception e)
	{
		LogError(LoadString(IDS_STRING110) + ": Stop Entry server");
	}

	// Destory it firstly at inherit class. Because their callback accesses member of this class.
	// 상속 클래스에서 먼저 파괴해야 합니다. 왜냐하면 이들의 콜백이 이 클래스의 멤버를 접근하기 때문입니다.

	m_dbClient.Free();
	m_farmClient.Free();
	m_netServer.Free();

}

/// Starts listening.
/// listening을 시작합니다.
void CEntryServer::Start()
{
	SetTitle(GetDescription().GetString());

	// Starts entry server.
	// entry server를 시작합니다.
	m_netServer.Attach(CNetServer::Create());
	m_netServer->SetEventSink(this);
	m_netServer->AttachProxy(&m_s2cProxy);
	m_netServer->AttachStub(this);

	CStartServerParameter p1;
	p1.m_protocolVersion = CSettings::GetSharedPtr()->GetEntryVersion();
	p1.m_tcpPorts.Add(CSettings::GetSharedPtr()->GetEntryServerPort());
	p1.m_timerCallbackContext = NULL;
	// ntick has to use OnTick which callBack form FarmClient.
	// Ontick은 FarmClient 에서 callBack되어져 오는 OnTick을 사용합니다.
	//p1.m_timerCallbackInterval = 1000;

	CFastArray<String> localAddresses;
	CNetUtil::GetLocalIPAddresses(localAddresses);
	m_serverAddr.m_addr = localAddresses[0];

	/*CFastArray<AddrPort> output;
	m_netServer->GetTcpListenerLocalAddrs(output);
	m_serverAddr.m_port = output[0].m_port;*/


	// Try to connect farm server
	// farm server에 연결을 시도합니다.
	SetStatusText(LoadString(IDS_STRING111));

	m_farmClient.Attach(new CFarmClient);
	CFarmClientConnectParameter p2;
	p2.m_delegate = this;
	p2.m_farmName = (LPCWSTR)CFarmSettings::GetSharedPtr()->m_FarmName;
	p2.m_farmClientName = L"Entry Server 1";
	p2.m_farmClientType = ServerMode_Entry;
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

	try
	{
		m_netServer->Start(p1);
	}
	catch (Exception& e)
	{
		printf("Server Start Error: %s\n", e.what());
	}

	m_dbClient->Connect(p3);
	m_farmClient->Connect();


}

/// Processing every specific time
/// 일정 시간마다의 프로세싱
void CEntryServer::OnTick(void* context)
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

/// Arriving handler when this server successfully connect with farm server
/// 이 서버가 Farm 서버와의 접속이 성공하면 도착하는 handler
void CEntryServer::OnJoinFarmServerComplete(ErrorInfo* info)
{
	if (info->m_errorType == ErrorType_Ok)
	{
		// Succeed to connect with server.
		// 서버와의 연결이 성공했습니다.
		CriticalSectionLock lock(m_ownerDlg->m_cs, true);
		// "Working correctly"
		// "정상 작동 중"
		CStringW logString;
		logString.LoadStringW(IDS_STRING102);
		SetStatusText(logString);
	}
	else
	{
		// It is failed to connect with farm server so close process.
		// Farm 서버와의 연결이 실패했으므로 프로세스를 종료합니다.
		String txt;
		txt.Format(L"%s : %s", LoadString(IDS_STRING115), info->ToString().GetString());
		LogError(txt.GetString());
		m_disposed = true;

	}
}

/// Event handler when it disconnect with farm server
/// Farm 서버와의 연결 해제시 이벤트 handler
void CEntryServer::OnLeaveFarmServer(ErrorType reason)
{
	// It disconnected with farm server so cancel server process that targeted this gamer.
	// Farm 서버와의 연결이 끊어졌으므로 이 게이머 대상 서버 프로세스도 종료합니다.
	m_disposed = true;

}
/// Handler when it synchronize information of other farm client
/// 타 Farm 클라이언트의 정보를 동기화받을 때의 handler
void CEntryServer::OnNotifyFarmClientInfo(const CFarmClientInfo& info)
{
	// If it is lobby server
	// 로비 서버인 경우
	if (info.m_farmClientType == ServerMode_Lobby)
	{
		// 		ATLTRACE("Get notice about lobby server %s: Server address=%s, Number of gamers=%d\n",
		// 		ATLTRACE("로비서버 %s에 대한 notify를 받음: 서버 주소=%s, 게이머수=%d\n",
		// 			(LPCSTR)CW2A(info.m_farmClientName),(LPCSTR)CW2A(info.m_serverAddr.ToString()),
		// 			info.m_gamerCount);
	}
}



/// Find out logged on client object receving client HostID as input value.
/// 클라이언트의 HostID 값을 입력값으로 받아, 로그온한 클라 객체를 찾습니다.
CRemoteEntryClientPtr CEntryServer::GetAuthedRemoteClientByHostID(HostID clientID)
{
	CRemoteEntryClientPtr rc = GetRemoteClientByHostID(clientID);
	if (rc && rc->m_dbLoadedGamerData)
	{
		return rc;
	}
	return CRemoteEntryClientPtr();
}

/// Find out logged on client object that based on gamer Guid.
/// 게이머 Guid를 근거로 로그온한 클라이언트 객체를 찾습니다.
CRemoteEntryClientPtr CEntryServer::GetRemoteClientByGamerGuid(Guid gamerGuid)
{
	CriticalSectionLock lock(m_cs, true);
	for (RemoteEntryClients::iterator i = m_remoteClients.begin(); i != m_remoteClients.end(); i++)
	{
		CRemoteEntryClientPtr rc = i->second;
		if (rc->m_dbLoadedGamerData && rc->m_dbLoadedGamerData->GetRootUUID() == gamerGuid)
			return rc;
	}

	return CRemoteEntryClientPtr();
}

/// Find out client object that based on client HostID.
/// 클라이언트의 HostID를 근거로 클라이언트 객체를 찾습니다.
CRemoteEntryClientPtr CEntryServer::GetRemoteClientByHostID(HostID clientID)
{
	CRemoteEntryClientPtr output;
	if (m_remoteClients.TryGetValue(clientID, output))
		return output;
	else
		return CRemoteEntryClientPtr();
}

CRemoteEntryClientPtr CEntryServer::GetRemoteClientByDbSessionGuid(Guid dbSessionGuid)
{
	for (RemoteEntryClients::iterator i = m_remoteClients.begin();
		i != m_remoteClients.end(); i++)
	{
		CRemoteEntryClientPtr rc = i->second;
		if (rc != NULL && rc->m_dbLoadedGamerData != NULL)
		{
			if (rc->m_dbLoadedGamerData->GetSessionGuid() == dbSessionGuid)
				return rc;
		}
	}

	return CRemoteEntryClientPtr();
}

/// Send 'Try communication without authentication' error to game client.
/// 게임 클라이언트에 '미인증 상태로 통신을 시도했음' 에러를 보냅니다.
void CEntryServer::NotifyUnauthedAccess(HostID clientID)
{
	m_s2cProxy.NotifyUnauthedAccess(clientID, RmiContext::ReliableSend);
}


/// Prepare lobby channel list for sending to gamer.
/// 게이머에게 전송될 로비 채널 리스트를 준비합니다.
void CEntryServer::RefreshPublishedLobbyList()
{
	// List up with adding con-current user of subordinated battle servers even lobby.
	// 로비 뿐만 아니라 해당 로비에 종속된 배틀 서버들의 동접자도 합산해서 리스트 업 합니다.
	CriticalSectionLock lock(m_cs, true);
	m_publishedLobbyList.Clear();
	for (FarmClientInfoMap::iterator i = m_farmClient->m_remoteFarmClientInfoMap.begin();
		i != m_farmClient->m_remoteFarmClientInfoMap.end(); i++)
	{
		CFarmClientInfoPtr info = i->second;

		String lobbyName;
		if (info->m_farmClientType == ServerMode_Lobby)
			lobbyName = info->m_farmClientName;
		else if (info->m_farmClientType == ServerMode_Battle)
			lobbyName = info->m_relatedLobbyName;

		if (lobbyName.GetLength() > 0)
		{
			CPublishedLobbyInfoPtr newItem;
			if (!m_publishedLobbyList.TryGetValue(lobbyName, newItem))
			{
				newItem = CPublishedLobbyInfoPtr(new CPublishedLobbyInfo);
				newItem->m_name = lobbyName;
				newItem->m_gamerCount = 0;
				m_publishedLobbyList.Add(lobbyName, newItem);
			}

			newItem->m_gamerCount += info->m_gamerCount;

			if (info->m_farmClientType == ServerMode_Lobby)
				newItem->m_serverAddr = info->m_serverAddr;

		}
	}
}

// Adds to DBMS and load to DbCache when it sign in
//회원가입시 DBMS에 추가 및 DbCache에 로드
DEFRMI_EntryC2S_RequestCreateNewGamer(CEntryServer)
{
	CriticalSectionLock lock(m_cs, true);

	// Find out client object who do not authenticate yet.
	// 아직 인증하지 않은 클라 객체를 찾습니다.
	CRemoteEntryClientPtr rc = GetRemoteClientByHostID(remote);
	if (!rc || rc->m_HostID == HostID_None)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Do create and load at the same time for new user information that save at DB.
	// DB에 새로 저장될 새 사용자 정보를 생성과 동시에 로딩합니다.
	CPropNodePtr newNode = CPropNodePtr(new CPropNode(L"Gamer"));
	newNode->Fields[L"GamerID"] = Proud::CVariant(gamerID);
	newNode->Fields[L"Password"] = Proud::CVariant(Password);

	rc->m_newData = true;
	m_dbClient->RequestExclusiveLoadNewData(L"Gamer", newNode, remote);

	return true;

}

// When it returns to EntryServer from LobbyServer. 
// LobbyServer에서 EntryServer로 다시 돌아온 경우 
DEFRMI_EntryC2S_RequestReturnToEntry(CEntryServer)
{
	CriticalSectionLock lock(m_cs, true);

	// Find out client object who do not authenticate yet.
	// 아직 인증하지 않은 클라 객체를 찾습니다.
	CRemoteEntryClientPtr rc = GetRemoteClientByHostID(remote);
	if (!rc || rc->m_HostID == HostID_None)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}
	m_farmClient->m_c2cProxy.RequestCheckCredential(m_farmClient->m_StatusServerHostID, RmiContext::ReliableSend, credential, remote, GetDescription());

	return true;
}

void CEntryServer::NotifyCheckCredentialFail(HostID userHostID)
{
	// Find out client object who do not authenticate yet.
	// 아직 인증하지 않은 클라 객체를 찾습니다.
	CRemoteEntryClientPtr rc = GetRemoteClientByHostID(userHostID);
	if (!rc || rc->m_HostID == HostID_None)
	{
		return;
	}

	ATLTRACE("%s: Credential 인증 실패\n", __FUNCTION__);
	m_remoteClients.Remove(userHostID);
	m_s2cProxy.NotifyReturnToEntryFailed(userHostID, RmiContext::ReliableSend, CasualGameErrorType_InvalidCredential);
}

void CEntryServer::NotifyCheckCredentialSuccess(const Guid &gamerGuid, HostID userHostID)
{
	// Find out client object who do not authenticate yet.
	// 아직 인증하지 않은 클라 객체를 찾습니다.
	CRemoteEntryClientPtr rc = GetRemoteClientByHostID(userHostID);
	if (!rc || rc->m_HostID == HostID_None)
	{
		NotifyUnauthedAccess(userHostID);
		return;
	}

	// CustomField Setting to divide into sort of Client at OnDataUnloadRequested
	// OnDataUnloadRequested 에서 어떤 Client인지 구분하기 위한 CustomField Setting
	ByteArray message;
	message.Add(ReturnToEntry);

	m_dbClient->RequestExclusiveLoadData(L"Gamer", L"RootUUID", gamerGuid, userHostID, message);

	return;
}

// Logon that based on ID, password of user
// 사용자의 ID,password를 기반으로 하는 로그온
DEFRMI_EntryC2S_RequestFirstLogon(CEntryServer)
{
	CriticalSectionLock lock(m_cs, true);

	// Find out client object who do not authenticate yet.
	// 아직 인증하지 않은 클라 객체를 찾습니다.
	CRemoteEntryClientPtr rc = GetRemoteClientByHostID(remote);
	if (!rc || rc->m_HostID == HostID_None)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	rc->m_gamerID = gamerID;

	// Do Load becasue Gamer is alread existed.
	// Logon with ID and password
	// Gamer가 이미 존재하므로 Load 합니다.
	//예전에 가입해놓고 로그인하는경우
	if (rc->m_newData == false)
	{
		// Setting to CustomField about new joined Entry Client
		// 새로 들어온 Entry Client임을 CustomField에 Setting
		ByteArray message;
		message.Add(NewEntryClient);

		m_dbClient->RequestExclusiveLoadData(L"Gamer", L"GamerID", Proud::CVariant(gamerID), remote, message);
	}
	// Logon with Id and password (already loaded)
	//회원가입하고 로그인 하는경우 (이미 로드되어있습니다)
	else
	{
		if (rc->m_dbLoadedGamerData != NULL)
		{
			rc->m_gamerUUID = rc->m_dbLoadedGamerData->GetRootUUID();
			m_farmClient->m_c2cProxy.RequestCreateCredential(m_farmClient->m_StatusServerHostID, RmiContext::ReliableSend, rc->m_gamerUUID, rc->m_HostID);
		}
	}

	return true;
}

// Request adding new player character to server.
// 새 플레이어 캐릭터 추가를 서버에게 요청합니다.
DEFRMI_EntryC2S_RequestAddHero(CEntryServer)
{

	CriticalSectionLock lock(m_cs, true);

	CRemoteEntryClientPtr rc = GetAuthedRemoteClientByHostID(remote);

	if (!rc || !rc->m_dbLoadedGamerData)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	CPropNodePtr newNode = CPropNodePtr(new CPropNode(L"Hero"));

	String name(heroName.GetString());
	newNode->Fields[L"Name"] = Proud::CVariant(name);
	newNode->Fields[L"HeroType"] = heroType;
	newNode->Fields[L"Sample_Score"] = LONGLONG(10);

	ErrorInfoPtr outerror = ErrorInfoPtr(new ErrorInfo);
	if (!m_dbClient->BlockedAddData(rc->m_dbLoadedGamerData->RootUUID, rc->m_dbLoadedGamerData->RootUUID, newNode, 10000, outerror))
	{
		// "AddData failed reason :%s"
		// "AddData실패 이유 :%s"
		CStringW loadString;
		loadString.LoadStringW(IDS_STRING108);
		String error;
		error.Format(loadString, outerror->ToString());
		LogError(error.GetString());
		m_s2cProxy.NotifyAddHeroFailed(rc->m_HostID, RmiContext::ReliableSend, (int)outerror->m_errorType);

		return true;
	}

	// Renew information because it is upadated.
	// 업데이트 되었으므로 정보를 새로 갱신시킵니다.
	rc->m_dbLoadedGamerData = m_dbClient->GetClonedLoadedDataByUUID(rc->m_dbLoadedGamerData->GetRootUUID());

	rc->m_dbLoadedGamerData->GetRootNode()->Fields[L"SelectedHeroGuid"] = newNode->GetUUID();
	m_dbClient->UnilateralRecursiveUpdateData(rc->m_dbLoadedGamerData, true);// Upadte immediatly  //즉시 업데이트

	m_s2cProxy.NotifyAddHeroSuccess(rc->m_HostID, RmiContext::ReliableSend);

	return true;
}

// Sends play character has selected which joining to lobby together.
// 로비에 갖고 들어갈 플레이어 캐릭터를 선택했음을 서버에 보냅니다.
DEFRMI_EntryC2S_RequestSelectHero(CEntryServer)
{
	CriticalSectionLock lock(m_cs, true);


	CRemoteEntryClientPtr rc = GetAuthedRemoteClientByHostID(remote);

	if (!rc || !rc->m_dbLoadedGamerData)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Check suitablility of hero guid
	// hero guid의 적합성 체크
	if (!rc->IsValidHeroGuid(heroGuid))
	{
		m_s2cProxy.NotifySelectHeroFailed(remote, RmiContext::ReliableSend, CasualGameErrorType_InvalidHeroName);
		return true;
	}

	// Write hero selection change to DBMS.
	// hero 선택 변경을 DBMS에 기록합니다.
	CPropNodePtr temp_gamer = rc->m_dbLoadedGamerData->GetRootNode();
	temp_gamer->Fields[L"SelectedHeroGuid"] = heroGuid;
	ErrorInfoPtr outerror = ErrorInfoPtr(new ErrorInfo);
	m_dbClient->BlockedUpdateData(temp_gamer, 30000, outerror);

	// Find out hero which is same as guid came to parameter from low grade nodes of gamer.
	// Gamer의 하위 노드들을 돌면서 파라미터로 넘어온 guid와 같은 hero찾습니다.
	CPropNode* hero_child = rc->m_dbLoadedGamerData->GetRootNode()->GetChild();
	CStringW heroname = L"";
	while (hero_child)
	{
		String comp = hero_child->TypeName;
		comp.MakeUpper();
		if (comp.Compare(L"HERO") == 0)
		{
			String temp = CVariant(hero_child->Fields[L"Name"]);
			heroname = temp.GetString();
			Guid guid = hero_child->GetUUID();
			if (guid == heroGuid)
				break;

		}
		hero_child = hero_child->Sibling;
	}


	m_s2cProxy.NotifySelectHeroSuccess(remote, RmiContext::ReliableSend, heroname);

	return true;
}

// Request local player character list.
// 로컬 플레이어 캐릭터 리스트를 요청합니다.
DEFRMI_EntryC2S_RequestHeroSlots(CEntryServer)
{
	CriticalSectionLock lock(m_cs, true);

	CRemoteEntryClientPtr rc = GetAuthedRemoteClientByHostID(remote);

	if (!rc || !rc->m_dbLoadedGamerData)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Sends hero list.
	// hero list를 보냅니다.
	m_s2cProxy.HeroList_Begin(remote, RmiContext::ReliableSend);

	// Sends HERO node to Client from low grade node of Gamer.	
	// Gamer의 하위 노드들을 돌면서 HERO 노드를 찾아 Client에게 보냅니다.	
	CPropNode* hero_child = rc->m_dbLoadedGamerData->GetRootNode()->GetChild();

	while (hero_child)
	{
		String comp = hero_child->TypeName;
		comp.MakeUpper();
		if (comp.Compare(L"HERO") == 0)
		{
			String temp = CVariant(hero_child->Fields[L"Name"]);
			CStringW name = temp.GetString();

			Guid guid = hero_child->GetUUID();

			int herotype = hero_child->GetField(L"HeroType");
			LONGLONG samplescore = hero_child->Fields[L"Sample_Score"];

			m_s2cProxy.HeroList_Add(remote, RmiContext::ReliableSend, name, guid, herotype, samplescore);
		}
		hero_child = hero_child->Sibling;
	}

	// End of hero list sedning
	// hero list 보내기 종료
	m_s2cProxy.HeroList_End(remote, RmiContext::ReliableSend);

	// Notify selected hero in the past by gamer.
	// 게이머가 과거 선택했던 hero가 뭔지를 notify합니다.
	Guid lastHeroSelection;

	if (!rc->m_dbLoadedGamerData->GetRootNode()->Fields[L"SelectedHeroGuid"].IsNull())
	{
		lastHeroSelection = rc->m_dbLoadedGamerData->GetRootNode()->Fields[L"SelectedHeroGuid"];
	}

	m_s2cProxy.NotifySelectedHero(remote, RmiContext::ReliableSend, lastHeroSelection);

	return true;
}

// Request removal of old player character to server.
// It has done immediatly.
// 구 플레이어 캐릭터 제거를 서버에게 요청합니다.
// 이 과정은 즉시 완료됩니다.
DEFRMI_EntryC2S_RequestRemoveHero(CEntryServer)
{

	// Process player character removal request from client.
	// 클라에서 온 플레이어 캐릭터 제거 요청을 처리합니다.
	CriticalSectionLock lock(m_cs, true);

	CRemoteEntryClientPtr rc = GetAuthedRemoteClientByHostID(remote);

	if (!rc || !rc->m_dbLoadedGamerData)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	// Order to remove hero to DB cache server.
	// hero 제거를 DB cache server에 지시합니다.
	ErrorInfoPtr outerror = ErrorInfoPtr(new ErrorInfo);
	if (!m_dbClient->BlockedRemoveData(rc->m_dbLoadedGamerData->GetRootNode()->GetUUID(), heroGuid, 30000, outerror))
		LogError(L"RemoveHero is failed");

	// Initialize SelectedHeroGuid of gamertable
	//게이머테이블의 SelectedHeroGuid를 초기화
	CPropNodePtr temp_gamer = rc->m_dbLoadedGamerData->GetRootNode();
	temp_gamer->Fields[L"SelectedHeroGuid"] = Guid();
	m_dbClient->BlockedUpdateData(temp_gamer, 30000, outerror);

	// Remove hero then refresh gamer data.
	// hero 제거를 한 후의 gamer data를 리프레쉬합니다.
	rc->m_dbLoadedGamerData = m_dbClient->GetClonedLoadedDataBySessionGuid(rc->m_dbLoadedGamerData->sessionGuid);

	m_s2cProxy.NotifyRemoveHeroSuccess(remote, RmiContext::ReliableSend);
	return true;
}

// Requset enterable lobby channel to server.
// 들어갈 수 있는 로비 채널 리스트를 서버에게 요청합니다.
DEFRMI_EntryC2S_RequestLobbyList(CEntryServer)
{
	ATLTRACE("%s\n", __FUNCTION__);
	CriticalSectionLock lock(m_cs, true);

	CRemoteEntryClientPtr rc = GetAuthedRemoteClientByHostID(remote);

	if (!rc || !rc->m_dbLoadedGamerData)
	{
		NotifyUnauthedAccess(remote);
		return true;
	}

	RefreshPublishedLobbyList();

	// Send lobby channel list to client.
	// 로비 채널 리스트를 클라에게 보냅니다.
	m_s2cProxy.LobbyList_Begin(remote, RmiContext::ReliableSend);

	for (PublishedLobbyInfoList::iterator i = m_publishedLobbyList.begin();
		i != m_publishedLobbyList.end(); i++)
	{
		CPublishedLobbyInfoPtr info = i->second;
		m_s2cProxy.LobbyList_Add(remote, RmiContext::ReliableSend,
			info->m_name.GetString(),
			info->m_serverAddr,
			info->m_gamerCount);
	}
	m_s2cProxy.LobbyList_End(remote, RmiContext::ReliableSend);

	return true;
}

void CEntryServer::OnExclusiveLoadDataComplete(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	HostID remote = (HostID)args.m_tag;
	if (args.m_items.GetCount() < 1)
	{
		return;
	}

	CRemoteEntryClientPtr rc = GetRemoteClientByHostID(remote);
	if (!rc)
	{
		return;
	}

	ErrorType result = args.m_items[0].m_reason;
	// 요청이 실패한 경우
	if (result != ErrorType_Ok)
	{

		// When it does Exculsive Load New Data
		// Exculsive Load New Data를 한경우
		if (rc->m_newData)
		{
			m_s2cProxy.NotifyCreateNewGamerFailed(remote, RmiContext::ReliableSend, (int)result);
		}
		// When it does Exculsive Load
		// Exculsive Load를 한경우
		else
		{
			m_s2cProxy.NotifyFirstLogonFailed(remote, RmiContext::ReliableSend, (int)result);
		}

	}
	// 요청이 성공한 경우
	else
	{
		// Register
		//회원가입하는상황
		if (rc->m_newData)
		{
			// If you use this note then it logon instantly.
			// 이 부분의 주석을 풀면 바로 로그인됩니다.
			// m_s2cProxy.NotifyLoadNewGamerSuccess(rc->m_HostID,RmiContext::ReliableSend,args.m_loadedData->RootUUID);
		}
		// First logon with loaded data
		//로드된 데이터로 처음 로그인하는상황
		else
		{
			rc->m_gamerUUID = args.m_items[0].m_rootUUID;
			m_farmClient->m_c2cProxy.RequestCreateCredential(m_farmClient->m_StatusServerHostID, RmiContext::ReliableSend, rc->m_gamerUUID, rc->m_HostID);
		}

		// Renewal data
		// 데이터 새로 갱신
		rc->m_dbLoadedGamerData = args.m_items[0].m_loadedData;
		ASSERT(rc->m_dbLoadedGamerData->sessionGuid != Guid());
	}
}

// Realize handling routine when exclusive loading is disappeared
//독점 로딩 증발시에 대한 handling 루틴구현
void CEntryServer::OnDataUnloadRequested(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	for (intptr_t i = 0; i < args.m_items.GetCount(); ++i)
	{
		CRemoteEntryClient* rc = GetRemoteClientByDbSessionGuid(args.m_items[i].m_sessionGuid);
		if (rc == NULL)
			return;

		// Pass exclusive rights to lobby server or farm client of other client...
		//로비서버에게 혹은 다른클라이언트의 Farm클라이언트에게 독점권을 넘겨 줍니다...
		if (!m_dbClient->UnloadDataBySessionGuid(rc->m_dbLoadedGamerData->sessionGuid, ByteArray()))
		{
			// "L"Exclusive rights do not pass correctly""
			// "L"독점권이 제대로 넘어가지 않았습니다""
			CStringW logString;
			logString.LoadStringW(IDS_STRING103);
			LogError(logString);
		}

		rc->m_dbLoadedGamerData = CLoadedData2Ptr();

		// 새로온 Client가 들어왔으므로 기존 Client의 연결을 종료합니다.
		if (args.m_items[i].m_message.FindByValue(NewEntryClient) != -1)
		{
			m_netServer->CloseConnection(rc->m_HostID);
		}
	}
}

void CEntryServer::OnUpdateDataFailed(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	// "Failed UpdateData. Reason:%s Comments:%s"
	// "UpdateData 실패. 이유:%s 코멘트:%s"
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING104);
	String text;
	text.Format(loadString, ErrorInfo::TypeToString(args.m_items[0].m_reason), args.m_items[0].m_comment.GetString());
	LogError(text.GetString());

}

void CEntryServer::OnUpdateDataSuccess(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);

	CRemoteEntryClientPtr rc = GetRemoteClientByGamerGuid(args.m_items[0].m_loadedData->GetRootUUID());
	rc->m_dbLoadedGamerData = args.m_items[0].m_loadedData;
}

void CEntryServer::OnAccessError(CCallbackArgs& args)
{
	CriticalSectionLock lock(m_cs, true);
	String comment;
	comment = L"AccessError : ";
	comment = comment + args.m_items[0].m_comment;
	LogError(comment.GetString());

}


// Make decision whether Hero is available or not.
//Hero가 유효한지 판단합니다.
bool CRemoteEntryClient::IsValidHeroGuid(Guid heroGuid)
{
	if (m_dbLoadedGamerData == NULL)
		return false;

	CPropNodePtr valid_hero = m_dbLoadedGamerData->GetNode(heroGuid);
	String comp = valid_hero->TypeName;
	comp.MakeUpper();
	if (valid_hero && (comp.Compare(L"HERO") == 0))
	{
		return true;
	}

	return false;
}

/************************************* Error Processing *************************************/
/************************************* 오류 처리 *************************************/
void CEntryServer::OnError(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogError(txt.GetString());
}

void CEntryServer::OnWarning(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}


void CEntryServer::OnInformation(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}

void CEntryServer::OnException(const Proud::Exception &e)
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()).GetString(), __FUNCTIONW__);
	LogError(txt.GetString());
}

/// When game client connect
/// 게임 클라의 접속이 들어오는 순간
void CEntryServer::OnClientJoin(CNetClientInfo *clientInfo)
{
	CriticalSectionLock lock(m_cs, true);
	// Creates new client object.
	// 새 클라 객체를 생성합니다.
	CRemoteEntryClientPtr rc(new CRemoteEntryClient);
	rc->m_HostID = clientInfo->m_HostID;
	rc->m_newData = false;
	m_remoteClients.Add(rc->m_HostID, rc);
}

/// When game client disconnect
/// 게임 클라가 접속 해제시
void CEntryServer::OnClientLeave(CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment)
{
	CriticalSectionLock lock(m_cs, true);
	// Destroy old client object.
	// 구 클라 객체를 파괴합니다.
	CRemoteEntryClientPtr rc = GetRemoteClientByHostID(clientInfo->m_HostID);
	if (rc != NULL)
	{
		// Do DB unload process.
		// DB unload 처리도 합니다.
		if (rc->m_dbLoadedGamerData)
		{
			m_farmClient->m_c2cProxy.UserLogOut(m_farmClient->m_StatusServerHostID, RmiContext::ReliableSend, rc->m_dbLoadedGamerData->GetRootUUID());
			m_dbClient->UnloadDataBySessionGuid(rc->m_dbLoadedGamerData->sessionGuid, ByteArray());

		}

		// Finishing data
		// 데이터 마무리
		m_remoteClients.Remove(rc->m_HostID);
	}
}

void CEntryServer::NotifyCreatedCredential(Guid credential, HostID userHostID)
{
	CRemoteEntryClientPtr rc = GetRemoteClientByHostID(userHostID);
	if (rc)
	{
		RmiContext rmi = RmiContext::ReliableSend;
		rmi.m_encryptMode = EM_Secure;
		m_s2cProxy.NotifyFirstLogonSuccess(rc->m_HostID, rmi, rc->m_gamerUUID, credential);
	}
}

