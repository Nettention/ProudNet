#include "StdAfx.h"
#include "JoinForm.h"
#include "world.h"
#include "clientbase.h"
#include "entryclient.h"
#include "resource.h"


/// Compose logon form to server.
/// 서버에 로그인하는 폼을 구성합니다.
void InitJoinForm()
{
	CStringW logonString;
	logonString.LoadStringW(IDS_STRING139);
	CWorld::GetSharedPtr()->m_joinForm.AddStatic(IDC_SERVER_ADDR_STATIC, logonString, 0, 0, 200, 20);

	CWorld::GetSharedPtr()->m_joinForm.SetCallback( JoinForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_joinForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_joinForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_joinForm.SetLocation(180, 180);

	CStringW nameString;
	nameString.LoadStringW(IDS_STRING135);
	CWorld::GetSharedPtr()->m_joinForm.AddStatic(IDC_USER_NAME_STATIC, nameString, 0,30, 200, 20);
	CWorld::GetSharedPtr()->m_joinForm.GetControl(IDC_USER_NAME_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	CWorld::GetSharedPtr()->m_joinForm.GetControl(IDC_USER_NAME_STATIC)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
	

	CWorld::GetSharedPtr()->m_joinForm.AddStatic(IDC_PASSWORD_STATIC, L"Password", 0, 100, 200, 20);
	CWorld::GetSharedPtr()->m_joinForm.GetControl(IDC_PASSWORD_STATIC)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	CWorld::GetSharedPtr()->m_joinForm.GetControl(IDC_PASSWORD_STATIC)->GetElement(0)->dwTextFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;

	CWorld::GetSharedPtr()->m_joinForm.AddEditBox( IDC_USER_NAME, L"", 0, 60, 200, 30);
	CWorld::GetSharedPtr()->m_joinForm.AddEditBox( IDC_PASSWORD, L"", 0, 130, 200, 30);
	
	CWorld::GetSharedPtr()->m_joinForm.AddButton( IDC_OK, logonString, 40, 200, 80, 35, VK_RETURN,true );
	
	CStringW backString;
	backString.LoadStringW(IDS_STRING106);
	CWorld::GetSharedPtr()->m_joinForm.AddButton( IDC_CANCEL, backString, 120, 200, 80, 35, VK_ESCAPE );



	
}

void ResetJoinForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_joinForm.SetLocation(180, 180);

	CWorld::GetSharedPtr()->m_joinForm.SetSize( Width, Height );

}



/// Event handler at server connection form
/// 서버 연결 폼에서의 이벤트 Handler
void CALLBACK JoinForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_OK:// Logon request  //로그인요청
	{
		CWorld::GetSharedPtr()->m_UserName = CW2T(CWorld::GetSharedPtr()->m_joinForm.GetEditBox(IDC_USER_NAME)->GetText());
		CWorld::GetSharedPtr()->m_Password = CW2T(CWorld::GetSharedPtr()->m_joinForm.GetEditBox(IDC_PASSWORD)->GetText());
		String UserName = CWorld::GetSharedPtr()->m_UserName.GetString();
		String password = CWorld::GetSharedPtr()->m_Password.GetString();

		RmiContext rmi = RmiContext::ReliableSend;
		rmi.m_encryptMode = EM_Secure;
		CWorld::GetSharedPtr()->GetEntryClient()->m_c2sProxy.RequestFirstLogon(HostID_Server,rmi,UserName,password);
		
	}
	break;
	case IDC_CANCEL:
		CDisconnectArgs args;
		args.m_comment.Add(DisConnectLogout);
		args.m_gracefulDisconnectTimeoutMs = 1000;

		CWorld::GetSharedPtr()->m_client->m_NetClient->Disconnect(args);
		CWorld::GetSharedPtr()->m_client.Free();
        CWorld::GetSharedPtr()->m_isConnectEntryServer = false;
		CWorld::GetSharedPtr()->GotoConnectForm();
		break;
	}
}
