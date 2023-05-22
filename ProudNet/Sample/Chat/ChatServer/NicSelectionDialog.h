#pragma once

#include "resource.h"
#include "afxwin.h"

extern CStringW NoNicText;
// CNicSelectionDialog dialog

class CNicSelectionDialog : public CDialog
{
	DECLARE_DYNAMIC(CNicSelectionDialog)

public:
	CStringW m_selectedNic;
	CStringArray m_candidates;
	CNicSelectionDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNicSelectionDialog();

// Dialog Data
	enum { IDD = IDD_NIC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_nicListCtl;
protected:
	virtual void OnOK();
};
