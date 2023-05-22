// ChatServer.cpp : Define class motion about application program.
// ChatServer.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "../../../include/tracer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatServerApp

BEGIN_MESSAGE_MAP(CChatServerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChatServerApp 생성
// Create CChatServerApp

CChatServerApp::CChatServerApp()
{
	// TODO: Adds creation code to here.
	// Place all improtant initialize work to InitInstance.

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// Only CChatServerApp object
// 유일한 CChatServerApp 개체입니다.

CChatServerApp theApp;


// Initialize CChatServerApp
// CChatServerApp 초기화

BOOL CChatServerApp::InitInstance()
{
	// If application program manifest assign using visual style with Comctl32.dll version 6 or higher then InitCommonControls() is necessary on Windows XP.
	// You cannot create window without InitCommonControls().

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다.
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(L"HKEY_CURRENT_USER\\Software\\Nettention\\ProudNet\\Chat Sample\\Server");

	// for MoveWindowALittle()
	time_t t;
	srand((unsigned)time(&t));

	/*Proud::CTracer::Instance().Enable(Proud::TID_Holepunch);*/
	CChatServerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Dialog box is closed so do not start message pump of application program and return "FALSE" to quit application program.
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고
	// 응용 프로그램을 끝낼 수 있도록 FALSE를 반환합니다.
	return FALSE;
}
