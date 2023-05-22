// MiniDump_TestDlg.h : Header file
// MiniDump_TestDlg.h : ��� ����
//

#pragma once


// CMiniDump_TestDlg dialog box
class CMiniDump_TestDlg : public CDialog
{
// Creation
// ����
public:
	CMiniDump_TestDlg(CWnd* pParent = NULL);	// Standard creator

// Dialog box data
// ��ȭ ���� ������
	enum { IDD = IDD_MINIDUMP_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Support DDX/DDV


// Realization
// ����
protected:
	HICON m_hIcon;

	// Created message map function.
	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedManualdump();

};
