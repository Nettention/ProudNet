#include "StdAfx.h"
#include "GameObject_C.h"
#include "BattleForm.h"
#include "World.h"
#include "resource.h"
#include "BattleClient.h"

D3DXVECTOR3 SignPostOffset(0, 1.3f, 0);

// 자기 캐릭터의 상태를 타 호스트에 전송하는 주기
// 통상적인 게임에서 이 값은 0.3~0.1 정도가 됩니다. 너무 작으면 되레 통신량이 증가하므로 주의할 것.
int64_t BroadcastLocalHeroStateInterval = 100;
const int64_t CGameObject_C::SynchedThruDirectP2PChangeShowInterval = 1500;

CGameObject_C::CGameObject_C(void)
{
	m_synchedThruDirectP2P = false;
	m_synchedThruDirectP2PChangeShowCoolTimeMs = SynchedThruDirectP2PChangeShowInterval;

	m_speed = 0;

	//초기위치
	m_position = D3DXVECTOR3(50, 0, 50);

	m_yaw = D3DX_PI / 180 * 270;
	m_pitch = 0;

	m_speed = 0;

	m_maxSpeed = 30;
	m_turnSpeed = D3DX_PI / 180 * 60;  //60도

	m_accelSpeed = 30;
	m_brakeSpeed = 60;

	m_viewersGroupID = HostID_None;

	m_broadcastStateCoolTimeMs = (CRandom::StaticGetInt64() * BroadcastLocalHeroStateInterval);
}

CGameObject_C::~CGameObject_C(void)
{
}

void CGameObject_C::Render_RemoteHero()
{
	D3DXMATRIX scaleTM,moveTM, rotTM;

	// 캐릭터 자체를 그립니다.
	D3DXMatrixScaling(&scaleTM, 1,1,1);
	D3DXMatrixRotationY(&rotTM, (float)(-D3DX_PI / 180*90 + m_yawFollower.GetFollowerAngle()));
	D3DXMatrixTranslation(&moveTM,
		(float)m_positionFollower.GetFollowerPosition().x,
		(float)m_positionFollower.GetFollowerPosition().y,
		(float)m_positionFollower.GetFollowerPosition().z);

	D3DXCOLOR redTone(1, 0.5f, 0.5f, 1);
	D3DXCOLOR blueTone(0.5f, 0.5f, 1, 1);
	RenderMesh(scaleTM*rotTM*moveTM, g_HeroMesh, m_synchedThruDirectP2P ? blueTone : redTone);
	
	// 캐릭터 위에 이름을 표시합니다.
	if (m_synchedThruDirectP2PChangeShowCoolTimeMs <= 0)
	{
		RenderSignPost(m_positionFollower.GetFollowerPosition() + SignPostOffset, m_name);
	}
	else
	{
		CString p2pString;
		if(m_synchedThruDirectP2P)
		{
			// Direct P2P
			p2pString = CWorld::GetSharedPtr()->LoadString(IDS_STRING126);
		}else
		{
			// Relay P2P
			p2pString = CWorld::GetSharedPtr()->LoadString(IDS_STRING127);
		}

		static CStringW text;
		text.Format(L"%s: %s", m_name,  p2pString);
		RenderSignPost(m_positionFollower.GetFollowerPosition() + SignPostOffset, text);
	}
}

void CGameObject_C::FrameMove(float elapsedTime)
{
	if(IsLocalHero())
		FrameMove_LocalHero(elapsedTime);
	else
		FrameMove_RemoteHero(elapsedTime);
}

void CGameObject_C::Accel( float elapsedTime)
{
	m_speed += elapsedTime * m_accelSpeed;
	m_speed = max(m_speed, -m_maxSpeed);
	m_speed = min(m_speed, m_maxSpeed);
}


void CGameObject_C::Brake( float elapsedTime)
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

D3DXVECTOR2 GetLookatOffsetFromCameraAndForceCursorToCenter()
{
	// 마우스 위치. 스크린 좌표계
	POINT cursorPosInt;
	GetCursorPos(&cursorPosInt);

	// TODO: 풀 스크린인 경우도 처리 해야 함. 아직은 안함.
	RECT windowArea;
	GetWindowRect(DXUTGetHWNDDeviceWindowed(), &windowArea);

	D3DXVECTOR2 windowCenter;
	windowCenter.x = float(windowArea.left + windowArea.right)/2;	
	windowCenter.y = float(windowArea.top + windowArea.bottom)/2;

	D3DXVECTOR2 windowSize;
	windowSize.x = float(windowArea.right - windowArea.left);
	windowSize.y = float(windowArea.bottom - windowArea.top);

	D3DXVECTOR2 cursorPos;
	cursorPos.x = (float)(cursorPosInt.x);
	cursorPos.y = (float)(cursorPosInt.y);

	D3DXVECTOR2 diff = cursorPos - windowCenter;

	D3DXVECTOR2 result;
	result.x = diff.x / windowSize.x;
	result.y = -diff.y / windowSize.y; // invert mouse

	result /= 1;

	SetCursorPos((int)windowCenter.x, (int)windowCenter.y);

	return result;
}

