// LogonDlg.cpp : Realization file
// LogonDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogonDlg.h"
#include "ChatClientDlg.h"
#include ".\logondlg.h"
#include "../ChatCommon/vars.h"


// CLogonDlg dialog box

IMPLEMENT_DYNAMIC(CLogonDlg, CDialog)
CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CLogonDlg::IDD, pParent)
		, m_serverAddr(L"")
		, m_userName(L"")
{
}

CLogonDlg::~CLogonDlg()
{
}

void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVER_ADDR, m_serverAddr);
	DDX_Text(pDX, IDC_USER_NAME, m_userName);
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
END_MESSAGE_MAP()

LPCWSTR SampleNames[] =
{
	L"Edward",
	L"Edwin",
	L"Enoch",
	L"Eugene",
	L"Evelyn",
	L"Ferdinand",
	L"Frances",
	L"Frederick",
	L"Gabriel",
	L"Geoffrey",
	L"George",
	L"Gilbert",
	L"Gregory",
	L"Harold",
	L"Henry",
	L"Herbert",
	L"Harace",
	L"Humphrey",
	L"Issac",
	L"Jacob",
	L"Jerome",
	L"John",
	L"Kenneth",
	L"Lawrence",
	L"Leonard",
	L"Leslie",
	L"Lewi",
	L"Martin",
	L"Matthew",
	L"Nicholas",
	L"Noel",
	L"Oliver",
	L"Oscar",
	L"Oswald",
	L"Owen",
	L"Patricia",
	L"Paul",
	L"Peter",
	L"Philip",
	L"Richard",
	L"Robert",
	L"Roland",
	L"Samuel",
	L"Sabastian",
	L"Theodore",
	L"Thomas",
	L"Vincent",
	L"Vivian",
	L"Wallace",
	L"William",
	L"Agatha",
	L"Agnes",
	L"Angela",
	L"Aileen",
	L"Alice",
	L"Amy",
	L"Beatrice",
	L"Bridget",
	L"Catherine",
	L"Cecil",
	L"Cordelia",
	L"Dorothy",
	L"Elizabeth",
	L"Edith",
	L"Emery",
};

// determine number of elements in an array (not bytes)
#define _COUNTOF(array) (sizeof(array)/sizeof(array[0]))

LPCWSTR GetRandomUserName()
{
	int index = CRandom::StaticGetInt() % _COUNTOF(SampleNames);
	return SampleNames[index];
}

// CLogonDlg message processor.

BOOL CLogonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_serverAddr = L"localhost";
	m_userName = GetRandomUserName();

	MoveWindowALittle(this);

	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// Exception: OCX property page has to return "FALSE".
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLogonDlg::OnCancel()
{
	DestroyWindow();

	PostQuitMessage(0);
	//CDialog::OnCancel();
}

void CLogonDlg::OnOK()
{
	UpdateData();

	CNetConnectionParam par;
	par.m_protocolVersion = gProtocolVersion;
	par.m_serverIP = m_serverAddr;
	par.m_serverPort = gServerPort;

	if (m_owner->m_client->Connect(par) == false)
	{
		// Wrong address.
		// 잘못된 주소입니다.
		CStringW logString;
		logString.LoadStringW(IDS_STRING102);
		AfxMessageBox( logString, MB_OK | MB_ICONHAND);
		PostQuitMessage(0);
	}
	EnableWindow(FALSE);

//	CDialog::OnOK();
}
