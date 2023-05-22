
// SimpleDB2ClientDlg.cpp : Realization file
//


#include "stdafx.h"
#include "SimpleDB2Client.h"
#include "SimpleDB2ClientDlg.h"
#include "..\SimpleDB2Common\Vars.h"
#include "DbClient.h"


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
// CAboutDlg dialog box that use for application program information.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	// Dialog box data.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Support DDX/DDV.

// 구현입니다.
// Realization
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSimpleDB2Client_2008Dlg 대화 상자
// CSimpleDB2Client_2008Dlg dialog box




CSimpleDB2ClientDlg::CSimpleDB2ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleDB2ClientDlg::IDD, pParent), m_sampleCase(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSimpleDB2ClientDlg::~CSimpleDB2ClientDlg()
{
}

void CSimpleDB2ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editCtl);
}



BEGIN_MESSAGE_MAP(CSimpleDB2ClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSimpleDB2ClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSimpleDB2ClientDlg::OnBnClickedCancel)
	ON_MESSAGE(MESSAGE_LOG, &CSimpleDB2ClientDlg::OnLogWrite)
END_MESSAGE_MAP()


void CSimpleDB2ClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
// When you add minimize button at dialog box, you may need following code for drawing icon.
// Framework does this operation automatically for MFC application program that use documation/view model.

void CSimpleDB2ClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Device context for drawing

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		// Set icon in the center at client square.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		// Draw icon.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
// Call this function to show cursor during user close minimized window.
HCURSOR CSimpleDB2ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CSimpleDB2ClientDlg 메시지 처리기
// CSimpleDB2ClientDlg message processor

BOOL CSimpleDB2ClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	// Adds "infor..." menu item to system menu.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	// IDM_ABOUTBOX has to stay within system command area.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CStringW strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set icon of this dialog box. If main windows of application program is not main winodw then framework do this operation automatically.
	SetIcon(m_hIcon, TRUE);			// Set icon with big size.
	SetIcon(m_hIcon, FALSE);		// Set icon with small size.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// TODO: Adds additional initialize work to here.
	m_client.Attach(new CDbClient(this));

	Connect();
	m_leaveServer = false;

	// 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
	// Return "TRUE" if you do not set focus about control.
	return TRUE;
}

void CSimpleDB2ClientDlg::Connect()
{
	m_client->RequestCompleteTrue();

	m_client->m_GamerName = GetRandomUserName();
	m_client->m_heroName = GetRandomUserName();


	CDbCacheClient2ConnectParameter p1;
	p1.m_authenticationKey = g_DbAuthKey;
	p1.m_delegate = this;
	p1.m_serverAddr = L"localhost";
	p1.m_serverPort = g_DbServerPort;
	if (!m_client->m_DbClient->Connect(p1))
	{
		// DB cache2 클라의 서버 연결 시도 실패
		// Failed to server connecting of DB cache2 client
		throw Proud::Exception(LoadString(IDS_STRING102));
	}

}

void CSimpleDB2ClientDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CSimpleDB2ClientDlg::OnBnClickedOk()
{
	m_client->SimpleTest();
}

/************************************************************* override ****************************************************************/

void CSimpleDB2ClientDlg::OnJoinDbCacheServerComplete(ErrorInfo *info)
{
	if (info->m_errorType == ErrorType_Ok)
	{
		m_leaveServer = false;
		// Succeed to connect DbCacheServer.
		LogEvent(LoadString(IDS_STRING117));
	}
	else
		// Failed to connect DbCacheServer.
		LogEvent(LoadString(IDS_STRING118));

	m_client->RequestCompleteTrue();
}

void CSimpleDB2ClientDlg::OnLeaveDbCacheServer(ErrorType reason)
{
	m_leaveServer = true;
}

