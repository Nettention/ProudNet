#pragma once

#include <afxinet.h>
#include "../../include/DumpClient.h"
#include "resource.h"

using namespace Proud;
class CDumpReporter;

// ����!! �̿����� �״�� ������ ���Ƕ��� Dlg ���ҽ��� �Ȱ��� �����ϼž��մϴ�.
// Warning!! If you use this sample, you have to copy exactly same Dlg resource.
class CDumpReportProgressDlg : public CDialog, public IDumpClientDelegate
{
	DECLARE_DYNAMIC(CDumpReportProgressDlg)

public:
	CDumpReportProgressDlg(CWnd* pParent = NULL);   // Standard creator
	virtual ~CDumpReportProgressDlg();

	// ��ȭ ���� �������Դϴ�.
	// Dialog box data
	enum { IDD = IDD_DUMP_REPORT_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Support DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	//static UINT StaticThreadProc(void*context);
	//void ThreadProc();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_stateCtl;
	volatile bool m_stopNow;
	CDumpReporter* m_owner;
	CAutoPtr<CDumpClient> m_dumpClient;

	bool MustStopNow() override;
	void OnException(const Exception &e) override;
	void OnComplete() override;
protected:
	virtual void OnCancel();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//void UseHttpSendReqEx(HINTERNET hRequest);
};