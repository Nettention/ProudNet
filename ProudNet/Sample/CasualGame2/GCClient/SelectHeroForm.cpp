#include "StdAfx.h"
#include "SelectHeroForm.h"
#include "world.h"
#include "entryclient.h"
#include "resource.h"

/// Compose character selection form.
/// 캐릭터 선택 폼을 구성합니다.
void InitSelectHeroForm()
{
	CWorld::GetSharedPtr()->m_selectHeroForm.SetCallback( SelectHeroForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_selectHeroForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_selectHeroForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_selectHeroForm.SetLocation(50, 50);

	// Select player character.
	// 플레이어 캐릭터를 선택하세요.
	CStringW selectString;
	selectString.LoadStringW(IDS_STRING123);
	CWorld::GetSharedPtr()->m_selectHeroForm.AddStatic(IDC_SELECT_HERO_STATIC, selectString, 0, 0, 200, 20);

	CWorld::GetSharedPtr()->m_selectHeroForm.AddListBox(IDC_HERO_LISTBOX, 0, 40, 200, 200);

	// Deactivate character select button until receiving player character list from server.
	// 서버에서 플레이어 캐릭터 리스트를 받기 전까지는 캐릭터 선택 버튼을 비활성화시킵니.
	CWorld::GetSharedPtr()->m_selectHeroForm.AddButton( IDC_OK, L"Ok", 0, 300, 90, 35, VK_RETURN );
	CWorld::GetSharedPtr()->m_selectHeroForm.GetButton( IDC_OK)->SetEnabled(false);

	// "Create new character"
	// "새 캐릭터 만들기"
	CStringW createHeroString;
	createHeroString.LoadStringW(IDS_STRING121);
	// "Delete character"
	// "캐릭터 없애기"
	CStringW deleteHeroString;
	deleteHeroString.LoadStringW(IDS_STRING122);
	CWorld::GetSharedPtr()->m_selectHeroForm.AddButton( IDC_CANCEL, L"Back", 100, 300, 90, 35, VK_ESCAPE );
	CWorld::GetSharedPtr()->m_selectHeroForm.AddButton( IDC_NEW_HERO_BUTTON, createHeroString, 200, 300, 90, 35, VK_ESCAPE );
	CWorld::GetSharedPtr()->m_selectHeroForm.AddButton( IDC_REMOVE_HERO_BUTTON, deleteHeroString, 300, 300, 90, 35, VK_ESCAPE );
}

void ResetSelectHeroForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_selectHeroForm.SetLocation(50, 50);

	CWorld::GetSharedPtr()->m_selectHeroForm.SetSize( Width, Height );

}

/// Event handler
/// 이벤트 Handler 입니다.
void CALLBACK SelectHeroForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_OK:
		// Find out selected character and notice that to server then move to channel selection form.
		// 선택한 캐릭터를 찾아서 그 캐릭터를 선택했습니다고 서버에 보낸 후 채널 선택 폼으로 천이합니다.
		if (CWorld::GetSharedPtr()->m_selectHeroForm.GetListBox(IDC_HERO_LISTBOX)->GetSelectedItem())
		{
			CEntryLocalHeroSlot* heroSlot = (CEntryLocalHeroSlot*)CWorld::GetSharedPtr()->m_selectHeroForm.GetListBox(IDC_HERO_LISTBOX)->GetSelectedItem()->pData;
			Guid heroGuid = heroSlot->m_guid;
			if (heroGuid != Guid())
			{
				CWorld::GetSharedPtr()->GetEntryClient()->m_c2sProxy.RequestSelectHero(HostID_Server, RmiContext::ReliableSend, heroGuid);
				// Deactivate button to prevent repeat clicking.
				// 중복해서 누르지 못하게 버튼을 비활성화합니다.
				CWorld::GetSharedPtr()->m_selectHeroForm.GetButton(IDC_OK)->SetEnabled(false);
			}
		}
		break;
	case IDC_CANCEL:
		{
			// Disconnect with server and return to connection form because clicked "Return" button.
			// 뒤로 가기 버튼을 눌렀으므로 서버와의 연결을 끊고 연결 폼으로 되돌아갑니다.
			CDisconnectArgs args;
			args.m_comment.Add(DisConnectServerChange);
			args.m_gracefulDisconnectTimeoutMs = 1000;
			CWorld::GetSharedPtr()->m_client->m_NetClient->Disconnect(args);
			CWorld::GetSharedPtr()->m_client.Free();
			Sleep(100);
			CWorld::GetSharedPtr()->m_isConnectEntryServer = false;
			CWorld::GetSharedPtr()->GotoConnectForm();
			break;
		}
	case IDC_NEW_HERO_BUTTON:
		// Move to new character creation form.
		// 새 캐릭터 생성 폼으로 이동합니다.
		CWorld::GetSharedPtr()->GotoNewHeroForm();
		break;
	case IDC_REMOVE_HERO_BUTTON:
		// Sends request about removal selected character to server.
		// 선택한 캐릭터를 제거하는 요청을 서버에 보냅니다.
		if (CWorld::GetSharedPtr()->m_selectHeroForm.GetListBox(IDC_HERO_LISTBOX)->GetSelectedItem())
		{
			CEntryLocalHeroSlot* heroSlot = (CEntryLocalHeroSlot*)CWorld::GetSharedPtr()->m_selectHeroForm.GetListBox(IDC_HERO_LISTBOX)->GetSelectedItem()->pData;
			Guid heroGuid = heroSlot->m_guid;
			if (heroGuid != Guid())
			{
				CWorld::GetSharedPtr()->GetEntryClient()->m_c2sProxy.RequestRemoveHero(HostID_Server, RmiContext::ReliableSend, heroGuid);
				// Deactivate button to prevent repeat clicking.
				// 중복해서 누르지 못하게 버튼을 비활성화합니다.
				CWorld::GetSharedPtr()->m_selectHeroForm.GetButton(IDC_REMOVE_HERO_BUTTON)->SetEnabled(false);
			}
		}
		break;
	}
}

