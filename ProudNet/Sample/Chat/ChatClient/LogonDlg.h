#pragma once

#include "resource.h"

class CChatClientDlg;

// CLogonDlg dialog box
class CLogonDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogonDlg)

public:
	CLogonDlg(CWnd* pParent = NULL);   // Standard creator
	virtual ~CLogonDlg();

// Dialog box data
	enum { IDD = IDD_LOGON_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Support DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CStringW m_serverAddr;
	CStringW m_userName;

	CChatClientDlg* m_owner;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
	virtual void OnOK();
};
