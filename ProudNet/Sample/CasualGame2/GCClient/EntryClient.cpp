#include "StdAfx.h"
#include "entryClient.h"
#include "world.h"
#include "resource.h"
#include "../GCCommon/Entry_common.cpp"
#include "../GCCommon/Entry_proxy.cpp"
#include "../GCCommon/Entry_stub.cpp"

CEntryClient::CEntryClient()
{
	m_NetClientName = L"EntryClient";
	// Creates client object
	// 클라 객체 생성
	m_NetClient.Attach(CNetClient::Create());
	m_NetClient->SetEventSink(this);

	// Initialize RMI
	// RMI 초기화
	m_NetClient->AttachStub(this);
	m_NetClient->AttachProxy(&m_c2sProxy);
}

CEntryClient::~CEntryClient(void)
{
	// Close client network module. It does role of disconnecting with server.
	// 클라이언트 네트워크 모듈을 종료합니다. 즉 서버와의 연결을 끊는 역할도 합니다.
	m_NetClient.Free();

	// Removes from other client form which refer this object.
	// 이 객체를 참조하는 타 클라 폼에서도 제거합니다.
	CWorld::GetSharedPtr()->m_selectHeroForm.GetListBox(IDC_HERO_LISTBOX)->RemoveAllItems();
	CWorld::GetSharedPtr()->m_selectChannelForm.GetListBox(IDC_LOBBY_LISTBOX)->RemoveAllItems();
}

/// Handler notice result whether it has succeed to connect with server ot not
/// 서버와의 연결 시도 후 연결이 성공했는지 실패했는지 결과가 나오는 Handler
void CEntryClient::OnJoinServerComplete(ErrorInfo *info, const ByteArray &replyFromServer)
{
	if (info->m_errorType != ErrorType_Ok)
	{
		// If it failed to connect with server, indicate error then go to close mode.
		// 서버와의 연결이 실패했으면 에러를 표시한 후 종료 모드로 돌입.
		CStringW txt;
		txt.Format(L"%s\n%s", CWorld::GetSharedPtr()->LoadString(IDS_STRING124), info->ToString());

		CWorld::GetSharedPtr()->GotoErrorForm(txt);

		CWorld::GetSharedPtr()->m_isConnectEntryServer = false;
	}
	else
	{
		CWorld::GetSharedPtr()->m_isConnectEntryServer = true;

		// When it returns to EntryServer from Lobby, it operates logon porcess again.
		// Lobby 에서 EntryServer로 되돌아온경우 다시 로그온 절차를 시행합니다.
		if (CWorld::GetSharedPtr()->m_isComebackFromLobby == true)
		{
			m_c2sProxy.RequestReturnToEntry(HostID_Server, RmiContext::SecureReliableSend,
				CWorld::GetSharedPtr()->m_credential);


			CWorld::GetSharedPtr()->m_isComebackFromLobby = false;
		}
		else
		{
			// Activate only button, if it is register window (m_connectForm). 
			// 회원가입창 (m_connectForm) 인경우엔 버튼만 활성화 합니다. 
			CWorld::GetSharedPtr()->m_connectForm.GetButton(IDC_CONNECT)->SetEnabled(false);
			CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_USER_NAME)->SetEnabled(true);
			CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_PASSWORD)->SetEnabled(true);
			CWorld::GetSharedPtr()->m_connectForm.GetEditBox(IDC_PASSWORDCHECK)->SetEnabled(true);
			CWorld::GetSharedPtr()->m_connectForm.GetButton(IDC_OK)->SetEnabled(true);
			CWorld::GetSharedPtr()->m_connectForm.GetButton(IDC_CANCEL)->SetEnabled(true);
		}
	}
}

DEFRMI_EntryS2C_NotifyCreateNewGamerSuccess(CEntryClient)
{
	// Move to logon window because it loaded new data correctly...
	//새로운 data가 잘 로드 됐으니 로그인 창으로.,..
	CWorld::GetSharedPtr()->m_gamerguid = Gamerguid;
	CWorld::GetSharedPtr()->GotoJoinForm();

	return true;
}

