// ChatClientDlg.cpp : Realization file
// ChatClientDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include ".\chatclientdlg.h"
#include "resource.h"
#include "ChatRoomDlg.h"
#include "../ChatCommon/ChatC2C_common.cpp"
#include "../ChatCommon/ChatC2S_common.cpp"
#include "../ChatCommon/ChatS2C_common.cpp"
#include "../ChatCommon/ChatC2C_proxy.cpp"
#include "../ChatCommon/ChatC2C_stub.cpp"
#include "../ChatCommon/ChatC2S_proxy.cpp"
#include "../ChatCommon/ChatS2C_stub.cpp"
#include "../ChatCommon/vars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CChatClientDlg dialog box
// CChatClientDlg 대화 상자



CChatClientDlg::CChatClientDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CChatClientDlg::IDD, pParent)
		, m_chatText(L"")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_LOG, m_chatLog);
	DDX_Text(pDX, IDC_CHAT_TEXT, m_chatText);
	DDX_Control(pDX, IDC_UESR_LIST_CTL, m_userListCtl);
	DDX_Control(pDX, IDC_EDIT1, m_editCtl);
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SEND, OnBnClickedSend)
	ON_BN_CLICKED(IDC_CREATE_ROOM, OnBnClickedCreateRoom)
	ON_MESSAGE(MESSAGE_LOG, &CChatClientDlg::ProcessPostedMessage)
END_MESSAGE_MAP()


// CChatClientDlg message processor
// CChatClientDlg 메시지 처리기

BOOL CChatClientDlg::OnInitDialog()
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
	
	// Initialize client
	// 클라 초기화
	m_client = CNetClient::Create();
	m_client->SetEventSink(this);
	m_client->AttachProxy(&m_C2SProxy);
	m_client->AttachProxy(&m_C2CProxy);
	m_client->AttachStub((ChatS2C::Stub*)this);
	m_client->AttachStub((ChatC2C::Stub*)this);

	// Open logon window
	// It is modeless because it calls CNetClient.FrameMove regularly.

	// 로그온 창을 띄운다
	// CNetClient.FrameMove를 지속적으로 호출해야 하므로 modeless이다.
	m_logonDlg.m_owner = this;

	m_logonDlg.Create(IDD_LOGON_DIALOG, this);
	m_logonDlg.ShowWindow(SW_SHOW);

	EnableWindow(FALSE);
	MoveWindowALittle(this);

	SetTimer(1, 10, 0); // 100Hz

	// Return "TRUE" if you dont set focus about control.
	// 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
	return TRUE;  
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Device context for drawing

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Set icon in the center at client square.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw icon.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Call this function to show cursor during user close minimized window.
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChatClientDlg::OnDestroy()
{
	delete m_client;

	CDialog::OnDestroy();

	KillTimer(1);
}

void CChatClientDlg::OnOK()
{
	// TODO: Add specialized code to here and/or call basic class.
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SendChat();

//	__super::OnOK();
}

void CChatClientDlg::OnCancel()
{
	// TODO: Add specialized code to here and/or call basic class.
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DestroyWindow();

//	__super::OnCancel();
}

void CChatClientDlg::OnJoinServerComplete( ErrorInfo *info, const ByteArray &replyFromServer )
{
	if (info->m_errorType != ErrorType_Ok)
	{
		CStringW txt;
		txt.Format( L"%s: %s", LoadString(IDS_STRING103), info->ToString());
		AfxMessageBox(txt, MB_OK | MB_ICONHAND);
		PostQuitMessage(0);
	}
	else
	{
		// Close logon window.
		// 로그온 창을 닫는다.
		m_logonDlg.ShowWindow(SW_HIDE);
		m_logonDlg.EnableWindow(FALSE);

		// Activate current dialog box.
		// 현재 대화상자를 활성화한다.
		EnableWindow(TRUE);
		SetFocus();

		// Send logon request message.
		// 로그온 요청 메시지를 보낸다.
		String userName = m_logonDlg.m_userName.GetString();
		m_C2SProxy.RequestLogon(HostID_Server, RmiContext::ReliableSend, userName);
	}
}
void CChatClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_client->FrameMove();
	__super::OnTimer(nIDEvent);
}

void CChatClientDlg::OnBnClickedSend()
{
	SendChat();
}

