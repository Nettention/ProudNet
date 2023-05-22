// DispatchStatusDlg.cpp : ���� �����Դϴ�.
// DispatchStatusDlg.cpp : Realization file
//

#include "stdafx.h"
#include "DumpReportProgressDlg.h"
#include "../../include/Exception.h"
#include "DumpReporterImpl.h"

#include <WinInet.h>
#pragma comment(lib,"wininet.lib")

using namespace std;

#define _COUNTOF(array) (sizeof(array)/sizeof(array[0]))

static const DWORD WM_THREAD_STOPPED = WM_USER + 1;
static const DWORD WM_THREAD_STOPPED2 = WM_USER + 2;

// CDumpReportProgressDlg ��ȭ �����Դϴ�.
// CDumpReportProgressDlg dialog box

IMPLEMENT_DYNAMIC(CDumpReportProgressDlg, CDialog)
CDumpReportProgressDlg::CDumpReportProgressDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CDumpReportProgressDlg::IDD, pParent),
		m_dumpClient(CDumpClient::Create(this))
{
}

CDumpReportProgressDlg::~CDumpReportProgressDlg()
{
}

void CDumpReportProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_stateCtl);
}


BEGIN_MESSAGE_MAP(CDumpReportProgressDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDumpReportProgressDlg �޽��� ó�����Դϴ�.
// CDumpReportProgressDlg message processor

//UINT CDumpReportProgressDlg::StaticThreadProc(void*context)
//{
//	CDumpReportProgressDlg* dlg=(CDumpReportProgressDlg*)context;
//	dlg->ThreadProc();
//	return 0;
//}

BOOL CDumpReportProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_stopNow = false;

	SetTimer(1, 100, 0);

	m_dumpClient->Start(m_owner->m_serverAddr.GetString(), m_owner->m_serverPort, m_owner->m_dumpFileName.GetString());

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
	// Exception: OCX property page has to return "FALSE".
}

void CDumpReportProgressDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_dumpClient->FrameMove();

	// ���� ���¸� ǥ���Ѵ�. ó���� �������� â�� �ݴ´�.
	// Indicate current state. If it finished process then close window.
	switch (m_dumpClient->GetState())
	{
	case CDumpClient::Connecting:
		m_stateCtl.SetWindowText(L"Connecting...");
		break;
	case CDumpClient::Sending:
	{
		WCHAR s[1000];
#if (_MSC_VER>=1400)
		_stprintf_s(s, _COUNTOF(s), L"Sending.(%d%%)", int(float(m_dumpClient->GetSendProgress()) / float(m_dumpClient->GetSendTotal())*100.f));
#else
		_stprintf(s, L"Sending.(%d%%)", int(float(m_dumpClient->GetSendProgress()) / float(m_dumpClient->GetSendTotal())*100.f));
#endif

		m_stateCtl.SetWindowText(s);
	}
	break;
	case CDumpClient::Closing:
		m_stateCtl.SetWindowText(L"Closing.");
		break;
	case CDumpClient::Stopped:
		m_stateCtl.SetWindowText(L"Complete");
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDumpReportProgressDlg::OnCancel()
{
	m_stopNow = true;

	CDialog::OnCancel();
}

//// ���ε� ó���� ������ ����ϴ� �Լ��̴�.
//// Function that actually in charged with upload process.
//void CDumpReportProgressDlg::ThreadProc()
//{
//	HINTERNET hSession=NULL,hConnect=NULL,hRequest=NULL;
//
//	bool exceptionOccurred=false;
//	try
//	{
//		hSession = InternetOpen( _T("HttpSendRequestEx"), INTERNET_OPEN_TYPE_PRECONFIG,
//			NULL, NULL, 0);
//		if(!hSession)
//			throw Exception(_T("InternetOpen"));
//		hConnect = InternetConnect(hSession, m_owner->m_serverAddr, m_owner->m_serverPort,
//			NULL, NULL, INTERNET_SERVICE_HTTP,NULL, NULL);
//		if (!hConnect)
//			throw Exception( _T("Failed to connect") );
//		hRequest = HttpOpenRequest(hConnect, _T("POST"), m_owner->m_pageAddr,
//			NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);
//		if (!hRequest)
//			throw Exception( _T("Failed to open request handle") );
//		m_state=Sending;
//		UseHttpSendReqEx(hRequest);
//	}
//	catch(CException*e)
//	{
//		exceptionOccurred=true;
//		PostMessage(WM_THREAD_STOPPED,(WPARAM)e);
//	}
//	catch(exception&e)
//	{
//		exceptionOccurred=true;
//		Exception* ne=new Exception(e);
//		PostMessage(WM_THREAD_STOPPED2,(WPARAM)ne);
//	}
//	m_state=Closing;
//	if(hRequest!=NULL)
//		InternetCloseHandle(hRequest);
//	if(hConnect!=NULL)
//		InternetCloseHandle(hConnect);
//	m_state=Stopped;
//	if(exceptionOccurred==false)
//		PostMessage(WM_THREAD_STOPPED,(WPARAM)0);
//}

LRESULT CDumpReportProgressDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_THREAD_STOPPED:
	{
		CException*e = (CException*)wParam;
		if (e)
		{
			e->ReportError(MB_OK | MB_ICONHAND);
			e->Delete();
		}
		else
			AfxMessageBox(L"Transfer complete");

		EndDialog(IDCANCEL);
	}
	break;
	case WM_THREAD_STOPPED2:
	{
		exception*e = (exception*)wParam;
		if (e)
		{
			AfxMessageBox(CA2T(e->what()), MB_OK | MB_ICONHAND);
			delete e;
		}
		else
			AfxMessageBox(L"Transfer complete");

		EndDialog(IDCANCEL);
	}
	break;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}


