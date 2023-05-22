#pragma once

//////////////////////////////////////////////////////////////////////////
// C++에서 C#으로 콜백을 할 때, C++ event delegate class를 C++로 래핑한 클래스들.
// C# code가 이 클래스의 함수들을 호출한다.


#include "NativeType.h"

class CEventWrap
{
public:
	CEventWrap();
	virtual ~CEventWrap();

	void* m_csharpHandle;
};

class CNetClientEventWrap : public Proud::INetClientEvent, public CEventWrap
{

public:
	CNetClientEventWrap();
	virtual ~CNetClientEventWrap();

	//////////////////////////////////////////////////////////////////////////
	// C 함수 포인터 변수
	CallbackJoinServerComplete m_callBackJoinServerComplete;
	CallbackLeaveServer m_callbackLeaveServer;
	CallbackP2PMemberJoin m_callbackP2PMemberJoin;
	CallbackP2PMemberLeave m_callbackP2PMemberLeave;
	CallbackChangeP2PRelayState m_callbackChangeP2PRelayState;
	CallbackChangeServerUdpState m_callbackChangeServerUdpState;
	CallbackSynchronizeServerTime m_callbackSynchronizeServerTime;

	CallbackError m_callbackError;
	CallbackWarning m_callbackWarning;
	CallbackInformation m_callbackInformation;
	CallbackException m_callbackException;

	CallbackServerOffline m_callbackServerOffline;
	CallbackServerOnline m_callbackServerOnline;
	CallbackP2PMemberOffline m_callbackP2PMemberOffline;
	CallbackP2PMemberOnline m_callbackP2PMemberOnline;

	CallbackNoRmiProcessed m_callbackNoRmiProcessed;
	CallbackReceiveUserMessage m_callbackReceiveUserMessage;

	//////////////////////////////////////////////////////////////////////////
	// INetClientEvent implementations
	virtual void OnJoinServerComplete(Proud::ErrorInfo *info, const Proud::ByteArray &replyFromServer);
	virtual void OnLeaveServer(Proud::ErrorInfo *errorinfo);
	virtual void OnP2PMemberJoin(Proud::HostID memberHostID, Proud::HostID groupHostID, int memberCount, const Proud::ByteArray &message);
	virtual void OnP2PMemberLeave(Proud::HostID memberHostID, Proud::HostID groupHostID, int memberCount);
	virtual void OnChangeP2PRelayState(Proud::HostID remoteHostID, Proud::ErrorType reason);
	virtual void OnChangeServerUdpState(Proud::ErrorType reason);
	virtual void OnSynchronizeServerTime();

	virtual void OnServerOffline(Proud::CRemoteOfflineEventArgs &args);
	virtual void OnServerOnline(Proud::CRemoteOnlineEventArgs &args);
	virtual void OnP2PMemberOffline(Proud::CRemoteOfflineEventArgs &args);
	virtual void OnP2PMemberOnline(Proud::CRemoteOnlineEventArgs &args);

	//////////////////////////////////////////////////////////////////////////
	// INetCoreEvent implementations

	virtual void OnError(Proud::ErrorInfo *errorInfo);
	virtual void OnWarning(Proud::ErrorInfo *errorInfo);
	virtual void OnInformation(Proud::ErrorInfo *errorInfo);
	virtual void OnException(const Proud::Exception &e);

	virtual void OnNoRmiProcessed(Proud::RmiID rmiID);
	virtual void OnReceiveUserMessage(Proud::HostID sender, const Proud::RmiContext &rmiContext, uint8_t* payload, int payloadLength);
};

//////////////////////////////////////////////////////////////////////////



class CRmiStubWrap : public Proud::IRmiStub, public CEventWrap
{
public:
	CRmiStubWrap();
	virtual ~CRmiStubWrap();

	CallbackGetRmiIDList m_callbackGetRmiIDList;
	CallbackGetRmiIDListCount m_callbackGetRmiIDListCount;
	CallbackProcessReceivedMessage m_callbackProcessReceivedMessage;

	//////////////////////////////////////////////////////////////////////////

	virtual Proud::RmiID* GetRmiIDList();
	virtual int GetRmiIDListCount();
	virtual bool ProcessReceivedMessage(Proud::CReceivedMessage& pa, void* hostTag);
};

//////////////////////////////////////////////////////////////////////////

class CRmiProxyWrap : public Proud::IRmiProxy, public CEventWrap
{
public:
	CRmiProxyWrap();
	virtual ~CRmiProxyWrap();

	CallbackGetRmiIDList m_callbackGetRmiIDList;
	CallbackGetRmiIDListCount m_callbackGetRmiIDListCount;

	//////////////////////////////////////////////////////////////////////////

	virtual Proud::RmiID* GetRmiIDList();
	virtual int GetRmiIDListCount();
	//virtual void NotifySendByProxy(const Proud::HostID* remotes, int remoteCount, const Proud::MessageSummary &summary, Proud::RmiContext &rmiContext, const Proud::CMessage &msg);
};