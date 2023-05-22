#include "stdafx.h"

#include "../Common/SynchWorldC2C_common.cpp"
#include "../Common/SynchWorldC2S_common.cpp"
#include "../Common/SynchWorldS2C_common.cpp"

#include "../Common/SynchWorldS2C_proxy.cpp"
#include "../Common/SynchWorldC2S_stub.cpp"
#include "../Common/SynchWorldC2C_stub.cpp"

#include "Server.h"

CSynchWorldServer::CSynchWorldServer()
{

}

CSynchWorldServer::~CSynchWorldServer()
{

}

bool CSynchWorldServer::Start()
{
	// Function that if NIC is not 1 select one from those.
	// 서버측 NIC가 여러개인 경우 하나를 선택하는 기능
	CFastArray<String> serverNicList;
	CNetUtil::GetLocalIPAddresses(serverNicList);
	
	std::cout << "Please select an IP address to bind to.\n";
	std::cout << "1. Any\n";
	for(int i = 0; i < serverNicList.GetCount(); i++)
	{
		// 숫자 출력 (Any 가 있으므로 2번으로 침)
		Tprintf(_PNT("%d. %s\n"), i + 2, serverNicList[i].GetString());
	}

	int selectedNetworkInterface = 0;
	std::cin >> selectedNetworkInterface;

	if(selectedNetworkInterface <= 0 || selectedNetworkInterface - 2 > serverNicList.GetCount())
	{
		return false;
	}

	// Start server.
	// 서버를 시작한다.
	CNetServer* srv = CNetServer::Create();

	// Put event handler.
	// 이벤트 핸들러를 부착한다.
	srv->OnClientJoin = [this](CNetClientInfo *clientInfo) { OnClientJoin(clientInfo); };
	srv->OnClientLeave = [this](CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment) { OnClientLeave(clientInfo, errorInfo, comment); };
	srv->OnClientOffline = [this](CRemoteOfflineEventArgs &args) { OnClientOffline(args); };
	srv->OnClientOnline = [this](CRemoteOnlineEventArgs &args) { OnClientOnline(args); };
	srv->OnError = [this](ErrorInfo *errorInfo) { OnError(errorInfo); };
	srv->OnWarning = [this](ErrorInfo *errorInfo) { OnWarning(errorInfo); };
	srv->OnInformation = [this](ErrorInfo *errorInfo) { OnInformation(errorInfo); };
	srv->OnException = [this](const Exception &e) { OnException(e); };
	srv->OnTick = [this](void* context) { OnTick(context); };

	// Set RMI default setting between client and server.
	// 클라이언트와 서버간 RMI 초기 설정을 한다.
	srv->AttachStub((SynchWorldC2S::Stub*)this);
	srv->AttachStub((SynchWorldC2C::Stub*)this);
	srv->AttachProxy(&m_S2CProxy);

	// Start server
	// 서버를 시작한다.
	CStartServerParameter p1;
	if (selectedNetworkInterface == 1)
	{
		p1.m_localNicAddr = _PNT("0.0.0.0");
	}
	else
	{
		p1.m_localNicAddr = serverNicList[selectedNetworkInterface - 2];
	}
	p1.m_protocolVersion = g_Version;
	p1.m_tcpPorts.Add(g_ServerPort);

	// Allow to add server to P2P GroupMember.
	// 서버도  P2P GroupMember로 추가를 허락한다.
	p1.m_allowServerAsP2PGroupMember = true;

	// For processing OnTick, you need to set it up.
	// 이곳을 설정하여야 OnTick 이 처리된다.
	p1.m_timerCallbackContext = NULL;

	// Function to use OnTick feature
	// OnTick 기능을 사용하기 위한 함수
	p1.m_timerCallbackIntervalMs = 10;

	ErrorInfoPtr err;
	srv->Start(p1, err);
	if (err)
	{
		Tprintf(_PNT("Server Start Error: %s\n"), ErrorInfo::TypeToString_Eng(err->m_errorType));
	}
	m_server = srv;

	return true;
}

void CSynchWorldServer::Stop()
{
	// Close server.
	// 서버를 종료한다.
	if (m_server != NULL)
	{
		m_server->Stop();
		delete m_server;
		m_server = NULL;
	}

	m_remoteClients.Clear();
}

