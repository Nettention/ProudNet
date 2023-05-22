#pragma once

#include "../Common/Common.h"

class CSynchWorldServer;
class CRemoteHero_S;
typedef RefCount<CRemoteHero_S> CRemoteHeroPtr_S;

// 각 접속해온 클라이언트를 가리키는 객체
// Object that point each connecting client
class CRemoteClient_S
{
private:
	CSynchWorldServer* m_owner;


public:
	// 소유하고 있는 캐릭터의 가시 가능한 클라이언트들 수집하는 처리를 하는 데 까지 남은 시간
	// Time left that correcting client with visible owned character
	double m_updateHeroViewerCoolTime;

	CRemoteClient_S(CSynchWorldServer* owner);
	~CRemoteClient_S(void);

	// 클라이언트 HostID
	// Client HostID
	HostID m_hostID;

	// 소유하고 있는 캐릭터 정보
	// Owned character information
	CRemoteHeroPtr_S m_hero;

	// 이 remote가 주시하고 있는 hero들의 목록
	// RefCount를 쓰지 않음. weak pointer이므로. 쓸 경우 strong pointer가 되며 쌍방 참조로 인한 영구 leak으로 이어질 수 있음.
	// hero list that observing by this remote
	// Do not use RefCount. Because it is weak pointer. If you use, it will be strong pointer then it may occur permenant leak.
	CFastArray<CRemoteHero_S*> m_tangibles;

	void FrameMove();
};

typedef RefCount<CRemoteClient_S> CRemoteClientPtr_S;

// 클라이언트가 조종하고 있는 캐릭터
class CRemoteHero_S
{
public:
	// 캐릭터의 이름
	String m_name;

	// 캐릭터의 위치
	Vector3 m_position;

	// 속도
	Vector3 m_velocity;

	// 바라보고 있는 방향(사실...속도로부터도 얻을 수 있는 정보이긴 하다.)
	float m_direction;

	// 이 캐릭을 가시하는 클라들의 목록
	CFastArray<CRemoteClient_S*> m_viewers;
	
	// 이 캐릭을 가시하는 클라들의 목록을 싸잡은 P2P group
	// RefCount를 쓰지 않음. weak pointer이므로. 쓸 경우 strong pointer가 되며 쌍방 참조로 인한 영구 leak으로 이어질 수 있음.
	HostID m_viewersGroupID;

	CRemoteHero_S();
};

typedef RefCount<CRemoteHero_S> CRemoteHeroPtr_S;
