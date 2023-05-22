#pragma once

#include "../gccommon/vars.h"
#include "../gccommon/battle_proxy.h"
#include "../gccommon/battle_stub.h"
#include "ServerBase.h"
#include "farmclient.h"
#include "BattleClient_S.h"
#include "Bullet_S.h"
#include "ITem_S.h"


/**
\~korean
배틀 서버 메인 객체입니다.

\~english
Battle server main object

\~chinese
作战服务器主对象

\~japanese
バトルサーバのメインオブジェクトです。

\~
*/
class CBattleServer: public CServerBase, public INetServerEvent, public IFarmClientDelegate, IDbCacheClientDelegate2, public BattleC2S::Stub, public BattleC2C::Stub
{

public:

	/**
	\~korean
	ProudNet 서버 객체

	\~english
	ProudNet server object

	\~chinese
	ProudNet 服务器对象

	\~japanese
	ProudNetサーバのオブジェクト

	\~
	*/
	CAutoPtr<CNetServer> m_netServer;
	
	/**
	\~korean
	배틀 서버는 Farm 클라이언트입니다.

	\~english
	Battle server is farm client.

	\~chinese
	作战服务器为Farm客户端

	\~japanese
	バトルサーバは、Farmクライアントです。
	\~
	*/
	CAutoPtr<CFarmClient> m_farmClient;
	
	/**
	\~korean
	ProudDB cache server에 접속하는 클라이언트

	\~english
	Client who connecting to ProudDB cache server

	\~chinese
	连接ProudDB cache server的客户端

	\~japanese
	ProudDB cache serverにアクセスするクライアント

	\~
	*/
	CAutoPtr<CDbCacheClient2> m_dbClient;

	/**
	\~korean
	이 배틀 서버에 접속하고 있는 클라 목록

	\~english
	Client list which logged on this battle server

	\~chinese
	连接该作战服务器的客户端目录

	\~japanese
	このバトルサーバに接続しているクライアントのリスト

	\~
	*/
	RemoteBattleClients m_remoteClients;

	/**
	\~korean
	멀티스레드 보호

	\~english
	Protect multi-thread

	\~chinese
	多线程保护

	\~japanese
	マルチスレッドの保護

	\~
	*/
	CriticalSection m_cs;
	
	/**
	\~korean
	서버->클라 RMI

	\~english
	Server->Client RMI

	\~chinese
	服务器->客户端 RMI

	\~japanese
	サーバ →クライアントRMI

	\~
	*/
	BattleS2C::Proxy m_s2cProxy;

	/**
	\~korean
	이 배틀 서버에서 갖고 있는 게임방 목록

	\~english
	Game room list that this battle server owned

	\~chinese
	该作战服务器上的游戏房间目录

	\~japanese
	このバトルサーバが持っているゲームルームのリスト

	\~
	*/
	BattleGameRooms m_gameRooms;

	int64_t m_lastGuiUpdateTimeMs;

	/**
	\~korean
	이 배틀 서버가 종속하고 있는 로비 서버의 Farm 클라이언트 HostID

	\~english
	Farm client HostID of lobby server which this battle server subordinated.

	\~chinese
	该作战服务器所属lobby服务器上的Farm客户端HostID

	\~japanese
	このバトルサーバが属しているロビーサーバのFarmクライアントHostID

	\~
	*/
	HostID m_lobbyServerHostID;
public:
	
	/**
	\~korean
	이 배틀 서버가 종속된 로비의 이름

	\~english
	Name of lobby which this battle server subordinated 

	\~chinese
	该作战服务器所属lobby名称

	\~japanese
	このバトルサーバが属しているロビーの名前

	\~
	*/
	String m_lobbyName;

	
	
	/**
	\~korean
	게임 클라가 들어올 수 있는 이 배틀 서버의 인터넷 주소

	\~english
	Internet address of this battle server that game client can join in.

	\~chinese
	游戏客户端加入所需该作战服务器的互联网地址

	\~japanese
	ゲームクライアントが入って来れるバトルサーバのインターネットアドレス

	\~
	*/
	NamedAddrPort m_serverAddr;

	CBattleServer(void);
	~CBattleServer(void);

	/**
	\~korean
	배틀 서버의 listening을 시작합니다.

	\~english
	Start listening of battle server.

	\~chinese
	开始监听作战服务器。

	\~japanese
	バトルサーバのlisteningを開始します。

	\~
	*/
	void Start() override;

	/**
	\~korean
	일정 시간마다의 프로세싱입니다.

	\~english
	Processing every specific time

	\~chinese
	每隔一定时间的处理。

	\~japanese
	一定時間ごとの処理です。

	\~
	*/
	void OnTick(void* context) override;
	void FrameMove_GameRoom(CGameRoom_S* gameRoom);
	void FrameMove_GameRoom_LoadingMode( CGameRoom_S* gameRoom );

