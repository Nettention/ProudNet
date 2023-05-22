// ChatServerDlg.cpp : Realization file
// ChatServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include ".\chatserverdlg.h"
#include "RemoteClient_S.h"
#include "../ChatCommon/ChatC2S_common.cpp"
#include "../ChatCommon/ChatS2C_common.cpp"
#include "../ChatCommon/ChatC2S_stub.cpp"
#include "../ChatCommon/ChatS2C_proxy.cpp"
#include "NicSelectionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_CLIENT_LIST_REMOVE (WM_USER+1)
#define WM_CLIENT_LIST_ADD (WM_USER+2)

// CAboutDlg dialog box that using for application program information.
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog box data
	// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

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


// CChatServerDlg dialog box
// CChatServerDlg 대화 상자



CChatServerDlg::CChatServerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CChatServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLIENT_LIST, m_clientList);
	DDX_Control(pDX, IDC_EDIT1, m_logEditCtl);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PRUNE, &CChatServerDlg::OnBnClickedPrune)
	ON_MESSAGE(MESSAGE_LOG, &CChatServerDlg::ProcessPostedMessage)
END_MESSAGE_MAP()


// CChatServerDlg message processor
// CChatServerDlg 메시지 처리기

BOOL CChatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add to "info..." menu item to system menu.
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX has to stay within system command area.
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

	// Select function that if server side NIC is not one.
	// 서버측 NIC가 여러개인 경우 하나를 선택하는 기능
	CFastArray<String> serverNicList;
	CNetUtil::GetLocalIPAddresses(serverNicList);

	ShowWindow(SW_SHOW);

	StringA localNicAddr;
	if(serverNicList.Count>=2)
	{
		CNicSelectionDialog nicDlg;
		nicDlg.m_candidates.Add(NoNicText);
		for(int i=0;i<serverNicList.Count;i++)
		{
			nicDlg.m_candidates.Add(serverNicList[i].GetString());
		}
		if(nicDlg.DoModal()!=IDOK)
		{
			PostQuitMessage(0);
			return TRUE;
		}
		else 
		{
			localNicAddr = StringW2A(nicDlg.m_selectedNic);
		}
	}

	mServer = CNetServer::Create();
	//mServer->EnableLog(L"ChatServer.log");

	CStartServerParameter p1;
	p1.m_tcpPorts.Add(gServerPort);
	p1.m_protocolVersion = gProtocolVersion;
	p1.m_localNicAddr = StringA2W(localNicAddr);
	//p1.m_enableUpnpUse = false;
	//mServer->SetDefaultFallbackMethod(FallbackMethod_ServerUdpToTcp);
	p1.m_udpAssignMode = ServerUdpAssignMode_PerClient;
	//p1.m_udpPorts.Add(0);
	// 게임 앱에서는 통상적으로 이것을 꺼주자. ProudNet 내부에서 Nagle 대용의 Coalesce 메카니즘이 작동하므로 안전하다.
	// Normally turns off it at game application. It is safe because Coalesce mechanism is working internally and it is substitue for Nagle.
	p1.m_enableNagleAlgorithm = false; 
	p1.m_enableP2PEncryptedMessaging = true;

#ifdef SUPPORTS_CPP11
	p1.m_webSocketParam.webSocketType = WebSocket_Ws;
	p1.m_webSocketParam.endpoint = _PNT("^/echo/?$");
	p1.m_webSocketParam.listenPort = 8080;
	p1.m_webSocketParam.threadCount = 4;
#endif

	mServer->SetEventSink(this);
	mServer->AttachProxy(&mS2CProxy);
	mServer->AttachStub(this);

	mServer->SetDirectP2PStartCondition(DirectP2PStartCondition_Always);
	//mServer->Start(p1); // 예전 코드

	ErrorInfoPtr err;
	try
	{
		mServer->Start(p1);
	}
	catch (const Proud::Exception &e)
	{
		printf("Server Start Error: %s\n", e.what());
	}

	MoveWindowALittle(this);

	// 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
	// Return "TRUE" if you do not set focus about control.
	return TRUE;  
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

// When you add minimize button at dialog box, you may need following code for drawing icon.
// Framework does this operation automatically for MFC application program that use documation/view model.

void CChatServerDlg::OnPaint()
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

// Call this function to show cursor during user close minimized window
// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChatServerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add message processor code to here.
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete mServer;
}

LRESULT CChatServerDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLIENT_LIST_REMOVE:
		for (int i = 0;i < m_clientList.GetCount();i++)
		{
			if (m_clientList.GetItemData(i) == wParam)
			{
				m_clientList.DeleteString(i);
				break;
			}
		}
		break;

	case WM_CLIENT_LIST_ADD:
		{
			CriticalSectionLock lock(mCS, true);
			HostID rcID = (HostID)wParam;
			CRemoteClientPtr_S	rc;
			if (m_remoteClients.TryGetValue(rcID, rc))
			{
				int index = m_clientList.AddString(rc->m_userName.c_str());
				m_clientList.SetItemData(index, rcID);
			}
		}
		break;

	}

	return __super::DefWindowProc(message, wParam, lParam);
}

