// ChatClientDlg.h : Header file
// ChatClientDlg.h : 헤더 파일
//

#pragma once

#include "LogonDlg.h"
#include "afxwin.h"
#include "../ChatCommon/ChatC2C_proxy.h"
#include "../ChatCommon/ChatC2C_stub.h"
#include "../ChatCommon/ChatC2S_proxy.h"
#include "../ChatCommon/ChatS2C_stub.h"
#include "../ChatCommon/vars.h"
#include "ChatRoomDlg.h"
#include "afxcmn.h"

using namespace Proud;

// CChatClientDlg dialog box
// CChatClientDlg 대화 상자
class CChatClientDlg : public CDialog, public INetClientEvent, public ChatS2C::Stub, public ChatC2C::Stub
{
// Creation
// 생성
public:
	CChatClientDlg(CWnd* pParent = NULL);	// Standard creator

// Dailog box data
// 대화 상자 데이터
	enum { IDD = IDD_CHATCLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Support DDX/DDV


// Realization
// 구현
protected:
	HICON m_hIcon;
	CLogonDlg m_logonDlg;

	// Created message map function.
	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	DECRMI_ChatS2C_ShowChat;
	DECRMI_ChatS2C_UserList_Add;
	DECRMI_ChatS2C_UserList_Remove;

	DECRMI_ChatC2C_P2P_Chat;
	DECRMI_ChatC2C_P2P_BigData;
	DECRMI_ChatC2C_P2P_BigDataAck;

	CNetClient* m_client;

	afx_msg void OnDestroy();

	virtual void OnJoinServerComplete(ErrorInfo *info, const ByteArray &replyFromServer) PN_OVERRIDE;
	virtual void OnLeaveServer(ErrorInfo *errorInfo) PN_OVERRIDE;
	virtual void OnP2PMemberJoin(HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField) PN_OVERRIDE;
	virtual void OnP2PMemberLeave(HostID memberHostID, HostID groupHostID, int memberCount) PN_OVERRIDE;
	virtual void OnChangeP2PRelayState(HostID remoteHostID, ErrorType reason)  PN_OVERRIDE{}
	virtual void OnSynchronizeServerTime()  PN_OVERRIDE {}
	virtual void OnError(ErrorInfo *errorInfo) PN_OVERRIDE;
	virtual void OnWarning(ErrorInfo *errorInfo) PN_OVERRIDE;
	virtual void OnInformation(ErrorInfo *errorInfo) PN_OVERRIDE;
	virtual void OnException(const Exception &e) PN_OVERRIDE;
	virtual void OnNoRmiProcessed(RmiID rmiID) {}

protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListBox m_chatLog;
	CStringW m_chatText;
	ChatC2S::Proxy m_C2SProxy;
	ChatC2C::Proxy m_C2CProxy;

	afx_msg void OnBnClickedSend();
	void SendChat();
	CListCtrl m_userListCtl;

public:
	// Log related
	//로그 관련
	CEdit m_editCtl;
	CriticalSection m_CSLogLock;
	CFastList<wstring> m_LogMsgList;

	// Occur log event and put message to m_LogMsgList.
	// 로그 이벤트를 발생시키고 m_LogMsgList에 메시지를 넣어줍니다.
	void LogEvent(String text);

	// Take message from m_LogMsgList and print at ListBox when it receives log event.
	// 로그 이벤트가 왔을 때 m_LogMsgList에서 메시지 꺼내어 ListBox에 출력합니다.
	afx_msg LRESULT ProcessPostedMessage( WPARAM, LPARAM );

	// Load resource String.
	// 리소스 String을 불어들입니다.
	CStringW LoadString(int stringResourceNum);
	
public:

	typedef CFastMap<HostID, CChatRoomDlgPtr> ChatRooms;
	ChatRooms m_chatRooms;
	
	wstring GetUserName(HostID clientID);

	afx_msg void OnBnClickedCreateRoom();

};
