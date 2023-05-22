#pragma once

#include "../gccommon/entry_proxy.h"
#include "../gccommon/entry_stub.h"
#include "../gccommon/vars.h"
#include "ServerBase.h"
#include "farmclient.h"
#include "../../../include/AdoWrap.h"


/**
\~korean
주의
Dbms에 필드들이 제대로 생겼는지 확인
필드가 없을시 Exeception발생
Gamer 테이블에  [UUID], [GamerID],[OwnerUUID],[RootUUID],[SelectedHeroGuid],[Password]
Hero 테이블에  [UUID],[OwnerUUID],[RootUUID],[Name],[Sample_Score],[HeroType]

\~english
Warning
Check that field created at Dbms correctly
Occur Exception, if ther are no field
At Gamer table  [UUID], [GamerID],[OwnerUUID],[RootUUID],[SelectedHeroGuid],[Password]
At Hero table  [UUID],[OwnerUUID],[RootUUID],[Name],[Sample_Score],[HeroType]

\~chinese
注意
确认Dbms中的字段是否正确生成
没有字段时发生Exeception
Gamer 表中  [UUID], [GamerID],[OwnerUUID],[RootUUID],[SelectedHeroGuid],[Password]
Hero 表中  [UUID],[OwnerUUID],[RootUUID],[Name],[Sample_Score],[HeroType]

\~japanese
注意
Dbmsにフィールドが正しく出来たかを確認
フィールドがないとExeception発生
Gamer テーブルに  [UUID], [GamerID],[OwnerUUID],[RootUUID],[SelectedHeroGuid],[Password]
Hero テーブルに  [UUID],[OwnerUUID],[RootUUID],[Name],[Sample_Score],[HeroType]

\~
*/

/**
\~korean
entry 서버에 접속한 클라이언트에 대응하는 객체입니다.

\~english
Object which opposite connected client at entry server

\~chinese
应对连接entry服务器的客户端的对象。

\~japanese
entryサーバに接続したクライアントに対応するオブジェクトです。

\~
*/
class CRemoteEntryClient
{
public:
	
	/**
	\~korean
	클라이언트의 HostID

	\~english
	HostID of client

	\~chinese
	客户端的HostID

	\~japanese
	クライアントのHostID

	\~
	*/
	HostID m_HostID;
	
	/**
	\~korean
	Gamer의 ID

	\~english
	ID of Gamer

	\~chinese
	玩家ID

	\~japanese
	GamerのID

	\~
	*/
	String m_gamerID;

	Guid m_gamerUUID;

	CLoadedData2Ptr m_dbLoadedGamerData;

	bool m_newData;
	
	/**
	\~korean
	Hero가 유효한지 판단합니다.

	\~english
	Make decision whether Hero is available or not.

	\~chinese
	判断Hero是否有效。

	\~japanese
	Heroが有効であるかを判断します。

	\~
	*/
	bool IsValidHeroGuid(Guid heroGuid);
};

typedef RefCount<CRemoteEntryClient> CRemoteEntryClientPtr;
typedef CFastMap<HostID,CRemoteEntryClientPtr> RemoteEntryClients;

/**
\~korean
entry client에 보내질 로비 채널의 정보

\~english
Information of lobby channel sending to entry client

\~chinese
向entry client发送的lobby channel信息

\~japanese
entry clientに送られるロビーチャンネルの情報

\~
*/
class CPublishedLobbyInfo
{
public:
	
	/**
	\~korean
	채널 이름

	\~english
	Channel name

	\~chinese
	Channel名称

	\~japanese
	チャンネル名

	\~
	*/
	String m_name;

	/**
	\~korean
	로비 서버 주소

	\~english
	Lobby server address

	\~chinese
	Lobby服务器地址

	\~japanese
	ロビーサーバのアドレス

	\~
	*/
	NamedAddrPort m_serverAddr;
	
	/**
	\~korean
	접속한 게이머 수

	\~english
	Number of connected gamer

	\~chinese
	连接的玩家数量

	\~japanese
	接続しているゲーマーの数

	\~
	*/
	int m_gamerCount;

	CPublishedLobbyInfo()
	{
		m_gamerCount = 0;
	}
};

typedef RefCount<CPublishedLobbyInfo> CPublishedLobbyInfoPtr;
typedef CFastMap<String, CPublishedLobbyInfoPtr,StringTraits> PublishedLobbyInfoList;

/**
\~korean
entry 서버 메인 클래스입니다.

\~english
entry server main class

\~chinese
Entry服务器的主类。

\~japanese
entryサーバのメインクラスです。

\~
*/
class CEntryServer: public CServerBase, public INetServerEvent, public IFarmClientDelegate, IDbCacheClientDelegate2, public EntryC2S::Stub
{	
	/**
	\~korean
	ProudNet 서버

	\~english
	ProudNet server

	\~chinese
	ProudNet服务器

	\~japanese
	ProudNetサーバ
	\~
	*/
	CHeldPtr<CNetServer> m_netServer;
	