	/**
	\~korean
	next logon을 하려고 했으나 credential이 안 맞을 때 호출됩니다.

	\~english
	It calls when it try to next logon but credential does not match.

	\~chinese
	尝试next logon，但credential不匹配时调用。

	\~japanese
	next logonをしようとしたが、credentialが合わない時に呼び出されます。

	\~
	*/
	virtual void NotifyCheckCredentialFail(HostID userHostID) override;
	virtual void NotifyCheckCredentialSuccess(const Guid &gamerGuid, HostID userHostID) override;


	void GameRoom_GotoPlayMode( CGameRoom_S* gameRoom );
	void GameRoom_GotoWaitingMode( CGameRoomPtr_S gameRoom );

	/**
	\~korean
	 로비 서버에 의해 게임방 들어가기 예약이 되어있던 클라이언트가, 정작 너무 오랫동안 들어오지 않으면 예약 정보에서 제거합니다.

	\~english
	 If client reserved to enter lobby server is not coming for a long time then remove it from reserve information.

	\~chinese
	Lobby服务器上预约进入游戏房间的客户端，如果长时间不进入则会在预约信息中被删除。

	\~japanese
	ロビーサーバによりゲームルームへの入場を予約したクライアントが、余りにも長い間入ってこないと予約情報から削除されます。

	\~
	*/
	void RemoveTooOldPendedJoinees();
	void PurgeLoadIncompleteGamers(CGameRoom_S* gameRoom);
	String GetDescription() { return L"Battle Server";}

	CBattleClientPtr_S GetRemoteClientByHostID(HostID clientID);

	/**
	\~korean
	클라이언트의 HostID를 입력받아 클라이언트 객체를 얻되 로그온한 객체에 한해서만 얻습니다.

	\~english
	Get client object with receiving HostID of client but get only for logged on object.

	\~chinese
	根据输入的客户端HostID获得客户端对象，但只限于已登录对象。

	\~japanese
	クライアントのHostID入力を受け取り、クライアントのオブジェクトを得ることができるが、ログオンしたオブジェクトに限って取得します。

	\~
	*/
	CBattleClientPtr_S GetAuthedRemoteClientByHostID(HostID clientID);

	/**
	\~korean
	배틀 클라이언트가 서버에 로그온 요청을 하는 동안 발급받았던 로딩 결과 대기 번호표를 입력해서, 클라이언트 객체를 찾습니다.

	\~english
	Find out client object with loading result waiting ticket that issued during battle client requst logon to server.

	\~chinese
	作战客户端想服务器请求登录时输入已获得的加载结果等待号码寻找客户端对象。

	\~japanese
	バトルクライアントがサーバにログオン要求をする間、引き渡されたロード結果の待ち受け番号を入力し、クライアントのオブジェクトを検索します。

	\~
	*/
	CBattleClientPtr_S GetRemoteClientBySessionGuid( Guid requestGuid );
	CBattleClientPtr_S GetRemoteClientByDbSessionGuid( Guid dbSessionGuid );
	

	void NotifyUnauthedAccess(HostID clientID);
	void GetAuthedClientList(CFastArray<HostID> &output);

	CGameRoomPtr_S GetGameRoomByGuid(Guid roomGuid);

	/**
	\~korean
	게임방 진입을 요청하는 게임 클라이언트의 진입 허가를 체크합니다.

	\~english
	Check enter permission of game client that request entering game room.

	\~chinese
	检查请求进入游戏房间的游戏客户端进入许可。

	\~japanese
	ゲームルームへの入場を要求するゲームクライアントの入場許可をチェックします。

	\~
	*/
	bool IsGameRoomJoinAllowed(CGameRoomPtr_S room, const CJoinGameRoomParameter &joinParam);

	virtual void BeforeRmiInvocation(LPCWSTR RMIName);
	
	/**
	\~korean
	INetServer 구현입니다.

	\~english
	Realize INetServer

	\~chinese
	实现INetServer。

	\~japanese
	INetServerの実装です。

	\~
	*/
	virtual void OnError(ErrorInfo *errorInfo) override;
	virtual void OnWarning(ErrorInfo *errorInfo) override;
	virtual void OnInformation(ErrorInfo *errorInfo) override;
	virtual void OnException(const Proud::Exception &e) override;
	virtual void OnNoRmiProcessed(RmiID rmiID) override {}

	/**
	\~korean
	배틀 클라이언트가 서버에 접속하는 순간 콜백됩니다.

	\~english
	When battle client connet to server

	\~chinese
	作战客户端连接服务器后回调。

	\~japanese
	バトルクライアントがサーバに接続した瞬間、コールバックされます。

	\~
	*/
	virtual void OnClientJoin(CNetClientInfo *clientInfo) override;

