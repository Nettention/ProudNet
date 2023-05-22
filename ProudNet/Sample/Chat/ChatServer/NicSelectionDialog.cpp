// NicSelectionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NicSelectionDialog.h"
#include "..\ChatCommon\vars.h"

CStringW NoNicText = L"<Any>";

// CNicSelectionDialog dialog

IMPLEMENT_DYNAMIC(CNicSelectionDialog, CDialog)

CNicSelectionDialog::CNicSelectionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNicSelectionDialog::IDD, pParent)
{

}

CNicSelectionDialog::~CNicSelectionDialog()
{
}

void CNicSelectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_nicListCtl);
}


BEGIN_MESSAGE_MAP(CNicSelectionDialog, CDialog)
END_MESSAGE_MAP()


// CNicSelectionDialog message handlers

BOOL CNicSelectionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i=0;i<m_candidates.GetSize();i++)
	{
		int selIdx = m_nicListCtl.AddString(m_candidates[i]);
		if(m_candidates[i]==NoNicText)
			m_nicListCtl.SetCurSel(selIdx);

		m_nicListCtl.SetItemData(selIdx,i);
	}

	MoveWindowALittle(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNicSelectionDialog::OnOK()
{
	int idx = m_nicListCtl.GetItemData(m_nicListCtl.GetCurSel());
	m_selectedNic = m_candidates[idx];

	if(m_selectedNic==NoNicText)
		m_selectedNic = L"";

	CDialog::OnOK();
}
