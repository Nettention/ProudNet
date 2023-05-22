#pragma once

using namespace Proud;

class CRemoteClient_S
{
public:
	HostID m_hostID;
	std::wstring m_userName;

	CRemoteClient_S(void);
	~CRemoteClient_S(void);
};

typedef RefCount<CRemoteClient_S> CRemoteClientPtr_S;
