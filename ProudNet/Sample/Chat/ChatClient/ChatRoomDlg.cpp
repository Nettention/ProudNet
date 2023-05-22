// ChatRoomDlg.cpp : Realization file.
// ChatRoomDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChatRoomDlg.h"
#include ".\chatroomdlg.h"
#include "ChatClientDlg.h"


// CChatRoomDlg dialog box.

IMPLEMENT_DYNAMIC(CChatRoomDlg, CDialog)
CChatRoomDlg::CChatRoomDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CChatRoomDlg::IDD, pParent)
		, m_chatText(L"")
{
	m_sendingBigData = false;
}

CChatRoomDlg::~CChatRoomDlg()
{
}

void CChatRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CHAT_TEXT, m_chatText);
	DDX_Control(pDX, IDC_MEMBER_LIST, m_memberListCtl);
	DDX_Control(pDX, IDC_CHATLOG, m_chatLog);
	DDX_Control(pDX, IDC_SEND_BIG_DATA, m_sendBigDataButton);
}


BEGIN_MESSAGE_MAP(CChatRoomDlg, CDialog)
	ON_BN_CLICKED(IDC_SEND, OnBnClickedSend)
	ON_BN_CLICKED(IDC_SEND_BIG_DATA, &CChatRoomDlg::OnBnClickedSendBigData)
END_MESSAGE_MAP()

void CChatRoomDlg::Activate()
{
	Create(IDD_CHAT_ROOM, m_owner);
	ShowWindow(SW_SHOW);
}

// CChatRoomDlg message processor.

void CChatRoomDlg::OnCancel()
{
	// Withdrawl request from P2P group.
	// P2P 그룹에서 탈퇴 요청을 한다.
	m_owner->m_C2SProxy.RequestLeaveP2PGroup(HostID_Server, RmiContext::ReliableSend, m_groupID);

	int count = m_memberListCtl.GetItemCount();
	for (int i = 0;i < count;i++)
	{
		CMemberInfo* info = (CMemberInfo*)m_memberListCtl.GetItemData(i);
		delete info;
	}

	//CDialog::OnCancel();
}

void CChatRoomDlg::OnOK()
{
	SendChat();
	//CDialog::OnOK();
}

void CChatRoomDlg::Deactivate()
{
	ShowWindow(SW_HIDE);
	DestroyWindow();
}

void CChatRoomDlg::SendChat()
{
	UpdateData();
	String text(m_chatText.GetString());
	m_chatText = L"";
	UpdateData(0);

	RmiContext context = RmiContext::ReliableSend;
	context.m_compressMode = CM_Zip;

	m_owner->m_C2CProxy.P2P_Chat(m_groupID, context, m_groupID, text);
}

void CChatRoomDlg::AddMember( HostID memberID )
{
	wstring txt;
// 	if(memberID==mOwner->mClient->GetLocalHostID())
// 		txt=CString("* ")+mOwner->GetUserName(memberID);
// 	else
// 		txt=mOwner->GetUserName(memberID);

	txt = m_owner->GetUserName(memberID);

	int index = m_memberListCtl.InsertItem(m_memberListCtl.GetItemCount(), txt.c_str());

	CMemberInfo* info = new CMemberInfo;
	info->m_memberID = memberID;

	m_memberListCtl.SetItemData(index, (INT_PTR)info);
}

void CChatRoomDlg::RemoveMember( HostID memberID )
{
	int count = m_memberListCtl.GetItemCount();
	for (int i = count-1;i >= 0;i--)
	{
		CMemberInfo* info = (CMemberInfo*)m_memberListCtl.GetItemData(i);
		if (memberID == info->m_memberID)
		{
			m_memberListCtl.DeleteItem(i);
			break;
		}
	}
}

int CChatRoomDlg::GetMemberCount()
{
	return m_memberListCtl.GetItemCount();
}

bool CChatRoomDlg::HasMember( HostID memberID )
{
	int count = m_memberListCtl.GetItemCount();
	for (int i = 0;i < count;i++)
	{
		CMemberInfo* info = (CMemberInfo*)m_memberListCtl.GetItemData(i);
		if (info->m_memberID == memberID)
		{
			return true;
		}
	}
	return false;
}

void CChatRoomDlg::ChatLog_Add(String userName, String text, bool relayed)
{
	CStringW prefix;
	//if (userName == mOwner->mUserName)
	if(-1 != userName.Find(L"* "))
		prefix = L"[Self]";
	else if (relayed == false)
		prefix = L"[P2P] ";
	else
		prefix = L"[Relayed] ";

	CStringW txt;
	txt.Format(L"%s: %s", userName, text);

	m_chatLog.AddString(prefix + txt);
	m_chatLog.SetCurSel(m_chatLog.GetCount() - 1);
}
void CChatRoomDlg::OnBnClickedSend()
{
	SendChat();
}

void CChatRoomDlg::OnBnClickedSendBigData()
{
	if(!m_sendingBigData)
	{
		// 버튼 누르면=>20KB단위로 보내기 시작. 처음에는 두개를 보낸다.
		// (3G에서는 0.2초 정도 걸림. 본 앱은 100Hz로 타이머가 도므로, 100 * 20KB = 2MB/sec 정도 된다.)
		m_bigData.SetCount(1024*20);

		// P2P 그룹 멤버 전원에게 보낸다.
		// 처음에는 두개를 보낸다. 그래야 ack-next data 사이의 트래픽 공허가 없기 때문이다.
		for (int i=0;i<2;i++)
		{
			RmiContext sendWay = RmiContext::ReliableSend;
			sendWay.m_enableLoopback = false; // 자신한테는 보내는 뻘짓 금지
			m_owner->m_C2CProxy.P2P_BigData(m_groupID, sendWay, m_bigData, m_groupID);
		}

		// 캡션.
		m_sendBigDataButton.SetWindowText(L"Cancel sending");

		// 보내는 중이라고 마킹을. 
		m_sendingBigData = true;
	}
	else
	{
		// 반대로.
		m_sendBigDataButton.SetWindowText(L"Send big data");

		m_sendingBigData = false;
	}
}

void CChatRoomDlg::ShowBigDataReceived( HostID remote, int dataLength )
{
	// 리스트 박스에서 해당 유저를 찾은 후 받은 양을 추가로 표시하자.
	int count = m_memberListCtl.GetItemCount();
	for (int i = 0;i < count;i++)
	{
		CMemberInfo* info = (CMemberInfo*)m_memberListCtl.GetItemData(i);
		if (info->m_memberID == remote)
		{
			info->m_receivedBigDataLength += dataLength;
			CString newText;
			newText.Format(L"%d", info->m_receivedBigDataLength);
			m_memberListCtl.SetItemText(i, 1, newText);
			break;
		}
	}
}


BOOL CChatRoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_memberListCtl.InsertColumn(0, L"Name");
	m_memberListCtl.SetColumnWidth(0, 80);
	m_memberListCtl.InsertColumn(1, L"Big data");
	m_memberListCtl.SetColumnWidth(1, 80);

	MoveWindowALittle(this);

	// EXCEPTION: OCX Property Pages should return FALSE
	return TRUE;
}