	/**
	\~korean
	배틀 클라이언트가 서버와 접속을 끊는 순간 콜백됩니다.

	\~english
	When battle client disconnect from server

	\~chinese
	作战客户端断开服务器连接时回调。

	\~japanese
	バトルクライアントがサーバとの接続を切断した瞬間、コールバックされます。

	\~
	*/
	virtual void OnClientLeave( CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment ) override;
	virtual void OnP2PGroupJoinMemberAckComplete(HostID groupHostID,HostID memberHostID,ErrorType result) override {}
	virtual void OnUserWorkerThreadBegin() override {}
	virtual void OnUserWorkerThreadEnd() override {}

	/**
	\~korean
	IFarmClientDelegate 구현입니ㅏㄷ.

	\~english
	Realize IFarmClientDelegate

	\~chinese
	实现IFarmClientDelegate。

	\~japanese
	IFarmClientDelegateの実装です。

	\~
	*/
	virtual void OnJoinFarmServerComplete( ErrorInfo* info) override;

	/**
	\~korean
	Farm 서버와의 연결이 끊어졌으므로 이 게이머 대상 서버 프로세스도 종료합니다.

	\~english
	Close server process targets to this gamer because disconnected with farm servr.

	\~chinese
	因Farm服务器的连接已断开，因此该玩家的服务器进程也终止。

	\~japanese
	Farmサーバとの接続が切断されたため、このゲーマーの対象サーバのプロセスも終了します。

	\~
	*/
	virtual void OnLeaveFarmServer(ErrorType reason) override;

	/**
	\~korean
	로비 서버 요청에 의해 게임방을 생성하는 과정

	\~english
	Game room creation process by request from lobby server

	\~chinese
	根据lobby服务器请求创建游戏房间的过程

	\~japanese
	ロビーサーバの要求によってゲームルームを生成する過程
	s
	\~
	*/
	CasualResult CreateGameRoomByLobbyServer(CGameRoomParameter& inoutParameter) override;
	CasualResult JoinGameRoomByLobbyServer(const CJoinGameRoomParameter& joinParam, CGameRoomParameter& outJoinAllowedGameRoomInfo) override;

	/**
	\~korean
	타 게임 서버의 개괄 상황을 동기화받는 handler입니다.

	\~english
	Handler that synchronize summarize state of other game server

	\~chinese
	同步其他游戏服务器整体状态的handler

	\~japanese
	他ゲームサーバの総括状況を同期化させるhandlerです。

	\~
	*/
	virtual void OnNotifyFarmClientInfo(const CFarmClientInfo& info) override;
	virtual int GetGamerCount() override;
	virtual NamedAddrPort GetServerAddr() override;
	virtual CriticalSection* GetCritSec() override { return &m_cs; }

	/**
	\~korean
	이 Farm 클라이언트(즉 게임 서버)가 연관된 로비 이름을 리턴합니다.

	\~english
	Returns lobby name that related with this farm client (game server)

	\~chinese
	返回与该Farm客户端(即游戏服务器)有关的lobby名称。

	\~japanese
	このFarmクライアント（つまりゲームサーバ）に関わるロビーの名前を返します。

	\~
	*/
	virtual String GetRelatedLobbyName() override; 


	// ******************************************
	// *** Realize IDbCacheClientDelegate2 ******
	// *****IDbCacheClientDelegate2 구현*********
	// ******************************************

	/**
	\~korean
	서버 연결 시도 결과를 리턴합니다.

	\~english
	Returns result of server connecting attempt

	\~chinese
	返回服务器连接尝试结果。

	\~japanese
	返回服务器连接尝试结果。

	\~
	*/
	virtual void OnJoinDbCacheServerComplete(ErrorInfo *info)override{}


	/**
	\~korean
	 DB cache server와의 연결이 해제됐음을 notify합니다.
	- 이것이 호출된 상황에서는, 이미 DB cache client의 모든 데이터가 unload된 상태입니다.

	\~english
	 Notify that disconnected with DB cache server.
	- When it called, all data has unloaded of DB cache client.

	\~chinese
	提示与DB cache server的连接已断开。
	-调用时DB cache client的所有数据已加载。

	\~japanese
	DB cache serverとの接続が解除されたことをnotifyします。
	- これが呼び出された状況では、既にDB cache clientのすべてのデータがunloadされた状態です。

	\~
	*/
	virtual void OnLeaveDbCacheServer(ErrorType reason)override{}

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
	virtual void OnDbmsWriteDone(DbmsWritePropNodePendType type,Proud::Guid loadedDataGuid)override{}

	virtual void OnExclusiveLoadDataComplete(CCallbackArgs& args)override;

	virtual void OnDataUnloadRequested(CCallbackArgs& args)override; 


	virtual void OnAddDataFailed(CCallbackArgs& args)override{}
	
	virtual void OnAddDataSuccess(CCallbackArgs& args)override{}