// Main function that processing every frames
// 매 프레임마다의 처리 메인 함수
void CSynchWorldServer::FrameMove()
{
	CriticalSectionLock lock(m_cs, true);

	// Processing for each client.
	// 각 클라에 대한 처리를 한다.
	for (Proud::Position i = m_remoteClients.GetStartPosition(); i;)
	{
		CRemoteClientPtr_S rc = m_remoteClients.GetNext(i)->m_value;

		rc->FrameMove();
	}
}

// Find out client object from client HostID.
// 클라 HostID로부터 클라 객체를 찾는다.
CRemoteClientPtr_S CSynchWorldServer::GetRemoteClient( HostID clientID )
{
	CRemoteClientPtr_S ret;
	if (!m_remoteClients.TryGetValue(clientID, ret))
		return CRemoteClientPtr_S();
	else
		return ret;
}

// Check viewer client observe character of heroOwner.
// heroOwner의 캐릭터를 viewer 클라이언트가 가시하는지 체크한다.
bool CSynchWorldServer::IsHeroVisibleToClient(CRemoteClient_S* heroOwner, CRemoteClient_S* viewer)
{
	// If it does not have hero even one side then consider it as invisible..
	// 한쪽이라도 hero가 없으면 가시 불능으로 간주한다.
	if (!heroOwner->m_hero || !viewer->m_hero)
		return false;

	// It considers always visible itself.
	// 자기 자신의 캐릭은 항상 가시로 간주한다.
	if (heroOwner == viewer)
		return true;

	// If it stays in specific area then decide it as visible.
	// 일정 반경 안에 있으면 가시한다고 판정한다.
	double length = (heroOwner->m_hero->m_position - viewer->m_hero->m_position).GetLength();
	return length < ViewportLength;
}

// Handler when client has connected
// 클라이언트가 접속을 들어왔을 때의 핸들러
void CSynchWorldServer::OnClientJoin( CNetClientInfo *clientInfo )
{
	CriticalSectionLock lock(m_cs, true);

	Tprintf(_PNT("%d client connected.\n"), clientInfo->m_HostID);

	// Create client object
	// 클라이언트 객체 생성
	CRemoteClientPtr_S rc = CRemoteClientPtr_S(new CRemoteClient_S(this));
	rc->m_hostID = clientInfo->m_HostID;

	m_remoteClients[rc->m_hostID] = rc;
}

// Handler when client has exited
// 클라이언트가 나갔을 떄의 핸들러
void CSynchWorldServer::OnClientLeave( CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment )
{
	CriticalSectionLock lock(m_cs, true);

	Tprintf(_PNT("%d client disconnected.\n"), clientInfo->m_HostID);

	// Find out client object.
	// 클라이언트 객체를 찾는다.
	CRemoteClient_S* leavingRemote = GetRemoteClient(clientInfo->m_HostID);
	if (leavingRemote && leavingRemote->m_hero)
	{
		// Cancel apply from object who watching hero with exiting client.
		// Note: Send to cancel message from server. Because client is exiting so sending cancel message is unreliable.

		// 나가는 클라가 가진 hero를 가시하는 주체들로부터 등록 해제를 한다.
		// 주의: 서버에서 해제 메시지를 보내야 한다. 클라는 나가는 입장이므로 해제 메시지를 보내는 것이 unreliable하기 때문.
		for (int i = 0;i < leavingRemote->m_hero->m_viewers.GetCount();i++)
		{
			CRemoteClient_S* otherRemote = leavingRemote->m_hero->m_viewers[i];
			m_S2CProxy.RemoteHero_Disappear(otherRemote->m_hostID, RmiContext::ReliableSend, leavingRemote->m_hostID);

			// views 의 tangible에서 나가는 hero를 삭제해야한다.
			leavingRemote->m_hero->m_viewers[i]->m_tangibles.RemoveOneByValue(leavingRemote->m_hero);
		}

		// Remove exiting client from viewers list of hero which observing exit client.
		// 나가는 클라가 가시하던 hero들의 viewers 목록에서 나가는 클라를 제거한다.
		for(int i=0;i< leavingRemote->m_tangibles.GetCount();i++)
		{
			CRemoteHero_S* otherHero = leavingRemote->m_tangibles[i];
			otherHero->m_viewers.RemoveOneByValue(leavingRemote);
		}

		// Cancel client list (P2P group) which observing character of exiting client.
		// 나가는 클라의 캐릭터를 가시하는 클라 목록(즉 P2P group)을 해제한다.
		m_server->DestroyP2PGroup(leavingRemote->m_hero->m_viewersGroupID);
	}

	// Destroy client object
	// 클라이언트 객체 파괴. CRemoteHero_S도 여기서 같이 파괴됨.
	m_remoteClients.RemoveKey(clientInfo->m_HostID);
}

