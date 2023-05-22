#pragma once

#include "ServerBase.h"
#include "farmclient.h"
#include "../GCCommon/vars.h"
#include "../GCCommon/Lobby_proxy.h"
#include "../GCCommon/Lobby_stub.h"


/**
\~korean
로비 서버에 접속한 각 로비 클라이언트 객체입니다.

\~english
Each lobby client object that connected at lobby server

\~chinese
连接lobby服务器的各lobby客户端对象。

\~japanese
ロビーサーバに接続した各ロビークライアントのオブジェクトです。

\~
*/
class CRemoteLobbyClient_S
{
public:
	
	/**
	\~korean
	로비 클라이언트의 HostID

	\~english
	HostID of lobby client

	\~chinese
	Lobby客户端的HostID。

	\~japanese
	ロビークライアントのHostID

	\~
	*/
	HostID m_hostID;

	/**
	\~korean
	이 로비 클라이언트의 로딩된 게이머 정보
	- 로그온 성공시에만 값이 채워져 있습니다.

	\~english
	Loaded gamer information at this lobby client
	- This value only fill up when it succeed to logon.

	\~chinese
	该lobby客户端上加载的玩家信息
	-只有在成功登录时值才被填充。

	\~japanese
	このロビークライアントにロードされたゲーマーの情報
	- ログオンが成功した時のみ値が埋まっています。

	\~
	*/
	CLoadedData2Ptr m_dbLoadedGamerData;

	ByteArray m_customfield;

	/**
	\~korean
	타 로비 클라이언트에게도 공개될, 이 클라이언트의 게이머 정보

	\~english
	Gamer information of this client that announce to other lobby client

	\~chinese
	向其他lobby客户端公开的该客户端玩家信息

	\~japanese
	他のロビークライアントにも公開される、このクライアントのゲーマーの情報

	\~
	*/
	CHeroPublishInfo m_info;
	
	/**
	\~korean
	방 생성 요청을 가장 마지막에 한 시간

	\~english
	Lastest time of room creation request

	\~chinese
	最有一次请求创建房间的时间

	\~japanese
	部屋作りの要請を最後に行った時間

	\~
	*/
	int64_t m_lastRequestTimeOfNewGameRoomMs;

	CRemoteLobbyClient_S();

	/**
	\~korean
	이 게이머의 guid를 얻습니다.

	\~english
	Gets guid of this gamer.

	\~chinese
	获取该玩家的guid。

	\~japanese
	このゲーマーのguidを取得します。

	\~
	*/
	Guid GetGamerGuid();

	/**
	\~korean
	로딩한 게이머라면 이 메서드는 마지막에 사용한 플레이어 캐릭터의 정보를 얻습니다.

	\~english
	Gives used player character information at last time, if it is loaded gamer.

	\~chinese
	如为已加载的玩家，则该method获取最后一次使用的玩家人物信息。

	\~japanese
	ロードしたゲーマーであれば、このメソッドは最後に使ったプレイヤーキャラクターの情報を取得します。

	\~
	*/
	Guid GetSelectedHeroGuid();

	/**
	\~korean
	로딩한 게이머라면 이 메서드는 마지막에 사용한 플레이어 캐릭터의 정보를 얻습니다.

	\~english
	Gives used player character information at last time, if it is loaded gamer.

	\~chinese
	如为已加载的玩家，则该method获取最后一次使用的玩家人物信息。

	\~japanese
	ロードしたゲーマーであれば、このメソッドは最後に使ったプレイヤーキャラクターの情報を取得します。

	\~
	*/
	String GetSelectedHeroName();
};

typedef RefCount<CRemoteLobbyClient_S> CRemoteLobbyClientPtr;
typedef CFastMap<HostID,CRemoteLobbyClientPtr> RemoteLobbyClients;


/**
\~korean
1개의 게임방 정보입니다.
- 배틀 서버로부터 동기화됩니다.

\~english
1 game room information
- Synchronize from battle server.

\~chinese
1个游戏房间信息。
-被作战服务器同步。

\~japanese
1つのゲームルーム情報です。
- バトルサーバから同期化されます。

\~
*/
class CLobbyGameRoom_S
{
public:

	/**
	\~korean
	로비 클라이언트에게도 공개될 수 있는 게임방 정보

	\~english
	Game room information that can be announced to lobby client

	\~chinese
	可以向lobby客户端公开的游戏房间信息

	\~japanese
	ロビークライアントにも公開できるゲームルームの情報

	\~
	*/
	CGameRoomParameter m_info;
};

typedef RefCount<CLobbyGameRoom_S> CLobbyGameRoomPtr_S;

//typedef Dictionary<Guid, CLobbyGameRoomPtr_S> LobbyGameRooms_S;
typedef CFastMap<Guid, CLobbyGameRoomPtr_S> LobbyGameRooms_S;

/**
\~korean
로비 서버 메인 객체입니다.

\~english
Lobby server main object

\~chinese
Lobby服务器的主对象。

\~japanese
ロビーサーバのメインオブジェクトです。

\~
*/
class CLobbyServer: public CServerBase, public INetServerEvent, public IFarmClientDelegate, IDbCacheClientDelegate2, public LobbyC2S::Stub
{

	/**
	\~korean
	네트워크 엔진 모듈

	\~english
	Network engine module

	\~chinese
	网络引擎模块

	\~japanese
	ネットワークエンジンのモジュール
	\~
	*/
	CAutoPtr<CNetServer> m_netServer;

	/**
	\~korean
	로비 서버도 Farm 클라이언트다.

	\~english
	Lobby server is farm client.

	\~chinese
	Lobby服务器也是 Farm客户端。

	\~japanese
	ロビーサーバもFarmクライアントである。

	\~
	*/
	CAutoPtr<CFarmClient> m_farmClient;

	/**
	\~korean
	 ProudDB cache client 객체

	\~english
	 ProudDB cache client object

	\~chinese
	ProudDB cache client 对象

	\~japanese
	ProudDB cache clientオブジェクト

	\~
	*/
	CAutoPtr<CDbCacheClient2> m_dbClient;

	/**
	\~korean
	이 로비 서버에 접속한 클라이언트 리스트

	\~english
	Client list which connected at this lobby server

	\~chinese
	连接该lobby服务器的客户端列表

	\~japanese
	ロビーサーバに接続したクライアントのリスト

	\~
	*/
	RemoteLobbyClients m_remoteClients;

	/**
	\~korean
	스레드 보호

	\~english
	Protect thread

	\~chinese
	保护线程

	\~japanese
	スレッドの保護

	\~
	*/
	CriticalSection m_cs;
	
	/**
	\~korean
	서버->클라이언트 RMI

	\~english
	Server->Client RMI

	\~chinese
	服务器->客户端RMI

	\~japanese
	サーバ →クライアントRMI

	\~
	*/
	LobbyS2C::Proxy m_s2cProxy;

	/**
	\~korean
	배틀 서버들로부터 동기화 받은 게임방 목록

	\~english
	Game room list that synchronized from battle servers

	\~chinese
	被作战服务器同步的游戏房间目录

	\~japanese
	バトルサーバから同期化されたゲームルームのリスト

	\~
	*/
	LobbyGameRooms_S m_gameRooms;

	__int64 m_lastGuiUpdateTime;
public:
	
	/**
	\~korean
	 이 로비의 인식 가능한 이름

	\~english
	 Identifiable name of this lobby

	\~chinese
	该lobby可识别的名称

	\~japanese
	このロビーで認識可能な名前

	\~
	*/
	String m_lobbyName;

	/**
	\~korean
	 이 로비 서버로 게임 클라이언트가 접속 가능한 인터넷 주소

	\~english
	 Internet address that game client can connect to this lobby server

	\~chinese
	游戏客户端连接该lobby服务器所需的互联网地址

	\~japanese
	のロビーサーバにゲームクライアントが接続可能なインターネットアドレス

	\~
	*/
	NamedAddrPort m_serverAddr;

	CLobbyServer(void);
	~CLobbyServer(void);

	/**
	\~korean
	listening을 시작합니다

	\~english
	Starts listening

	\~chinese
	开始监听。

	\~japanese
	listeningを開始します

	\~
	*/
	void Start();
	virtual void OnTick(void* context) override;
	String GetDescription() {
		return L"Lobby Server";
	}
	