void CSimpleDB2ClientDlg::OnDbmsWriteDone(DbmsWritePropNodePendType type, Proud::Guid loadedDataGuid)
{
	// 일정 시간에 한번씩 cache서버에서 DB에 데이터를 입력하면 오게 되는 이벤트 입니다.
	// 각 이벤트 들과는 별도로 cache에 변동이 있다면 일정 시간 후 Db에 저장한 후에 이벤트가 발생됩니다.
	// Receiving event when enter data to DB from cache server every specific time.
	// Apart from each event, if cache has changed then event occur after saving to DB.
	m_lastWriteDoneGuid = loadedDataGuid;

	LogEvent(L"OnDbmsWriteDone");


	String strType = Proud::TypeToString(type);

	// "Client : Dbms에 Write를 성공했습니다. HostID : %d 타입 : %s, Guid : %s "
	// "Client : Succeed to Write at Dbms. HostID : %d Type : %s, Guid : %s "
	String text;
	text.Format(LoadString(IDS_STRING103),
		(int)m_client->m_DbClient->GetLocalHostID(), strType.GetString(), loadedDataGuid.ToString().GetString());
	LogEvent(text.GetString());
}

void CSimpleDB2ClientDlg::OnExclusiveLoadDataComplete(CCallbackArgs& args)
{
	for (int i = 0; i < args.m_items.GetCount(); ++i)
	{
		if (args.m_items[i].m_reason == ErrorType_Ok)
		{
			LogEvent(LoadString(IDS_STRING116) + L" : OnExclusiveLoadData");

			m_client->m_data = args.m_items[i].m_loadedData; // 데이터 최신화 // Data renewal
			m_client->m_SessionGuid = args.m_items[i].m_sessionGuid;

			if (!m_client->m_data)
			{
				// Guid가 잘못되었 습니다.
				// Wrong Guid.
				LogError(LoadString(IDS_STRING105));
				m_client->m_DbClient->UnloadDataBySessionGuid(args.m_items[i].m_sessionGuid);
				return;
			}

			// "로딩 요청 결과 대기 성공"
			// "Succeed to wait loading request result"
			LogEvent(LoadString(IDS_STRING106));

			// Request형의 경우 반드시 응답을 받은 것을 확인한 후에 처리 합니다.
			// If it is Request type then process it once check about receiving request.
			m_client->RequestCompleteTrue();
		}
		else
		{
			m_client->RequestCompleteTrue();
			LogEvent(LoadString(IDS_STRING113) + L" : OnExclusiveLoadData");

			// "독점 데이터 로딩 실패! 이유: "	
			// "Failed exclusive data loading! Reason: "	
			LogError(LoadString(IDS_STRING104) + ErrorInfo::TypeToString(args.m_items[i].m_reason));
		}
	}
}

void CSimpleDB2ClientDlg::OnDataUnloadRequested(CCallbackArgs& args)
{
	LogEvent(L"OnDataUnload");
	m_client->m_data = args.m_items[0].m_loadedData; // 데이터 최신화 // Data renewal

	// Request형의 경우 반드시 응답을 받은 것을 확인한 후에 처리 합니다.
	// If it is Request type then process it once check about receiving request.
	m_client->RequestCompleteTrue();
}
void CSimpleDB2ClientDlg::OnDataForceUnloaded(CCallbackArgs& args)
{
	LogEvent(L"OnDataForceUnloaded");
}

void CSimpleDB2ClientDlg::OnAddDataFailed(CCallbackArgs& args)
{

	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	m_client->RequestCompleteTrue();

	// "OnAddData 실패. 이유:%s 코멘트:%s"
	// "OnAddData failed. Reason:%s Comment:%s"
	String text;
	text.Format(LoadString(IDS_STRING108), ErrorInfo::TypeToString(args.m_items[0].m_reason), args.m_items[0].m_comment);
	LogError(text.GetString());
}

void CSimpleDB2ClientDlg::OnAddDataSuccess(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	m_client->m_data = args.m_items[0].m_loadedData;

	// TypeName값은 대문자로 들어옵니다.
	// TypeName value enteringt as upper case.
	if (!args.m_items[0].m_data->GetStringTypeName().Compare(L"HERO"))
	{
		m_client->m_heroGuid = args.m_items[0].m_data->UUID;
	}
	else if (!args.m_items[0].m_data->GetStringTypeName().Compare(L"ITEM"))
	{
		m_client->m_itemGuid = args.m_items[0].m_data->UUID;
	}
	else if (!args.m_items[0].m_data->GetStringTypeName().Compare(L"FRIEND"))
	{
		m_client->m_friendGuid = args.m_items[0].m_data->UUID;
	}

	// Request형의 경우 반드시 응답을 받은 것을 확인한 후에 처리 합니다.
	// If it is Request type then process it once check about receiving request.
	m_client->RequestCompleteTrue();
	LogEvent(LoadString(IDS_STRING116) + L" : AddData");
}

