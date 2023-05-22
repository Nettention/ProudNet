#include "stdafx.h"
#include "Client.h"
#include "world.h"
#include "localhero.h"
#include "ErrorForm.h"
#include "resource.h"

#include "../Common/SynchWorldC2C_common.cpp"
#include "../Common/SynchWorldC2S_common.cpp"
#include "../Common/SynchWorldS2C_common.cpp"

#include "../Common/SynchWorldC2S_proxy.cpp"
#include "../Common/SynchWorlds2c_stub.cpp"
#include "../Common/SynchWorldC2c_proxy.cpp"
#include "../Common/SynchWorldc2c_stub.cpp"

CClient::CClient(int ID)
{
	// 클라 객체 생성
	// Create client object
	m_netClient = CNetClient::Create();

	m_netClient->OnJoinServerComplete = [this](ErrorInfo* e, const ByteArray& b) { OnJoinServerComplete(e, b); };
	m_netClient->OnLeaveServer = [this](ErrorInfo *errorInfo) { OnLeaveServer(errorInfo); };
	m_netClient->OnP2PMemberOffline = [this](CRemoteOfflineEventArgs &args) { OnP2PMemberOffline(args); };
	m_netClient->OnP2PMemberOnline = [this](CRemoteOnlineEventArgs &args) { OnP2PMemberOnline(args); };
	m_netClient->OnError = [this](ErrorInfo *errorInfo) { OnError(errorInfo); };
	m_netClient->OnException = [this](const Exception &e) { OnException(e); };


	// RMI 초기화
	// Initialize RMI
	m_netClient->AttachStub((SynchWorldS2C::Stub*)this);
	m_netClient->AttachStub((SynchWorldC2C::Stub*)this);
	m_netClient->AttachProxy(&m_C2SProxy);
	m_netClient->AttachProxy(&m_C2CProxy);

	m_markDisposed = false;
	m_ID = ID;
}

CClient::~CClient(void)
{
	// 클라이언트 네트웍 모듈을 종료한다. 즉 서버와의 연결을 끊는 역할도 한다.
	// Close client network module. It does disconnect with server.
	delete m_netClient;
}

// 서버와의 연결 시도 후 연결이 성공했는지 실패했는지 결과가 나오는 핸들러
// Handler that notice result about success or fail of connecting after connecting attempt with server
void CClient::OnJoinServerComplete( ErrorInfo *info, const ByteArray &replyFromServer )
{
	CStringW loadString;
	loadString.LoadString(IDS_STRING109);
	CStringW txt;
	txt.Format(L"%s\n%s",  loadString, info->ToString());

	if (IsPrimaryClient())
	{
		// 최초로 접속 시도를 한 클라인 경우, 즉 프라이머리 클라이언트인 경우
		// If it is first connecting attempted client which mean it is primary client.
		if (info->m_errorType != ErrorType_Ok)
		{
			g_World->GoToErrorForm(txt);
		}
		else
		{
			// 로그온 상자를 닫고 메인 폼을 활성화한다.
			// Close logon box then activate main form.
			g_World->GoToMainForm();
		}
	}
	else
	{
		// 추가로 증설한 클라이언트의 연결 실패 상황의 경우
		// Situation that connection failed of additional created client
		if (info->m_errorType != ErrorType_Ok)
		{
			g_World->Log(txt);
			m_markDisposed = true;
		}
		else
		{
			// 보조 클라이언트 %d의 서버 연결이 성공
			// Succeed server connecting of sub client %d
			CStringW loadString;
			loadString.LoadStringW(IDS_STRING102);
			CStringW connectsuccessString;
			//txt.Format(L"보조 클라이언트 %d의 서버 연결이 성공", m_netClient->GetLocalHostID());
			//txt.Format(L"Succeed server connecting of sub client %d", m_netClient->GetLocalHostID());
			connectsuccessString.Format(loadString, m_netClient->GetLocalHostID());
			g_World->Log(connectsuccessString);
		}
	}

	// 서버와의 연결이 성공했으면 로컬 캐릭터의 등장을 서버에 신고한다.
	// Once it succeed to connect with server then report appearing of local character to server.
	if (info->m_errorType == ErrorType_Ok)
	{
		m_localHero.Attach(new CLocalHero(this));
		m_localHero->m_name = GetHeroName();
		m_C2SProxy.RequestLocalHeroSpawn(HostID_Server, RmiContext::ReliableSend,
		                                 m_localHero->m_name,
		                                 m_localHero->m_position, m_localHero->m_direction);
	}
}

