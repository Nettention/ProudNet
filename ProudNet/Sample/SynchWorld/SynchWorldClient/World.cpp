#include "StdAfx.h"
#include "World.h"
#include "mainform.h"
#include "resource.h"

const float AdjustDesiredClientCountInterval = 0.5f;

CWorld::CWorld(void)
{
	m_clientIDSerial = 0;
	m_formState = ConnectForm;
	m_clientUnderUserControl = -1;
	m_desiredClientCount = 1;
	m_adjustDesiredClientCountCoolTime = AdjustDesiredClientCountInterval;
}

CWorld::~CWorld(void)
{
}

// 현재 유저가 컨트롤하겠다고 선택한 캐릭터를 조종한다.
// Control selected character by current user.
void CWorld::LocalHeroes_FrameMove(float fElapsedTime)
{
	SHORT leftKey = GetAsyncKeyState(VK_LEFT);
	SHORT rightKey = GetAsyncKeyState(VK_RIGHT);
	SHORT upKey = GetAsyncKeyState(VK_UP);
	SHORT downKey = GetAsyncKeyState(VK_DOWN);

	// 키 입력을 받으면 local char의 위치를 업데이트한다.
	// Update position of local char when it receives key entering.
	if (!DXUTIsActive())
	{
		leftKey = rightKey = upKey = downKey = 0;
	}

	for (int i = 0; i < m_clients.Count; i++)
	{
		CClient* client = m_clients[i];
		if (client->m_localHero)
		{
			if (client->m_localHero == GetLocalHeroUnderUserControl())
			{
				// 키 입력에 따른 이동 처리를 한다.
				// Movement process by key entering.
				if (upKey)
				{
					client->m_localHero->Accel(fElapsedTime);
				}
				else if (downKey)
				{
					client->m_localHero->Accel(fElapsedTime);
				}
				else
				{
					client->m_localHero->Brake(fElapsedTime);
				}
				if (leftKey)
				{
					client->m_localHero->TurnLeft(fElapsedTime);
				}
				if (rightKey)
				{
					client->m_localHero->TurnRight(fElapsedTime);
				}
			}
			else
			{
				// 계속 우회전 전진을 하고 있는 것처럼 처리한다.
				// Process like keep turn right then move straight.
				client->m_localHero->Accel(fElapsedTime);
				client->m_localHero->TurnLeft(fElapsedTime);
			}
			client->m_localHero->FrameMove(fElapsedTime);
		}
	}
}

void CWorld::Render()
{
	// 활성화된 클라의 로컬 캐릭만을 그린다.
	// Draw only local character of activated client.
	CClient* client = GetClientUnderUserControl();
	if (client)
	{
		if (client->m_localHero)
			client->m_localHero->Render();

		// 활성화된 클라 관점에서의 원격 객체들을 그린다.
		// Draw a remote object from activated client side.
		for (Proud::Position iRH = client->m_remoteHeroes.GetStartPosition();iRH;)
		{
			CRemoteHeroPtr_C remoteHero = client->m_remoteHeroes.GetNextValue(iRH);
			remoteHero->Render();
		}
	}
}

// 에러가 나서 더 이상 플레이 불능 상태의 모드로 전환한다.
// Turn to unplayable mode because of error.
void CWorld::GoToErrorForm( LPCWSTR text )
{
	if (m_formState != ErrorForm)
	{
		m_errorForm.GetStatic(IDC_STATIC)->SetText(text);
		m_formState = ErrorForm;
	}
}

// 현재 유저가 선택해서 조종하고 있는 캐릭터 객체를 얻는다.
// Get character object that selected by current user and under control.
CLocalHero* CWorld::GetLocalHeroUnderUserControl()
{
	if (m_clientUnderUserControl < 0)
		return NULL;
	return m_clients[m_clientUnderUserControl]->m_localHero;
}

// 메인폼 모드로 전환한다.
// Turn to main form mode.
void CWorld::GoToMainForm()
{
	if (m_formState != MainForm)
	{
		m_formState = MainForm;
	}
}

