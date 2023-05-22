#pragma once

/* 타 클라이언트의 동기화되고 있는 캐릭터(클라이언트측) */
/* Synchronizing character of other client (Client side) */
class CRemoteHero_C
{
public:
	// 캐릭터의 위치. CPositionFollower이므로, 네트워크로 받은 실제 위치와 화면에 그리기 위해 보정되는 위치를 모두 갖는다.
	// Position of character. It is CPositionFollower so it has actual postion from network and correction position for drawing on the screen.
	CPositionFollower m_positionFollower;
	// 캐릭터의 바라보는 방향. 네트워크로 받은 실제 위치와 화면에 그리기 위해 보정되는 위치를 모두 갖는다.
	// Watching direction of character. It has actual postion from network and correction position for drawing on the screen.
	CAngleFollower m_directionFollower;
	// 캐릭터 이름
	// Character name
	CStringW m_name;
	// 캐릭터를 조종하는 클라의 HostID
	// HostID of client who control character
	HostID m_ownerID;
	// 가장 마지막으로 받은 캐릭터의 위치 정보가, direct P2P 인지 Relayed P2P인지?
	// Is lastest received position information of character direct P2P or Replayed P2P?
	bool m_synchedThruDirectP2P;
	// m_synchedThruDirectP2P 상태를 화면에 표시하되, 일정 시간이 지나면 표시하지 않게 하기 위함
	// Indicate m_synchedThruDirectP2P state on the screen but do not indicate after specific time
	float m_synchedThruDirectP2PChangeShowCoolTime;

	// 캐릭터의 m_synchedThruDirectP2P 상태가 바뀔 때마다 화면에 보여주는 한계 시간
	// Limited time that shows on the screen when m_synchedThruDirectP2P state of character has changed
	static const float SynchedThruDirectP2PChangeShowInterval;

	CRemoteHero_C(void);
	~CRemoteHero_C(void);

	void Render();
	void FrameMove(float fElapsedTime);

};

typedef RefCount<CRemoteHero_C> CRemoteHeroPtr_C;