// 서버와의 접속이 종료된 경우
// When connecting with server is closed
void CClient::OnLeaveServer( ErrorInfo *errorInfo )
{
	if (IsPrimaryClient())
	{
		// 프라이머리 클라가 나가면 프로그램 종료
		// Once primary client exit then close program
		PostQuitMessage(0);
	}
	else
	{
		// 이 보조 클라는 파괴되어야 함을 표기한다.
		// Mark this sub client has to destroy.
		m_markDisposed = true;
	}
}

void CClient::OnP2PMemberOnline(CRemoteOnlineEventArgs &args)
{
	g_World->Log(StringW::NewFormat(L"Online peer %d", args.m_remoteHostID).GetString());
}

void CClient::OnP2PMemberOffline(CRemoteOfflineEventArgs &args)
{
	g_World->Log(StringW::NewFormat(L"Offline peer %d", args.m_remoteHostID).GetString());
}


// 최초로 만들어진 클라이언트 객체인가?
// Is it very first created client object ?
bool CClient::IsPrimaryClient()
{
	return m_ID == 1;
}

// 서버와의 연결을 시도한다.
// Try to connect with server.
bool CClient::Connect()
{
	CNetConnectionParam param;

	param.m_protocolVersion = g_Version;
	param.m_serverPort = g_ServerPort;
	param.m_serverIP = g_World->m_serverAddr;

	return m_netClient->Connect(param);

}

// 이 클라이언트가 다루는 캐릭터의 이름을 얻는다.
// Get name of character which handling by this client.
CStringW CClient::GetHeroName()
{
	CStringW ret;
	ret.Format(L"%s-%d", g_World->m_userName, m_ID);

	return ret;
}

// 매 프레임마다의 처리
// Processing every frames
void CClient::FrameMove( float fElapsedTime )
{
	// CNetClient.FrameMove를 호출.
	// Call CNetClient.FrameMove.
	m_netClient->FrameMove();

	// 보유한 원격 캐릭에 대한 처리도 한다.
	// Do processing about owned remote character.
	for (Proud::Position i = m_remoteHeroes.GetStartPosition();i;)
	{
		CRemoteHeroPtr_C remoteHero = m_remoteHeroes.GetNextValue(i);
		remoteHero->FrameMove(fElapsedTime);
	}
}

// RMI 핸들러. 자세한 것은 PIDL 파일을 참고할 것.
// RMI handler. Please refer PIDL file for more detail.
DEFRMI_SynchWorldS2C_NotifyLocalHeroViewersGroupID(CClient)
{
	assert(m_localHero);
	m_localHero->m_viewersGroupID = localHeroViewersGroupID;

	return true;
}

// RMI 핸들러. 자세한 것은 PIDL 파일을 참고할 것.
// RMI handler. Please refer PIDL file for more detail.
DEFRMI_SynchWorldC2C_P2P_LocalHero_Move(CClient)
{
	CRemoteHeroPtr_C remoteHero = GetRemoteHeroByClientID(remote);
	if (remoteHero)
	{
		// 캐릭터의 위치를 갱신한다.
		// Renew position of character.
		remoteHero->m_positionFollower.SetTargetPosition(position);
		remoteHero->m_positionFollower.SetTargetVelocity(velocity);
		remoteHero->m_directionFollower.SetTargetAngle(direction);

		// 캐릭의 위치를 direct P2P로 받았는지 여부를 마킹한다.
		// Mark whether receiving character postion by direct P2P or not.
		bool oldValue = remoteHero->m_synchedThruDirectP2P;
		remoteHero->m_synchedThruDirectP2P = rmiContext.m_relayed ? false : true;
		if (oldValue != remoteHero->m_synchedThruDirectP2P)
		{
			remoteHero->m_synchedThruDirectP2PChangeShowCoolTime = CRemoteHero_C::SynchedThruDirectP2PChangeShowInterval;

			if (IsUnderUserControl())
			{
				// 원격 캐릭터 %s가 직접 P2P로 정보를 보내기 시작
				// Remote character %s started to send information with P2P itself
				CStringW loadString;
				loadString.LoadStringW(IDS_STRING103);
				CStringW logString;
				logString.Format(loadString, remoteHero->m_name);
				g_World->Log(logString);
			}
		}

	}

	return true;
}