// 메인 폼에서, 여러가지 로그를 출력한다.
// Print several log at main form.
void CWorld::Log( LPCWSTR text )
{
	m_mainForm.GetListBox(IDC_LOG)->AddItem(text, NULL);
	m_mainForm.GetListBox(IDC_LOG)->SelectItem(m_mainForm.GetListBox(IDC_LOG)->GetSize() - 1);
}

// 현재 활성화된 폼을 렌더한다.
// CWorld.Render와 같이 하지 않는 이유는, 폼과 3D 월드는 렌더 순서를 명확히 나눠야 하기 때문이다.
// Render current activated form.
// It does not do like CWorld.Render because it has to separate render order of form and 3D world.
void CWorld::RenderForm(float elapsedTime)
{
	switch (m_formState)
	{
	case ConnectForm:
		m_connectForm.OnRender(elapsedTime);
		break;
	case MainForm:
		m_mainForm.OnRender(elapsedTime);
		break;
	case ErrorForm:
		m_errorForm.OnRender(elapsedTime);
		break;
	}
}

// UI 메시지 핸들러
// UI message handler
bool CWorld::FormMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (m_formState)
	{
	case ConnectForm:
		return m_connectForm.MsgProc(hWnd,  uMsg,  wParam,  lParam);
	case MainForm:
		return m_mainForm.MsgProc(hWnd,  uMsg,  wParam,  lParam);
	case ErrorForm:
		return m_errorForm.MsgProc(hWnd,  uMsg,  wParam,  lParam);
	}
	return false;
}

// 새 클라이언트 객체를 만들어 등록시킨다.
// Creates new client object then apply it.
CClient* CWorld::AddNewClient()
{
	m_clientIDSerial++;

	CClientPtr newCli(new CClient(m_clientIDSerial) );
	m_clients.Add(newCli);
	CStringW txt;
	txt.Format(L"%d", newCli->m_ID);
	m_mainForm.GetListBox(IDC_CLIENT_LIST)->AddItem(txt, newCli);
	UpdateClientCountCtl();

	m_clientUnderUserControl = min(m_clients.Count - 1, m_clientUnderUserControl);
	m_clientUnderUserControl = max(0, m_clientUnderUserControl);

	return newCli;

}

// 1회 프레임 처리
// 1 frame process
void CWorld::FrameMove( float fElapsedTime )
{
	// 각 클라에 대한 진행 처리를 한다.
	// Do process about each client.
	LocalHeroes_FrameMove(fElapsedTime);

	for (int i = 0;i < m_clients.Count;i++)
	{
		CClient* client = m_clients[i];
		client->FrameMove(fElapsedTime);
	}

	DisposeMarkedClient();

	MoveCamera(fElapsedTime);

	AdjustDesiredClientCountOnNeed(fElapsedTime);
}

// 유저가 직접 키보드로 제어하고 있는 캐릭터의 클라이언트를 얻는다.
// Get client of character that controlling by user with keyboard.
CClient* CWorld::GetClientUnderUserControl()
{
	if (m_clientUnderUserControl < 0)
		return NULL;
	return m_clients[m_clientUnderUserControl];
}

// 사용자가 입력한 '원하는 클라이언트 갯수'에 따라 클라이언트 인스턴스를 증가 또는 증감을 한다.
// Do increase or decrease client instance depend on 'Wanted client number' that entered by user.
void CWorld::AdjustDesiredClientCount()
{
	// 최소한 1개의 클라는 건재해야 하므로
	// At least 1 client lives
	ASSERT(m_desiredClientCount >= 1); 

	// 아직 서버와 프라이머리 클라와의 연결 시도를 시작이라도 한 상태이어야 한다.
	// It has to started connecting attempt with server and primary client.
	if (m_clients.Count > 0) 
	{
		if (m_desiredClientCount > m_clients.Count)
		{
			// 클라를 1개 더 추가한다.
			// Adds 1 more client.
			CClient* newCli = AddNewClient();
			newCli->Connect();
		}
		else if (m_desiredClientCount < m_clients.Count)
		{
			// 클라를 1개 제거함을 표기해둔다.
			// Mark remove 1 client.
			RemoveLastOneClient();
		}
	}
}

