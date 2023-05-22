#include "StdAfx.h"
#include "World.h"
#include "MainWindow.h"
#include "entryclient.h"
#include "lobbyclient.h"
#include "battleclient.h"
#include "BattleWaitingForm.h"

#include "BattleForm.h"

const int64_t AdjustDesiredClientCountInterval = 500;


CEntryLocalHeroSlot::CEntryLocalHeroSlot()
{
	m_type = 0;
	m_score = 0;
}

CEntryLobbySlot::CEntryLobbySlot()
{
	m_gamerCount = 0;
}

/// Change it unplayable mode with error.
/// 에러가 나서 더 이상 플레이 불능 상태의 모드로 전환합니다.
void CWorld::GotoErrorForm(LPCWSTR text)
{
	if (m_FormState != ErrorForm)
	{
		m_errorForm.GetStatic(IDC_STATIC)->SetText(text);
		m_FormState = ErrorForm;
	}
}

CStringW CWorld::LoadString(int stringResourceNum)
{
	CStringW loadString;
	if (!loadString.LoadStringW(stringResourceNum))
	{
		GotoErrorForm(L"Failed to load string");
	}
	return loadString;
}

/// Change it to game waiting room form.
/// 게임방 대기실 폼으로 전환합니다.
void CWorld::GotoBattleWaitingForm()
{
	if (m_FormState != BattleWaitingForm)
	{
		m_FormState = BattleWaitingForm;

		m_battleWaitingForm.GetButton(IDC_START_BUTTON)->SetEnabled(true);
		if (GetBattleClient())
		{
			GetBattleClient()->m_startPlayModeUnderProgress = false;
			BattleWaitingForm_RefreshButtons();
		}
	}
}

CWorld::CWorld(void)
{
	m_FormState = ConnectForm;
	m_isConnectEntryServer = false;
	m_isComebackFromLobby = false;
	m_adjustDesiredClientCountCoolTimeMs = AdjustDesiredClientCountInterval;
}

CWorld::~CWorld(void)
{
	m_client.Free();
}

/// Render current activated form.
/// Reason that why do not like CWorld.Render because render order of form and 3D world has to separate clearly.
/// 현재 활성화된 폼을 렌더링 합니다.
/// CWorld.Render와 같이 하지 않는 이유는, 폼과 3D 월드는 렌더 순서를 명확히 나눠야 하기 때문입니다.
void CWorld::RenderForm(float elapsedTime)
{
	switch (m_FormState)
	{
	case ConnectForm:
		m_connectForm.OnRender(elapsedTime);
		break;
	case ErrorForm:
		m_errorForm.OnRender(elapsedTime);
		break;
	case SelectHeroForm:
		m_selectHeroForm.OnRender(elapsedTime);
		break;
	case NewHeroForm:
		m_newHeroForm.OnRender(elapsedTime);
		break;
	case SelectChannelForm:
		m_selectChannelForm.OnRender(elapsedTime);
		break;
	case LobbyChannelForm:
		m_lobbyChannelForm.OnRender(elapsedTime);
		break;
	case NewRoomForm:
		m_newRoomForm.OnRender(elapsedTime);
		break;
	case BattleWaitingForm:
		m_battleWaitingForm.OnRender(elapsedTime);
		break;
	case BattleForm:
		m_battleForm.OnRender(elapsedTime);
		break;
	case JoinForm:
		m_joinForm.OnRender(elapsedTime);
		break;
	}
}

