#include "stdafx.h"
#include "StatusServer.h"
#include "GCServerDlg.h"

CStatusServer::CStatusServer()
{

}

CStatusServer::~CStatusServer()
{
	m_farmClient.Free();
}

void CStatusServer::Start()
{
	SetTitle(GetDescription().GetString());

	m_farmClient.Attach(new CFarmClient);

	CFarmClientConnectParameter p1;
	p1.m_delegate = this;
	p1.m_farmName = (LPCWSTR)CFarmSettings::GetSharedPtr()->m_FarmName;
	p1.m_farmClientName = m_statusName;
	p1.m_farmClientType = ServerMode_Status;
	m_farmClient->Init(p1, this, GetDescription());
	m_farmClient->Connect();
}

Proud::String CStatusServer::GetDescription()
{
	return L"Status Server";
}

CGamerInfoPtr_S CStatusServer::GetClientByGamerUUID(Guid gamerUUID)
{
	CriticalSectionLock lock(m_cs, true);
	CGamerInfoPtr_S output;
	if ( !m_gamerGuidToGamerMap.TryGetValue(gamerUUID, output) )
		return CGamerInfoPtr_S();
	else
		return output;
}


CGamerInfoPtr_S CStatusServer::GetClientByCredentialUUID(Guid credentialUUID)
{
	CriticalSectionLock lock(m_cs, true);
	CGamerInfoPtr_S output;
	if ( !m_credentialToGamerMap.TryGetValue(credentialUUID, output) )
		return CGamerInfoPtr_S();
	else
		return output;
}


void CStatusServer::OnJoinFarmServerComplete(ErrorInfo* info)
{
	if ( info->m_errorType == ErrorType_Ok )
	{
		// Succeed to connect with server.
		// 서버와의 연결이 성공했습니다.
		CriticalSectionLock lock(m_ownerDlg->m_cs, true);
		CStringW logString;
		logString.LoadStringW(IDS_STRING102);
		SetStatusText(logString);
	}
	else
	{
		// Close precess because it failed to connect with farm server.
		// Farm 서버와의 연결이 실패했으므로 프로세스를 종료합니다.
		String txt;
		txt.Format(L"%s : %s", LoadString(IDS_STRING115), info->ToString().GetString());
		LogError(txt.GetString());
		m_disposed = true;

	}
}

void CStatusServer::OnLeaveFarmServer(ErrorType reason)
{
	// It disconnected with farm server so close process that targeting this gamer.
	// Farm 서버와의 연결이 끊어졌으므로 이 게이머 대상 서버 프로세스도 종료합니다.
	m_disposed = true;
}

int CStatusServer::GetGamerCount()
{
	CriticalSectionLock lock(m_cs, true);
	return m_gamerGuidToGamerMap.Count;

}

Proud::NamedAddrPort CStatusServer::GetServerAddr()
{
	return Proud::NamedAddrPort();
}


void CStatusServer::OnTick(void* context)
{
	FrameMove();
}

void CStatusServer::FrameMove()
{
	if ( m_farmClient != NULL )
		m_farmClient->FarmClientFrameMove();

	// Renew indicated detail at server GUI with ervery specific time.
	// (Note: Actually server cannot have GUI so do not realize it. Please use for only reference)
	// 일정 시간마다 서버 GUI에 표시 내용을 갱신합니다.
	// (주의: 실제 만드는 서버는 GUI를 가질 수 없기에 이런 식으로 구현하면 곤란합니다. 그냥 참고 바람.)

	if ( GetPreciseCurrentTimeMs() - m_lastGuiUpdateTime > 1000 )
	{
		m_lastGuiUpdateTime = GetPreciseCurrentTimeMs();

		int userCount = m_gamerGuidToGamerMap.Count;

		String txt;
		txt.Format(L"%s : %d", LoadString(IDS_STRING118), userCount);
		SetStatusText(txt.GetString());


		CriticalSectionLock lock(m_cs, true);

		// Once it logged out then WaitLogOutTime later, delete credential key.
		// 로그아웃 후 WaitLogOutTime 시간이 지나면 해당 credential 키를 지웁니다.
		for ( GuidToGamerInfoMap::iterator iter = m_gamerGuidToGamerMap.begin(); iter!=m_gamerGuidToGamerMap.end(); )
		{
			if ( iter->second->m_isLoggedOn == false && GetPreciseCurrentTimeMs() > iter->second->m_clientLogOutTime+WaitLogOutTime )
			{
				m_credentialToGamerMap.Remove(iter->second->m_credential);
				iter = m_gamerGuidToGamerMap.erase(iter);
			}
			else
			{
				++iter;
			}
		}

	}
}


bool CStatusServer::RequestCreateCredential(const HostID remote, const Guid &gamerUUID, const HostID &UserHostID)
{
	CriticalSectionLock lock(m_cs, true);
	CGamerInfoPtr_S client;
	if ( m_gamerGuidToGamerMap.TryGetValue(gamerUUID, client) == false )
	{
		client=CGamerInfoPtr_S(new CGamerInfo_S);
		client->m_credential = Win32Guid::NewGuid();
	}

	client->m_isLoggedOn = true;
	client->m_serverHostID = remote;
	client->m_gamerUUID = gamerUUID;


	m_gamerGuidToGamerMap.Add(client->m_gamerUUID, client);
	m_credentialToGamerMap.Add(client->m_credential, client);

	m_farmClient->m_c2cProxy.NotifyCreatedCredential(remote, RmiContext::ReliableSend, client->m_credential, UserHostID);

	return true;
}

bool CStatusServer::RequestCheckCredential(Proud::HostID remote, const Proud::Guid &credential, const HostID &userHostID, const String &ServerName)
{
	CriticalSectionLock lock(m_cs, true);

	CGamerInfoPtr_S rc;
	ATLTRACE("%s\n", __FUNCTION__);
	if ( m_credentialToGamerMap.TryGetValue(credential, rc) == false )
	{
		m_farmClient->m_c2cProxy.NotifyCheckCredentialFail(remote, RmiContext::ReliableSend, userHostID);
		return true;
	}


	if ( rc->m_isLoggedOn == false )
	{
		rc->m_serverHostID=remote;
		rc->m_serverName=ServerName;

		rc->m_isLoggedOn = true;

		m_farmClient->m_c2cProxy.NotifyCheckCredentialSuccess(remote, RmiContext::ReliableSend,
			rc->m_gamerUUID, userHostID);
	}
	else
	{
		m_farmClient->m_c2cProxy.NotifyCheckCredentialFail(remote, RmiContext::ReliableSend, userHostID);
	}

	return true;
}

bool CStatusServer::UserLogOut(Proud::HostID remote, const Guid &gamerUUID)
{
	CriticalSectionLock lock(m_cs, true);

	CGamerInfoPtr_S rc;
	if ( m_gamerGuidToGamerMap.TryGetValue(gamerUUID, rc) == false )
	{
		return true;
	}

	rc->m_isLoggedOn = false;
	rc->m_clientLogOutTime = GetPreciseCurrentTimeMs();

	return true;
}

