#include "StdAfx.h"
#include "resource.h"

#include "LobbyClient.h"
#include "battleclient.h"
#include "mainwindow.h"
#include "world.h"
#include "../GCCommon/Lobby_common.cpp"
#include "../GCCommon/Lobby_proxy.cpp"
#include "../GCCommon/Lobby_stub.cpp"

CLobbyClient::CLobbyClient()
{
	m_NetClientName = L"LobbyClient";
	// 클라 객체 생성
	m_NetClient.Attach(CNetClient::Create());
	m_NetClient->SetEventSink(this);

	// Initialize RMI
	// RMI 초기화
	m_NetClient->AttachStub(this);
	m_NetClient->AttachProxy(&m_c2sProxy);

}

CLobbyClient::~CLobbyClient(void)
{
	// Close client network module. It does disconnecting with server.
	// 클라이언트 네트워크 모듈을 종료합니다. 즉 서버와의 연결을 끊는 역할도 합니다.
	m_NetClient.Free();

	// Removes from other client form that refer this object.
	// 이 객체를 참조하는 타 클라 폼에서도 제거합니다.
	CWorld::GetSharedPtr()->m_lobbyChannelForm.GetListBox(IDC_HERO_LISTBOX)->RemoveAllItems();
}

/// Handler notice result whether it has succeed to connect with server ot not
/// 서버와의 연결 시도 후 연결이 성공했는지 실패했는지 결과가 나오는 Handler입니다.
void CLobbyClient::OnJoinServerComplete( ErrorInfo *info, const ByteArray &replyFromServer )
{
	if (info->m_errorType != ErrorType_Ok)
	{
		// If it failed to connect with server, indicate error then go to close mode.
		// 서버와의 연결이 실패했으면 에러를 표시한 후 종료 모드로 돌입.
		CStringW txt;
		txt.Format(L"%s\n%s", CWorld::GetSharedPtr()->LoadString(IDS_STRING124), info->ToString());

		CWorld::GetSharedPtr()->GotoErrorForm(txt);
	}
	else
	{
		// Once it succeed to connect with Lobby server then starts logon.
		// Lobby 서버와의 연결이 성공했으면 로그온을 시작합니다.
		RmiContext rmi = RmiContext::ReliableSend;
		rmi.m_encryptMode = EM_Secure;
		m_c2sProxy.RequestNextLogon(HostID_Server, rmi, CWorld::GetSharedPtr()->m_credential);
	}
}

/// When it closed connection with server
/// 서버와의 접속이 종료되었을 경우 콜백 됩니다.
void CLobbyClient::OnLeaveServer( ErrorInfo *errorInfo )
{
	
}

/// Try to connect with server.
/// 서버와의 연결을 시도합니다.
bool CLobbyClient::Connect()
{
	CNetConnectionParam param;

	param.m_protocolVersion = CSettings::GetSharedPtr()->GetLobbyVersion();
	param.m_serverIP = CWorld::GetSharedPtr()->m_lobbyServerAddr.m_addr;
	param.m_serverPort = CWorld::GetSharedPtr()->m_lobbyServerAddr.m_port;

	return m_NetClient->Connect(param);
}

/// Processing each frame
/// 매 프레임마다의 처리
void CLobbyClient::FrameMove( float fElapsedTime )
{
	// Call CNetClient.FrameMove.
	// CNetClient.FrameMove를 호출.
	m_NetClient->FrameMove();
}

/// Find out opposite game room object by entering guid of game room.
/// 게임방  guid를 입력받아 대응하는 게임방 객체를 찾습니다.
CLobbyGameRoomPtr CLobbyClient::GetLobbyGameRoomByGuid( Guid roomGuid )
{
	CLobbyGameRoomPtr output;
	if (m_gameRooms.TryGetValue(roomGuid, output))
		return output;
	else
		return CLobbyGameRoomPtr();
}


/// Find out opposited player character information item (hero slot) by entering hero guid.
/// hero guid를 입력받아 대응하는 플레이어 캐릭터 정보 항목(hero slot)를 찾습니다.
CLobbyHeroSlotPtr CLobbyClient::GetHeroSlotByGuid( Guid heroGuid )
{
	CLobbyHeroSlotPtr output;
	if (m_heroSlots.TryGetValue(heroGuid, output))
		return output;
	else
		return CLobbyHeroSlotPtr();
}