void CSimpleDB2ClientDlg::OnUpdateDataFailed(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}
	// Request형의 경우 반드시 응답을 받은 것을 확인한 후에 처리 합니다.
	// If it is Request type then process it once check about receiving request.
	m_client->RequestCompleteTrue();

	String text;
	text.Format(L"%s : UpdateData - %s:%s Comment:%s", LoadString(IDS_STRING113), LoadString(IDS_STRING120), ErrorInfo::TypeToString(args.m_items[0].m_reason), args.m_items[0].m_comment);
	LogError(text.GetString());
}

void CSimpleDB2ClientDlg::OnUpdateDataSuccess(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	m_client->m_data = args.m_items[0].m_loadedData; // 데이터 최신화 // Data renewal

	// Request형의 경우 반드시 응답을 받은 것을 확인한 후에 처리 합니다.
	// If it is Request type then process it once check about receiving request.
	m_client->RequestCompleteTrue();


	LogEvent(LoadString(IDS_STRING116) + L" : UpdateData ");
}

void CSimpleDB2ClientDlg::OnRemoveDataFailed(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	// Request형의 경우 반드시 응답을 받은 것을 확인한 후에 처리 합니다.
	// If it is Request type then process it once check about receiving request.
	m_client->RequestCompleteTrue();

	String text;
	text.Format(L"%s : RemoveData - %s:%s Comment:%s", LoadString(IDS_STRING113), LoadString(IDS_STRING120), ErrorInfo::TypeToString(args.m_items[0].m_reason), args.m_items[0].m_comment);
	LogError(text.GetString());
}

void CSimpleDB2ClientDlg::OnRemoveDataSuccess(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	m_client->m_data = args.m_items[0].m_loadedData; // 데이터 최신화 // Data renewal

	// Request형의 경우 반드시 응답을 받은 것을 확인한 후에 처리 합니다.
	// If it is Request type then process it once check about receiving request.
	m_client->RequestCompleteTrue();

	LogEvent(LoadString(IDS_STRING116) + L"RemoveData");
}

void CSimpleDB2ClientDlg::OnException(const Exception &e)
{
	String str;
	str.Format(L"Exception! %s: %s", LoadString(IDS_STRING120), StringA2W(e.what()).GetString());
	LogError(str.GetString());
}

void CSimpleDB2ClientDlg::OnWarning(ErrorInfo *errorInfo)
{
	LogError(L"Warning! %s: %s", LoadString(IDS_STRING120), errorInfo->ToString());
}

void CSimpleDB2ClientDlg::OnError(ErrorInfo *errorInfo)
{
	LogError(L"Error! %s: %s", LoadString(IDS_STRING120), errorInfo->ToString());
}

void CSimpleDB2ClientDlg::OnAccessError(CCallbackArgs& args)
{
	LogError(L"AccessError! Comment: %s", args.m_items[0].m_comment);
}

/************* NonExclusive ***************/

// void CSimpleDB2ClientDlg::OnNonExclusiveSnapshotDataFailed( CCallbackArgs& args )
// {
// 	if(m_client->m_diffCustomArg != args.m_tag)
// 	{
// 		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
// 		// "Waiting ticket does not match. It is wrong setting or server error."
// 		LogError(LoadString(IDS_STRING107));
// 	}
// 	
// 	m_client->RequestCompleteTrue();
// 	
// 	// "비독점 로드 스냅샷 실패. 이유:  "
// 	// "Failed unexclusive load snapshot. Reason:  "
// 	LogError(LoadString(IDS_STRING109) + ErrorInfo::TypeToString(args.m_reason));
// }
// 
// void CSimpleDB2ClientDlg::OnNonExclusiveSnapshotDataSuccess(CCallbackArgs& args )
// {
// 	if(m_client->m_diffCustomArg != args.m_tag)
// 	{
// 		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
// 		// "Waiting ticket does not match. It is wrong setting or server error."
// 		CStringW logString;
// 		logString.LoadStringW(IDS_STRING107);
// 		LogError(logString);
// 	}
// 	
// 	m_client->m_data = args.m_loadedData;
// 
// 	m_client->RequestCompleteTrue();
// 
// 	// "비독점 로드 스냅샷 성공!"
// 	// "Succeed unexclusive load snapshot!"
// 	LogEvent(LoadString(IDS_STRING110));
// }