	/**
	\~korean
	entry 서버도 Farm 클라이언트입니다.

	\~english
	entry server is farm client.

	\~chinese
	Entry服务器为Farm客户端。

	\~japanese
	entryサーバもFarmクライアントです。

	\~
	*/
	CHeldPtr<CFarmClient> m_farmClient;

	/**
	\~korean
	
	\~english
	ProudDB cache client

	\~chinese
	\~japanese
	\~
	*/
	CHeldPtr<CDbCacheClient2> m_dbClient;
	
	/**
	\~korean
	entry 서버에 접속한 클라 목록

	\~english
	Client list that connected at entry server

	\~chinese
	连接entry服务器的客户端目录

	\~japanese
	entryサーバに接続しているクライアントのリスト

	\~
	*/
	RemoteEntryClients m_remoteClients;
	
	/**
	\~korean
	스레드 블럭 보호차 필요

	\~english
	Require to protect thread block

	\~chinese
	需要保护thread block

	\~japanese
	スレッドブロック保護のために必要

	\~
	*/
	CriticalSection m_cs;

	/**
	\~korean
	
	\~english
	server to client RMI

	\~chinese
	\~japanese
	\~
	*/
	EntryS2C::Proxy m_s2cProxy;

	__int64 m_lastGuiUpdateTime;
public:
	
	/**
	\~korean
	이 entry server의 인터넷 주소

	\~english
	Internet address of this entry server

	\~chinese
	该entry server的互联网地址

	\~japanese
	このentry serverのインターネットアドレス

	\~
	*/
	NamedAddrPort m_serverAddr;
	
	/**
	\~korean
	entry client에 보낼 로비 채널 리스트

	\~english
	Lobby channel list that send to entry client

	\~chinese
	需要向entry客户端发送的lobby channel列表

	\~japanese
	entry clientに送られるロビーチャンネルのリスト

	\~
	*/
	PublishedLobbyInfoList m_publishedLobbyList;

	/**
	\~korean
	게이머에게 전송될 로비 채널 리스트를 준비합니다.

	\~english
	Prepare lobby channel list for sending to gamer.

	\~chinese
	准备要向玩家发送的lobby channel列表。

	\~japanese
	ゲーマーに送られるロビーチャンネルのリストを準備します。

	\~
	*/
	void RefreshPublishedLobbyList();

	CEntryServer(void);
	~CEntryServer(void);

	virtual void NotifyCreatedCredential(Guid, HostID) override;
	
	/**
	\~korean
	처음 로그인되는 서버이기 때문에 밑의 두 함수는 쓰이지 않습니다.

	\~english
	Following two functions are not using because it is first time to logon server.

	\~chinese
	第一次登录的服务器，因此不适用如下两个函数。

	\~japanese
	初めてログインされるサーバであるため、下の二つの関数は使われません。

	\~
	*/
	virtual void NotifyCheckCredentialFail(HostID userHostID) override;
	virtual void NotifyCheckCredentialSuccess(const Guid &gamerGuid, HostID userHostID) override;

	/**
	\~korean
	listening을 시작합니다.

	\~english
	Starts listening.

	\~chinese
	开始监听。

	\~japanese
	listeningを開始します。

	\~
	*/
	void Start();

	/**
	\~korean
	일정 시간마다의 프로세싱입니다.

	\~english
	Processing every specific time

	\~chinese
	每隔一定时间进行处理。

	\~japanese
	一定時間ごとの処理です。

	\~
	*/
	virtual void OnTick(void* context) override;
	String GetDescription() {
		return L"Entry Server";
	}

	/**
	\~korean
	클라이언트의 HostID를 근거로 클라이언트 객체를 찾습니다.

	\~english
	Find out client object that based on client HostID.

	\~chinese
	根据客户端HostID寻找客户端对象。

	\~japanese
	クライアントのHostIDに基づいてクライアントのオブジェクトを検索します。

	\~
	*/
	CRemoteEntryClientPtr GetRemoteClientByHostID(HostID clientID);

	/**
	\~korean
	클라이언트의 HostID 값을 입력값으로 받아, 로그온한 클라이언트 객체를 찾습니다.

	\~english
	Find out logged on client object receving client HostID as input value.

	\~chinese
	根据客户端HostID值寻找已登录的客户端对象。

	\~japanese
	クライアントのHostID値を入力値として受け取り、ログオンしたクライアントのオブジェクトを検索します。

	\~
	*/
	CRemoteEntryClientPtr GetAuthedRemoteClientByHostID(HostID clientID);

	/**
	\~korean
	게이머 Guid를 근거로 로그온한 클라이언트 객체를 찾습니다.

	\~english
	Find out logged on client object that based on gamer Guid.

	\~chinese
	根据玩家Guid寻找已登录的客户端对象。

	\~japanese
	ゲーマーGuidに基づいてログオンしたクライアントのオブジェクトを検索します。

	\~
	*/
	CRemoteEntryClientPtr GetRemoteClientByGamerGuid(Guid gamerGuid);
	CRemoteEntryClientPtr GetRemoteClientByDbSessionGuid( Guid dbSessionGuid );

