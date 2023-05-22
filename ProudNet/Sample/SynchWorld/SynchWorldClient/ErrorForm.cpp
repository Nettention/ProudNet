#include "StdAfx.h"
#include "DXUTForm.h"
#include "ErrorForm.h"

// 에러 표시 폼에서의 이벤트 핸들러
// Event handler at error report form
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
