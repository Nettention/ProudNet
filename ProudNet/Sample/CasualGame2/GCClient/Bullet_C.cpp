#include "StdAfx.h"
#include "Bullet_C.h"
#include "MainWindow.h"

CBullet_C::CBullet_C(void)
{
}

CBullet_C::~CBullet_C(void)
{
}
void CBullet_C::FrameMove(float ElapsedTime)
{
	m_PositionFollower.FrameMove(ElapsedTime);
}

// It just draws bullet at client (simulation process at server).
//클라이언트에서는 총알을 그리는 역할만 합니다. 시뮬레이션은 서버에서 합니다.
void CBullet_C::Render()
{
	D3DXMATRIX moveTM, rotTM, scaleTM;

	if ((m_PositionFollower.GetFollowerPosition().x >=0 && m_PositionFollower.GetFollowerPosition().x <=300) && 
		(m_PositionFollower.GetFollowerPosition().z >= 0 && m_PositionFollower.GetFollowerPosition().z <=300))
	{
		if (m_directionState != TOP)
		{
			D3DXMatrixScaling(&scaleTM, 0.3f,0.3f,0.3f);
			D3DXMatrixRotationY(&rotTM, -D3DX_PI / 180*90 + m_direction);
			D3DXMatrixTranslation(&moveTM, 
								(float)m_PositionFollower.GetFollowerPosition().x, 
								(float)m_PositionFollower.GetFollowerPosition().y,
								(float)m_PositionFollower.GetFollowerPosition().z);
			if(m_position.y == 0)
				RenderMesh(scaleTM*moveTM,g_BulletMesh,D3DXCOLOR(255,255,255,1));
			else
				RenderMesh(scaleTM*moveTM,g_cylinderMesh,D3DXCOLOR(255,255,255,1));
		} 
		else
		{
			D3DXMatrixScaling(&scaleTM, 1,1,1);
			D3DXMatrixRotationY(&rotTM, -D3DX_PI / 180*90 + m_direction);
			D3DXMatrixTranslation(&moveTM, 
				(float)m_PositionFollower.GetFollowerPosition().x, 
				(float)m_PositionFollower.GetFollowerPosition().y,
				(float)m_PositionFollower.GetFollowerPosition().z);
			RenderMesh(scaleTM*moveTM,g_BombMesh,D3DXCOLOR(255,255,255,1));
		}
		
	}

}