void CWorld::RemoveLastOneClient()
{
	m_clients[m_clients.Count-1]->m_markDisposed = true;
}

int CWorld::FindClientByValue( CClient* client )
{
	for (int i = 0;i < m_clients.Count;i++)
	{
		if (m_clients[i] == client)
			return i;
	}
	return -1;
}

// 폐기하라는 표식이 된 클라들을 제거.
// Remove makred client as "dispose".
void CWorld::DisposeMarkedClient()
{
	for (int i = m_clients.Count - 1;i >= 0;i--)
	{
		CClient* client = m_clients[i];
		if (client->m_markDisposed)
		{
			// 보조 클라이언트 %s가 제거됐습니다.
			// Sub client %s was removed
			CStringW loadString;
			loadString.LoadStringW(IDS_STRING108);
			CStringW logString;
			logString.Format(loadString, client->m_localHero ? client->m_localHero->m_name : L"<None>");
			Log(logString);

			// 폼에서도 제거한다.
			// Remove from form.
			CDXUTListBox* listCtl = m_mainForm.GetListBox(IDC_CLIENT_LIST);
			for (int j = 0;j < listCtl->GetSize();j++)
			{
				if (listCtl->GetItem(j)->pData == client)
				{
					if (listCtl->GetSelectedIndex() == j)
					{
						// 다른 것을 선택하게 한다.
						// Make select another one.
						listCtl->SelectItem(j - 1);
						CClient* otherClient = (CClient*)listCtl->GetSelectedItem()->pData;
						m_clientUnderUserControl = FindClientByValue(otherClient);
						ASSERT(m_clientUnderUserControl >= 0);
					}

					listCtl->RemoveItem(j);

					UpdateClientCountCtl();
					break;
				}
			}

			// 실제 클라 제거
			// Remove actual client
			m_clients.RemoveAt(i);

		}
	}
}

// 카메라 위치를 이동시킨다.
// Move camera position.
void CWorld::MoveCamera( float fElapsedTime )
{
	// 카메라 위치는 position follower를 써서 local char가 위치한 곳을 쫓아간다.
	// 이를 위해 position follower를 PN에 추가하자.
	// Camera location follow place where local char located with using position follower.
	// Adds position follower to PN for this.
	if (GetLocalHeroUnderUserControl())
	{
		m_cameraPosition.SetTargetPosition(GetLocalHeroUnderUserControl()->m_position);
	}
	else
		m_cameraPosition.SetTargetPosition(Vector3(100, 0, 100));

	m_cameraPosition.EnableAutoFollowDuration(false);
	m_cameraPosition.SetFollowDuration(0.3);
	m_cameraPosition.FrameMove(fElapsedTime);
	
#ifdef _DEBUG
	Vector3 p=m_cameraPosition.GetFollowerPosition();
	if(p.x>100000)
	{
		int a=0;
	}
#endif
}

void CWorld::AdjustDesiredClientCountOnNeed( float fElapsedTime )
{
	// 희망하는 클라 갯수에 따라 점층적으로 클라를 새로 만들거나 이미 있는 클라를 폐기해 나간다.
	// Create new client or dispose existed client depends on number of client that you wish.
	m_adjustDesiredClientCountCoolTime -= fElapsedTime;
	if (m_adjustDesiredClientCountCoolTime < 0)
	{
		m_adjustDesiredClientCountCoolTime = AdjustDesiredClientCountInterval;

		AdjustDesiredClientCount();
	}
}

// 클라이언트 갯수 관련 UI를 업데이트한다.
// Update client number related UI.
void CWorld::UpdateClientCountCtl()
{
	CStringW txt;
	txt.Format(L"Client Count: %d", m_mainForm.GetListBox(IDC_CLIENT_LIST)->GetSize());
	m_mainForm.GetStatic(IDC_STATIC_CLIENTCOUNT)->SetText(txt);
}
CWorld *g_World;

// 월드의 가로세로 길이
// Length and breadth of world
const float CWorld::ArenaLength = 500;