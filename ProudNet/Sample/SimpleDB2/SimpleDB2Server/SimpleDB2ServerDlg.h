
// SimpleDB2ServerDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
//#include "afxwin.h"

// CSimpleDB2ServerDlg 대화 상자
class CSimpleDB2ServerDlg : public CDialog, public IDbCacheServerDelegate2
{
// 생성입니다.
// Creation
public:
	CSimpleDB2ServerDlg(CWnd* pParent = NULL);	// Standard creator.

// 대화 상자 데이터입니다.
// Dialog box data.
	enum { IDD = IDD_SIMPLEDB2SERVER_DIALOG };

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
	DECLARE_MESSAGE_MAP()

protected:
	CAutoPtr<CDbCacheServer2> m_db2Server;
	
	CCachedTableName m_gamerTable;

	virtual void OnException(const Exception& e) PN_OVERRIDE;
	virtual void OnError(ErrorInfo *errorInfo) PN_OVERRIDE;
	virtual void OnWarning(ErrorInfo *errorInfo) PN_OVERRIDE;

	bool TrialStartDbServer( String dbmsConnectionString,String &outErrMsg );
	
public: // Log
	void LogEvent( LPCWSTR text );

protected:		
	CEdit m_editCtl;
	CFastList<String> m_LogMsgList;
	CriticalSection m_CSLogLock;

	afx_msg LRESULT OnLogWrite(WPARAM, LPARAM);

};
