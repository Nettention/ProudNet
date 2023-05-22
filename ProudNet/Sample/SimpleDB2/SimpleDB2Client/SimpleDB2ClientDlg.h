
// SimpleDB2ClientDlg.h : Header file
//

#pragma once
#include "afxwin.h"

enum
{
	eCacheSample,
	eNonExclusiveSample
};

class CDbClient;
// CSimpleDB2ClientDlg dialog box
class CSimpleDB2ClientDlg : public CDialog, public IDbCacheClientDelegate2
{
// 생성입니다.
// Creation
public:
	CSimpleDB2ClientDlg(CWnd* pParent = NULL);	// Standard creator
	~CSimpleDB2ClientDlg();
// 대화 상자 데이터입니다.
// Dialog box data
	enum { IDD = IDD_SIMPLEDB2CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Support DDX/DDV

// 구현입니다.
// Realization
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	// Created message map function
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()
	
	CAutoPtr<CDbClient> m_client;
	bool m_leaveServer;

	// 일방적 메서드를 확인하기위한 guid
	// Guid that to check one-sided method
	Proud::Guid m_lastWriteDoneGuid;

protected:
	CEdit m_editCtl;
	CFastList<String> m_LogMsgList;
	CriticalSection m_CSLogLock;

	afx_msg LRESULT OnLogWrite(WPARAM, LPARAM);

public:	
	void LogEvent( LPCWSTR text );
	void LogError( LPCWSTR text, ... );

public:
	void Connect();
	CStringW LoadString(int stringResourceNum);

public:
	int m_sampleCase;
	CLoadedData2Ptr m_nonExclusiveSnapshot;
	
private:
    /** 서버 연결 시도 결과를 리턴한다. */
    /** Return server connectnion attempt */
	virtual void OnJoinDbCacheServerComplete(ErrorInfo *info)PN_OVERRIDE;

	/** DB cache server와의 연결이 해제됐음을 노티한다.
	- 이것이 호출된 상황에서는, 이미 DB cache client의 모든 데이터가 unload된 상태이다. */
	/** Notify disconnected with DB cache server.
	- When this called, all data of DB cache client has already unloaded. */
	virtual void OnLeaveDbCacheServer(ErrorType reason)PN_OVERRIDE;

	/** DB cache server에서 DBMS에 기록하는 과정이 끝날 때마다 도착하는 이벤트
	\param type 완료된 기록의 종류
	\param loadedDataGuid 완료된 기록을 가리키는 Guid. RootNode의 UUID이다. */

	/** Arriving event when it has done writing process to DBMS at DB cache server
	\param type Type of finished writing
	\param loadedDataGuid Guid that point finished writing. UUID of RootNode. */
    virtual void OnDbmsWriteDone(DbmsWritePropNodePendType type,Proud::Guid loadedDataGuid)PN_OVERRIDE;

	virtual void OnExclusiveLoadDataComplete(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnDataUnloadRequested(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnDataForceUnloaded(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnAddDataFailed(CCallbackArgs& args)PN_OVERRIDE;
	
	virtual void OnAddDataSuccess(CCallbackArgs& args)PN_OVERRIDE;


	virtual void OnUpdateDataFailed(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnUpdateDataSuccess(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnRemoveDataFailed(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnRemoveDataSuccess(CCallbackArgs& args)PN_OVERRIDE;

// 	virtual void OnNonExclusiveSnapshotDataFailed(CCallbackArgs& args)PN_OVERRIDE;
// 	virtual void OnNonExclusiveSnapshotDataSuccess(CCallbackArgs& args)PN_OVERRIDE;
	virtual void OnNonExclusiveSnapshotDataComplete(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnNonExclusiveAddDataAck(CCallbackArgs& args)PN_OVERRIDE;
	virtual void OnNonExclusiveRemoveDataAck(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnNonExclusiveModifyValueSuccess(CCallbackArgs& args)PN_OVERRIDE;
	virtual void OnNonExclusiveModifyValueFailed(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnNonExclusiveSetValueIfSuccess(CCallbackArgs& args)PN_OVERRIDE;
	virtual void OnNonExclusiveSetValueIfFailed(CCallbackArgs& args)PN_OVERRIDE;

	
	virtual void OnSomeoneAddData(CCallbackArgs& args)PN_OVERRIDE;	
	virtual void OnSomeoneRemoveData(CCallbackArgs& args)PN_OVERRIDE;
	
	virtual void OnSomeoneModifyValue(CCallbackArgs& args)PN_OVERRIDE;
	virtual void OnSomeoneSetValue(CCallbackArgs& args)PN_OVERRIDE;

	virtual void OnException(const Exception &e) PN_OVERRIDE;
	virtual void OnWarning(ErrorInfo *errorInfo)PN_OVERRIDE;
	virtual void OnError(ErrorInfo *errorInfo)PN_OVERRIDE;

	virtual void OnAccessError(CCallbackArgs& args)PN_OVERRIDE; 

	virtual void OnIsolateDataFailed(CCallbackArgs& args)PN_OVERRIDE {}
	virtual void OnIsolateDataSuccess(CCallbackArgs& args)PN_OVERRIDE {}
	virtual void OnDeisolateDataFailed(CCallbackArgs& args)PN_OVERRIDE {}
	virtual void OnDeisolateDataSuccess(CCallbackArgs& args)PN_OVERRIDE {}
};