//void CDumpReportProgressDlg::UseHttpSendReqEx(HINTERNET hRequest)
//{
//	CFile file(m_owner->m_finder.GetFilePath(),CFile::modeRead|CFile::shareDenyWrite);
//
//	INTERNET_BUFFERS BufferIn;
//	BYTE pBuffer[4096];
//
//	BufferIn.dwStructSize = sizeof( INTERNET_BUFFERS ); // Must be set or error will occur
//	BufferIn.Next = NULL;
//	BufferIn.lpcszHeader = NULL;
//	BufferIn.dwHeadersLength = 0;
//	BufferIn.dwHeadersTotal = 0;
//	BufferIn.lpvBuffer = NULL;
//	BufferIn.dwBufferLength = 0;
//	BufferIn.dwBufferTotal = (DWORD)m_owner->m_finder.GetLength(); // This is the only member used other than dwStructSize
//	BufferIn.dwOffsetLow = 0;
//	BufferIn.dwOffsetHigh = 0;
//
//	if(!HttpSendRequestEx( hRequest, &BufferIn, NULL, 0, 0))
//	{
//		throw Exception( _T("Error on HttpSendRequestEx %d\n"),GetLastError() );
//	}
//
//	while(1)
//	{
//		if(MustStopNow())
//			//goto cancelled; // HTTP end req�� �����ϰ� �ٷ� hRequest�� close�Ѵ�. �̷��� �ϸ� �۽��� ��ҵɶ�?
//			goto cancelled; // Skip HTTP end req then cloase hRequest. It may cancel transmission of message.
//
//		UINT r=file.Read(pBuffer,4096);
//
//		DWORD dwBytesWritten;
//		BOOL bRet=InternetWriteFile( hRequest, pBuffer, 4096, &dwBytesWritten);
//		if(!bRet)
//			throw Exception( _T("Error on InternetWriteFile %lu"),GetLastError() );
//		m_sendProgress+=r;
//		if(r<4096)
//			break;
//	}
//
//	if(!HttpEndRequest(hRequest, NULL, 0, 0))
//	{
//		throw Exception( _T("Error on HttpEndRequest %lu \n"), GetLastError());
//	}
//cancelled:
//	;
//}

bool CDumpReportProgressDlg::MustStopNow()
{
	return m_stopNow;
}

void CDumpReportProgressDlg::OnException(const Exception &e)
{
	PostMessage(WM_THREAD_STOPPED2, (WPARAM)new Exception(e));
}

void CDumpReportProgressDlg::OnComplete()
{
	PostMessage(WM_THREAD_STOPPED, 0);
}