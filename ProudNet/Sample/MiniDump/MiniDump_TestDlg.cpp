// MiniDump_TestDlg.cpp : Realization file
// MiniDump_TestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MiniDump_Test.h"
#include "MiniDump_TestDlg.h"
#include ".\minidump_testdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMiniDump_TestDlg::CMiniDump_TestDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CMiniDump_TestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniDump_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMiniDump_TestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMiniDump_TestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_MANUALDUMP, &CMiniDump_TestDlg::OnBnClickedManualdump)
END_MESSAGE_MAP()


// CMiniDump_TestDlg message processor
// CMiniDump_TestDlg 메시지 처리기

BOOL CMiniDump_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set icon of this dialog box. If main windows of application program is not main winodw then framework do this operation automatically. 
	SetIcon(m_hIcon, TRUE);			// Set icon with big size.
	SetIcon(m_hIcon, FALSE);		// Set icon with small size.

	// TODO: Adds additional initialize work to here.
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // If you do not set focus about control then returns "TRUE".
}

// When you add minimize button at dialog box, you may need following code for drawing icon.
// Framework does this operation automatically for MFC application program that use documation/view model.

void CMiniDump_TestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Device context for drawing

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Set icon in the center at client square.
		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw icon.
		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Call this function to show cursor during user close minimized window.
// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMiniDump_TestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMiniDump_TestDlg::OnBnClickedButton1()
{
	 int* a = 0;
	 *a = 1;
}


// Release모드에서는 Optimize하면서 Debug모드일 때와의 Stack의 크기가 달라진다.
// 이로인해 Release모드에서는 stack overflow가 발생되지 않는다.
// 의도적으로 stack overflow 발생시키기 위해 아래의 구문을 추가하였다.
#ifndef _DEBUG // DEBUG모드가 아니면 optimize를 끈다.
#pragma optimize( "", off)
#endif

// Occur stack overflow to reflexive function
// 재귀함수로 stack overflow발생
void StackOverFlow()
{
	return StackOverFlow();
}


#ifndef _DEBUG
#pragma optimize( "", on)
#endif

void CMiniDump_TestDlg::OnBnClickedButton2()
{
	StackOverFlow();
}

void CMiniDump_TestDlg::OnBnClickedManualdump()
{
	CMiniDumper::GetUnsafeRef().ManualMiniDump();
}
