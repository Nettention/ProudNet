// ChatServerDlg.h : Header file
// ChatServerDlg.h : 헤더 파일
//

#pragma once

using namespace Proud;

#include "RemoteClient_S.h"
#include "../ChatCommon/ChatC2S_stub.h"
#include "../ChatCommon/ChatS2C_proxy.h"
#include "../ChatCommon/vars.h"
#include "afxwin.h"

// CChatServerDlg dialog box
// CChatServerDlg 대화 상자
class CChatServerDlg : public CDialog, public INetServerEvent, public ChatC2S::Stub
{
// Creation
// 생성
public:
	CChatServerDlg(CWnd* pParent = NULL);	// Standard creator

// Dialog box data
// 대화 상자 데이터
	enum { IDD = IDD_CHATSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Support DDX/DDV


// Realization
// 구현
protected:
	HICON m_hIcon;

	// Created message map function.
	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_clientList;


public:
	// Log related
	// 로그관련

	CEdit m_logEditCtl;
	CriticalSection m_CSLogLock;
	CFastList<String> m_LogMsgList;

	// Occur log event and put message to m_LogMsgList.
	// 로그 이벤트를 발생시키고 m_LogMsgList에 메시지를 넣어줍니다.
	void LogEvent(String text);

	// Take message from m_LogMsgList and print at ListBox when it receives log event.
	// 로그 이벤트가 왔을 때 m_LogMsgList에서 메시지 꺼내어 ListBox에 출력합니다.
	afx_msg LRESULT ProcessPostedMessage(WPARAM, LPARAM);

public:
	typedef CFastMap<HostID, CRemoteClientPtr_S> RemoteClients; 
	RemoteClients m_remoteClients;

	ChatS2C::Proxy mS2CProxy;
	CNetServer* mServer;
	CriticalSection mCS;

	DECRMI_ChatC2S_RequestLogon;
	DECRMI_ChatC2S_Chat;
	DECRMI_ChatC2S_RequestP2PGroup;
	DECRMI_ChatC2S_RequestLeaveP2PGroup;

	afx_msg void OnDestroy();

	virtual void OnClientJoin(CNetClientInfo *clientInfo)PN_OVERRIDE;
	virtual void OnClientLeave(CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment)PN_OVERRIDE;

	/** Event handler that accept/dine connection request of client
	\param playerID Requested client
	\param data Sent data block from client when it request logon
	\param reply [out] Sending data block to client when it accept logon
	\return If you return "false" that client is kicked out. */

	/** client 접속 요청을 수용/거부하는 이벤트 핸들러
	\param playerID 요청을 한 클라이언트
	\param data 클라이언트에서 접속 요청시 보내준 데이터 블럭
	\param reply [out] 접속 허용시 클라이언트에게 보내줄 데이터 블럭
	\return false를 리턴하면 그 클라이언트는 쫓겨난다. */
	virtual bool OnConnectionRequest(AddrPort clientAddr, ByteArray &userDataFromClient, ByteArray &reply) PN_OVERRIDE{
		return true;
	}

	/** Occured event when process of adding member to group has finished.
	- Let's say group G has current member M1, M2, M3 and now add new member M4.
	At this point M4 is added by JoinP2PGroup() but it has not finished instantly. 
	Because each client (M1, M2, M3) does not know about M4 and M4 does not know about M1, M2, M3.
	This event only occur when server confirmed about receiving M1, M2, M3, M4.
	- This event occur for CreateP2PGroup() as same as above. */

	/** 그룹에 멤버가 추가되는 과정이 모든 클라이언트에서도 완료되면 발생하는 이벤트.
	- 만약 그룹 G에 기 멤버 M1,M2,M3가 있다고 가정할 경우 신규 멤버 M4를 넣는다고 하자.
	이때 JoinP2PGroup()에 의해 M4가 추가되더라도 즉시 완료되는 것은 아니다. 각 클라이언트에서는
	아직 각각 M1,M2,M3에서는 M4를 받지 않았고 M4에서도 M1,M2,M3,M4를 받지 않았기 때문이다.
	이 이벤트는 M1,M2,M3,M4에서 이들을 모두 받은 것을 서버에서 확인한 후에야 발생된다.
	- CreateP2PGroup()에 대해서도 마찬가지로 이 이벤트가 발생한다. */
	virtual void OnP2PGroupJoinMemberAckComplete(HostID groupHostID,HostID memberHostID,ErrorType result) PN_OVERRIDE{}

	/** This message is called when thread of user work thread pool has started. */
	/** user work thread pool의 스레드가 시작할 때 이 메서드가 호출된다. */
	virtual void OnUserWorkerThreadBegin() PN_OVERRIDE{}

	/** When thread of user work thread pool is closed, this method is called. */
	/** user work thread pool의 스레드가 종료할 때 이 메서드가 호출된다. */
	virtual void OnUserWorkerThreadEnd() PN_OVERRIDE{}

	virtual void OnError(ErrorInfo *errorInfo) PN_OVERRIDE;

	/** Calling when internal error has occured. Report problem which lower priority than error.
	\param result Report type
	\param remote Which remote host
	\param where Where it occurs from
	*/
	/** 내부 경고 발생시 호출됨. 에러보다는 낮은 수위의 문제를 보고함.
	\param result 알림 종류
	\param remote 어떤 remote host인가
	\param where 어디서 발생한 것인가
	*/
	virtual void OnWarning(ErrorInfo *errorInfo)PN_OVERRIDE;

	/** Calling when internal error has occured. It does not mean problem situation such as tracking internal state.
	\param result Report type
	\param remote which remote host
	\param where Where it occurs from
	*/
	/** 내부 알림 발생시 호출됨. 내부 상황 추적 등을 위한 것으로 문제 상황을 말하는 것은 아님.
	\param result 알림 종류
	\param remote 어떤 remote host인가
	\param where 어디서 발생한 것인가
	*/
	virtual void OnInformation(ErrorInfo *errorInfo)PN_OVERRIDE;

    /** Merge to Proud.Exception
	- Normally process error that occur from user code.
	- User need to realize thing that need.
	- _com_error, void*, std::exception is going through parameter.
	Also there is OnUnhandledException() that process catch(...).
	- void* can receive type of exception object by casting. For example, it uses receiving MFC CException. */

    /** Proud.Exception으로 통합
	- 일반적으로 유저 코드에서 나는 에러를 처리한다.
	- 유저는 필요한 것들을 구현하면 된다.
	- _com_error, void*, std::exception이 파라메터로 넘어간다.
	또한 catch(...)를 처리하는 OnUnhandledException()도 있다.
	- void*는 예외 객체의 타입을 캐스팅해서 받을 수 있다. 가령 MFC CException을 받는데 사용된다. */
	virtual void OnException(const Exception &e) PN_OVERRIDE;
	virtual void OnUnhandledException() {}

	/** When RMI has called but there is not opposite RMI from attached stub then it calls back. */
	/** RMI가 호출이 들어왔으나 Attach된 Stub 중에 대응하는 RMI가 전혀 없으면 이것이 콜백된다. */
	virtual void OnNoRmiProcessed(RmiID rmiID) PN_OVERRIDE{}

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void GetAllClientList( CFastArray<HostID> &allClients );
public:
	afx_msg void OnBnClickedPrune();

};
