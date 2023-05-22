// GCClientDlg.cpp : Realization file
// GCClientDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "GCClient.h"
#include "GCClientDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// CAboutDlg dialog box that uses for application program information.
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog box data
// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Support DDX/DDV

// Realization
// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGCClientDlg dialog box
// CGCClientDlg 대화 상자



CGCClientDlg::CGCClientDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CGCClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGCClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CGCClientDlg message processor
// CGCClientDlg 메시지 처리기

BOOL CGCClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Adds "Info..." menu item at system menu.
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX has to stay within system order area.
	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CStringW strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set icon of this dialog box. If main windows of application program is not main winodw then framework do this operation automatically. 
	SetIcon(m_hIcon, TRUE);			// Set icon with big size.
	SetIcon(m_hIcon, FALSE);		// Set icon with small size.

	// TODO: Adds additional initialize work to here.
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // If you do not set focus about control then returns "TRUE".
}

void CGCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// When you add minimize button at dialog box, you may need following code for drawing icon.
// Framework does this operation automatically for MFC application program that use documation/view model.
// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CGCClientDlg::OnPaint()
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
HCURSOR CGCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

