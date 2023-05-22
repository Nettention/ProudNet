#include "StdAfx.h"
#include "BattleWaitingForm.h"
#include "world.h"
#include "clientbase.h"
#include "entryclient.h"
#include "BattleClient.h"
#include "Resource.h"

/// Compose game waiting room form.
/// 게임방 대기실 폼을 구성합니다.
void InitBattleWaitingForm()
{
	CWorld::GetSharedPtr()->m_battleWaitingForm.SetCallback( BattleWaitingForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_battleWaitingForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_battleWaitingForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_battleWaitingForm.SetLocation(10, 10);

	CStringW title;
	title.LoadStringW(IDS_STRING132);
	CWorld::GetSharedPtr()->m_battleWaitingForm.AddStatic(IDC_TITLE, title, 0, 0, 200, 20);

	CWorld::GetSharedPtr()->m_battleWaitingForm.AddListBox(IDC_HERO_LISTBOX, 220, 50, 230, 200);

	// "Return"
	// "돌아가기"
	CStringW returnString;
	returnString.LoadStringW(IDS_STRING106);
	// If it is room master then indicate this button to 'Starts Game' otherwise 'NOT READY' or 'READY'.
	// 이 버튼은 방장일 때는 '게임 시작' 버튼이며 방장이 아닐때는 'NOT READY' 'READY'를 표시합니다.
	CWorld::GetSharedPtr()->m_battleWaitingForm.AddButton(IDC_START_BUTTON, L"<START>", 100, 260, 80, 35);
	CWorld::GetSharedPtr()->m_battleWaitingForm.AddButton(IDC_CANCEL, returnString, 200, 260, 80, 35);

	CWorld::GetSharedPtr()->m_battleWaitingForm.AddListBox(IDC_LOG, 0, 300, 400, 130);
	CWorld::GetSharedPtr()->m_battleWaitingForm.AddIMEEditBox(IDC_CHAT_IN, L"", 0, 430, 400, 30);
}

void ResetBattleWaitingForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_battleWaitingForm.SetLocation(10, 10);

	CWorld::GetSharedPtr()->m_battleWaitingForm.SetSize( Width, Height );
}

/// Event handler
/// 이벤트 Handler
void CALLBACK BattleWaitingForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_OK:
		break;
	case IDC_CANCEL:
		// Cancel connecting with battle server and back to lobby server because clicked "Return" button.
		// 뒤로 가기 버튼을 눌렀으므로 배틀 서버와의 연결을 중단하고 로비 서버로 돌아갑니다.
		ATLTRACE("::PostMessage(DXUTGetHWND(), WM_LAUNCH_LOBBY_CLIENT , 0, 0);");
		::PostMessage(DXUTGetHWND(), WM_LAUNCH_LOBBY_CLIENT, 0, 0);
		break;
	case IDC_CHAT_IN:
		switch ( nEvent )
		{
		case EVENT_EDITBOX_STRING:
		{
			// Send chatting string to server and empty chatting entry window.
			// 채팅 문자열을 서버에 보내고 채팅 입력창을 비웁니.
			CStringW chatTxt = ((CDXUTEditBox*)pControl)->GetText();
			((CDXUTEditBox*)pControl)->SetText(L"");

			CWorld::GetSharedPtr()->GetBattleClient()->m_c2cProxy.P2P_Chat(
			    CWorld::GetSharedPtr()->GetBattleClient()->m_p2pGroupID, RmiContext::ReliableSend,
			    chatTxt);

			break;
		}
		}
		break;
	case IDC_START_BUTTON:
		// If it is room master, send game start request to server
		// If it is not room master, send toggle result whether ready or not
		// 방장인 경우 게임 시작 요청을 서버로 전송
		// 방장이 아닌 경우 레디 여부 토글을 서버로 전송
		if (CWorld::GetSharedPtr()->GetBattleClient()->IsLocalHeroRoomMaster())
		{
			CWorld::GetSharedPtr()->GetBattleClient()->m_c2sProxy.RequestStartPlayMode(HostID_Server, RmiContext::ReliableSend);
			CWorld::GetSharedPtr()->GetBattleClient()->m_startPlayModeUnderProgress = true;
			BattleWaitingForm_RefreshButtons();

		}
		else
		{
			CWorld::GetSharedPtr()->GetBattleClient()->m_c2sProxy.RequestToggleBattleReady(HostID_Server, RmiContext::ReliableSend);
		}
		break;
	}
}


void BattleWaitingForm_RefreshButtons()
{
	// Renew state of buttons that depends on gamer list, room information.
	// For example, it indicates status of button whether local player is room master or not.
	// 게이머 리스트, 방 정보에 입각해서 버튼들의 상태를 갱신합니다.
	// 예를 들어 로컬 플레이어가 방장이냐 여부에 따라 버튼의 상태를 표시합니다.
	CDXUTButton* btn = CWorld::GetSharedPtr()->m_battleWaitingForm.GetButton(IDC_START_BUTTON);

	if (CWorld::GetSharedPtr()->GetBattleClient()->IsLocalHeroRoomMaster())
	{
		CStringW startString;
		startString.LoadString(IDS_STRING133);
		btn->SetText(startString);
		btn->SetEnabled(CWorld::GetSharedPtr()->GetBattleClient()->IsEveryHeroPlayReady() &&
		                !CWorld::GetSharedPtr()->GetBattleClient()->m_startPlayModeUnderProgress);
	}
	else
	{
		btn->SetText(L"Ready?");
		btn->SetEnabled(true);
		CBattleHeroSlot* localheroSlot = CWorld::GetSharedPtr()->GetBattleClient()->GetLocalHeroSlot();
		if (localheroSlot)
		{
			if (localheroSlot->m_info.m_gamingState == GamingState_Ready)
				btn->SetText(L"Ready!");
		}
	}
}