// RMI 핸들러. 자세한 것은 PIDL 파일을 참고할 것.
// RMI handler. Please refer PIDL file for more detail.
DEFRMI_SynchWorldS2C_RemoteHero_Appear(CClient)
{
	// 자기 캐릭터에 대해서 날아온 것은 무시한다.
	// Ignore information about own character.
	if (remote != m_netClient->GetLocalHostID())
	{
		// 타 캐릭터에 대한 인스턴스를 찾거나 만든다.
		// Find or create instance about other character.
		CRemoteHeroPtr_C remoteHero = GetRemoteHeroByClientID(heroOwnerID);
		if (!remoteHero)
		{
			remoteHero = CRemoteHeroPtr_C(new CRemoteHero_C);
			remoteHero->m_ownerID = heroOwnerID;
			remoteHero->m_name = heroName.GetString();
			m_remoteHeroes.Add(remoteHero->m_ownerID, remoteHero);
		}
		// 초기 캐릭터 값을 세팅한다.
		// Set default character value.
		remoteHero->m_positionFollower.SetTargetPosition(position);
		remoteHero->m_positionFollower.SetTargetVelocity(velocity);
		remoteHero->m_directionFollower.SetTargetAngle(direction);

		// 이것도 설정해야 한다. 왜냐하면 바로 갓 등장한 것이기 때문이다.
		// Set this because it is just appeared.
		remoteHero->m_positionFollower.EnableAutoFollowDuration(true);

		remoteHero->m_positionFollower.SetFollowerPosition(position);
		remoteHero->m_positionFollower.SetFollowerVelocity(velocity);

		// 각도에 대한 값도 세팅한다.
		// Set value about angle.
		remoteHero->m_directionFollower.SetFollowerAngleVelocity(D3DX_PI / 180*60);
		remoteHero->m_directionFollower.SetFollowerAngle(direction);
		remoteHero->m_directionFollower.SetTargetAngle(direction);

		// 이벤트를 화면에 표시한다.
		// Indicate event on screen.
		if (IsUnderUserControl())
		{
			// 가시 영역에 원격 캐릭터 %s가 등장
			// Appearing remote character %s in visible area
			// Remote client %s appear in visual field
			CStringW loadString;
			loadString.LoadStringW(IDS_STRING106);
			CStringW logString;
			logString.Format(loadString, heroName);
			g_World->Log(logString);
		}
	}
	return true;
}

// RMI 핸들러. 자세한 것은 PIDL 파일을 참고할 것.
// RMI handler. Please refer PIDL file for more detail.
DEFRMI_SynchWorldS2C_RemoteHero_Disappear(CClient)
{
	// 대응하는 타 클라의 캐릭터를 제거한다.
	// Remove character of other opposited client.
	m_remoteHeroes.RemoveKey(heroOwnerID);

	if (IsUnderUserControl())
	{
		// 가시 영역에서 원격 캐릭터가 사라짐
		// Disappearing remote character from visible area
		// Remote client disappear in visual field
		CStringW logString;
		logString.LoadStringW(IDS_STRING107);
		g_World->Log(logString);
	}
	return true;
}

// 타 클라의 HostID를 갖고 타 클라의 캐릭터 객체를 찾는다.
// Find out character object of other client with HostID of other client.
CRemoteHeroPtr_C CClient::GetRemoteHeroByClientID( HostID remoteClientID )
{
	CRemoteHeroPtr_C ret;
	if (!m_remoteHeroes.TryGetValue(remoteClientID, ret))
		return CRemoteHeroPtr_C();
	else
		return ret;
}

// 현재 이 클라가 직접 유저가 키보드로 조종하고 있는 클라인가?
// Is this client controlling by user with keyboard at the moment?
bool CClient::IsUnderUserControl()
{
	return g_World->GetClientUnderUserControl() == this;
}

/************************************* 오류 출력 *************************************/
/************************************* Print Error *************************************/
void CClient::OnError(ErrorInfo *errorInfo) 
{
	String txt;
	txt.Format(L"%s occured at %s", errorInfo->ToString().GetString(), __FUNCTIONW__);
	GotoErrorForm(txt.GetString());
}

void CClient::OnException(const Proud::Exception &e)
{
	String txt;
	txt.Format(L"%s occured at %s", StringA2W(e.what()).GetString(), __FUNCTIONW__);
	GotoErrorForm(txt.GetString());
}

void CClient::GotoErrorForm( LPCWSTR text )
{
	g_World->m_errorForm.GetStatic(IDC_STATIC)->SetText(text);
	g_World->m_formState = CWorld::ErrorForm;
}