void CChatServerDlg::OnClientJoin( CNetClientInfo *clientInfo )
{
	CriticalSectionLock lock(mCS, true);

	CRemoteClientPtr_S newItem(new CRemoteClient_S);
	newItem->m_hostID = clientInfo->m_HostID;

	m_remoteClients.Add(clientInfo->m_HostID, newItem);
}

void CChatServerDlg::OnClientLeave( CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment )
{
	CriticalSectionLock lock(mCS, true);

	CRemoteClientPtr_S rc;
	if (m_remoteClients.TryGetValue(clientInfo->m_HostID, rc))
	{

		// Report state.
		// 상황 공지를 한다.
		CFastArray<HostID> allClients;
		GetAllClientList(allClients);

		mS2CProxy.UserList_Remove(allClients.GetData(), allClients.Count, RmiContext::ReliableSend, rc->m_hostID);

		m_remoteClients.Remove(clientInfo->m_HostID);

		PostMessage(WM_CLIENT_LIST_REMOVE, (WPARAM)clientInfo->m_HostID);
	}
}

void CChatServerDlg::LogEvent( String text )
{	
	if(!m_hWnd)
		return;
	CriticalSectionLock TRLock(m_CSLogLock,true);

	m_LogMsgList.AddTail(text);
	PostMessage(MESSAGE_LOG, NULL, NULL);
	return;
}

LRESULT CChatServerDlg::ProcessPostedMessage(WPARAM, LPARAM)
{
	if(!m_hWnd)
		return FALSE;

	std::wstring text;
	{
		CriticalSectionLock TRLock(m_CSLogLock,true);
		text = m_LogMsgList.RemoveHead();
	}

	int leng = m_logEditCtl.GetWindowTextLength();
	m_logEditCtl.SetSel(leng, leng);
	m_logEditCtl.ReplaceSel((text + std::wstring(L"\r\n")).c_str());

	return	true;
}


DEFRMI_ChatC2S_RequestLogon(CChatServerDlg)
{
	CriticalSectionLock lock(mCS, true);

	CRemoteClientPtr_S rc;
	if (!m_remoteClients.TryGetValue(remote, rc))
		return true;

	rc->m_userName = userName;

	// Report state.
	// 상황 공지를 한다.
	CFastArray<HostID> allClients;
	GetAllClientList(allClients);

	mS2CProxy.UserList_Add(allClients.GetData(), allClients.Count, RmiContext::ReliableSend, rc->m_userName, rc->m_hostID);

	// To new user.
	// 신입 유저에게도.
	for (RemoteClients::iterator i = m_remoteClients.begin();i != m_remoteClients.end();i++)
	{
		if (i->first != remote)
			mS2CProxy.UserList_Add(remote, RmiContext::ReliableSend, i->second->m_userName, i->first);
	}

	PostMessage(WM_CLIENT_LIST_ADD, remote);

	return true;
}

DEFRMI_ChatC2S_Chat(CChatServerDlg)
{
	CriticalSectionLock lock(mCS, true);

	CRemoteClientPtr_S rc;
	if (!m_remoteClients.TryGetValue(remote, rc))
		return true;

	// Broadcast to connected client.
	// 접속한 클라에게 브로드캐스트한다.
	CFastArray<HostID> allClients;
	GetAllClientList(allClients);

	RmiContext context = RmiContext::ReliableSend;
	context.m_compressMode = CM_Zip;

	mS2CProxy.ShowChat(allClients.GetData(), allClients.Count,context, rc->m_userName, text);

	return true;
}

void CChatServerDlg::GetAllClientList( CFastArray<HostID> &allClients )
{
	int allClientCount = m_remoteClients.Count;
	allClients.SetCount(0);
	for (RemoteClients::iterator i = m_remoteClients.begin();i != m_remoteClients.end();i++)
	{
		allClients.Add(i->first);
	}
}

DEFRMI_ChatC2S_RequestP2PGroup(CChatServerDlg)
{
	CriticalSectionLock lock(mCS, true);

	CRemoteClientPtr_S rc;
	if (!m_remoteClients.TryGetValue(remote, rc))
		return true;

	mServer->CreateP2PGroup(groupMemberList.GetData(), groupMemberList.Count, ByteArray());

	return true;
}

DEFRMI_ChatC2S_RequestLeaveP2PGroup(CChatServerDlg)
{
	CriticalSectionLock lock(mCS, true);

	CRemoteClientPtr_S rc;
	if (!m_remoteClients.TryGetValue(remote, rc))
		return true;

	mServer->LeaveP2PGroup(remote, groupID);

	return true;
}


void CChatServerDlg::OnBnClickedPrune()
{
	mServer->CloseEveryConnection();
}

void CChatServerDlg::OnError(ErrorInfo *errorInfo) 
{ 
	String txt;
	txt.Format(L"%s occured at %s", (LPCWSTR)errorInfo->ToString().c_str(), __FUNCTIONW__);
	LogEvent(txt);
}
void CChatServerDlg::OnWarning(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", (LPCWSTR)errorInfo->ToString().c_str(), __FUNCTIONW__);
	LogEvent(txt);
}
void CChatServerDlg::OnInformation(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", (LPCWSTR)errorInfo->ToString().c_str(), __FUNCTIONW__);
	LogEvent(txt);
}
void CChatServerDlg::OnException(const Proud::Exception &e)
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()), __FUNCTIONW__);
	LogEvent(txt);
}

