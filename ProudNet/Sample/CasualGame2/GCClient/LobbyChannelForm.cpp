#include "StdAfx.h"
#include "LobbyChannelForm.h"
#include "world.h"
#include "clientbase.h"
#include "entryclient.h"
#include "lobbyclient.h"
#include "resource.h"

/// Compose lobby channel form.
/// 로비 채널 폼을 구성합니다.
void InitLobbyChannelForm()
{
	CWorld::GetSharedPtr()->m_lobbyChannelForm.SetCallback( LobbyChannelForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_lobbyChannelForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_lobbyChannelForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_lobbyChannelForm.SetLocation(10, 10);

	CStringW titleString;
	titleString.LoadStringW(IDS_STRING130);
	CWorld::GetSharedPtr()->m_lobbyChannelForm.AddStatic(IDC_SELECT_HERO_STATIC, titleString, 0, 0, 200, 20);

	CWorld::GetSharedPtr()->m_lobbyChannelForm.AddListBox(IDC_ROOM_LISTBOX, 0, 50, 300, 200);
	CWorld::GetSharedPtr()->m_lobbyChannelForm.AddListBox(IDC_HERO_LISTBOX, 320, 50, 130, 200);

	CStringW creatRoomString;
	creatRoomString.LoadStringW(IDS_STRING128);
	CWorld::GetSharedPtr()->m_lobbyChannelForm.AddButton(IDC_NEW_ROOM_BUTTON, creatRoomString, 0, 260, 80, 35);
	
	CStringW joinString;
	joinString.LoadStringW(IDS_STRING131);
	CWorld::GetSharedPtr()->m_lobbyChannelForm.AddButton(IDC_JOIN_ROOM_BUTTON, joinString, 100, 260, 80, 35);

	CStringW backString;
	backString.LoadStringW(IDS_STRING106);
	CWorld::GetSharedPtr()->m_lobbyChannelForm.AddButton(IDC_CANCEL, backString, 250, 260, 80, 35);

	CWorld::GetSharedPtr()->m_lobbyChannelForm.AddListBox(IDC_LOG, 0, 300, 400, 130);
	CWorld::GetSharedPtr()->m_lobbyChannelForm.AddIMEEditBox(IDC_CHAT_IN, L"", 0, 430, 400, 30);
}

void ResetLobbyChannelForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_lobbyChannelForm.SetLocation(10, 10);

	CWorld::GetSharedPtr()->m_lobbyChannelForm.SetSize( Width, Height );
}

/// Event handler at server connection form
/// 서버 연결 폼에서의 이벤트 Handler
void CALLBACK LobbyChannelForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_OK:
		break;
	case IDC_CANCEL:
		{
			// Disconnect with lobby server and start to connect to ENTRY SERVER because clicked "Return" button.
			// 뒤로 가기 버튼을 눌렀으므로 로비 서버와의 연결을 중단하고 ENTRY SERVER로의 연결을 시작합니다.
			CDisconnectArgs args;
			args.m_comment.Add(DisConnectServerChange);
			args.m_gracefulDisconnectTimeoutMs = 1000;

			CWorld::GetSharedPtr()->m_client->m_NetClient->Disconnect(args);
			CWorld::GetSharedPtr()->m_client.Free();
			Sleep(100);
			CWorld::GetSharedPtr()->m_client.Attach(new CEntryClient);
			CWorld::GetSharedPtr()->m_isComebackFromLobby = true;
			CWorld::GetSharedPtr()->GetEntryClient()->Connect();
			break;

		}
	case IDC_CHAT_IN:
		switch ( nEvent )
		{
		case EVENT_EDITBOX_STRING:
		{
			// Send chatting string to server and empty chatting entry window.
			// 채팅 문자열을 서버에 보내고 채팅 입력창을 비웁니.
			CStringW chatTxt = ((CDXUTEditBox*)pControl)->GetText();
			((CDXUTEditBox*)pControl)->SetText(L"");

			CWorld::GetSharedPtr()->GetLobbyClient()->m_c2sProxy.Chat(HostID_Server, RmiContext::ReliableSend,
			        chatTxt);
			break;
		}
		}
		break;
	case IDC_NEW_ROOM_BUTTON:
		// Open room creation form.
		// 방 만들기 폼을 엽니다.
		CWorld::GetSharedPtr()->GotoNewRoomForm();
		break;
	case IDC_JOIN_ROOM_BUTTON:
		// Starts joinning room.
		// 방 들어가기를 시작합니다.
	{
		DXUTListBoxItem* sel = CWorld::GetSharedPtr()->m_lobbyChannelForm.GetListBox(IDC_ROOM_LISTBOX)->GetSelectedItem();
		if (sel)
		{
			CLobbyGameRoom* room = (CLobbyGameRoom*)sel->pData;
			CJoinGameRoomParameter p1;

			// TODO: Please fills up additional data to enter room.
			// TODO: 방 진입에 필요한 추가 데이터는 여기 채워넣어라.
			p1.m_guid = room->m_info.m_guid;

			CWorld::GetSharedPtr()->GetLobbyClient()->m_c2sProxy.RequestJoinGameRoom(HostID_Server, RmiContext::ReliableSend, p1);
			pControl->SetEnabled(false);
		}
	}
	break;
	}
}