/// UI message handler
/// UI 메시지 Handler 입니다.
bool CWorld::FormMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CDisconnectArgs args;

	switch (uMsg)
	{
	case WM_LAUNCH_LOBBY_CLIENT:
		if (CWorld::GetSharedPtr()->GetBattleClient() != NULL)
		{
			CWorld::GetSharedPtr()->GetBattleClient()->ClearWorld();
		}

		args.m_comment.Add(DisConnectServerChange);
		args.m_gracefulDisconnectTimeoutMs = 1000;
		CWorld::GetSharedPtr()->m_client->m_NetClient->Disconnect(args);
		CWorld::GetSharedPtr()->m_client.Free();
		Sleep(100);
		CWorld::GetSharedPtr()->m_client.Attach(new CLobbyClient);
		CWorld::GetSharedPtr()->GetLobbyClient()->Connect();
		break;

	case WM_LAUNCH_BATTLE_CLIENT:
		args.m_comment.Add(DisConnectServerChange);
		args.m_gracefulDisconnectTimeoutMs = 1000;
		CWorld::GetSharedPtr()->m_client->m_NetClient->Disconnect(args);
		CWorld::GetSharedPtr()->m_client.Free();
		Sleep(100);
		CWorld::GetSharedPtr()->m_client.Attach(new CBattleClient);
		CWorld::GetSharedPtr()->GetBattleClient()->Connect();
		break;
	}

	switch (m_FormState)
	{
	case ConnectForm:
		return m_connectForm.MsgProc(hWnd, uMsg, wParam, lParam);
	case JoinForm:
		return m_joinForm.MsgProc(hWnd, uMsg, wParam, lParam);
	case ErrorForm:
		return m_errorForm.MsgProc(hWnd, uMsg, wParam, lParam);
	case SelectHeroForm:
		return m_selectHeroForm.MsgProc(hWnd, uMsg, wParam, lParam);
	case NewHeroForm:
		return m_newHeroForm.MsgProc(hWnd, uMsg, wParam, lParam);
	case SelectChannelForm:
		return m_selectChannelForm.MsgProc(hWnd, uMsg, wParam, lParam);
	case LobbyChannelForm:
		return m_lobbyChannelForm.MsgProc(hWnd, uMsg, wParam, lParam);
	case NewRoomForm:
		return m_newRoomForm.MsgProc(hWnd, uMsg, wParam, lParam);
	case BattleWaitingForm:
		return m_battleWaitingForm.MsgProc(hWnd, uMsg, wParam, lParam);
	case BattleForm:
		return m_battleForm.MsgProc(hWnd, uMsg, wParam, lParam);
	}
	return false;
}

/// Once frame process
/// 1회 프레임 처리입니다.
void CWorld::FrameMove(float fElapsedTime)
{

	// 	// 각 클라에 대한 진행 처리를 합니다.
	// 	if (m_FormState == BattleForm)
	// 	{
	// 		LocalHeroes_FrameMove(fElapsedTime);
	// 		MoveCamera(fElapsedTime);
	// 		//AdjustDesiredClientCountOnNeed(fElapsedTime);
	// 	}

	if (m_client)
		m_client->FrameMove(fElapsedTime);
}

// Moves camera position.
// 카메라 위치를 이동시킵니다.
void CWorld::MoveCamera(float fElapsedTime)
{
	// Camera position is following place where local char is located with using postion follower.
	// Adds position follower to PN for that.
	// 카메라 위치는 position follower를 써서 local char가 위치한 곳을 쫓아갑니다.
	// 이를 위해 position follower를 PN에 추가하자.
	CBattleClient* client = GetBattleClient();
	if (client != NULL && client->m_localHero != NULL)
	{
		m_CameraPosition.SetTargetPosition(client->m_localHero->m_position);

		m_CameraPosition.EnableAutoFollowDuration(false);
		m_CameraPosition.SetFollowDuration(0.3);

		m_CameraPosition.SetTargetPosition(client->m_localHero->m_position);


		m_CameraPosition.FrameMove(fElapsedTime);
	}
#ifdef _DEBUG
	Vector3 p = m_CameraPosition.GetFollowerPosition();
	if (p.x > 100000)
	{
		int a = 0;
	}
#endif
}

void CWorld::AdjustDesiredClientCountOnNeed(float fElapsedTime)
{
	// Create or destroy client that depends on number of client you wish.
	// 희망하는 클라 갯수에 따라 점층적으로 클라를 새로 만들거나 이미 있는 클라를 폐기해 나갑니다.
	m_adjustDesiredClientCountCoolTimeMs -= fElapsedTime;
	if (m_adjustDesiredClientCountCoolTimeMs < 0)
	{
		m_adjustDesiredClientCountCoolTimeMs = AdjustDesiredClientCountInterval;

		//AdjustDesiredClientCount();
	}
}

