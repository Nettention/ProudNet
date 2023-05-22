#include "StdAfx.h"
#include "Bullet_S.h"
#include "BattleServer.h"

CBullet_S::CBullet_S(CBattleServer* owner) : m_AccumTimeForMoveBullet(0), m_owner(owner)
{
}

CBullet_S::~CBullet_S(void)
{
}
void CBullet_S::FrameMove(float m_ElapsedTime)
{
	m_speed += m_ElapsedTime * m_accelspeed;
	m_speed = max(m_speed,-m_maxSpeed);
	m_speed = min(m_speed,m_maxSpeed);

	m_velocity = D3DXVECTOR3(m_speed,0,0);
	D3DXMATRIX rotMat;
	if(m_directionState != TOP)
		D3DXMatrixRotationY(&rotMat,m_direction);
	else
	{
		m_velocity = D3DXVECTOR3(0,m_speed,0);
		D3DXMatrixRotationX(&rotMat,m_direction);
	}
	D3DXVec3TransformCoord(&m_velocity,&m_velocity,&rotMat);

	m_position += m_velocity * m_ElapsedTime;

	//Area Boundary Check...
	m_position.x = max(m_position.x, 0); //0
	m_position.x = min(m_position.x,300);


	m_position.y = max(m_position.y, 0);
	m_position.y = min(m_position.y, 200);

	m_position.z = max(m_position.z,0);//0
	m_position.z = min(m_position.z,300);


	m_AccumTimeForMoveBullet += m_ElapsedTime;
	if (m_AccumTimeForMoveBullet >= 0.1)
	{
		m_AccumTimeForMoveBullet = 0;

		// Sends Move packet to client every specific time (0.1 second)
		// 일정시간(0.1초)마다 클라에게 Move 패킷을 보냅니다.
		for (RemoteBattleClients::iterator i = m_owner->m_remoteClients.begin(); i != m_owner->m_remoteClients.end();i++)
		{
			CBattleClientPtr_S rc = i->second;
			m_owner->m_s2cProxy.NotifyBullet_Move(rc->m_HostID,RmiContext::UnreliableSend,m_id,m_speed,m_direction,m_position,m_velocity);
		}	
	}
}
