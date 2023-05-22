
// SimpleDB2ServerDlg.cpp : 구현 파일
// SimpleDB2ServerDlg.cpp : Realization file
//

#include "stdafx.h"
#include "SimpleDB2Server.h"
#include "SimpleDB2ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// MySql
//String g_DbmsConnectionString= L"Data Source=MySql Unicode;Database=ProudDB2-Test;Trusted_Connection=yes";

// MySQL for ODBC 5.3 
//String g_DbmsConnectionString = L"Driver={MySQL ODBC 5.3 Unicode Driver};Server=YourServerIP;UID=YourUserID;PWD=YourUserPassword;DB=ProudDB2-Test;Port=3306";

// MsSql
String g_DbmsConnectionString = L"Server=.;Database=ProudDB2-Test;Trusted_Connection=yes";


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
// CAboutDlg dialog box that use for application program information.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	// Dialog box data.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
// Realization
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


// CSimpleDB2ServerDlg 대화 상자
// CSimpleDB2ServerDlg dialog box




CSimpleDB2ServerDlg::CSimpleDB2ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleDB2ServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleDB2ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editCtl);
}

BEGIN_MESSAGE_MAP(CSimpleDB2ServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_MESSAGE(MESSAGE_LOG, &CSimpleDB2ServerDlg::OnLogWrite)
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CSimpleDB2ServerDlg 메시지 처리기
// CSimpleDB2ServerDlg message processor

BOOL CSimpleDB2ServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	// Adds "info..." menu item to system menu.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	// IDM_ABOUTBOX has to stay within system command area.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CStringW strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set icon of this dialog box. If main windows of application program is not main winodw then framework do this operation automatically.
	SetIcon(m_hIcon, TRUE);			// Set icon with big size.
	SetIcon(m_hIcon, FALSE);		// Set icon with small size.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// TODO: Adds additional initialize work to here.

	CoInitialize(NULL);
	// DB cache 서버를 준비한다.
	// Prepare DB cache server.
	m_db2Server.Attach(CDbCacheServer2::New());

	String errMsg;
	if (!TrialStartDbServer(g_DbmsConnectionString, errMsg))
	{
		AfxMessageBox(errMsg.GetString(), MB_ICONHAND | MB_OK);
		PostQuitMessage(0);
	}
	else
	{
		CStringW text;
		text.LoadStringW(IDS_SERVER_STARTED);
		LogEvent(text);
	}
	return true;

	// 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
	// Return "TRUE" if you dont set focus about control.
	return TRUE;
}

void CSimpleDB2ServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

// When you add minimize button at dialog box, you may need following code for drawing icon.
// Framework does this operation automatically for MFC application program that use documation/view model.

void CSimpleDB2ServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Device context for drawing

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		// Set icon in the center at client square.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		// Draw an icon.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
// Call this function to show cursor during user close minimized window.
HCURSOR CSimpleDB2ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



bool CSimpleDB2ServerDlg::TrialStartDbServer(String dbmsConnectionString, String &outErrMsg)
{
	CDbCacheServer2StartParameter p1;
	p1.m_authenticationKey = g_DbAuthKey;
	p1.m_DbmsConnectionString = g_DbmsConnectionString;
	p1.m_delegate = this;
	p1.m_serverIP = L"localhost";
	p1.m_tcpPort = g_DbServerPort;
	p1.m_allowNonExclusiveAccess = true;

	m_gamerTable.m_rootTableName = L"Gamer";
	m_gamerTable.m_childTableNames.Add(L"Hero");
	m_gamerTable.m_childTableNames.Add(L"Item");
	m_gamerTable.m_childTableNames.Add(L"Friend");

	// 서버 시작시 부모와자식 테이블의 이름을 추가합니다.
	// 주의~!! 이걸 해야 stored procedure 를 DBMS에 생성하기때문에 꼭 해야합니다.
	// 이름이 DBMS에 있는 테이블 이름과 틀리다면 exception이 발생합니다. 단 Start를 try catch로 잡아야합니다.

	// When you start server, adds name of parents and child table.
	// Warning~!! It only creates stored procedure to DBMS when you do it.
	// If name is different than table name in DBMS, it occurs exception. Start has to catch with try catch.
	p1.m_tableNames.Add(m_gamerTable);

	try
	{
		m_db2Server->Start(p1);
	}
	catch (std::exception &e)
	{
		outErrMsg = StringA2W(e.what());
		LogEvent(outErrMsg.GetString());
		return false;
	}
	return true;
}

/*************************************************************** LOG ******************************************************************/

LRESULT CSimpleDB2ServerDlg::OnLogWrite(WPARAM, LPARAM)
{
	if (!m_hWnd)
	{
		return true;
	}

	CStringW text;
	{
		CriticalSectionLock TRLock(m_CSLogLock, true);
		text = m_LogMsgList.RemoveHead().GetString();
	}

	int leng = m_editCtl.GetWindowTextLength();
	m_editCtl.SetSel(leng, leng);
	m_editCtl.ReplaceSel((LPCWSTR)(text + L"\r\n"));

	return true;
}

void CSimpleDB2ServerDlg::LogEvent(LPCWSTR text)
{
	if (!m_hWnd)
	{
		return;
	}

	CriticalSectionLock TRLock(m_CSLogLock, true);

	m_LogMsgList.AddTail(text);
	PostMessage(MESSAGE_LOG, NULL, NULL);
	return;
}

void CSimpleDB2ServerDlg::OnException(const Exception& e)
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()).GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}

void CSimpleDB2ServerDlg::OnError(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}

void CSimpleDB2ServerDlg::OnWarning(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	LogEvent(txt.GetString());
}
