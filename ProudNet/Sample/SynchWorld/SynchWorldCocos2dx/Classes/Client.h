﻿#pragma once

#include "../../../../include/ProudNetClient.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace Proud;

#include "../Common/Common.h"
#include "../Common/SynchWorldC2S_proxy.h"
#include "../Common/SynchWorldS2C_stub.h"
#include "../Common/SynchWorldC2C_proxy.h"
#include "../Common/SynchWorldC2C_stub.h"
#include "LocalHero.h"
#include "RemoteHero_C.h"

/* 1개의 클라이언트 객체
로컬 캐릭 뿐만 아니라 동기화되는 원격 캐릭들의 목록도 가진다.
즉, 완전한 1개의 클라 역할을 한다.

이 샘플에서는 이 객체가 여럿 존재하지만, 그것은 여러 더미 클라를 두어 테스트하기 위한 목적일 뿐,
실제 게임에서는 1개의 클라만 있으면 된다. */

/* 1 client object
It owns list of synchronizing remote character even local character.
It does role as 1 perfect client.

In this sample, there is several existed this object but that is only for testing purpose with dummy client.
So you need only 1 client at actual game. */
class CClient : public INetClientEvent, public SynchWorldS2C::Stub, public SynchWorldC2C::Stub
{
public:
	// ProudNet 클라이언트 모듈
	// ProudNet client module
	CNetClient* m_netClient;
	// 이 값이 세팅되면, 곧 이 클라이언트는 파괴됨을 의미한다.
	// When this value is set, soon this client is destroyed.
	bool m_markDisposed;
	// 클라이언트에게 배정된 일련번호
	// 참고: 클라이언트의 HostID는 CNetClient.GetLocalHostID로 얻을 수 있다.
	// Serial number that assigned to clients
	// Note: HostID of client can get with CNetClient.GetLocalHostID.
	int m_ID;
	// 이 클라이언트가 조종하는 캐릭터 객체
	// 클라이언트가 서버 연결이 성공한 후에야 이 객체가 만들어진다.
	// Character object that controlled by this client
	// This object is created once client succeeed to connect with server.
	CHeldPtr<CLocalHero> m_localHero; // 이 로컬 클라가 조종하는 캐릭터

	// 서버로 보내는 RMI의 프록시
	// Proxy of RMI that send to server
	SynchWorldC2S::Proxy m_C2SProxy;
	// P2P로 보내는 RMI의 프록시
	// Proxy of RMI that send to P2P
	SynchWorldC2C::Proxy m_C2CProxy;

	// 이 클라이언트에서 동기화되고 있는 다른 클라이언트의 캐릭터들
	// Character of other client that synchronize in this client
	typedef CFastMap<HostID, CRemoteHeroPtr_C> RemoteHeroes;
	RemoteHeroes m_remoteHeroes;

	CClient(int ID);
	~CClient(void);

	bool Connect();
	void FrameMove(float fElapsedTime);

	virtual void OnJoinServerComplete( ErrorInfo *info, const ByteArray &replyFromServer)PN_OVERRIDE;
	virtual void OnLeaveServer(ErrorInfo *errorInfo)PN_OVERRIDE;
	virtual void OnServerOffline(CRemoteOfflineEventArgs &args)PN_OVERRIDE;
	virtual void OnServerOnline(CRemoteOnlineEventArgs &args)PN_OVERRIDE;
	virtual void OnP2PMemberJoin(HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField) PN_OVERRIDE{}
	virtual void OnP2PMemberLeave(HostID memberHostID, HostID groupHostID, int memberCount) PN_OVERRIDE{}
	virtual void OnChangeP2PRelayState(HostID remoteHostID, ErrorType reason) PN_OVERRIDE{}
	virtual void OnSynchronizeServerTime() PN_OVERRIDE{}
	virtual void OnError(ErrorInfo *errorInfo) PN_OVERRIDE;
	// 이상상태 발생시 ErrorForm으로 넘어가기 때문에 OnWarning과 OnInformation은 무시합니다.
	// When error occured, it moves to ErrorForm. So ignore OnWarning and OnInformation.
	virtual void OnWarning(ErrorInfo *errorInfo) PN_OVERRIDE{}
	virtual void OnInformation(ErrorInfo *errorInfo) PN_OVERRIDE{}
	virtual void OnException(Exception &e) PN_OVERRIDE;
	virtual void OnNoRmiProcessed(RmiID rmiID) PN_OVERRIDE{}

	bool IsPrimaryClient();
	bool IsUnderUserControl();
	CCString GetHeroName();
	CRemoteHeroPtr_C GetRemoteHeroByClientID(HostID remoteClientID);
	StringA GetLocalAddress();

	// RMI 수신 함수 선언부
	// Declaratives of RMI receiving function
	DECRMI_SynchWorldS2C_NotifyLocalHeroViewersGroupID;
	DECRMI_SynchWorldS2C_RemoteHero_Appear;
	DECRMI_SynchWorldS2C_RemoteHero_Disappear;
	DECRMI_SynchWorldC2C_P2P_LocalHero_Move;

private:
	void GotoErrorForm( const char* text );
};

typedef RefCount<CClient> CClientPtr;
