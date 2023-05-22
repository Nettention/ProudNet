// GCServerDlg.cpp : Realization file
//

#include "stdafx.h"
#include "GCServer.h"
#include "GCServerDlg.h"
#include "../GCCommon/vars.h"
#include ".\gcserverdlg.h"
#include "FarmServer.h"
#include "EntryServer.h"
#include "LobbyServer.h"
#include "BattleServer.h"
#include "StatusServer.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


// CAboutDlg dialog box that use for application program information.
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


// CGCServerDlg dialog box
// CGCServerDlg 대화 상자



CGCServerDlg::CGCServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_LIST, m_serverListCtl);
	DDX_Control(pDX, IDC_EDIT1, m_logEditCtl);
}

BEGIN_MESSAGE_MAP(CGCServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGCServerDlg message processor
// CGCServerDlg 메시지 처리기


BOOL CGCServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Adds "info..." menu item to system menu.
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
	SetIcon(m_hIcon, FALSE);		// Set icon small size.

	// Server list column setting
	// 서버 리스트 컬럼 세팅
	m_serverListCtl.InsertColumn(0, L"Type");
	m_serverListCtl.SetColumnWidth(0, 150);
	m_serverListCtl.InsertColumn(1, L"State");
	m_serverListCtl.SetColumnWidth(1, 150);

	// Running server depends on excution parameter.
	// 실행 파라메터에 따른 서버를 실행합니다.
	AddServerByPredefinedSettings();

	// Timer for UI
	// UI를 위한 타이머
	SetTimer(1, 100, 0);

	return TRUE;  // Return "TRUE", if you do not set focus about control.
}

void CGCServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGCServerDlg::OnPaint()
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
HCURSOR CGCServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGCServerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(1);

}

enum TokenMode
{
	TM_None, TM_LobbyName,
};

/// This program decide role of server that based on excution parameter.
/// 실행 파라메터를 근거로 이 프로그램이 어떤 역할을 하는 서버가 되어야 할지를 결정합니다.
void CGCServerDlg::GetLaunchSettingsByCommandLine(String cmdParam)
{
	LPCWSTR splitter = L" ";

	int ofs = 0;
	String token = cmdParam.Tokenize(splitter, ofs);

	TokenMode mode = TM_None;
	m_launchMode = ServerMode_Farm;

	while (1)
	{
		if (token == L"")
			break;
		switch (mode)
		{
		case TM_None:
			if (token.CompareNoCase(L"-entry") == 0)
			{
				m_launchMode = ServerMode_Entry;
			}
			else if (token.CompareNoCase(L"-lobby") == 0)
			{
				m_launchMode = ServerMode_Lobby;
				mode = TM_LobbyName;
			}
			else if (token.CompareNoCase(L"-battle") == 0)
			{
				m_launchMode = ServerMode_Battle;
				mode = TM_LobbyName;
			}
			else if (token.CompareNoCase(L"-status") == 0)
			{
				m_launchMode = ServerMode_Status;

			}
			else if (token.CompareNoCase(L"-debug") == 0)
			{
				m_debugging = true;
			}
			break;
		case TM_LobbyName:
			m_launchLobbyName = token;
			break;
		}
		token = cmdParam.Tokenize(splitter, ofs);
	}
}

/// Starts server role that depends on excution option already analysed before calling this method.
/// 이 메서드 호출 전에 이미 분석된 실행 옵션에 따라 서버 역할을 시작합니다.
void CGCServerDlg::AddServerByPredefinedSettings()
{
	CServerBasePtr srv;
	// Starts running server.
	// 서버 실행을 시작합니다.
	switch (m_launchMode)
	{
	case ServerMode_Farm:
		srv = CServerBasePtr(new CFarmServer);
		srv->m_serverMode = ServerMode_Farm;
		break;
	case ServerMode_Entry:
		srv = CServerBasePtr(new CEntryServer);
		srv->m_serverMode = ServerMode_Entry;
		break;
	case ServerMode_Lobby:
	{
		srv = CServerBasePtr(new CLobbyServer);
		((CLobbyServer*)srv.get())->m_lobbyName = m_launchLobbyName;
		srv->m_serverMode = ServerMode_Lobby;
	}
	break;
	case ServerMode_Battle:
	{
		srv = CServerBasePtr(new CBattleServer);
		((CBattleServer*)srv.get())->m_lobbyName = m_launchLobbyName;
		srv->m_serverMode = ServerMode_Battle;
	}
	break;
	case ServerMode_Status:
	{
		srv = CServerBasePtr(new CStatusServer);
		((CStatusServer*)srv.get())->m_statusName = L"StatusServer";
		srv->m_serverMode = ServerMode_Status;
	}
	}
	m_serverList.Add(srv);
	srv->m_ownerDlg = this;
	srv->Start();
}