DEFRMI_EntryS2C_NotifyCreateNewGamerFailed(CEntryClient)
{
	// "Failed to new GamerData Load! Reason: %s"
	// "새로운 GamerData Load가  실패했습니Farm! 이유: %s"
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING113);
	CStringW txt;
	txt.Format(loadString, reason);
	CWorld::GetSharedPtr()->GotoErrorForm(txt);
	return true;
}

/// When it ends connection with server
/// 서버와의 접속이 종료된 경우
void CEntryClient::OnLeaveServer(ErrorInfo *errInfo)
{

}

/// Try to connect with server.
/// 서버와의 연결을 시도합니다.
bool CEntryClient::Connect()
{
	CNetConnectionParam param;

	param.m_protocolVersion = CSettings::GetSharedPtr()->GetEntryVersion();
	param.m_serverPort = CSettings::GetSharedPtr()->GetEntryServerPort();
	param.m_serverIP = CWorld::GetSharedPtr()->m_ServerAddr;

	return m_NetClient->Connect(param);

}

/// Processing each frame
/// 매 프레임마다의 처리
void CEntryClient::FrameMove(float fElapsedTime)
{
	// Call CNetClient.FrameMove.
	// CNetClient.FrameMove를 호출.
	m_NetClient->FrameMove();
}


DEFRMI_EntryS2C_NotifyFirstLogonFailed(CEntryClient)
{
	// Indicate error then go to quit mode.
	// 에러를 표시한 후 종료 모드로 돌입.
	CStringW loadString;
	loadString.LoadString(IDS_STRING137);
	CStringW txt;
	txt.Format(L"%s\n%d", loadString, reason);

	CWorld::GetSharedPtr()->GotoErrorForm(txt);

	return true;
}

DEFRMI_EntryS2C_NotifyFirstLogonSuccess(CEntryClient)
{
	// Save received credential.
	// 받은 credential을 저장해둡니다.
	CWorld::GetSharedPtr()->m_gamerguid = Gamerguid;
	CWorld::GetSharedPtr()->m_credential = credential;

	// It succeed logon then move to character selection UI.
	// 로그온이 성공했으므로 캐릭터 선택 UI로 이동합니다.
	CWorld::GetSharedPtr()->GotoSelectHeroForm();

	return true;
}

DEFRMI_EntryS2C_NotifySelectHeroFailed(CEntryClient)
{
	// It will not be happend! Shows error form then close it.
	// "Failed to player character selection! Reason: %s"
	// 있어서는 안되는 상황입니다! 에러 폼을 표시하고 종료하도록 하자.
	// "플레이어 캐릭터 선택이 실패했습니Farm! 이유: %s"
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING114);

	CStringW txt;
	txt.Format(loadString, reason);
	CWorld::GetSharedPtr()->GotoErrorForm(txt);
	return true;
}

DEFRMI_EntryS2C_NotifySelectHeroSuccess(CEntryClient)
{
	// Succeed to select player character. Now go to channel form.
	// 플레이어 캐릭터 선택이 성공했습니다. 이제 채널 폼으로 이동하도록 하자.
	CWorld::GetSharedPtr()->m_localHeroInfo.m_name = heroname;
	CWorld::GetSharedPtr()->GotoSelectChannelForm();

	return true;
}

DEFRMI_EntryS2C_NotifyUnauthedAccess(CEntryClient)
{
	// "Error : Try to play withou logon!"
	// "Error : 로그온하지 않고 게이밍을 시도했습니Farm!"
	CStringW logString;
	logString.LoadStringW(IDS_STRING102);
	CWorld::GetSharedPtr()->GotoErrorForm(logString);

	return true;
}

DEFRMI_EntryS2C_HeroList_Begin(CEntryClient)
{
	// Clean up HERO SLOT because it started to receive HERO LIST.
	// HERO LIST를 받기 시작하므로 HERO SLOT을 청소합니다.
	m_entryLocalHeroSlots.Clear();
	CWorld::GetSharedPtr()->m_selectHeroForm.GetListBox(IDC_HERO_LISTBOX)->RemoveAllItems();

	return true;
}