void CSimpleDB2ClientDlg::OnNonExclusiveSnapshotDataComplete(CCallbackArgs& args)
{
	for (int i = 0; i < args.m_items.GetCount(); ++i)
	{
		if (args.m_items[i].m_reason == ErrorType_Ok)
		{
			if (m_client->m_diffCustomArg != args.m_tag)
			{
				// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
				// "Waiting ticket does not match. It is wrong setting or server error."
				CStringW logString;
				logString.LoadStringW(IDS_STRING107);
				LogError(logString);
			}

			m_client->m_data = args.m_items[i].m_loadedData;

			m_client->RequestCompleteTrue();

			// "비독점 로드 스냅샷 성공!"
			// "Succeed unexclusive load snapshot!"
			LogEvent(LoadString(IDS_STRING110));
		}
		else
		{
			if (m_client->m_diffCustomArg != args.m_tag)
			{
				// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
				// "Waiting ticket does not match. It is wrong setting or server error."
				LogError(LoadString(IDS_STRING107));
			}

			m_client->RequestCompleteTrue();

			// "비독점 로드 스냅샷 실패. 이유:  "
			// "Failed unexclusive load snapshot. Reason:  "
			LogError(LoadString(IDS_STRING109) + ErrorInfo::TypeToString(args.m_items[i].m_reason));
		}
	}
}

// 아래의 스넵샷 이벤트 들은 args.m_loadedData값을 주지 않습니다.
// 아래의 스넵샷 이벤트에서의 args.m_loadedData은 NULL 이며, args.m_data만을 지원합니다.
// Following snapshot events do not provide args.m_loadedData value.
// args.m_loadedData of following snapshot event is NULL and only support args.m_data.
void CSimpleDB2ClientDlg::OnNonExclusiveAddDataAck(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
		return;
	}

	if (args.m_items[0].m_reason != ErrorType_Ok)
	{
		String text;
		text.Format(L"%s : NonExclusiveAddData - %s", LoadString(IDS_STRING113), args.m_items[0].m_comment.GetString());
		LogError(text.GetString());
		return;
	}

}

void CSimpleDB2ClientDlg::OnNonExclusiveRemoveDataAck(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	m_client->RequestCompleteTrue();

	if (args.m_items[0].m_reason != ErrorType_Ok)
	{
		CStringW txt;
		txt.Format(L"%s : NonExclusiveRemoveData - %s", LoadString(IDS_STRING113), args.m_items[0].m_comment);
		LogError(txt.GetString(), args.m_items[0].m_comment);
	}
}

void CSimpleDB2ClientDlg::OnNonExclusiveModifyValueSuccess(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	m_client->RequestCompleteTrue();

	if (args.m_items[0].m_reason != ErrorType_Ok)
	{
		CStringW txt;
		txt.Format(L"%s : NonExclusiveModifyValue - %s", LoadString(IDS_STRING113), args.m_items[0].m_comment);
		LogError(txt.GetString(), args.m_items[0].m_comment);
	}
}

void CSimpleDB2ClientDlg::OnNonExclusiveModifyValueFailed(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	m_client->RequestCompleteTrue();

	if (args.m_items[0].m_reason != ErrorType_Ok)
	{
		CStringW txt;
		txt.Format(L"%s : NonExclusiveModifyValue - %s", LoadString(IDS_STRING113), args.m_items[0].m_comment);
		LogError(txt.GetString(), args.m_items[0].m_comment);
	}
}

void CSimpleDB2ClientDlg::OnNonExclusiveSetValueIfSuccess(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	m_client->RequestCompleteTrue();

	if (args.m_items[0].m_reason != ErrorType_Ok)
	{
		CStringW txt;
		txt.Format(L"%s : NonExclusiveSetValue - %s", LoadString(IDS_STRING113), args.m_items[0].m_comment);
		LogError(txt.GetString(), args.m_items[0].m_comment);
	}
}

