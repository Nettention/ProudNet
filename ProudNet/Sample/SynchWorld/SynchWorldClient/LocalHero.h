#pragma once

class CClient;

// 클라이언트가 소유한 자기 캐릭터
// Own character which owned by client
class CLocalHero
{
	CClient* m_owner;
public:
	CLocalHero(CClient* owner);
	~CLocalHero(void);

	// 위치
	// Position
	D3DXVECTOR3 m_position;

	// 바라보는 방향
	// Watching direction
	float m_direction;
	// 속력(스칼라)
	// Speed(Scalar)
	float m_speed;
	// 최대 낼 수 있는 속력
	// Maximum speed
	float m_maxSpeed;
	// 선회속도(라디안)
	// Turning speed (Radian)
	float m_turnSpeed;
	// 가속시 가속도
	// Accelating speed
	float m_accelSpeed;
	// 감속시 가속도
	// Braking speed
	float m_brakeSpeed;
	// 캐릭터의 이름
	// Name of character
	String m_name;

	// 이 로컬 캐릭을 가시하는 타 클라이언트들을 묶은 P2P group ID
	// P2P group ID that bind other clients who watching this local character
	HostID m_viewersGroupID;

	// 이 로컬 캐릭의 위치를 브로드캐스트하기까지 남은 시간
	// Time left until broadcast postion of this local character
	float m_broadcastStateCoolTime;

	void Accel(float elapsedTime);
	void Brake(float elapsedTime);
	void TurnLeft(float elapsedTime);
	void TurnRight(float elapsedTime);
	void FrameMove(float elapsedTime);
	void Render();

	CClient* GetClient()
	{
		return m_owner;
	}
};

extern float BroadcastLocalHeroStateInterval;
