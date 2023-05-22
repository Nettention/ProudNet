#pragma once

#include "../../../include/ProudNetServer.h"

#include "../Common/Common.h"
#include "../Common/SynchWorldC2S_stub.h"
#include "../Common/SynchWorldS2C_proxy.h"
#include "../Common/SynchWorldC2C_stub.h"

#include "RemoteClient.h"

class CSynchWorldServer : public SynchWorldC2S::Stub, public SynchWorldC2C::Stub
{
public:
	CFastMap<HostID, CRemoteClientPtr_S> m_remoteClients;

	// ProudNet server instance
	// ProudNet 서버 인스턴스
	CNetServer* m_server;

	// critical section that protect server memory
	// 서버 메모리를 보호하는 critical section
	CriticalSection m_cs;

	// Proxy of RMI that send to client from server
	// 서버에서 클라로 보내는 RMI의 프록시
	SynchWorldS2C::Proxy m_S2CProxy;

public:
	CSynchWorldServer();
	~CSynchWorldServer();

public:
	void FrameMove();
	bool Start();
	void Stop();

public:
	CRemoteClientPtr_S GetRemoteClient(HostID clientID);
	bool IsHeroVisibleToClient(CRemoteClient_S* heroOwner, CRemoteClient_S* viewer);

public:
	/* NetServer event functions */
	void OnClientJoin(CNetClientInfo *clientInfo);
	void OnClientLeave(CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment);
	void OnClientOffline(CRemoteOfflineEventArgs &args);
	void OnClientOnline(CRemoteOnlineEventArgs &args);
	void OnError(ErrorInfo *errorInfo);
	void OnWarning(ErrorInfo *errorInfo);
	void OnInformation(ErrorInfo *errorInfo);
	void OnException(const Exception &e);
	void OnTick(void* context);

public:
	// Declaratives of RMI stub function
	// RMI stub 함수 선언부
	DECRMI_SynchWorldC2S_RequestLocalHeroSpawn;

	// P2P group messaging that server will receive and process
	// 서버도 받아 처리할 P2P 그룹 메시징
	DECRMI_SynchWorldC2C_P2P_LocalHero_Move;
};
