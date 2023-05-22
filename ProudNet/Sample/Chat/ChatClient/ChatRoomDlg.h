#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"

using namespace Proud;

class CChatClientDlg;
class CChatRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CChatRoomDlg)

public:
	CChatRoomDlg(CWnd* pParent = NULL);   // Standard creator.
	virtual ~CChatRoomDlg();

// Dialog box data
	enum { IDD = IDD_CHAT_ROOM };

	CChatClientDlg* m_owner;
	HostID m_groupID;

	void Activate();
	void Deactivate();
	void AddMember(HostID memberID);
	void RemoveMember(HostID memberID);
	int GetMemberCount();
	bool HasMember(HostID memberID);
	void ChatLog_Add(String userName, String text, bool relayed);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Support DDX/DDV

	DECLARE_MESSAGE_MAP()

	virtual void OnCancel();
	virtual void OnOK();

	void SendChat();
public:
	CStringW m_chatText;
	CListCtrl m_memberListCtl;
	CListBox m_chatLog;
	
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedSendBigData();
	void ShowBigDataReceived( HostID remote, int dataLength );

	CButton m_sendBigDataButton;
	bool m_sendingBigData;
	ByteArray m_bigData;

	class CMemberInfo
	{
	public:
		HostID m_memberID;
		int m_receivedBigDataLength;

		CMemberInfo()
		{
			m_memberID = HostID_None;
			m_receivedBigDataLength = 0;
		}
	};
	virtual BOOL OnInitDialog();
};

typedef Proud::RefCount<CChatRoomDlg> CChatRoomDlgPtr;