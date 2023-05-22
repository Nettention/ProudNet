#include "StdAfx.h"
#include "ConnectForm.h"
#include "world.h"
#include "client.h"
#include "Resource.h"

//CConnectForm g_ConnectForm;

// 서버 연결 폼에서의 이벤트 핸들러
// Event handler at server connection form
void CALLBACK ConnectForm_OnGuiEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch ( nControlID )
	{
	case IDC_OK:
		{
			g_World->m_serverAddr = g_World->m_connectForm.GetEditBox(IDC_SERVER_ADDR)->GetText();
			g_World->m_userName = g_World->m_connectForm.GetEditBox(IDC_USER_NAME)->GetText();

			// 중복 연결 시도를 방지하기 위해
			// To prevent repeat connection attempt
			g_World->m_connectForm.GetButton(IDC_OK)->SetEnabled(false);

			// 클라 1개를 만든 후 서버에 연결 시도를 시작
			// Create 1 client then start connecting to server
			CClient* newCli = g_World->AddNewClient();

			bool ret = newCli->Connect();
			if (!ret)
			{
				CStringW txt;
				txt.LoadString(IDS_STRING110);
				g_World->GoToErrorForm(txt);
			}
			else
			{
				g_World->m_connectForm.GetButton(IDC_OK)->SetText(L"connecting...");
			}
			//		g_World->m_LocalHero.Attach(new CLocalHero);
		}
		break;
	case IDC_CANCEL:
		::PostQuitMessage(0);
		break;

	}
}
