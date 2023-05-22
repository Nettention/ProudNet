// BlitzDumpDispatcherDlg.cpp : Realization file
// BlitzDumpDispatcherDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "DumpReporterDlg.h"
#include "DumpReportProgressDlg.h"
#include "DumpReporter.h"

CDumpReporterDlg::CDumpReporterDlg(CDumpReporter* owner)
		: CDialog(CDumpReporterDlg::IDD, NULL), m_userText(L""), m_owner(owner)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDumpReporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDumpReporterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDumpReporterDlg message processor
// CDumpReporterDlg �޽��� ó����

BOOL CDumpReporterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//// Add "info..." menu item at system menu.
	//// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	//// IDM_ABOUTBOX has to stay within system command area.
	//// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	CString strAboutMenu;
	//	strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	// Set icon of this dialog box. If main windows of application program is not main winodw then framework do this operation automatically. 
	SetIcon(m_hIcon, TRUE);			// Set icon with big size.
	SetIcon(m_hIcon, FALSE);		// Set icon with small size.

	// TODO: Adds additional initialize work to here.
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // If you do not set focus about control then returns "TRUE".
}

//void CDumpReporterDlg::OnSysCommand(UINT nID, LPARAM lParam)
//{
//	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
//	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
//	}
//	else
//	{
//		CDialog::OnSysCommand(nID, lParam);
//	}
//}

// When you add minimize button at dialog box, you may need following code for drawing icon.
// Framework does this operation automatically for MFC application program that use documation/view model.

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDumpReporterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Device context for drawing

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Set icon in the center at client square.
		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw icon.
		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Call this function to show cursor during user close minimized window.
// ����ڰ� �ּ�ȭ�� â�� ��� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CDumpReporterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDumpReporterDlg::OnOK()
{
	UpdateData();

	CDumpReportProgressDlg dlg;
	dlg.m_owner = m_owner;
	dlg.DoModal();

	// Delete sent dump file.
	// ���� ���� ������ �����.
	//DeleteFile(AfxGetApp()->m_lpCmdLine);

	CDialog::OnOK();
}

// Delete dump file then end.
// ���� ������ ����� ��.
void CDumpReporterDlg::OnCancel()
{
	//DeleteFile(AfxGetApp()->m_lpCmdLine);

	CDialog::OnCancel();
}

void CDumpReporterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