/// Creates game room string which indicate on screen.
/// 화면에 표시할 게임방 문자열을 만듭니다.
CStringW CLobbyClient::GetGameRoomText( const CGameRoomParameter &info )
{
	CStringW markTxt;
	if (info.m_mode == RoomMode_Waiting)
		markTxt = L"";
	else
		markTxt = L"(Playing)";

	CStringW roomTxt;
	roomTxt.Format(L"%s[%d/%d]:%s:%s", markTxt, info.m_gamerCount, info.m_maxGamerCount, info.m_masterHeroName, info.m_name);

	return roomTxt;
}
DEFRMI_LobbyS2C_NotifyNextLogonFailed(CLobbyClient)
{
	// Indicate error then goes to close mode.
	// "Failed to authentication at lobby server.\n%s"
	// 에러를 표시한 후 종료 모드로 돌입.
	// "로비 서버에서의 인증이 실패했습니Farm.\n%s"
	CStringW loadString;
	loadString.LoadStringW(IDS_STRING116);
	
	CStringW txt;
	txt.Format(loadString, ErrorInfo::TypeToString(reason));

	CWorld::GetSharedPtr()->GotoErrorForm(txt);

	return true;
}

DEFRMI_LobbyS2C_NotifyNextLogonSuccess(CLobbyClient)
{
	// Save received credential.
	// 받은 credential을 저장해둡니다.
	CWorld::GetSharedPtr()->m_gamerguid = Gamerguid;

	// If there are any possibility of changing selected character with cracking then it goes through error. Therefore use official method which is renewal it.
	// 만약 해킹등으로 인하여 중간에 선택된 케릭터가 바뛰는 경우가 존재할 수 있습니다면 바로 오류로 이어지기 때문에 정보를 다시 갱신해 주는 정식 방법을 사용합니다.
	CWorld::GetSharedPtr()->m_localHeroInfo = selectedHeroInfo;
	
	// Move to waiting room form because it succeed logon.
	// 로그온이 성공했으므로 채널 대기실로 이동합니다.
	CWorld::GetSharedPtr()->GotoLobbyChannelForm(true);

	return true;
}

DEFRMI_LobbyS2C_NotifyUnauthedAccess(CLobbyClient)
{	
	CStringW logString;
	logString.LoadStringW(IDS_STRING102);
	CWorld::GetSharedPtr()->GotoErrorForm(logString);

	return true;
}

DEFRMI_LobbyS2C_ShowError(CLobbyClient)
{
	CWorld::GetSharedPtr()->GotoErrorForm(errTxt.GetString());
	return true;
}

DEFRMI_LobbyS2C_ShowChat(CLobbyClient)
{
	// Spread chatting string.
	// 채팅 문자열을 뿌립니다.
	CDXUTListBox* ctl = CWorld::GetSharedPtr()->m_lobbyChannelForm.GetListBox(IDC_LOG);

	CLobbyHeroSlot* heroSlot = GetHeroSlotByGuid(heroGuid);
	if (heroSlot)
	{
		CStringW txt;
		txt.Format(L"[%s] %s", heroSlot->m_info.m_name, chatText);
		ctl->AddItem(txt, NULL);
		ctl->SelectItem(ctl->GetSize() - 1);
	}

	return true;
}

DEFRMI_LobbyS2C_HeroSlot_Appear(CLobbyClient) //([in] CHeroPublishInfo info);
{
	// 대기자 목록에 추가합니다.
	CLobbyHeroSlotPtr hero(new CLobbyHeroSlot);
	hero->m_info = info;

	CStringW heroTxt;
	heroTxt.Format(L"%s [%I64d]", info.m_name, info.m_score);

	m_heroSlots.Add(info.m_guid, hero);
	CWorld::GetSharedPtr()->m_lobbyChannelForm.GetListBox(IDC_HERO_LISTBOX)->AddItem(heroTxt, hero.get());

	return true;

}

DEFRMI_LobbyS2C_HeroSlot_Disappear(CLobbyClient)
{
	// 대기자 목록에서 제거합니다.
	CLobbyHeroSlotPtr hero = GetHeroSlotByGuid(heroGuid);
	if (!hero)
		return true;

	CDXUTListBox* lbox = CWorld::GetSharedPtr()->m_lobbyChannelForm.GetListBox(IDC_HERO_LISTBOX);
	for (int i = 0;i < lbox->GetSize();i++)
	{
		if (lbox->GetItem(i)->pData == hero.get())
		{
			lbox->RemoveItem(i);
			break;
		}
	}

	m_heroSlots.Remove(heroGuid);

	return true;
}

DEFRMI_LobbyS2C_LocalHeroSlot_Appear(CLobbyClient)
{
	// Receiving state of my character and save it.
	CWorld::GetSharedPtr()->m_localHeroInfo = info;

	return true;
}

DEFRMI_LobbyS2C_GameRoom_Appear(CLobbyClient)
{
	// Adds to game room list.
	// 게임방 목록에 추가합니다.

	CLobbyGameRoomPtr room(new CLobbyGameRoom);
	room->m_info = info;

	m_gameRooms.Add(info.m_guid, room);
	CWorld::GetSharedPtr()->m_lobbyChannelForm.GetListBox(IDC_ROOM_LISTBOX)->AddItem(GetGameRoomText(info), room.get());

	return true;
}

