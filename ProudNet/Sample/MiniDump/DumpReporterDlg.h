// BlitzDumpDispatcherDlg.h : Header file
// BlitzDumpDispatcherDlg.h : ��� ����
//

#pragma once

#include "resource.h"

class CDumpReporter;

// CDumpReporterDlg dialog box
// Warning!! If you use this sample, you have to copy exactly same Dlg resource.
// CDumpReporterDlg ��ȭ ����
// ����!! �̿����� �״�� ������ ���Ƕ��� Dlg ���ҽ��� �Ȱ��� �����ϼž��մϴ�.
class CDumpReporterDlg : public CDialog
{
	// Creation
	// ����
public:
	CDumpReporterDlg(CDumpReporter* owner);	// Standard creator // ǥ�� ������

	// Dialog box data
	// ��ȭ ���� ������
	enum { IDD = IDD_DUMP_REPORTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Support DDX/DDV


	// Realization
	// ����
protected:
	HICON m_hIcon;

	// Created message map function.
	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	CStringW m_customMsg;
	CStringW m_userText;
	afx_msg void OnBnClickedOk();
	CDumpReporter* m_owner;
};