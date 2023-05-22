#include "StdAfx.h"
#include "RemoteHero_C.h"
#include "mainform.h"
#include "Resource.h"

const float CRemoteHero_C::SynchedThruDirectP2PChangeShowInterval = 1.5f;

CRemoteHero_C::CRemoteHero_C(void)
{
	m_synchedThruDirectP2P = false;
	m_synchedThruDirectP2PChangeShowCoolTime = SynchedThruDirectP2PChangeShowInterval;
}

CRemoteHero_C::~CRemoteHero_C(void)
{
}

// 캐릭터를 화면에 그린다.
// Draw a character on the screen.
void CRemoteHero_C::Render()
{
	D3DXMATRIX moveTM, rotTM;

	// 캐릭터 자체를 그린다.
	// Draw a character itself.
	D3DXMatrixRotationY(&rotTM,m_directionFollower.GetFollowerAngle());
	D3DXMatrixTranslation(&moveTM,
	                      m_positionFollower.GetFollowerPosition().x,
	                      m_positionFollower.GetFollowerPosition().y,
	                      m_positionFollower.GetFollowerPosition().z);
	D3DXCOLOR redTone(1, 0.5f, 0.5f, 1);
	D3DXCOLOR blueTone(0.5f, 0.5f, 1, 1);

	RenderMesh(rotTM*moveTM, g_heroMesh, m_synchedThruDirectP2P ? blueTone : redTone,true);


	// 캐릭터 위에 이름을 표시한다.
	// Indicate name on the character.
	if (m_synchedThruDirectP2PChangeShowCoolTime <= 0)
	{
		RenderSignPost(m_positionFollower.GetFollowerPosition() + D3DXVECTOR3(0, 5, 0), m_name);
	}
	else
	{
		CStringW stateString;
		if(m_synchedThruDirectP2P)
		{
			stateString.LoadStringW(IDS_STRING113);
		}else
		{
			stateString.LoadStringW(IDS_STRING112);
		}

		static CStringW text;
		text.Format(L"%s: %s", m_name, stateString);
		RenderSignPost(m_positionFollower.GetFollowerPosition() + D3DXVECTOR3(0, 5, 0), text);
	}
}

// 캐릭터를 1 프레임만큼의 처리를 한다.
// Process character equal to 1.
void CRemoteHero_C::FrameMove( float fElapsedTime )
{
	if (m_synchedThruDirectP2PChangeShowCoolTime >= 0)
		m_synchedThruDirectP2PChangeShowCoolTime -= fElapsedTime;
	m_positionFollower.FrameMove(fElapsedTime);
	m_directionFollower.FrameMove(fElapsedTime);
}