	/**
	\~korean
	게임 클라이언트에 '미인증 상태로 통신을 시도했음' 에러를 보냅니다.

	\~english
	Send 'Try communication without authentication' error to game client.

	\~chinese
	向游戏客户端发送‘未认证状态下尝试连接’的出错信息。

	\~japanese
	ゲームクライアントに「未認証状態で通信を試みました。」というエラーを送ります。

	\~
	*/
	void NotifyUnauthedAccess(HostID clientID);
	
	/**
	\~korean
	INetServer 구현

	\~english
	Realize INetServer

	\~chinese
	实现INetServer

	\~japanese
	INetServer実装

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
	实现IFarmClientDelegate

	\~japanese
	IFarmClientDelegate実装です。

	\~
	*/
	virtual void OnJoinFarmServerComplete( ErrorInfo* info) override;
	virtual void OnLeaveFarmServer(ErrorType reason) override;
	virtual void OnNotifyFarmClientInfo(const CFarmClientInfo& info) override;
	
	virtual int GetGamerCount()
	{
		CriticalSectionLock lock(m_cs, true);
		return m_remoteClients.Count;
	}
	virtual NamedAddrPort GetServerAddr()
	{
		return m_serverAddr;
	}
	virtual CriticalSection* GetCritSec() override { return &m_cs;}


	// ******************************************
	// **** Realize IDbCacheClientDelegate2 *****
	// *****IDbCacheClientDelegate2 구현*********
	// ******************************************

	/**
	\~korean
	서버 연결 시도 결과를 리턴합니다.

	\~english
	Returns result of server connecting attempt.

	\~chinese
	返回尝试连接服务器结果。

	\~japanese
	サーバに接続しようとした結果を返します。

	\~
	*/
	virtual void OnJoinDbCacheServerComplete(ErrorInfo *info)override {}

	/**
	\~korean
	DB cache server와의 연결이 해제됐음을 notify합니다.
	- 이것이 호출된 상황에서는, 이미 DB cache client의 모든 데이터가 unload된 상태입니다.

	\~english
	Notify disconnected with DB cache server.
	- When it called, all data of DB cache client is unloaded.

	\~chinese
	提示与DB cache server的连接已断开。
	- 调用时DB cache client的所有数据以全部加载。

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

	virtual void OnExclusiveLoadDataComplete(CCallbackArgs& args)override; //Gamer
	

	/**
	\~korean
	독점 로딩 증발시에 대한 handling 루틴 구현입니다.

	\~english
	Realize handling routine when exclusive loading is disappeared

	\~chinese
	实现独占加载消失时的handling规则。

	\~japanese
	独占ロードがなくなったことに対するhandlingルーチンの実装です。

	\~
	*/
	virtual void OnDataUnloadRequested(CCallbackArgs& args)override;
  
	virtual void OnAddDataFailed(CCallbackArgs& args)override {}
	
	virtual void OnAddDataSuccess(CCallbackArgs& args)override {}


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
	virtual void OnDataForceUnloaded(CCallbackArgs& args)override {}
	/**
	\~korean
	DBMS 에 먼저 반영되는 요청 응답형 메서드 및 blocked 메서드 사용 방법 혹은 비독점적으로 데이터를 접근하는 방법이
	실행되는 동안 다른 곳에서 일방적 데이터 변경하기 방법 를 사용하면 호출됩니다.

	\~english
	It calls when you use method of changing data unliterally from other place druing request response type method that apply to DBMS first and usage of blocked method or non-exclusively access data 

	\~chinese
	优先应用到DBMS的请求应答类型方法和blocked method使用方法或非独占数据连接方法施行期间，如其他地方单方面使用数据变更方法则进行调用。

	\~japanese
	DBMSに最初に反映される要求応答型のメソッド及びblockedメソッドの使用方法、あるいは非独占的にデータにアクセスする方法が
	実行中に他の場所で一方的にデータを変更するという方法を使うと呼び出されます。

	\~
	*/
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
	DECRMI_EntryC2S_RequestFirstLogon;
	DECRMI_EntryC2S_RequestHeroSlots;
	DECRMI_EntryC2S_RequestSelectHero;
	DECRMI_EntryC2S_RequestAddHero;
	DECRMI_EntryC2S_RequestRemoveHero;
	DECRMI_EntryC2S_RequestLobbyList;
	DECRMI_EntryC2S_RequestCreateNewGamer;
	DECRMI_EntryC2S_RequestReturnToEntry;
	

public:		
		/**
		\~korean
		여기서 사용되지는 않으나 RequestADD,RequestRemove,RequestUpdate 등을 쓸때는 사용자가 넣어줄 식별값.
		CCallbackArg.m_tag로 들어갑니다.

		\~english
		Identifying value when you use RequestADD,RequestRemove,RequestUpdate that assigned by user. It does not use at this point.
		Enter to CCallbackArg.m_tag.

		\~chinese
		这里不使用，但使用RequestADD,RequestRemove,RequestUpdate 等时用户需输入的识别值
		进入CCallbackArg.m_tag。

		\~japanese
		ここで使われることはないがRequestADD、RequestRemove、RequestUpdateなどを使う時にユーザーが入力する識別値。
		CCallbackArg.m_tagに入ります。

		\~
		*/
	    INT_PTR	m_diffCustomArg;
};
