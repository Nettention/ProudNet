// ChatClient.cpp : Define class motion about application program.
// ChatClient.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatClientApp

BEGIN_MESSAGE_MAP(CChatClientApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// Create CChatClientApp
// CChatClientApp 생성

CChatClientApp::CChatClientApp()
{
	// TODO: Add creation code to here.
	// Place all important initialize work to InitInstance.
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// Only CChatClientApp object.
// 유일한 CChatClientApp 개체입니다.

CChatClientApp theApp;


// Initialize CChatClientApp
// CChatClientApp 초기화

BOOL CChatClientApp::InitInstance()
{

	// If application program manifest assign using visual style with ComCtl32.dll version 6 or higher then InitCommonControls() is necessary on Windows XP.
	// You cannot create window without InitCommonControls().
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(L"HKEY_CURRENT_USER\\Software\\Nettention\\ProudNet\\Chat Sample\\Client");

	// for MoveWindowALittle()
	time_t t;
	srand((unsigned)time(&t));


	CChatClientDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Dialog box is closed so do not start message pump of application program and return "FALSE" to quit application program.
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고
	// 응용 프로그램을 끝낼 수 있도록 FALSE를 반환합니다.
	return FALSE;
}