DEFRMI_EntryS2C_RemovedHeroList_Begin(CEntryClient)
{
	// Clean up HERO SLOT because it started to receive HERO LIST.
	// HERO LIST를 받기 시작하므로 HERO SLOT을 청소합니다.
	m_entryRemovedHeroSlots.Clear();
	CWorld::GetSharedPtr()->m_recoverHeroForm.GetListBox(IDC_HERO_LISTBOX)->RemoveAllItems();

	return true;
}

DEFRMI_EntryS2C_HeroList_Add(CEntryClient)
{
	// Adds to hero list slot. Same as UI.
	// hero list slot에 추가합니다. UI에도 마찬가지.
	CEntryLocalHeroSlotPtr newSlot(new CEntryLocalHeroSlot);
	newSlot->m_guid = heroGuid;
	newSlot->m_name = heroName;
	newSlot->m_type = heroType;
	newSlot->m_score = heroScore;
	m_entryLocalHeroSlots.Add(newSlot);

	CStringW txt;
	txt.Format(L"%s [%I64d]", heroName, heroScore);
	CDXUTListBox *lbox = CWorld::GetSharedPtr()->m_selectHeroForm.GetListBox(IDC_HERO_LISTBOX);
	lbox->AddItem(txt, newSlot.get());

	// If first item is added, make it selected automatically.
	// 첫 항목이 추가된 것이라면 자동으로 선택을 해둡니다.
	if (lbox->GetSize() == 1)
		lbox->SelectItem(0);

	return true;
}

DEFRMI_EntryS2C_RemovedHeroList_Add(CEntryClient)
{
	// Adds to hero list slot. Same as UI.
	// hero list slot에 추가합니다. UI에도 마찬가지.
	CEntryLocalHeroSlotPtr newSlot(new CEntryLocalHeroSlot);
	newSlot->m_guid = heroGuid;
	newSlot->m_name = heroName;
	m_entryRemovedHeroSlots.Add(newSlot);

	CStringW txt;
	txt.Format(L"%s", heroName);
	CDXUTListBox *lbox = CWorld::GetSharedPtr()->m_recoverHeroForm.GetListBox(IDC_HERO_LISTBOX);
	lbox->AddItem(txt, newSlot.get());

	// If first item is added, to make it selected automatically.
	// 첫 항목이 추가된 것이라면 자동으로 선택을 해둡니다.
	if (lbox->GetSize() == 1)
		lbox->SelectItem(0);

	return true;
}

DEFRMI_EntryS2C_HeroList_End(CEntryClient)
{
	// Activate button UI.
	// 버튼 UI를 활성화합니다.
	CWorld::GetSharedPtr()->m_selectHeroForm.GetButton(IDC_OK)->SetEnabled(true);
	CWorld::GetSharedPtr()->m_selectHeroForm.GetButton(IDC_REMOVE_HERO_BUTTON)->SetEnabled(true);

	return true;
}

DEFRMI_EntryS2C_RemovedHeroList_End(CEntryClient)
{
	// Activate button UI.
	// 버튼 UI를 활성화합니다.
	CWorld::GetSharedPtr()->m_recoverHeroForm.GetButton(IDC_OK)->SetEnabled(true);

	return true;
}

DEFRMI_EntryS2C_NotifyAddHeroFailed(CEntryClient)
{
	// Activate 'OK' button and notice failure.
	// "Failed to add player character: %s"
	// 그냥 확인 버튼을 활성화합니다. 그리고 실패했음을 표시합니다.
	// "플레이어 캐릭터 추가가 실패했습니Farm: %s"
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING115);
	String txt;
	txt.Format(loadString, reason);
	CWorld::GetSharedPtr()->m_newHeroForm.GetStatic(IDC_ERROR_TEXT)->SetText(txt.GetString());
	CWorld::GetSharedPtr()->m_newHeroForm.GetButton(IDC_OK)->SetEnabled(true);

	return true;
}