void CSynchWorldServer::OnClientOffline(CRemoteOfflineEventArgs &args)
{
	Tprintf(_PNT("%d client offline.\n"), args.m_remoteHostID);
}

void CSynchWorldServer::OnClientOnline(CRemoteOnlineEventArgs &args)
{
	Tprintf(_PNT("%d client online.\n"), args.m_remoteHostID);
}

// Timer event
// 타이머 이벤트
void CSynchWorldServer::OnTick(void* context)
{
	CriticalSectionLock lock(m_cs, true);

	FrameMove();
}

void CSynchWorldServer::OnError( ErrorInfo *errorInfo )
{
	String txt;
	txt.Format(_PNT("%s occurred at %s"), errorInfo->ToString().GetString(), __FUNCTIONT__);
	//LogEvent(txt);
}

void CSynchWorldServer::OnWarning( ErrorInfo *errorInfo )
{
	String txt;
	txt.Format(_PNT("%s occurred at %s"), errorInfo->ToString().GetString(), __FUNCTIONT__);
	//LogEvent(txt);
}

void CSynchWorldServer::OnInformation( ErrorInfo *errorInfo )
{
	String txt;
	txt.Format(_PNT("%s occurred at %s"), errorInfo->ToString().GetString(), __FUNCTIONT__);
	//LogEvent(txt);
}

void CSynchWorldServer::OnException(const Exception &e)
{
	String txt;
	txt.Format(_PNT("%s occurred"), e.what());
	//LogEvent(txt);
}

// RMI handler. Please refer PIDL file for more detail.
// RMI 핸들러. 자세한 것은 PIDL 파일을 참고할 것.
DEFRMI_SynchWorldC2S_RequestLocalHeroSpawn(CSynchWorldServer)
{
	CriticalSectionLock lock(m_cs, true);

	CRemoteClientPtr_S newRC = GetRemoteClient(remote);
	// If 'No that client' or 'Character already created' state then ignore it.
	// 만약 '그런 클라 없음' 혹은 '이미 캐릭 생성을 노티받았음' 상태이면 무시한다.
	if (!newRC || newRC->m_hero)
		return true;

	// Client is ready for everything so create character object that synchronize state.
	// 클라이언트가 모든게 준비가 됐으므로, 상태를 동기화할 캐릭 객체를 생성한다.
	newRC->m_hero = CRemoteHeroPtr_S(new CRemoteHero_S);
	newRC->m_hero->m_position = position;
	newRC->m_hero->m_direction = direction;
	newRC->m_hero->m_name = heroName;

	// Create P2P group of viewer that observing new character.
	// Centainly it include P2P group member itself so create like this.

	// 신규 캐릭을 가시하는 viewer의 P2P group을 만든다.
	// 반드시 자기 자신은 P2P group member에 포함되어 있으므로, 이렇게 만든다.
	newRC->m_hero->m_viewersGroupID = m_server->CreateP2PGroup(&newRC->m_hostID, 1, ByteArray());
	newRC->m_hero->m_viewers.Add(newRC);
	newRC->m_tangibles.Add(newRC->m_hero);

	assert(newRC->m_hero->m_viewersGroupID != HostID_None);

	// Include Server to P2P GoupMember. Server use it for filtering visible area.
	// Server도 P2P GoupMember에 포함한다. 서버는 이를 받아서 가시영역 필터링에 쓸 것이니까.
	m_server->JoinP2PGroup(HostID_Server, newRC->m_hero->m_viewersGroupID, ByteArray());

	for (Proud::Position i = m_remoteClients.GetStartPosition();i;)
	{
		CRemoteClientPtr_S oldRC = m_remoteClients.GetNextValue(i);
		if (newRC != oldRC)
		{
			// Add other client who can watching new character to viewers list then notify appearance of new character to other client.
			// And adds to 'P2P group which meaning viewers of new character'.

			// 신규 캐릭을 가시할 수 있는 타 클라를 viewers 리스트에 추가하고, 타 클라에게 신규 캐릭 출현 노티를 한다.
			// 그리고 '신규 캐릭의 viewers를 의미하는 P2P group'에도 추가한다.
			if (IsHeroVisibleToClient(newRC, oldRC))
			{
				newRC->m_hero->m_viewers.Add(oldRC);
				oldRC->m_tangibles.Add(newRC->m_hero);

				m_server->JoinP2PGroup(oldRC->m_hostID, newRC->m_hero->m_viewersGroupID, ByteArray());

				m_S2CProxy.RemoteHero_Appear(oldRC->m_hostID, RmiContext::ReliableSend,
				                            newRC->m_hostID,
				                            newRC->m_hero->m_name, newRC->m_hero->m_position, newRC->m_hero->m_velocity, newRC->m_hero->m_direction);
			}

			// Adds new user to viewers list of other character for watching other user's character by new user then notify appearance of other character to new client.
			// And adds to 'P2P group which meaning viewers of new character'.

			// 신규 유저가 가시하는 타 유저들의 캐릭에 대해, 타 캐릭의 viewers 리스트에 신규 유저를 추가하고,
			// 신규 클라에게 타 캐릭의 출현 노티를 한다. 그리고 '타 캐릭의 viewers를 의미하는 P2P group'에도 추가한다.
			if (IsHeroVisibleToClient(oldRC, newRC))
			{
				oldRC->m_hero->m_viewers.Add(newRC);
				newRC->m_tangibles.Add(oldRC->m_hero);

				assert(newRC->m_hero->m_viewersGroupID != HostID_None);
				m_server->JoinP2PGroup(newRC->m_hostID, oldRC->m_hero->m_viewersGroupID, ByteArray());
				m_S2CProxy.RemoteHero_Appear(newRC->m_hostID, RmiContext::ReliableSend,
				                            oldRC->m_hostID,
				                            oldRC->m_hero->m_name, oldRC->m_hero->m_position, oldRC->m_hero->m_velocity, oldRC->m_hero->m_direction);
			}
		}
	}

	// Respond to client about P2P group that called viewers of local character.
	// 로컬 캐릭의 viewers를 지칭하는 P2P group이 뭔지를 클라에게 회답한다.
	m_S2CProxy.NotifyLocalHeroViewersGroupID(remote, RmiContext::ReliableSend, newRC->m_hero->m_viewersGroupID);

	return true;
}