void CChatClientDlg::SendChat()
{
	UpdateData();
	String text = m_chatText.GetString();
	m_chatText = L"";
	UpdateData(0);

	RmiContext context = RmiContext::ReliableSend;
	context.m_compressMode = CM_Zip;
	// Send taken string to server.
	// 가져온 문자열을 서버로 보내기.
	m_C2SProxy.Chat(HostID_Server, context, text);

}

void CChatClientDlg::LogEvent( String text )
{	
	if(!m_hWnd)
		return;
	CriticalSectionLock TRLock(m_CSLogLock,true);

	m_LogMsgList.AddTail(text);
	PostMessage(MESSAGE_LOG, NULL, NULL);
	return;
}

LRESULT CChatClientDlg::ProcessPostedMessage( WPARAM, LPARAM )
{
	if(!m_hWnd)
		return FALSE;

	wstring text;
	{
		CriticalSectionLock TRLock(m_CSLogLock,true);
		text = m_LogMsgList.RemoveHead();
	}

	int leng = m_editCtl.GetWindowTextLength();
	m_editCtl.SetSel(leng, leng);
	m_editCtl.ReplaceSel((text+wstring(L"\r\n")).c_str());

	return	true;
}

CStringW CChatClientDlg::LoadString( int stringResourceNum )
{
	CStringW loadString;
	if( !loadString.LoadStringW(stringResourceNum) )
	{
		LogEvent(L"String load failed");
	}
	return loadString;
}

DEFRMI_ChatS2C_ShowChat(CChatClientDlg)
{
	CStringW txt;
	txt.Format(L"%s: %s", userName, text);
	m_chatLog.AddString(txt);
	m_chatLog.SetCurSel(m_chatLog.GetCount() - 1);

	return true;
}

DEFRMI_ChatS2C_UserList_Add(CChatClientDlg)
{
	wstring txt;
	if (hostID == m_client->GetLocalHostID())
	{
		txt = wstring(L"* ") + wstring(userName);
	}
	else
		txt = userName;

	int index = m_userListCtl.InsertItem(m_userListCtl.GetItemCount(), txt.c_str());
	m_userListCtl.SetItemData(index, hostID);

	return true;
}

DEFRMI_ChatS2C_UserList_Remove(CChatClientDlg)
{
	// Seek opposite item and destroy it.
	// 대응 항목을 찾아 제거한다.
	int count = m_userListCtl.GetItemCount();
	for (int i = 0;i < count;i++)
	{
		if (m_userListCtl.GetItemData(i) == hostID)
		{
			m_userListCtl.DeleteItem(i);
			break;
		}
	}

	return true;
}

void CChatClientDlg::OnBnClickedCreateRoom()
{
	// Send P2P group creation request to server.
	// 서버에 P2P 그룹 생성 요청을 보낸다.
	Proud::HostIDArray groupMembers;

	for (POSITION i = m_userListCtl.GetFirstSelectedItemPosition();i;)
	{
		int index = m_userListCtl.GetNextSelectedItem(i);
		groupMembers.Add((HostID)m_userListCtl.GetItemData(index));
	}
	// 자신도 포함해야.
	// Include itself.
	groupMembers.Add(m_client->GetLocalHostID()); 

	m_C2SProxy.RequestP2PGroup(HostID_Server, RmiContext::ReliableSend,
	                          groupMembers);
}

void CChatClientDlg::OnP2PMemberJoin( HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField )
{
	// If chatting window is existed then add to there.
	// Otherwise create new chatting window and adds.

	// 이미 만들어진 채팅창이 있으면 거기에 등록시킨다.
	// 그렇지 않으면 새 채팅창을 만들고 등록시킨다.

	CChatRoomDlgPtr chatRoom;

	if (!m_chatRooms.TryGetValue(groupHostID, chatRoom))
	{
		chatRoom = CChatRoomDlgPtr(new CChatRoomDlg);

		chatRoom->m_owner = this;
		chatRoom->m_groupID = groupHostID;

		m_chatRooms.Add(groupHostID, chatRoom);

		chatRoom->Activate();
	}

	chatRoom->AddMember(memberHostID);
}