DEFRMI_LobbyS2C_GameRoom_Disappear(CLobbyClient)
{
	// Remove from game room list.
	// 게임방 목록에서 제거합니다.
	CLobbyGameRoomPtr room = GetLobbyGameRoomByGuid(roomGuid);
	if (room)
	{
		CDXUTListBox* lbox = CWorld::GetSharedPtr()->m_lobbyChannelForm.GetListBox(IDC_ROOM_LISTBOX);
		for (int i = 0;i < lbox->GetSize();i++)
		{
			CLobbyGameRoom* r = (CLobbyGameRoom*)lbox->GetItem(i)->pData;
			if (r == room)
			{
				lbox->RemoveItem(i);
				break;
			}
		}
		m_gameRooms.Remove(roomGuid);
	}
	return true;
}

DEFRMI_LobbyS2C_GameRoom_ShowState(CLobbyClient)
{
	// Renew game room list
	// 게임방 목록 갱신
	CLobbyGameRoomPtr room = GetLobbyGameRoomByGuid(info.m_guid);
	if (room)
	{
		CDXUTListBox* lbox = CWorld::GetSharedPtr()->m_lobbyChannelForm.GetListBox(IDC_ROOM_LISTBOX);
		for (int i = 0;i < lbox->GetSize();i++)
		{
			CLobbyGameRoom* r = (CLobbyGameRoom*)lbox->GetItem(i)->pData;
			if (r == room)
			{
				wcscpy(lbox->GetItem(i)->strText, GetGameRoomText(info));
				break;
			}
		}
	}
	return true;
}

DEFRMI_LobbyS2C_NotifyCreateRoomSuccess(CLobbyClient)
{
	// Disconnect with lobby server, starts connecting to battle server which have created game room.
	// Once it finished connection, logon process then it changes to game waiting room form.
	// 로비 서버와의 연결을 끊고, 생성된 게임방이 있는 배틀 서버로의 접속을 시작합니다.
	// 접속,로그온 과정이 완료되면 게임방 대기실 폼으로 전환될 것입니다.

	ATLTRACE("%s\n",__FUNCTION__);
	CWorld::GetSharedPtr()->m_roomToJoin = info;
	ASSERT(info.m_guid != Guid());
	ASSERT(info.m_serverAddr.m_addr.GetLength() > 0);
	ASSERT(info.m_serverAddr.m_port > 0);

	::PostMessage(DXUTGetHWND(), WM_LAUNCH_BATTLE_CLIENT, 0, 0);

	return true;
}

DEFRMI_LobbyS2C_NotifyCreateRoomFailed(CLobbyClient)
{
	// Failed to room creation request. Give another change to attempt it again.
	// 방 생성 요청이 실패했습니다. 다시 시도할 기회를 .
	CWorld::GetSharedPtr()->m_newRoomForm.GetButton(IDC_OK)->SetEnabled(true);

	return true;
}

DEFRMI_LobbyS2C_NotifyJoinRoomFailed(CLobbyClient)
{
	// Failed to room entry request. Give another change to attempt it again.
	// 방 진입 요청이 실패했습니다. 다시 시도할 기회를 .
	CWorld::GetSharedPtr()->m_lobbyChannelForm.GetButton(IDC_JOIN_ROOM_BUTTON)->SetEnabled(true);

	return true;
}

DEFRMI_LobbyS2C_NotifyJoinRoomSuccess(CLobbyClient)
{
	// Disconnect with lobby server and starts to connect with battle server which has created game room.
	// Once it finished connection, logon process then it changes to game waiting room form.
	// 로비 서버와의 연결을 끊고, 생성된 게임방이 있는 배틀 서버로의 접속을 시작합니다.
	// 접속,로그온 과정이 완료되면 게임방 대기실 폼으로 전환될 것입니다.

	CWorld::GetSharedPtr()->m_roomToJoin = info;
	ASSERT(info.m_guid != Guid());
	ASSERT(info.m_serverAddr.m_addr.GetLength() > 0);
	ASSERT(info.m_serverAddr.m_port > 0);

	::PostMessage(DXUTGetHWND(), WM_LAUNCH_BATTLE_CLIENT, 0, 0);

	return true;
}




/************************************* Error Processing *************************************/
/************************************* 오류 처리 *************************************/
void CLobbyClient::OnError(ErrorInfo *errorInfo) 
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	GotoErrorForm(txt.GetString());
}

void CLobbyClient::OnException(const Proud::Exception &e) 
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()), __FUNCTIONW__);
	GotoErrorForm(txt.GetString());
}

void CLobbyClient::ErrorTest()
{
	ErrorInfoPtr errinfo = ErrorInfoPtr();
	OnInformation(errinfo);
	OnWarning(errinfo);
	OnError(errinfo);
}