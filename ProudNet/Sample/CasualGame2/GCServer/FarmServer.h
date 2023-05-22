#pragma once

#include "farmcommon.h"
#include "ServerBase.h"
#include "../GCCommon/Farm_proxy.h"
#include "../GCCommon/Farm_stub.h"

/**
\~korean
Farm 서버에 접속한 각 Farm 클라이언트에 대한 정보를 가진 객체입니다.

\~english
Object that owned information about each farm client who connected farm server

\~chinese
具有连接Farm服务器的各Farm客户端信息的对象。

\~japanese
Farmサーバに接続した各Farmクライアントに関する情報を持つオブジェクトです。

\~
*/
class CRemoteFarmClient
{
public:

	/**
	\~korean
	Farm 클라이언트의 HostID

	\~english
	HostID of farm client

	\~chinese
	Farm客户端的HostID

	\~japanese
	FarmクライアントのHostID

	\~
	*/
	HostID m_hostID;

	/**
	\~korean
	이 Farm 클라이언트가 로그온이 성공했는가

	\~english
	Is this farm client succeed to logon?

	\~chinese
	该Farm客户端的登录是否成功？

	\~japanese
	このFarmクライアントがログオンに成功したか

	\~
	*/
	bool m_authed;

	/**
	\~korean
	이 Farm 클라이언트의 이름

	\~english
	Name of this farm client

	\~chinese
	该Farm客户端的名称

	\~japanese
	このFarmクライアントの名前

	\~
	*/
	String m_farmClientName;
	
	CRemoteFarmClient();
};

typedef RefCount<CRemoteFarmClient> CRemoteFarmClientPtr;

/**
\~korean
Farm 서버 메인 객체입니다.

\~english
Main object of farm server

\~chinese
Farm服务器主对象。

\~japanese
Farmサーバのメインオブジェクトです。

\~
*/
class CFarmServer: public CServerBase, public INetServerEvent, public IDbCacheServerDelegate2, public FarmC2S::Stub
{
	//CAutoPtr<CNetServer> m_netServer;
	CAutoPtr<CNetServer> m_lanServer;

	CAutoPtr<CDbCacheServer2> m_dbServer;

	CCachedTableName m_gamerTable;

	CriticalSection m_cs;

	//typedef Dictionary<HostID, CRemoteFarmClientPtr> RemoteFarmClients;
	typedef CFastMap<HostID,CRemoteFarmClientPtr> RemoteFarmClients;

	/**
	\~korean
	이 Farm 서버에 접속한 Farm 클라이언트의 리스트

	\~english
	Farm client list who connected this farm server

	\~chinese
	连接该Farm服务器的Farm客户端列表

	\~japanese
	このFarmサーバに接続したFarmクライアントのリスト

	\~
	*/
	RemoteFarmClients m_remoteClients;
	FarmS2C::Proxy m_s2cProxy;

	/**
	\~korean
	이 Farm 서버의 이름

	\~english
	Name of this farm server

	\~chinese
	该Farm服务器的名称

	\~japanese
	このFarmサーバの名前
	\~
	*/
	String m_farmName;

	/**
	\~korean
	모든 farm client를 묶은 p2p group

	\~english
	P2P group that binded all farm client

	\~chinese
	绑定所有farm client的p2p组

	\~japanese
	すべてのfarm clientを結んだp2p group

	\~
	*/
	HostID m_p2pGroupID;

public:
	CFarmServer(void);
	~CFarmServer(void);

	/**
	\~korean
	 Farm 서버의 listening을 시작합니다.

	\~english
	 Start listening of farm server.

	\~chinese
	开始监听Farm服务器。

	\~japanese
	Farmサーバのlisteningを開始します。

	\~
	*/
	void Start();
	String GetDescription();

	/**
	\~korean
	 HostID 값을 근거로 Farm 클라이언트 객체를 찾습니다.

	\~english
	 Find out farm client object that based on HostID value.

	\~chinese
	根据HostID值寻找Farm客户端对象。

	\~japanese
	HostIDの値に基づいてFarmクライアントのオブジェクトを検索します。

	\~
	*/
	CRemoteFarmClientPtr GetClientByHostID(HostID hostID);

	virtual void OnError(ErrorInfo *errorInfo) override;
	virtual void OnWarning(ErrorInfo *errorInfo) override;
	virtual void OnInformation(ErrorInfo *errorInfo) override;
	virtual void OnException(const Proud::Exception &e) override;
	virtual void OnNoRmiProcessed(RmiID rmiID) override {}

	virtual void OnClientJoin(CNetClientInfo *clientInfo) override;
	virtual void OnClientLeave( CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment ) override;
	virtual void OnP2PGroupJoinMemberAckComplete(HostID groupHostID,HostID memberHostID,ErrorType result) override {}
	virtual void OnUserWorkerThreadBegin() override {}
	virtual void OnUserWorkerThreadEnd() override {}

	
	DECRMI_FarmC2S_RequestFarmLogon;
};
