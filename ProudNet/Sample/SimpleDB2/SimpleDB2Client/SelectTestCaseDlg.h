#pragma once
#include "resource.h"
#include "afxwin.h"

// SelectTestCaseDlg dialog box.

class SelectTestCaseDlg : public CDialog
{
	DECLARE_DYNAMIC(SelectTestCaseDlg)

public:
	SelectTestCaseDlg(CWnd* pParent = NULL);   // Standard creator.
	virtual ~SelectTestCaseDlg();

// Dialog box data
	enum { IDD = IDD_SELECT_DLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // Support DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int m_SampleCase;
	afx_msg void OnBnClickedOk();
	CButton m_Cache;
	CButton m_NonCache;
};