void CChatClientDlg::OnP2PMemberLeave( HostID memberHostID, HostID groupHostID, int memberCount )
{
	// Find out chatting windows and remove member. And destroy it, if chatting windows is empty or you are exiting member of that chatting window.
	// 채팅창을 찾아서 멤버를 제거한다. 그리고 채팅창이 비어있거나 자신이 나가는 멤버이면 채팅 그룹도 뽀갠다.
	CChatRoomDlgPtr chatRoom;
	if (m_chatRooms.TryGetValue(groupHostID, chatRoom))
	{
		chatRoom->RemoveMember(memberHostID);

		if (chatRoom->GetMemberCount() == 0 || memberHostID == m_client->GetLocalHostID())
		{
			chatRoom->Deactivate();
			m_chatRooms.Remove(groupHostID);
		}
	}
}

DEFRMI_ChatC2C_P2P_Chat(CChatClientDlg)
{
	// Seek all attached room and add chatting text to all.
	//소속된 모든 방을 찾아서 모두 채팅 문구 추가
// 	for (ChatRooms::iterator i = m_chatRooms.begin();i != m_chatRooms.end();i++)
// 	{
// 		CChatRoomDlg* room = i->second;
// 		if (room->HasMember(remote))
// 		{
// 			room->ChatLog_Add(GetUserName(remote), text, rmiContext.m_relayed);
// 		}
// 	}

	//
	// 이 메시지를 전송한 P2P Group room을 찾아서 채팅 문구 추가
	ChatRooms::iterator iter = m_chatRooms.find(p2pGroupID);
	if (iter == m_chatRooms.end())
		return true;

	CChatRoomDlg* room = iter->second;
	if (room->HasMember(remote))
	{
		room->ChatLog_Add(GetUserName(remote), text, rmiContext.m_relayed);
	}

	return true;
}

DEFRMI_ChatC2C_P2P_BigData(CChatClientDlg)
{
	// 송신자가 이것을 보낼 때, 소속된 채팅방은 하나다. 그것을 찾아서 받았다는 누계를 표시하자.
	CChatRoomDlgPtr room;
	if(m_chatRooms.TryGetValue(p2pGroupID, room))
	{
		if (room->HasMember(remote))
		{
			room->ShowBigDataReceived(remote, data.GetCount());

			// 해당 송신자에게 ack를 전송. 그러면 상대방은 다음 data를 보내줄 것이다.
			m_C2CProxy.P2P_BigDataAck(remote, RmiContext::ReliableSend, p2pGroupID);
		}
	}


	return true;
}

DEFRMI_ChatC2C_P2P_BigDataAck(CChatClientDlg)
{
	CChatRoomDlgPtr room;

	// 연관되는 채팅방을 찾고 있고 보내기 진행중이면 다음 big data를 보내야.
	if(m_chatRooms.TryGetValue(p2pGroupID, room))
	{
		if (room->m_sendingBigData)
		{
			m_C2CProxy.P2P_BigData(remote, RmiContext::ReliableSend, room->m_bigData, p2pGroupID);
		}
	}

	return true;
}

wstring CChatClientDlg::GetUserName( HostID clientID )
{
	int count = m_userListCtl.GetItemCount();

	for (int i = 0;i < count;i++)
	{
		if (clientID == m_userListCtl.GetItemData(i))
		{
			return String(m_userListCtl.GetItemText(i, 0).GetString());
		}
	}
	return String();
}

void CChatClientDlg::OnLeaveServer( ErrorInfo *errorInfo )
{
	if (ErrorType_DisconnectFromLocal != errorInfo->m_errorType)
		AfxMessageBox(errorInfo->ToString().c_str());

	PostQuitMessage(0);
}

void CChatClientDlg::OnError( ErrorInfo *errorInfo ) 
{
	String txt;
	txt.Format(L"%s occured at %s", (LPCWSTR)errorInfo->ToString().c_str(), __FUNCTIONW__);
	LogEvent(txt);
}

void CChatClientDlg::OnWarning( ErrorInfo *errorInfo ) 
{
	String txt;
	txt.Format(L"%s occured at %s", (LPCWSTR)errorInfo->ToString().c_str(), __FUNCTIONW__);
	LogEvent(txt);
}

void CChatClientDlg::OnInformation( ErrorInfo *errorInfo ) 
{
	String txt;
	txt.Format(L"%s occured at %s", (LPCWSTR)errorInfo->ToString().c_str(), __FUNCTIONW__);
	LogEvent(txt);
}

void CChatClientDlg::OnException(const Exception &e)
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()), __FUNCTIONW__);
	LogEvent(txt);
}
