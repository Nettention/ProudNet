#include "StdAfx.h"
#include "NewRoomForm.h"
#include "world.h"
#include "battleclient.h"
#include "lobbyclient.h"
#include "resource.h"

/// Compose game room creation form UI.
/// 게임방 만들기 폼 UI를 구성합니다.
void InitNewRoomForm()
{
	CWorld::GetSharedPtr()->m_newRoomForm.SetCallback( NewRoomForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_newRoomForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_newRoomForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_newRoomForm.SetLocation(180, 180);

	CStringW creatRoomString;
	creatRoomString.LoadStringW(IDS_STRING128);
	CWorld::GetSharedPtr()->m_newRoomForm.AddStatic(IDC_SELECT_HERO_STATIC, creatRoomString, 0, 0, 200, 20);

	CWorld::GetSharedPtr()->m_newRoomForm.AddIMEEditBox(IDC_ROOM_NAME, L"", 0, 50, 200, 30);
	CWorld::GetSharedPtr()->m_newRoomForm.AddListBox(IDC_ROOM_TYPE, 0, 100, 100, 70);
	CWorld::GetSharedPtr()->m_newRoomForm.GetListBox(IDC_ROOM_TYPE)->AddItem(L"Room Type 1", (void*)1L);
	CWorld::GetSharedPtr()->m_newRoomForm.GetListBox(IDC_ROOM_TYPE)->AddItem(L"Room Type 2", (void*)2L);

	CStringW maxUserString;
	maxUserString.LoadStringW(IDS_STRING129);
	CWorld::GetSharedPtr()->m_newRoomForm.AddStatic(IDC_STATIC_LOG, maxUserString, 0, 200, 80, 24);
	CWorld::GetSharedPtr()->m_newRoomForm.AddEditBox(IDC_MAX_GAMER_COUNT, L"8", 100, 200, 80, 30);

	CWorld::GetSharedPtr()->m_newRoomForm.AddButton( IDC_OK, L"OK", 0, 230, 80, 35, VK_RETURN );

	CWorld::GetSharedPtr()->m_newRoomForm.AddButton( IDC_CANCEL, L"Cancel", 100, 230, 80, 35, VK_ESCAPE );
}

void ResetNewRoomForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_newRoomForm.SetLocation(180, 180);

	CWorld::GetSharedPtr()->m_newRoomForm.SetSize( Width, Height );

}

/// Event handler
/// 이벤트 Handler 입니다.
void CALLBACK NewRoomForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_OK:
	{
		// Check type selection and room title setup whether it is correct or not.
		// 타입 선택과 방 제목 설정이 제대로 되어있는지 체크합니다.
		CStringW roomName = CWorld::GetSharedPtr()->m_newRoomForm.GetIMEEditBox(IDC_ROOM_NAME)->GetText();
		roomName = roomName.Trim();
		int roomType = 0;
		if (CWorld::GetSharedPtr()->m_newRoomForm.GetListBox(IDC_ROOM_TYPE)->GetSelectedItem())
			roomType = (int)CWorld::GetSharedPtr()->m_newRoomForm.GetListBox(IDC_ROOM_TYPE)->GetSelectedItem()->pData;

		int maxGamerCount = _wtoi(CWorld::GetSharedPtr()->m_newRoomForm.GetEditBox(IDC_MAX_GAMER_COUNT)->GetText());

		// Sends type selection and player character name to server.
		// 타입 선택과 플레이어 캐릭터 이름을 서버에 보냅니다.
		if (roomName.GetLength() > 0 && roomType > 0 && maxGamerCount >= 1)
		{
			CGameRoomParameter roomParam;
			roomParam.m_name = roomName;
			roomParam.m_type = roomType;
			roomParam.m_maxGamerCount = maxGamerCount;
			CWorld::GetSharedPtr()->GetLobbyClient()->m_c2sProxy.RequestCreateGameRoom(
			    HostID_Server, RmiContext::ReliableSend,
			    roomParam);

			// Deactivate button. This button keep deactivating until it receive request.
			// 버튼을 비활성화시킵니. 이 버튼은 응답이 올 때까지 비활성을 유지합니다.
			pControl->SetEnabled(false);
		}
	}
	break;
	case IDC_CANCEL:
		// Back to lobby main form.
		// 로비 메인 폼으로 돌아갑니다.
		CWorld::GetSharedPtr()->GotoLobbyChannelForm(false);
		break;
	}
}

