// GCServer.cpp : Define class progress about application program.
// GCServer.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "GCServer.h"
#include "GCServerDlg.h"
#include "ServerBase.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// CGCServerApp

BEGIN_MESSAGE_MAP(CGCServerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGCServerApp 생성

CGCServerApp::CGCServerApp()
{
	// TODO: Adds creation code to here.
	// Place all important initialize work to InitInstance.
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// Only CGCServerApp object.
// 유일한 CGCServerApp 개체입니다.

CGCServerApp theApp;


// Initialize CGCServerApp
// CGCServerApp 초기화

BOOL CGCServerApp::InitInstance()
{
	// If application program manifest assign using visual style with Comctl32.dll version 6 or higher then InitCommonControls() is necessary on Windows XP.
	// You cannot create window without InitCommonControls().
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다.
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(60981);
	

	// Standard initialization
	// If you want to reduce final excution file size without this function then delete specific initialize routine.
	// Change registry key that saved specific setting.
	// TODO: Please modify this string to name of company or organization.
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(L"로컬 응용 프로그램 마법사에서 생성한 응용 프로그램");

	CCoInitializer coinit;

	CGCServerDlg dlg;
	dlg.m_debugging = false;
	m_pMainWnd = &dlg;

	// If it is first time running server then run Entry server, Lobby server, Battle server. And this process does role of Farm server.
	// 서버를 처음 실행하는 것이면 Entry 서버, Lobby 서버, Battle 서버를 실행시킵니다. 그리고 본 프로세스는 Farm 서버 역할을 합니다.
	dlg.GetLaunchSettingsByCommandLine(m_lpCmdLine);

	dlg.DoModal();

	// Dialog box is closed so do not start message pump of application program and return "FALSE" to quit application program.
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고
	// 응용 프로그램을 끝낼 수 있도록 FALSE를 반환합니다.
	return FALSE;
}

