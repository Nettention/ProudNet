#include "StdAfx.h"
#include "NewHeroForm.h"
#include "world.h"
#include "entryclient.h"
#include "resource.h"

/// Compose form UI that creates new character.
/// 새 캐릭터를 만드는 폼 UI를 구성합니다.
void InitNewHeroForm()
{
	CWorld::GetSharedPtr()->m_newHeroForm.SetCallback( NewHeroForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_newHeroForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_newHeroForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_newHeroForm.SetLocation(100, 100);

	// "Creates new player character"
	// "새 플레이어 캐릭터를 만듭니다"
	CStringW createNewHeroString;
	createNewHeroString.LoadStringW(IDS_STRING118);
	CWorld::GetSharedPtr()->m_newHeroForm.AddStatic(IDC_SELECT_HERO_STATIC, createNewHeroString, 0, 0, 200, 20);

	CWorld::GetSharedPtr()->m_newHeroForm.AddRadioButton(IDC_HERO_TYPE1, 1, L"Type 1", 0, 50, 220, 24, true);
	CWorld::GetSharedPtr()->m_newHeroForm.AddRadioButton(IDC_HERO_TYPE2, 1, L"Type 2", 0, 80, 220, 24, true);

	CWorld::GetSharedPtr()->m_newHeroForm.AddEditBox(IDC_HERO_NAME, L"", 0, 110, 220, 30);

	CWorld::GetSharedPtr()->m_newHeroForm.AddButton( IDC_OK, L"Ok", 0, 150, 80, 35, VK_RETURN );
	CWorld::GetSharedPtr()->m_newHeroForm.GetButton(IDC_OK)->SetEnabled(true);

	CWorld::GetSharedPtr()->m_newHeroForm.AddButton( IDC_CANCEL, L"Cancel", 100, 150, 80, 35, VK_ESCAPE );

	CWorld::GetSharedPtr()->m_newHeroForm.AddStatic(IDC_ERROR_TEXT, L"", 0, 200, 420, 24);

}

void ResetNewHeroForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_newHeroForm.SetLocation(100, 100);

	CWorld::GetSharedPtr()->m_newHeroForm.SetSize( Width, Height );

}

/// Event handler
/// 이벤트 Handler 입니다.
void CALLBACK NewHeroForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_OK:
	{
		// Check type selection and setting of player character name whether correct or not.
		// 타입 선택과 플레이어 캐릭터 이름 설정이 제대로 되어있는지 체크합니다.
		CStringW heroName(CWorld::GetSharedPtr()->m_newHeroForm.GetEditBox(IDC_HERO_NAME)->GetText());

		int heroType = 0;
		if (CWorld::GetSharedPtr()->m_newHeroForm.GetRadioButton(IDC_HERO_TYPE1)->GetChecked())
			heroType = 1;
		if (CWorld::GetSharedPtr()->m_newHeroForm.GetRadioButton(IDC_HERO_TYPE2)->GetChecked())
			heroType = 2;

		// Send type selelction and player character name to server.
		// 타입 선택과 플레이어 캐릭터 이름을 서버에 보냅니다.
		if (heroName.GetLength() > 0 && heroType > 0)
		{
			CWorld::GetSharedPtr()->GetEntryClient()->m_c2sProxy.RequestAddHero(
			    HostID_Server, RmiContext::ReliableSend, heroName, heroType);

			// Deactivate button. This button keep devactivating until receiving request.
			// 버튼을 비활성화시킵니다. 이 버튼은 응답이 올 때까지 비활성을 유지합니다.
			pControl->SetEnabled(false);
		}
	}
	break;
	case IDC_CANCEL:
		// Clicked "Return" button so goes to character selection form.
		// 뒤로 가기 버튼을 눌렀으므로 캐릭터 선택 폼으로 되돌아갑니다.
		CWorld::GetSharedPtr()->GotoSelectHeroForm();
		break;

	case IDC_NEW_HERO_BUTTON:
		// Moves to new character creation form.
		// 새 캐릭터 생성 폼으로 이동합니다.
		CWorld::GetSharedPtr()->GotoNewHeroForm();
		break;
	}
}