void CWorld::GotoJoinForm()
{
	if (m_FormState != JoinForm)
	{
		m_FormState = JoinForm;
		m_joinForm.GetEditBox(IDC_USER_NAME)->SetText(L"");
		m_joinForm.GetEditBox(IDC_PASSWORD)->SetText(L"");
	}
}
void CWorld::GotoSelectHeroForm()
{
	if (m_FormState != SelectHeroForm)
	{
		m_FormState = SelectHeroForm;
	}

	// Request player character list to server.
	// 플레이어 캐릭터 리스트를 서버에 요청합니다.
	GetEntryClient()->m_c2sProxy.RequestHeroSlots(HostID_Server, RmiContext::ReliableSend);
}

void CWorld::GotoConnectForm()
{
	if (m_FormState != ConnectForm)
	{
		m_FormState = ConnectForm;
		m_connectForm.GetEditBox(IDC_USER_NAME)->SetText(L"");
		m_connectForm.GetEditBox(IDC_PASSWORD)->SetText(L"");
		m_connectForm.GetEditBox(IDC_PASSWORDCHECK)->SetText(L"");

		m_connectForm.GetEditBox(IDC_USER_NAME)->SetEnabled(false);
		m_connectForm.GetEditBox(IDC_PASSWORD)->SetEnabled(false);
		m_connectForm.GetEditBox(IDC_PASSWORDCHECK)->SetEnabled(false);

		m_connectForm.GetButton(IDC_OK)->SetEnabled(false);
		m_connectForm.GetButton(IDC_CANCEL)->SetEnabled(false);
		m_connectForm.GetButton(IDC_CONNECT)->SetEnabled(true);
	}
}

void CWorld::GotoNewHeroForm()
{
	if (m_FormState != NewHeroForm)
	{
		m_FormState = NewHeroForm;
	}
}


void CWorld::GotoSelectChannelForm()
{
	if (m_FormState != SelectChannelForm)
	{
		m_FormState = SelectChannelForm;

		// Request channel list to server.
		// 채널 리스트를 서버에 요청합니다.
		GetEntryClient()->m_c2sProxy.RequestLobbyList(HostID_Server, RmiContext::ReliableSend);
	}
}

void CWorld::GotoLobbyChannelForm(bool withDisconnectingOldServer)
{
	if (m_FormState != LobbyChannelForm)
	{
		m_FormState = LobbyChannelForm;

		if (withDisconnectingOldServer)
		{
			GetLobbyClient()->m_c2sProxy.NotifyChannelFormReady(HostID_Server, RmiContext::ReliableSend);

			// Clean up all old data of lobby form that used before.
			// 앞서 사용했던 로비 폼의 구 데이터들을 모두 청소합니다.
			m_lobbyChannelForm.GetListBox(IDC_ROOM_LISTBOX)->RemoveAllItems();
			m_lobbyChannelForm.GetListBox(IDC_HERO_LISTBOX)->RemoveAllItems();
			m_lobbyChannelForm.GetListBox(IDC_LOG)->RemoveAllItems();

			// Initialize button status
			// 버튼 상태 초기화
			m_lobbyChannelForm.GetButton(IDC_JOIN_ROOM_BUTTON)->SetEnabled(true);
		}
	}
}



void CWorld::GotoNewRoomForm()
{
	if (m_FormState != NewRoomForm)
	{
		m_FormState = NewRoomForm;

		// Clean up all string of old control.
		// 구 컨트롤의 문자열을 모두 청소합니다.
		m_newRoomForm.GetIMEEditBox(IDC_ROOM_NAME)->SetText(NoRoomNameText);
		m_newRoomForm.GetListBox(IDC_ROOM_TYPE)->SelectItem(0);
		m_newRoomForm.GetButton(IDC_OK)->SetEnabled(true);
	}
}

CEntryClient* CWorld::GetEntryClient()
{
	return dynamic_cast<CEntryClient*> (m_client.m_p);
}

CLobbyClient* CWorld::GetLobbyClient()
{
	return dynamic_cast<CLobbyClient*> (m_client.m_p);
}

CBattleClient* CWorld::GetBattleClient()
{
	return dynamic_cast<CBattleClient*> (m_client.m_p);
}


void CWorld::GotoBattleForm()
{
	if (m_FormState != BattleForm)
	{
		m_FormState = BattleForm;
	}
}

void CWorld::Render()
{
	CBattleClient* client = GetBattleClient();

	//battleform일때 그립니다.
	if (client != NULL && m_FormState == BattleForm)
	{
		client->Render();
	}
}

void CWorld::SetCamera()
{
	if (m_client != NULL)
		m_client->SetCamera();
}