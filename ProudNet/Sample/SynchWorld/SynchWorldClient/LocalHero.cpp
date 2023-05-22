#include "StdAfx.h"
#include "client.h"
#include "LocalHero.h"
#include "mainform.h"
#include "world.h"

// 자기 캐릭터의 상태를 타 호스트에 전송하는 주기
// 통상적인 게임에서 이 값은 0.3~0.1 정도가 된다. 너무 작으면 되레 통신량이 증가하므로 주의할 것.
// Period that send state of own character to other host.
// Normally this value is 0.3~0.1 in a game. If it is too small, traffic may increase so be careful about it.
float BroadcastLocalHeroStateInterval = 0.1f;

CLocalHero::CLocalHero(CClient* owner)
{
	m_owner = owner;
	
	// 캐릭의 초기 위치는 랜덤하게 배치한다.
	// Place defualt position of character randomly.
	m_position = D3DXVECTOR3(
	                 (float)Proud::CRandom::StaticGetFloat() * CWorld::ArenaLength / 2 + CWorld::ArenaLength / 4,
	                 0,
	                 (float)Proud::CRandom::StaticGetFloat() * CWorld::ArenaLength / 2 + CWorld::ArenaLength / 4);

	// 프라이머리 클라인 경우 위치를 한 곳으로 정하자. 이래야 테스트가 편하다.
	// If it is primary client, fix position to 1 point. That will make easier to test.
	if (owner->IsPrimaryClient())
	{
		m_position = D3DXVECTOR3(100, 0, 100);
	}
	m_direction = 0;
	m_speed = 0;

	m_maxSpeed = 30;
	m_accelSpeed = 30;

	m_turnSpeed = D3DX_PI / 180 * 60;

	m_brakeSpeed = 60;

	m_viewersGroupID = HostID_None;

	m_broadcastStateCoolTime = static_cast<float>(CRandom::StaticGetFloat() * BroadcastLocalHeroStateInterval);
}

CLocalHero::~CLocalHero(void)
{
}

void CLocalHero::Accel( float elapsedTime)
{
	m_speed += elapsedTime * m_accelSpeed;
	m_speed = max(m_speed, -m_maxSpeed);
	m_speed = min(m_speed, m_maxSpeed);

	//ATLTRACE("%f elapsedtime\n",elapsedTime);
}

void CLocalHero::Brake( float elapsedTime)
{
	float positiveSpeed = abs(m_speed);
	positiveSpeed -= elapsedTime * m_brakeSpeed;
	if (positiveSpeed < 0)
		positiveSpeed = 0;

	if (m_speed >= 0)
		m_speed = positiveSpeed;
	else
		m_speed = -positiveSpeed;
}

void CLocalHero::TurnLeft( float elapsedTime )
{
	m_direction -= elapsedTime * m_turnSpeed;
}

void CLocalHero::TurnRight( float elapsedTime )
{
	m_direction += elapsedTime * m_turnSpeed;
}

void CLocalHero::FrameMove( float elapsedTime )
{
	// 속도, 위치 값 갱신
	// Renew speed, position value
	D3DXVECTOR3 velocity(0, 0, m_speed);
	D3DXMATRIX rotMat;
	D3DXMatrixRotationY(&rotMat,m_direction);
	D3DXVec3TransformCoord(&velocity, &velocity, &rotMat);
	m_position += velocity * elapsedTime;

	//ATLTRACE("%f, %f %f\n", m_position.x, m_position.y, m_position.z );

	// 캐릭터 위치는 바운더리를 벗어나지 못하게 한다.
	// Make character position stay within boundary.
	m_position.x = max(m_position.x, 0);
	m_position.x = min(m_position.x, CWorld::ArenaLength);

	m_position.z = max(m_position.z, 0);
	m_position.z = min(m_position.z, CWorld::ArenaLength);

	

	// 일정 간격으로 자신의 위치를 서버와 p2p group member에게 보낸다.
	// Send own location to server and p2p group member every specific time.
	m_broadcastStateCoolTime -= elapsedTime;
	if (m_broadcastStateCoolTime < 0)
	{
		m_broadcastStateCoolTime = BroadcastLocalHeroStateInterval;

		// UniqueID를 지정해서 과량 송신하는 경우 최종적으로 결정된 위치만 보내도록 한다.
		// 즉 스로틀링 기법을 쓴다.
		// If it overload to send then send to only lastest location by assign UniqueID.
		// Use throttling method.
		RmiContext rctx = RmiContext::UnreliableSend;
		rctx.m_uniqueID = 333;

		// P2P 그룹에는 서버도 포함되어 있는 상태이다. 따라서 P2P 그룹에 보내더라도 서버 또한 수신을 하게 된다.
		// 서버는 이를 받아서 가시영역 동기화 연산에 쓸 것이다.
		// P2P group includes server too. So even you send to P2P group, server also receive it too.
		// Server receive it then use visible area synchronize operation.
		GetClient()->m_C2CProxy.P2P_LocalHero_Move(m_viewersGroupID, rctx, m_position, velocity, m_direction);
	}
}

void CLocalHero::Render()
{
	D3DXMATRIX moveTM, rotTM, scaleTM;

	D3DXMatrixScaling(&scaleTM, ViewportLength*2, 1, ViewportLength*2);
	D3DXMatrixRotationY(&rotTM, m_direction);
	D3DXMatrixTranslation(&moveTM, m_position.x, m_position.y, m_position.z);

	// 로컬 캐릭 근처의 가시 영역을 원형으로 표시한다.
	// Mark visible area around local character with circle.
	RenderMesh(scaleTM*moveTM, g_circleMesh, D3DXCOLOR(1, 1, 1, 0.3f));

	// 캐릭터를 그린다.
	// Draw a character.
	RenderMesh(rotTM*moveTM, g_heroMesh, D3DXCOLOR(1, 1, 1, 1));

	// 캐릭터 위에 이름을 표시한다.
	// Indicate name on the character.
	RenderSignPost(m_position + D3DXVECTOR3(0, 5, 0), m_name.GetString());
}