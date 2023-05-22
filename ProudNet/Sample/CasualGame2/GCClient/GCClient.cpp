// GCClient.cpp : Define class processing about application program.
// GCClient.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "GCClient.h"
#include "GCClientDlg.h"
#include "BattleForm.h"
#include "world.h"
#include "entryclient.h"
#include "lobbyclient.h"
#include "battleclient.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// CGCClientApp

BEGIN_MESSAGE_MAP(CGCClientApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// Creates CGCClientApp
// CGCClientApp 생성

CGCClientApp::CGCClientApp()
{
	// TODO: Add creation code to here.
	// Place all important initialize work at InitInstance.
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// It is unique CGCClientApp object.
// 유일한 CGCClientApp 개체입니다.

CGCClientApp theApp;


/// Initialize CGCClientApp
/// CGCClientApp 초기화
BOOL CGCClientApp::InitInstance()
{
	// If application program manifest assign using visual style with ComCtl32.dll version 6 or higher then InitCommonControls() is necessary on Windows XP.
	// You cannot create window without InitCommonControls().

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다.
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard Initializing
	// If you want to reduce excution file size without those functions, you should remove unnecessary specific initialize routine.
	// Please change registry key that saved specific setting.
	// TODO: You should modify this string to name of company or organization.
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	SetRegistryKey(L"로컬 응용 프로그램 마법사에서 생성한 응용 프로그램");

	MainForm();

	CWorld::GetSharedPtr()->m_client.Free();

	// Dialog box is closed so do not start message pump of application program and return "FALSE" to quit application program.
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고
	// 응용 프로그램을 끝낼 수 있도록 FALSE를 반환합니다.
	return FALSE;
}