void CSimpleDB2ClientDlg::OnNonExclusiveSetValueIfFailed(CCallbackArgs& args)
{
	if (m_client->m_diffCustomArg != args.m_tag)
	{
		// "대기표가 같지 않습니다. 설정이 잘못되었거나 서버 오류입니다."
		// "Waiting ticket does not match. It is wrong setting or server error."
		LogError(LoadString(IDS_STRING107));
	}

	m_client->RequestCompleteTrue();

	if (args.m_items[0].m_reason != ErrorType_Ok)
	{
		CStringW txt;
		txt.Format(L"%s : NonExclusiveSetValue - %s", LoadString(IDS_STRING113), args.m_items[0].m_comment);
		LogError(txt.GetString(), args.m_items[0].m_comment);
	}
}

// 밑의 이벤트 독점 로딩하고 있는 Client에게 발생됩니다.
// It occurs to Client who does exclusive loading of following event.
void CSimpleDB2ClientDlg::OnSomeoneAddData(CCallbackArgs& args)
{
	m_client->m_data = args.m_items[0].m_loadedData;

	// TypeName값은 대문자로 들어옵니다.
	// TypeName value receive as upper case.
	if (!args.m_items[0].m_data->GetStringTypeName().Compare(L"HERO"))
	{
		m_client->m_heroGuid = args.m_items[0].m_data->UUID;
	}
	else if (!args.m_items[0].m_data->GetStringTypeName().Compare(L"ITEM"))
	{
		m_client->m_itemGuid = args.m_items[0].m_data->UUID;
	}
	else if (!args.m_items[0].m_data->GetStringTypeName().Compare(L"FRIEND"))
	{
		m_client->m_friendGuid = args.m_items[0].m_data->UUID;
	}

	m_client->RequestCompleteTrue();

	LogEvent(L"SomeoneAddData ack");
}

void CSimpleDB2ClientDlg::OnSomeoneRemoveData(CCallbackArgs& args)
{
	m_client->m_data = args.m_items[0].m_loadedData;

	m_client->RequestCompleteTrue();

	LogEvent(L"SomeoneRemoveData ack");
}

void CSimpleDB2ClientDlg::OnSomeoneModifyValue(CCallbackArgs& args)
{
	m_client->m_data = args.m_items[0].m_loadedData;

	m_client->RequestCompleteTrue();

	LogEvent(L"SomeoneModifyValue ack");
}

void CSimpleDB2ClientDlg::OnSomeoneSetValue(CCallbackArgs& args)
{
	m_client->m_data = args.m_items[0].m_loadedData;

	m_client->RequestCompleteTrue();

	LogEvent(L"SomeoneSetValue ack");
}


/*************************************************************** LOG ******************************************************************/

LRESULT CSimpleDB2ClientDlg::OnLogWrite(WPARAM, LPARAM)
{
	if (!m_hWnd)
	{
		return true;
	}

	CStringW text;
	{
		CriticalSectionLock TRLock(m_CSLogLock, true);
		text = m_LogMsgList.RemoveHead().GetString();
	}

	int leng = m_editCtl.GetWindowTextLength();
	m_editCtl.SetSel(leng, leng);
	m_editCtl.ReplaceSel((LPCWSTR)(text + L"\r\n"));

	return true;
}

void CSimpleDB2ClientDlg::LogEvent(LPCWSTR text)
{
	if (!m_hWnd)
	{
		return;
	}

	CriticalSectionLock TRLock(m_CSLogLock, true);

	String logString;
	logString.Format(L"[Info] %s", text);
	m_LogMsgList.AddTail(logString);

	PostMessage(MESSAGE_LOG, NULL, NULL);
	return;
}

void CSimpleDB2ClientDlg::LogError(LPCWSTR text, ...)
{
	if (!m_hWnd)
	{
		return;
	}

	String logString;
	logString.Format(L"[Error] %s", text);

	va_list	pArg;
	va_start(pArg, text);
	logString.FormatV(text, pArg);
	va_end(pArg);

	CriticalSectionLock TRLock(m_CSLogLock, true);

	String addString = L"[Error]";
	addString += logString;

	m_LogMsgList.AddTail(addString);

	PostMessage(MESSAGE_LOG, NULL, NULL);
	return;

}

CStringW CSimpleDB2ClientDlg::LoadString(int stringResourceNum)
{
	CStringW loadString;
	if (!loadString.LoadStringW(stringResourceNum))
	{
		LogError(L"Failed to string load");
	}
	return loadString;
}

