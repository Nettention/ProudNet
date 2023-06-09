﻿
// SimpleDB2Client.cpp : Define class motion about application program.
// SimpleDB2Client.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "SimpleDB2Client.h"
#include "SimpleDB2ClientDlg.h"
#include "SelectTestCaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimpleDB2ClientApp

BEGIN_MESSAGE_MAP(CSimpleDB2ClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Create CSimpleDB2ClientApp
// CSimpleDB2ClientApp 생성

CSimpleDB2ClientApp::CSimpleDB2ClientApp()
{
	// TODO: Adds creation code to here.
	// Place all important initialize work to InitInstance.
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// Unique CSimpleDB2ClientApp object.
// 유일한 CSimpleDB2ClientApp 개체입니다.

CSimpleDB2ClientApp theApp;


// Initialize CSimpleDB2ClientApp
// CSimpleDB2ClientApp 초기화

BOOL CSimpleDB2ClientApp::InitInstance()
{
	// If application program manifest assign using visual style with ComCtl32.dll version 6 or higher then InitCommonControls() is necessary on Windows XP.
	// You cannot create window without InitCommonControls().
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this item to include all common control class using at application program.
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	//_CrtSetBreakAlloc(21493);

	// Standard Initializing
	// If you want to reduce excution file size without those functions, you should remove unnecessary specific initialize routine.
	// Please change registry key that saved specific setting.
	// TODO: You should modify this string to name of company or organization.
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(L"로컬 응용 프로그램 마법사에서 생성된 응용 프로그램");
	
	INT_PTR nResponse;
	int sampleCase=0;
	{
		SelectTestCaseDlg Selectdlg;
		nResponse = Selectdlg.DoModal();
		sampleCase = Selectdlg.m_SampleCase;
	}
	if(nResponse == IDOK)
	{
		CSimpleDB2ClientDlg dlg;
		m_pMainWnd = &dlg;
		dlg.m_sampleCase = sampleCase;
		nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code to here that process disappearing dialog box by clicking "OK"
			// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
			//  코드를 배치합니다.
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code to here that process disappearing dialog box by clicking "Cancel"
			// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
			//  코드를 배치합니다.
		}

	}
	
	// Dialog box is closed so do not start message pump of application program and return "FALSE" to quit application program.
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}