DEFRMI_EntryS2C_NotifyAddHeroSuccess(CEntryClient)
{
	// It succeed to add character so return to back.
	// 캐릭터 추가가 성공했으므로 뒤로 돌아갑니다.
	CWorld::GetSharedPtr()->m_newHeroForm.GetButton(IDC_OK)->SetEnabled(true);
	CWorld::GetSharedPtr()->GotoSelectHeroForm();
	CWorld::GetSharedPtr()->GetEntryClient()->m_c2sProxy.RequestHeroSlots(HostID_Server, RmiContext::ReliableSend);

	return true;
}


// Notice that selected one of local player.
// 로컬 플레이어 중 하나를 선택했음을 notify합니다.
DEFRMI_EntryS2C_NotifySelectedHero(CEntryClient)
{
	// Make that list is pointing selected player character.
	// 선택한 플레이어 캐릭터를 리스트가 가리키게 합니다.
	CDXUTListBox* lbox = CWorld::GetSharedPtr()->m_selectHeroForm.GetListBox(IDC_HERO_LISTBOX);
	for (int i = 0; i < lbox->GetSize(); i++)
	{
		CEntryLocalHeroSlot* slot = (CEntryLocalHeroSlot*)lbox->GetItem(i)->pData;
		if (slot->m_guid == heroGuid)
		{
			lbox->SelectItem(i);
			break;
		}
	}

	return true;
}

DEFRMI_EntryS2C_NotifyRemoveHeroSuccess(CEntryClient)
{
	// Renew player character list.
	// 플레이어 캐릭터 목록을 갱신합니다.
	m_c2sProxy.RequestHeroSlots(HostID_Server, RmiContext::ReliableSend);

	return true;
}

DEFRMI_EntryS2C_LobbyList_Begin(CEntryClient)
{
	// Clean up Lobby slot because it started to receive Lobby list.
	// Lobby list를 받기 시작하므로 Lobby slot을 청소합니다.
	m_entryLobbySlots.Clear();
	CWorld::GetSharedPtr()->m_selectChannelForm.GetListBox(IDC_LOBBY_LISTBOX)->RemoveAllItems();

	return true;
}

DEFRMI_EntryS2C_LobbyList_Add(CEntryClient)
{
	// Adds to Lobby list slot. Same as UI.
	// Lobby list slot에 추가합니다. UI에도 마찬가지.
	CEntryLobbySlotPtr newSlot(new CEntryLobbySlot);
	newSlot->m_name = lobbyName;
	newSlot->m_gamerCount = gamerCount;
	newSlot->m_serverAddr = serverAddr;
	m_entryLobbySlots.Add(newSlot);

	CStringW txt;
	txt.Format(L"%s [%d]", lobbyName, gamerCount);
	CDXUTListBox* lbox = CWorld::GetSharedPtr()->m_selectChannelForm.GetListBox(IDC_LOBBY_LISTBOX);
	lbox->AddItem(txt, newSlot.get());

	// If first item is added, to make it selected automatically.
	// 최초로 추가되는 항목이면 자동으로 선택되게 합니다.
	if (lbox->GetSize() == 1)
		lbox->SelectItem(0);

	return true;
}

DEFRMI_EntryS2C_LobbyList_End(CEntryClient)
{
	ATLTRACE("%s\n", __FUNCTION__);
	// Activate button UI.
	// 버튼 UI를 활성화합니다.
	CWorld::GetSharedPtr()->m_selectChannelForm.GetButton(IDC_OK)->SetEnabled(true);

	return true;
}

DEFRMI_EntryS2C_ShowError(CEntryClient)
{
	CWorld::GetSharedPtr()->GotoErrorForm(errTxt.GetString());
	return true;
}

DEFRMI_EntryS2C_NotifyReturnToEntryFailed(CEntryClient)
{
	CStringW txt;
	txt.Format(L"%s : %d", CWorld::GetSharedPtr()->LoadString(IDS_STRING125), reason);
	CWorld::GetSharedPtr()->GotoErrorForm(txt);
	return true;
}

/************************************* Error Processing *************************************/
/************************************* 오류 처리 *************************************/
void CEntryClient::OnError(ErrorInfo *errorInfo)
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	GotoErrorForm(txt.GetString());
}

void CEntryClient::OnException(const Proud::Exception &e)
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()).GetString(), __FUNCTIONW__);
	GotoErrorForm(txt.GetString());
}
