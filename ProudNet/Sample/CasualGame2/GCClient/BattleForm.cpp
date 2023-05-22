#include "StdAfx.h"
#include "BattleForm.h"
#include "world.h"
#include "clientbase.h"
#include "entryclient.h"
#include "BattleClient.h"
#include "resource.h"

/// Ready UI form during battle.
/// 전투중 UI 폼을 준비합니다.
void InitBattleForm()
{
	CWorld::GetSharedPtr()->m_battleForm.SetCallback( BattleForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_battleForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_battleForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_battleForm.SetLocation(10, 10);

	// "Playing"
	// "게임플레이중"
	CStringW playString;
	playString.LoadStringW(IDS_STRING103);
	// "Exit"
	// "방나가기"
	CStringW goOutString;
	goOutString.LoadStringW(IDS_STRING104);
	// "Return to lobby"
	// "로비로 돌아가기"
	CStringW returnLobbyString;
	returnLobbyString.LoadStringW(IDS_STRING105);
	CWorld::GetSharedPtr()->m_battleForm.AddStatic(IDC_TITLE, playString, 200, 0, 200, 20);
	CWorld::GetSharedPtr()->m_battleForm.AddButton(IDC_LEAVE_BATTLEROOM, goOutString, 200, 20, 100, 40);
	CWorld::GetSharedPtr()->m_battleForm.AddButton(IDC_CANCEL, returnLobbyString, 300, 20, 100, 40);
}

void ResetBattleForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_battleForm.SetLocation(0, 0);

	CWorld::GetSharedPtr()->m_battleForm.SetSize( Width, Height );
}

// Event handler at server connecting form
// 서버 연결 폼에서의 이벤트 Handler
void CALLBACK BattleForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_CANCEL:
		// Cancel connecting with battle server and back to lobby server because clicked "Return" button.
		// 뒤로 가기 버튼을 눌렀으므로 배틀 서버와의 연결을 중단하고 로비 서버로 돌아갑니다.
		CWorld::GetSharedPtr()->GetBattleClient()->m_c2sProxy.NotifyGotoLobbyServer(HostID_Server,RmiContext::ReliableSend);
		ATLTRACE("::PostMessage(DXUTGetHWND(), WM_LAUNCH_LOBBY_CLIENT , 0, 0);");
		::PostMessage(DXUTGetHWND(), WM_LAUNCH_LOBBY_CLIENT, 0, 0);
		break;
	case IDC_LEAVE_BATTLEROOM:
		// Request mission clear to server.
		// 미션 클리어를 서버에 요청합니다.
		CWorld::GetSharedPtr()->GetBattleClient()->m_c2sProxy.LeaveBattleRoom(HostID_Server, RmiContext::ReliableSend);
		break;
	}
}
