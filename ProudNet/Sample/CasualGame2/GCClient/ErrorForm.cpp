#include "StdAfx.h"
#include "DXUTForm.h"
#include "ErrorForm.h"
#include "world.h"

/// Compose form when it appears quit situation with error.
/// 에러가 나서 종료해야 하는 상황에 나타나는 폼을 구성합니다.
void InitErrorForm()
{
	CWorld::GetSharedPtr()->m_errorForm.AddButton( IDC_OK, L"OK", 78, 62, 102, 37, VK_RETURN);
	CWorld::GetSharedPtr()->m_errorForm.SetCallback( ErrorForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_errorForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_errorForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_errorForm.SetLocation(180, 180);

	CWorld::GetSharedPtr()->m_errorForm.AddStatic(IDC_STATIC, L"<String Error>", 0, 0, 257, 48);
	CWorld::GetSharedPtr()->m_errorForm.GetControl(IDC_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	CWorld::GetSharedPtr()->m_errorForm.GetControl(IDC_STATIC)->GetElement(0)->dwTextFormat = DT_CENTER | DT_VCENTER | DT_WORDBREAK;

}

void ResetErrorForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_errorForm.SetLocation(180, 180);
	CWorld::GetSharedPtr()->m_errorForm.SetSize( Width, Height );
}


/// Event handler at error indication form
/// 에러 표시 폼에서의 이벤트 Handler
void CALLBACK ErrorForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_OK:
	{
		::PostQuitMessage(0);
	}
	break;
	case IDC_CANCEL:
		::PostQuitMessage(0);
		break;
	}
}