// Even if it is server, it has to receive P2P_LocalHero_Move. It is for visible area filtering.
// 서버이지만 P2P_LocalHero_Move를 받아야 합니다. 가시 영역 필터링을 위해서입니다.
DEFRMI_SynchWorldC2C_P2P_LocalHero_Move(CSynchWorldServer)
{
	CriticalSectionLock lock(m_cs, true);

	CRemoteClient_S* rc = GetRemoteClient(remote);
	if (!rc)
		return true;

	/* Update state of character but do not broadcast.
	Broadcast process with P2P. ProudNet can do P2P communication just after P2P grouping request this type is safe(Refer: ProudNet Introduction Document). */

	/* 캐릭의 상태를 업데이트한다. 하지만, 브로드캐스트는 하지 않는다.
	브로드캐스트는 P2P로 진행되기 때문이다. ProudNet은 P2P group 맺기 요청 직후
	즉시 P2P 통신이 가능하므로 (참고: ProudNet 소개 문서) 이런 방식이 안전하다. */
	if (rc->m_hero)
	{
		rc->m_hero->m_direction = direction;
		rc->m_hero->m_position = position;
		rc->m_hero->m_velocity = velocity;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int main(void)
{
	CSynchWorldServer server;
	if(!server.Start())
	{
		goto out;
	}

	std::cout << "The server is running.\n";
	std::cout << "q : Shut down the server.\n";
	std::cout << "1 : Full P2P\n";
	std::cout << "2 : UDP Relay\n";
	std::cout << "3 : TCP Relay\n";

	// The main thread loop.
	// 메인 스레드 루프입니다.
	while (1)
	{
		char ch = 0;
		std::cin >> ch;

		switch (ch)
		{
			case 'q':
				goto out;

			case '1':
				printf("Changed to Full P2P!\n");
				server.m_server->SetDefaultFallbackMethod(FallbackMethod_None);
				break;

			case '2':
				printf("Changed to UDP Relay!\n");
				server.m_server->SetDefaultFallbackMethod(FallbackMethod_PeersUdpToTcp);
				break;

			case '3':
				printf("Changed to TCP Relay!\n");
				server.m_server->SetDefaultFallbackMethod(FallbackMethod_ServerUdpToTcp);
				break;
		}
	}

out:
	server.Stop();
}
