#include "StdAfx.h"
#include "ConnectForm.h"
#include "world.h"
#include "clientbase.h"
#include "entryclient.h"
#include "resource.h"

/// Gets lastest entered game ID by gamer from registry.
/// 가장 마지막에 게이머가 입력했던 게이머 ID를 레지스트리로부터 얻습니다.
CStringW GetLastUsedID()
{
	CRegKey reg;
	if (ERROR_SUCCESS != reg.Open(HKEY_CURRENT_USER, L"Software\\Nettention\\Casual Game Sample"))
		return L"";
	WCHAR output[1000];
	output[0] = 0;
	ULONG outputLen = 999;
	reg.QueryStringValue(L"LastUserID", output, &outputLen);
	return output;
}

/// Compose server connecting form.
/// 서버에 연결하는 폼을 구성합니다.
void InitConnectForm()
{

	CWorld::GetSharedPtr()->m_connectForm.SetCallback( ConnectForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_connectForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_connectForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_connectForm.SetLocation(180, 180);

	CStringW title;
	title.LoadStringW(IDS_STRING134);
	CWorld::GetSharedPtr()->m_connectForm.AddStatic(IDC_SERVER_ADDR_STATIC, title, 0, 0, 200, 20);
	
	CStringW addressString;
	addressString.LoadStringW(IDS_STRING138);
	CWorld::GetSharedPtr()->m_connectForm.AddStatic(IDC_SERVER_ADDR_STATIC, addressString, 0, 20, 200, 20);
	CWorld::GetSharedPtr()->m_connectForm.GetControl(IDC_SERVER_ADDR_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	CWorld::GetSharedPtr()->m_connectForm.GetControl(IDC_SERVER_ADDR_STATIC)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
	CWorld::GetSharedPtr()->m_connectForm.AddButton(IDC_CONNECT,L"Connect",220,40,80,35,VK_RETURN,true);

	CStringW nameString;
	nameString.LoadStringW(IDS_STRING135);
	CWorld::GetSharedPtr()->m_connectForm.AddStatic(IDC_USER_NAME_STATIC, nameString, 0, 75, 200, 20);
	CWorld::GetSharedPtr()->m_connectForm.GetControl(IDC_USER_NAME_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	CWorld::GetSharedPtr()->m_connectForm.GetControl(IDC_USER_NAME_STATIC)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	CWorld::GetSharedPtr()->m_connectForm.AddStatic(IDC_PASSWORD_STATIC, L"Password", 0, 125, 200, 20);
	CWorld::GetSharedPtr()->m_connectForm.GetControl(IDC_PASSWORD_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	CWorld::GetSharedPtr()->m_connectForm.GetControl(IDC_PASSWORD_STATIC)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	CStringW rePassword;
	rePassword.LoadStringW(IDS_STRING136);
	CWorld::GetSharedPtr()->m_connectForm.AddStatic(IDC_PASSWORDCHECK_STATIC, rePassword, 0, 175, 200, 20);
	CWorld::GetSharedPtr()->m_connectForm.GetControl(IDC_PASSWORDCHECK_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	CWorld::GetSharedPtr()->m_connectForm.GetControl(IDC_PASSWORDCHECK_STATIC)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	// "Register"
	// "가입"
	CStringW okString;
	okString.LoadStringW(IDS_STRING108);
	// "Return"
	// "돌아가기"
	CStringW outString;
	outString.LoadStringW(IDS_STRING106);
	// "Go to Logon..."
	// "로그인창으로..."
	CStringW goToLogonString;
	goToLogonString.LoadStringW(IDS_STRING110);
	CWorld::GetSharedPtr()->m_connectForm.AddButton( IDC_OK, okString, 40, 250, 80, 35, VK_RETURN,true );
	CWorld::GetSharedPtr()->m_connectForm.AddButton( IDC_CANCEL, outString, 120, 250, 80, 35, VK_ESCAPE );
	CWorld::GetSharedPtr()->m_connectForm.AddButton( IDC_GOTOLOGON, goToLogonString,300,250,100,35,VK_RETURN,true);

	CWorld::GetSharedPtr()->m_connectForm.AddEditBox( IDC_SERVER_ADDR, L"localhost", 0, 40, 200, 30);
	CWorld::GetSharedPtr()->m_connectForm.AddEditBox( IDC_USER_NAME, L"", 0, 90, 200, 30);
	CWorld::GetSharedPtr()->m_connectForm.AddEditBox( IDC_PASSWORD, L"", 0, 145, 200, 30);
	CWorld::GetSharedPtr()->m_connectForm.AddEditBox( IDC_PASSWORDCHECK, L"", 0, 195, 200, 30);
	
	CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_USER_NAME)->SetEnabled(false);
	CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_PASSWORD)->SetEnabled(false);
	CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_PASSWORDCHECK)->SetEnabled(false);
	CWorld::GetSharedPtr()->m_connectForm.GetButton(IDC_OK)->SetEnabled(false);
	CWorld::GetSharedPtr()->m_connectForm.GetButton(IDC_CANCEL)->SetEnabled(false);

}

void ResetConnectForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_connectForm.SetLocation(180, 180);

	CWorld::GetSharedPtr()->m_connectForm.SetSize( Width, Height );

}

/// Save entered gamer ID by gamer to registry.
/// 게이머가 입력했던 게이머 ID를 레지스트리에 저장합니다.
void SetLastUsedID(CStringW userID)
{
	CRegKey reg;
	reg.Create(HKEY_CURRENT_USER, L"Software\\Nettention\\Casual Game Sample2");
	reg.Open(HKEY_CURRENT_USER, L"Software\\Nettention\\Casual Game Sample2");
	reg.SetStringValue(L"LastUserID", userID);
}

/// Event handler at server connecting form
/// 서버 연결 폼에서의 이벤트 Handler
void CALLBACK ConnectForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{

	case IDC_CONNECT:
		{	
			CWorld::GetSharedPtr()->m_ServerAddr = CW2T(CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_SERVER_ADDR)->GetText());
			if(CWorld::GetSharedPtr()->m_client==NULL)
				CWorld::GetSharedPtr()->m_client.Attach(new CEntryClient);

			bool ret = CWorld::GetSharedPtr()->GetEntryClient()->Connect();// Once it succeed then keep connection with server...   //성공하면 서버에는 연결된 상태 유지...
			if (!ret)
			{
				// "Wrong server address"
				// "잘못된 서버 주소입니다."
				CStringW logString;
				logString.LoadStringW(IDS_STRING107);
				CWorld::GetSharedPtr()->GotoErrorForm(logString);
				break;
			} 
			CDXUTButton *pButton = CWorld::GetSharedPtr()->m_connectForm.GetButton(IDC_CONNECT);
			pButton->SetEnabled(false);
		}
		break;
	case IDC_GOTOLOGON:
		{
			if (CWorld::GetSharedPtr()->m_isConnectEntryServer == true) 
			{
				CWorld::GetSharedPtr()->GotoJoinForm();
			}
			else
			{
				// "Does not connected to server"
				// "서버에 접속되지 않았습니다."
				CStringW logString;
				logString.LoadStringW(IDS_STRING111);
				AfxMessageBox(logString, MB_OK);
			}

		}
		break;
	case IDC_OK:
		{
			CWorld::GetSharedPtr()->m_UserName = CW2T(CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_USER_NAME)->GetText());
			CWorld::GetSharedPtr()->m_Password = CW2T(CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_PASSWORD)->GetText());
			CWorld::GetSharedPtr()->m_CheckedPassword = CW2T(CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_PASSWORDCHECK)->GetText());

			String UserName;
			UserName = (CWorld::GetSharedPtr()->m_UserName).GetString();

			CStringW password1 = CWorld::GetSharedPtr()->m_Password;
			CStringW password2 = CWorld::GetSharedPtr()->m_CheckedPassword;

			if(password1.Compare(password2) == 0) 
			{
				String Password = password1.GetString();

				RmiContext rmi = RmiContext::ReliableSend;
				rmi.m_encryptMode = EM_Secure;
				CWorld::GetSharedPtr()->GetEntryClient()->m_c2sProxy.RequestCreateNewGamer(HostID_Server, rmi, UserName,Password);
				// To prevent repeat connection attempt
				// 중복 연결 시도를 방지하기 위해
				CWorld::GetSharedPtr()->m_connectForm.GetButton(IDC_OK)->SetEnabled(false);
			}
			else 
			{
				// "Password does not match"
				// "이전입력한 비밀번호와 일치하지 않습니다."
				CStringW logString;
				logString.LoadStringW(IDS_STRING112);
				AfxMessageBox(logString, MB_OK);
				CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_PASSWORD)->SetText(L"");
				CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_PASSWORDCHECK)->SetText(L"");
			}
		}
		break;
	case IDC_CANCEL:
		::PostQuitMessage(0);
		break;

	}
}
