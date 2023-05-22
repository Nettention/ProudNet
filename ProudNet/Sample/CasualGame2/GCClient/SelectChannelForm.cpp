#include "StdAfx.h"
#include "SelectChannelForm.h"
#include "world.h"
#include "entryclient.h"
#include "lobbyclient.h"
#include "resource.h"

/// Compose lobby channel selection form with connecting entry client.
/// entry client에 접속한 상태에서, 로비 채널 선택 폼을 구성합니다.
void InitSelectChannelForm()
{
	CWorld::GetSharedPtr()->m_selectChannelForm.SetCallback( SelectChannelForm_OnGuiEvent );
	CWorld::GetSharedPtr()->m_selectChannelForm.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	CWorld::GetSharedPtr()->m_selectChannelForm.SetFont( 2, L"Courier New", 16, FW_NORMAL );
	CWorld::GetSharedPtr()->m_selectChannelForm.SetLocation(50, 50);

	// "Select joining channel"
	// "참여할 채널을 선택하세요"
	CStringW selectString;
	selectString.LoadStringW(IDS_STRING119);
	CWorld::GetSharedPtr()->m_selectChannelForm.AddStatic(IDC_SELECT_HERO_STATIC, selectString, 0, 0, 200, 20);

	CWorld::GetSharedPtr()->m_selectChannelForm.AddListBox(IDC_LOBBY_LISTBOX, 0, 40, 200, 200);

	// Deactivate select button until receiving lobby channel list has finished at server.
	// 서버에서 로비 채널 리스트를 모두 받기 전까지는 선택 버튼을 비활성화시킵니.
	CWorld::GetSharedPtr()->m_selectChannelForm.AddButton( IDC_OK, L"Ok", 0, 300, 80, 35, VK_RETURN );
	CWorld::GetSharedPtr()->m_selectChannelForm.GetButton( IDC_OK)->SetEnabled(false);

	CWorld::GetSharedPtr()->m_selectChannelForm.AddButton( IDC_CANCEL, L"Back", 100, 300, 80, 35, VK_ESCAPE );
}

void ResetSelectChannelForm(UINT Width, UINT Height)
{
	CWorld::GetSharedPtr()->m_selectChannelForm.SetLocation(50, 50);

	CWorld::GetSharedPtr()->m_selectChannelForm.SetSize( Width, Height );

}

/// Event handler
/// 이벤트 Handler 입니다.
void CALLBACK SelectChannelForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_OK:
		// Quit ENTRY CLIENT and find out selected channel then starts server connection to that channel.
		// ENTRY CLIENT를 종료하고, 선택한 채널을 찾아서 해당 채널로의 서버 연결을 시작합니다.
		if (CWorld::GetSharedPtr()->m_selectChannelForm.GetListBox(IDC_LOBBY_LISTBOX)->GetSelectedItem())
		{
			CEntryLobbySlot* lobbySlot = (CEntryLobbySlot*)CWorld::GetSharedPtr()->m_selectChannelForm.GetListBox(IDC_LOBBY_LISTBOX)->GetSelectedItem()->pData;

			// Do this one first before close old client.
			// 구 클라 종료 전에 이걸 먼저 합니다.
			CWorld::GetSharedPtr()->m_lobbyServerAddr = lobbySlot->m_serverAddr;

			// Disconnection process of old client
			// 구 클라이언트의 연결 종료 처리
			CDisconnectArgs args;
			args.m_comment.Add(DisConnectServerChange);
			args.m_gracefulDisconnectTimeoutMs = 1000;

			CWorld::GetSharedPtr()->m_client->m_NetClient->Disconnect(args);
			CWorld::GetSharedPtr()->m_client.Free();
			Sleep(100);
			// Prepare lobby client then starts connecting process.
			// 로비 클라를 준비 후 접속 과정을 시작합니다.
			CWorld::GetSharedPtr()->m_client.Attach(new CLobbyClient);

			//ATLTRACE("Instance().GetLobbyClient()->Connect();\n");
			bool ret = CWorld::GetSharedPtr()->GetLobbyClient()->Connect();
			if (!ret)
			{
				// "Wrong server address."
				// "잘못된 서버 주소 입니다."
				CStringW logString;
				logString.LoadStringW(IDS_STRING107);
				CWorld::GetSharedPtr()->GotoErrorForm(logString);
			}
			else
			{
				// "Entering"
				// "진입중"
				CStringW logString;
				logString.LoadStringW(IDS_STRING120);
				CWorld::GetSharedPtr()->m_connectForm.GetButton(IDC_OK)->SetText(logString);
			}

			// Deactivate button to prevent repeat clicking.
			// 중복해서 누르지 못하게 버튼을 비활성화합니다.
			CWorld::GetSharedPtr()->m_selectChannelForm.GetButton(IDC_OK)->SetEnabled(false);
		}
		break;
	case IDC_CANCEL:
		// Moves to player character selection form because clicked "Return" button.
		// 뒤로 가기 버튼을 눌렀으므로 플레이어 캐릭터 선택 폼으로 귀환합니다.
		CWorld::GetSharedPtr()->m_selectChannelForm.GetButton(IDC_OK)->SetEnabled(false);
		CWorld::GetSharedPtr()->GotoSelectHeroForm();
		break;
	}
}