	CRemoteLobbyClientPtr GetRemoteClientByHostID(HostID clientID);
	CRemoteLobbyClientPtr GetAuthedRemoteClientByHostID(HostID clientID);
	CRemoteLobbyClientPtr GetRemoteClientByGamerGuid(Guid gamerGuid);
	CRemoteLobbyClientPtr GetRemoteClientBySessionGuid(Guid requestSessionGuid);
	CLobbyGameRoomPtr_S GetGameRoomByGuid(Guid roomGuid);

	/**
	\~korean
	가장 가볍게 작동중인 배틀 서버를 찾습니다.
	단, 이 로비 서버와 연관된 배틀 서버만 찾아야 합니다.

	\~english
	Find out battle server which running lightly.
	However, only find out battle server which related with this lobby server.

	\~chinese
	寻找运行强度最小的作战服务器。
	但，只寻找与该lobby服务器有关的作战服务器。

	\~japanese
	最も軽く動いているバトルサーバを検索します。
	ただし、このロビーサーバに関わるバトルサーバのみを検索する必要があります。

	\~
	*/
	CFarmClientInfoPtr GetLightestBattleServer();

	/**
	\~korean
	게임방 guid를 입력받아, 그 게임방을 주관하고 있는 배틀 서버 객체를 얻습니다.

	\~english
	Gets guid of game room and gets battle server object that manage game room.

	\~chinese
	根据输入的游戏房间guid获取该游戏房间主管的作战服务器对象。

	\~japanese
	ゲームルームguidの入力を受け取り、そのゲームルームを主管するバトルサーバのオブジェクトを取得します。

	\~
	*/
	CFarmClientInfoPtr GetBattleServerByGameRoom(Guid gameRoomGuid);

	/**
	\~korean
	미인증 게이머의 통신 오류입니다.

	\~english
	Communication error of unauthenticated gamer

	\~chinese
	未认证玩家的通讯错误。

	\~japanese
	未認証ゲーマーの通信エラーです。

	\~
	*/
	void NotifyUnauthedAccess(HostID clientID);
	void GetAuthedClientList(CFastArray<HostID> &output);


	/**
	\~korean
	INetServer 구현입니다.

	\~english
	Realize INetServer

	\~chinese
	实现INetServer。

	\~japanese
	INetServer実装です。

	\~
	*/
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

	/**
	\~korean
	IFarmClientDelegate 구현입니다.

	\~english
	Realize IFarmClientDelegate 

	\~chinese
	实现IFarmClientDelegate。

	\~japanese
	IFarmClientDelegate実装です。

	\~
	*/
	virtual void OnJoinFarmServerComplete( ErrorInfo* info) override;
	virtual void OnLeaveFarmServer(ErrorType reason) override;
	virtual void OnCreateGameRoomResult(HostID requestedLobbyClientID, CasualResult result, const CGameRoomParameter& createdGameRoomParam) override;
	virtual void OnJoinGameRoomResult(HostID requestedLobbyClientID, CasualResult result, const CGameRoomParameter& gameRoomParam) override;
	virtual CriticalSection* GetCritSec() override { return &m_cs;}
	virtual int GetGamerCount();
	virtual NamedAddrPort GetServerAddr();

	virtual void NotifyCheckCredentialFail(HostID userHostID) override;
	virtual void NotifyCheckCredentialSuccess(const Guid &gamerGuid, HostID userHostID) override;

	
	// ******************************************
	// *****Realize IDbCacheClientDelegate2******
	// *****IDbCacheClientDelegate2 구현*********
	// ******************************************

	/**
	\~korean
	서버 연결 시도 결과를 리턴합니다.

	\~english
	Return server connection attempt result.

	\~chinese
	返回尝试连接服务器的结果。

	\~japanese
	サーバ接続を試みた結果を返します。

	\~
	*/
	virtual void OnJoinDbCacheServerComplete(ErrorInfo *info)override {}

	/**
	\~korean
	DB cache server와의 연결이 해제됐음을 notify합니다.
	- 이것이 호출된 상황에서는, 이미 DB cache client의 모든 데이터가 unload된 상태입니다.

	\~english
	Notify disconnect with DB cache server.
	- When this called, all data of DB cache client has unloaded.

	\~chinese
	提示与DB cache server的连接已解除。
	-调用时DB cache client的所有数据已被加载。

	\~japanese
	DB cache serverとの接続が解除されたことをnotifyします。
	- これが呼び出された状況では、既にDB cache clientのすべてのデータがunloadされた状態です。

	\~
	*/
	virtual void OnLeaveDbCacheServer(ErrorType reason)override {}