/// Starts role of server by receive excution parameter.
/// 실행 파라메터를 입력받아 서버 역할을 시작시킵니다.
void CGCServerDlg::AddServer(LPCWSTR param)
{
	if (m_debugging)
	{
		// Creates virtual process (in-process instance) based on excution parameter.
		// 실행 파라메터를 근거로 가상의 프로세스(즉 in-process instance)를 생성합니다.
		GetLaunchSettingsByCommandLine(param);
		AddServerByPredefinedSettings();
	}
	else
	{
		// Execute actual process.
		// 진짜 프로세스를 실행합니다.
		WCHAR thisModuleName[2000];
		GetModuleFileName(NULL, thisModuleName, 2000);

		ShellExecute(NULL, NULL, thisModuleName, param, NULL, SW_SHOWNORMAL);
	}
}

int CGCServerDlg::FindOrAddListViewIndex(CServerBase* srv)
{
	CriticalSectionLock lock(m_cs, true);

	// Find out opposite list item to this server object at control.
	// If it is nothing, adds new one.
	// 컨트롤에서, 이 서버 객체에 대응하는 리스트 항목을 찾습니다.
	// 없으면 새로 하나를 추가합니다.
	int index = -1;
	for (int i = 0; i < m_serverListCtl.GetItemCount(); i++)
	{
		if (m_serverListCtl.GetItemData(i) == (DWORD_PTR)srv)
		{
			index = i;
			break;
		}
	}
	if (index < 0)
	{
		index = m_serverListCtl.InsertItem(m_serverListCtl.GetItemCount(), L"Unknown");
		m_serverListCtl.SetItemData(index, (DWORD_PTR)srv);
	}

	return index;
}

void CGCServerDlg::OnTimer(UINT_PTR)
{
	CriticalSectionLock lock(m_cs, true);

	for (int i = 0; i < m_serverList.Count; i++)
	{
		if (m_serverList[i] && m_serverList[i]->m_disposed == true)
		{
			// Delete really.
			// 실제로 제거를 합니다.
			int index = FindOrAddListViewIndex(m_serverList[i]);
			m_serverListCtl.DeleteItem(index);

			m_serverList[i] = CServerBasePtr();
		}
	}

	// If all servers are removed then quit program.
	// 모든 서버가 제거된 상태이면 프로그램을 종료합니다.
	for (int i = 0; i < m_serverList.Count; i++)
	{
		if (m_serverList[i])
			return;
	}
	PostQuitMessage(0);
}

LRESULT CGCServerDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SET_TITLE:
	{
		UiAccessParameter* p = (UiAccessParameter*)wParam;

		CriticalSectionLock lock(m_cs, true);

		int index = FindOrAddListViewIndex(p->m_serverObj);
		m_serverListCtl.SetItemText(index, 0, p->m_text.GetString());
		// If title of first time has changd then change text of main window.
		// 첫 항목의 타이틀이 바뀌는 경우, 메인 윈도우의 텍스트도 바꿉니다.
		if (index == 0)
		{
			String txt;
			txt.Format(L"%s  - ProudNet Casual Game Sample2", p->m_text.GetString());
			SetWindowText(txt.GetString());
		}

		delete p;
	}
	break;
	case WM_SET_STATUS:
	{
		UiAccessParameter* p = (UiAccessParameter*)wParam;
		CriticalSectionLock lock(m_cs, true);

		int index = FindOrAddListViewIndex(p->m_serverObj);

		m_serverListCtl.SetItemText(index, 1, p->m_text.GetString());
		delete p;
	}
	break;
	case WM_LOG_ERROR:
	{
		UiAccessParameter* p = (UiAccessParameter*)wParam;
		CriticalSectionLock lock(m_cs, true);

		int leng = m_logEditCtl.GetWindowTextLength();
		String text;
		text.Format(L"[Error] %s \r\n", p->m_text.GetString());
		m_logEditCtl.SetSel(leng, leng);
		m_logEditCtl.ReplaceSel((p->m_text + L"\r\n").GetString());

		delete p;
	}
	break;
	case WM_LOG_EVENT:
	{
		UiAccessParameter* p = (UiAccessParameter*)wParam;
		CriticalSectionLock lock(m_cs, true);

		int leng = m_logEditCtl.GetWindowTextLength();
		String text;
		text.Format(L"[Info] %s \r\n", p->m_text.GetString());
		m_logEditCtl.SetSel(leng, leng);
		m_logEditCtl.ReplaceSel(text.GetString());

		delete p;
	}
	break;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CGCServerDlg::OnOK()
{
	StopServers();
	DestroyWindow();
}

void CGCServerDlg::OnCancel()
{
	StopServers();
	DestroyWindow();
}

/// Stop all servers.
/// 서버를 모두 중지시킵니다.
void CGCServerDlg::StopServers()
{
	CriticalSectionLock lock(m_cs, true);
	for (int i = 0; i < m_serverList.Count; i++)
	{
		if (m_serverList[i])
			m_serverList[i]->m_disposed = true;
	}
}