	/**
	\~korean
	request형일때 오는 콜백(여기서는  UnilateralUpdateData()했기 때문에 실행되진 않습니다...)
	requestUpdateData()를 할시에는 여기로 콜백이 옵니다.

	\~english
	Coming callback when it request type (At this point, it does UnilateralUpdateData() so it cannot execute...)
	When you do requestUpdateData() then callback comes to here.

	\~chinese
	Request类型时的回调(这里为UnilateralUpdateData()，因此不执行...)
	当requestUpdateData()，则回调到这里。

	\~japanese
	request形の時のコールバック（ここではUnilateralUpdateData()を行ったため、実行しません...）
	requestUpdateData()を行った時はここにコールバックします。

	\~
	*/
	virtual void OnUpdateDataFailed(CCallbackArgs& args)override;

	/**
	\~korean
	request형일때 오는 콜백(여기서는  UnilateralUpdateData()했기 때문에 실행되진 않습니다...)
	requestUpdateData()를 할시에는 여기로 콜백이 옵니다.

	\~english
	Coming callback when it request type (At this point, it does UnilateralUpdateData() so it cannot execute...)
	When you do requestUpdateData() then callback comes to here.

	\~chinese
	Request类型时的回调(这里为UnilateralUpdateData()，因此不执行...)
	当requestUpdateData()，则回调到这里。
	
	\~japanese
	request形の時のコールバック（ここではUnilateralUpdateData()を行ったため、実行しません...）
	requestUpdateData()を行った時はここにコールバックします。

	\~
	*/
	virtual void OnUpdateDataSuccess(CCallbackArgs& args)override;

	virtual void OnRemoveDataFailed(CCallbackArgs& args)override{}

	virtual void OnRemoveDataSuccess(CCallbackArgs& args)override{}

	virtual void OnNonExclusiveSnapshotDataComplete(CCallbackArgs& args)override{}

	virtual void OnNonExclusiveAddDataAck(CCallbackArgs& args)override{}
	virtual void OnNonExclusiveRemoveDataAck(CCallbackArgs& args)override{}

	virtual void OnNonExclusiveModifyValueSuccess(CCallbackArgs& args)override{}
	virtual void OnNonExclusiveModifyValueFailed(CCallbackArgs& args)override{}

	virtual void OnNonExclusiveSetValueIfSuccess(CCallbackArgs& args)override{}
	virtual void OnNonExclusiveSetValueIfFailed(CCallbackArgs& args)override{}

	
	virtual void OnSomeoneAddData(CCallbackArgs& args)override{}	
	virtual void OnSomeoneRemoveData(CCallbackArgs& args)override{}
	
	virtual void OnSomeoneModifyValue(CCallbackArgs& args)override{}
	virtual void OnSomeoneSetValue(CCallbackArgs& args)override{}

	virtual void OnIsolateDataFailed(CCallbackArgs& args)override {}
	virtual void OnIsolateDataSuccess(CCallbackArgs& args)override {}
	virtual void OnDeisolateDataFailed(CCallbackArgs& args)override {}
	virtual void OnDeisolateDataSuccess(CCallbackArgs& args)override{}
	virtual void OnDataForceUnloaded(CCallbackArgs& args)override{}

	virtual void OnAccessError(CCallbackArgs& args)override; 
	
	/**
	\~korean
	클라이어늩->서버 RMI 구현입니다.

	\~english
	Realize Client->Server RMI

	\~chinese
	实现客户端->服务器RMI。

	\~japanese
	クライアント→サーバRMIの実装です。

	\~
	*/
	DECRMI_BattleC2S_RequestNextLogon;
	DECRMI_BattleC2S_RequestToggleBattleReady;
	DECRMI_BattleC2S_RequestStartPlayMode;
	DECRMI_BattleC2S_NotifyLoadFinished;
	DECRMI_BattleC2S_LeaveBattleRoom;

	DECRMI_BattleC2S_RequestLocalHeroSpawn;
	DECRMI_BattleC2S_RequestBulletSpawn;
	DECRMI_BattleC2S_LocalHero_Move;
	DECRMI_BattleC2S_NotifyGotoLobbyServer;

	double m_AccumTimeForCreateBullet;
	

	/**
	\~korean
	heroOwner의 캐릭터를 viewer 클라이언트가 가시하는지 체크합니다.

	\~english
	Check that viewer client is watching character of heroOwner.

	\~chinese
	检查viewer是否在看heroOwner的人物。

	\~japanese
	heroOwnerのキャラクターをviewerクライアントが見れるのかをチェックします。

	\~
	*/
	bool IsHeroVisibleToClient( CBattleClient_S* heroOwner, CBattleClient_S* viewer );

	void FrameMove_GameRoom_PlayingMode( CGameRoom_S* gameRoom );
	INT_PTR m_diffCustomArg;
	
};