	/**
	\~korean
	DB cache server에서 DBMS에 기록하는 과정이 끝날 때마다 도착하는 이벤트입니다.
	\param type 완료된 기록의 종류
	\param loadedDataGuid 완료된 기록을 가리키는 Guid. RootNode의 UUID입니다.

	\~english
	Arriving event when it has done writing process to DBMS at DB cache server
	\param type Type of finished writing
	\param loadedDataGuid Guid that point finished writing. UUID of RootNode.

	\~chinese
	每次DB cache server向DBMS记录过程结束时接收的事件。
	\param type 记录结束的种类
	\param loadedDataGuid 只向完成记录Guid.RootNode的UUID。

	\~japanese
	DB cache serverでDBMSに記録する過程が終わるたびに到着するイベントです。
	\ param type　完了した記録の種類
	\ param loadedDataGuid　完了した記録を指すGuid。 RootNodeのUUIDです。

	\~
	*/
	virtual void OnDbmsWriteDone(DbmsWritePropNodePendType type,Proud::Guid loadedDataGuid)override {}

	virtual void OnExclusiveLoadDataComplete(CCallbackArgs& args)override;
	
	virtual void OnDataUnloadRequested(CCallbackArgs& args)override;

	
	virtual void OnAddDataFailed(CCallbackArgs& args)override{}
	virtual void OnAddDataSuccess(CCallbackArgs& args)override{}


	virtual void OnUpdateDataFailed(CCallbackArgs& args)override; 
	virtual void OnUpdateDataSuccess(CCallbackArgs& args)override;

	virtual void OnRemoveDataFailed(CCallbackArgs& args)override {}
	virtual void OnRemoveDataSuccess(CCallbackArgs& args)override {}

	virtual void OnNonExclusiveSnapshotDataComplete(CCallbackArgs& args)override{}

	virtual void OnNonExclusiveAddDataAck(CCallbackArgs& args)override {}
	virtual void OnNonExclusiveRemoveDataAck(CCallbackArgs& args)override {}

	virtual void OnNonExclusiveModifyValueSuccess(CCallbackArgs& args)override{}
	virtual void OnNonExclusiveModifyValueFailed(CCallbackArgs& args)override{}

	virtual void OnNonExclusiveSetValueIfSuccess(CCallbackArgs& args)override{}
	virtual void OnNonExclusiveSetValueIfFailed(CCallbackArgs& args)override{}

	
	virtual void OnSomeoneAddData(CCallbackArgs& args)override {}
	virtual void OnSomeoneRemoveData(CCallbackArgs& args)override {}
	
	virtual void OnSomeoneModifyValue(CCallbackArgs& args)override {}
	virtual void OnSomeoneSetValue(CCallbackArgs& args)override {}

	virtual void OnIsolateDataFailed(CCallbackArgs& args)override {}
	virtual void OnIsolateDataSuccess(CCallbackArgs& args)override {}
	virtual void OnDeisolateDataFailed(CCallbackArgs& args)override {}
	virtual void OnDeisolateDataSuccess(CCallbackArgs& args)override {}
	virtual void OnDataForceUnloaded(CCallbackArgs& args)override{}

	virtual void OnAccessError(CCallbackArgs& args)override; 

	/**
	\~korean
	클라이언트->서버 RMI 구현입니다.

	\~english
	Realize Client->Server RMI

	\~chinese
	实现客户端->服务器RMI。

	\~japanese
	クライアント →サーバRMIの実装です。

	\~
	*/
	DECRMI_LobbyC2S_RequestNextLogon;
	DECRMI_LobbyC2S_NotifyChannelFormReady;
	DECRMI_LobbyC2S_Chat;
	DECRMI_LobbyC2S_RequestCreateGameRoom;
	DECRMI_LobbyC2S_RequestJoinGameRoom;

	void OnGameRoomAppear(const CGameRoomParameter& info) override;
	void OnGameRoomShowState(const CGameRoomParameter& info) override;
	void OnGameRoomDisappear(Guid roomGuid) override;

public:
	

	INT_PTR	m_diffCustomArg; 
};
