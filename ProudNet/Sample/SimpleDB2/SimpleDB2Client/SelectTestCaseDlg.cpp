// SelectTestCaseDlg.cpp : Realization file.
// SelectTestCaseDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SelectTestCaseDlg.h"


// SelectTestCaseDlg dialog box.

IMPLEMENT_DYNAMIC(SelectTestCaseDlg, CDialog)

SelectTestCaseDlg::SelectTestCaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SelectTestCaseDlg::IDD, pParent), m_SampleCase(0)
{

}

SelectTestCaseDlg::~SelectTestCaseDlg()
{
}

void SelectTestCaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Cache);
	DDX_Control(pDX, IDC_RADIO2, m_NonCache);
	m_Cache.SetCheck(true);
}


BEGIN_MESSAGE_MAP(SelectTestCaseDlg, CDialog)
	ON_BN_CLICKED(IDOK, &SelectTestCaseDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SelectTestCaseDlg message processor.
// SelectTestCaseDlg 메시지 처리기입니다.

void SelectTestCaseDlg::OnBnClickedOk()
{
	if(m_Cache.GetCheck())
	{
		m_SampleCase = 1;
	}else if(m_NonCache.GetCheck())
		m_SampleCase = 2;
	
	// TODO: Adds control report processor to here.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}