void CGameObject_C::FrameMove_LocalHero( float elapsedTime )
{
	D3DXVECTOR3 velocity(0,0,0);
	if(DXUTIsActive() && !GetAsyncKeyState(VK_CONTROL)) // CTRL 키 누르고 있으면 마우스를 자유롭에 움직여 메뉴 선택 가능
	{
		// 플레이어가 바라보는 방향을 얻고 마우스 위치를 윈도 중심으로 강제 이동합니다.
		D3DXVECTOR2 lookatOffset = GetLookatOffsetFromCameraAndForceCursorToCenter();
		m_yaw += lookatOffset.x;
		m_pitch += lookatOffset.y;

		// 키 입력에 따라 플레이어 이동
		SHORT leftKey = GetAsyncKeyState('A');		
		SHORT rightKey = GetAsyncKeyState('D');		
		SHORT forwardKey = GetAsyncKeyState('W');		
		SHORT backwardKey = GetAsyncKeyState('S');

		if(leftKey)
			velocity.z = 0.4f;
		else if(rightKey) 
			velocity.z = -0.4f;

		if(forwardKey)
			velocity.x = 1;
		else if (backwardKey)
			velocity.x = -1;

		velocity *= 5;  // 플레이어 캐릭터의 실제 이동 속도

		D3DXMATRIX yawMat;
		D3DXMatrixRotationY(&yawMat, m_yaw);
		D3DXVec3TransformCoord(&velocity, &velocity, &yawMat);

		m_position += velocity * elapsedTime;
	}

	// 캐릭터 위치는 바운더리를 벗어나지 못하게 합니다.
	m_position.x = max(m_position.x, 0);
	m_position.x = min(m_position.x,300);

	m_position.y = max(m_position.y, 0);
	m_position.y = min(m_position.y, 100);

	m_position.z = max(m_position.z,0);
	m_position.z = min(m_position.z,300);

	// 일정 간격으로 자신의 위치를 서버와 p2p group member에게 보냅니다.
	m_broadcastStateCoolTimeMs -= (int64_t)(elapsedTime*1000);
	if (m_broadcastStateCoolTimeMs < 0)
	{
		m_broadcastStateCoolTimeMs = BroadcastLocalHeroStateInterval;

		// UniqueID를 지정해서 과량 송신하는 경우 최종적으로 결정된 위치만 보내도록 합니다.
		// 즉 스로틀링 기법을 씁니다.
		RmiContext rctx = RmiContext::UnreliableSend;
		rctx.m_uniqueID = 33333;

		// 서버는 이를 받아서 가시영역 동기화 연산에 쓸 것입니다.
		m_owner->m_c2cProxy.P2P_LocalHero_Move(m_viewersGroupID, rctx, m_position, velocity, m_yaw);
		m_owner->m_c2sProxy.LocalHero_Move(HostID_Server,rctx,m_position,velocity,m_yaw);
	}
}

void CGameObject_C::Render_LocalHero()
{
	D3DXMATRIX move0TM, moveTM, rotTM, synchAreaScaleMat,heroScaleMat;

	D3DXMatrixScaling(&synchAreaScaleMat, SynchAreaRange*2, 1, SynchAreaRange*2);
	D3DXMatrixScaling(&heroScaleMat, 1.f, 1.f, 1.f);

	D3DXMatrixRotationY(&rotTM, -D3DX_PI / 180*90 + m_yaw);
	D3DXMatrixTranslation(&move0TM, m_position.x, m_position.y + 0.1f, m_position.z); // 동기화 영역 원은 지형보다 살짝 위에 나오게 하기
	D3DXMatrixTranslation(&moveTM, m_position.x, m_position.y, m_position.z);

	// 로컬 캐릭 근처의 가시 영역을 원형으로 표시합니다.
	RenderMesh(synchAreaScaleMat*move0TM, g_synchAreaMesh, D3DXCOLOR(1, 1, 1, 0.3f));

	// 캐릭터를 그립니다.
	RenderMesh(heroScaleMat*rotTM*moveTM, g_HeroMesh, D3DXCOLOR(1, 1, 1, 1));

	// 캐릭터 위에 이름을 표시합니다.
	RenderSignPost(m_position + SignPostOffset, m_name);

	if (m_position.x <=0 || m_position.x >= 300 ||m_position.z >=300 || m_position.z <= 0)
	{
		// "더이상 진행할수 없습니다";
		CStringW warning;
		warning.LoadStringW(IDS_STRING117);
		RenderSignPost(m_position+D3DXVECTOR3(0,15,0),(LPCWSTR)warning);
	}

	if (m_score>0)
	{
		CStringW staticscore = L"Score : ";
		CStringW myscore;
		myscore.Format(L"%d",m_score);
		myscore = staticscore + myscore;
		RenderSignPost(m_position + D3DXVECTOR3(0,10,0),myscore);
	}
}

void CGameObject_C::FrameMove_RemoteHero( float fElapsedTime )
{
	if (m_synchedThruDirectP2PChangeShowCoolTimeMs >= 0)
	{
		m_synchedThruDirectP2PChangeShowCoolTimeMs -= (int64_t)(fElapsedTime*1000);
	}
	m_positionFollower.FrameMove(fElapsedTime);
	m_yawFollower.FrameMove(fElapsedTime);
}

// 객체를 파괴하기 전에 콜 해야 하는 함수
// m_gameObjects 외의 인덱스에서 지우는 역할도 합니다.
void CGameObject_C::Dispose()
{
	if(m_owner->m_localHero == this)
	{
		m_owner->m_localHero = NULL;
	}
	else
	{
		m_owner->m_remoteHeroes.Remove(this->m_ownerHostID);
	}
}

void CGameObject_C::Render()
{
	if(IsLocalHero())
		Render_LocalHero();
	else
		Render_RemoteHero();
}

bool CGameObject_C::IsLocalHero()
{
	return (m_owner->m_NetClient->GetLocalHostID() == m_ownerHostID);
}

