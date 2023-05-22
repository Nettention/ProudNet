#pragma once

#include "farmcommon.h"
#include "../GCCommon/farm_proxy.h"
#include "../GCCommon/farm_stub.h"
#include "../../../include/DbLogWriter.h"
#include "../../../include/ProudNetClient.h"


class CFarmClientInfo;
class CServerBase;
class CTestLogWriterDelegate;

/**
\~korean
Farm 클라이언트의 콜백을 받거나 Farm 클라이언트의 추가 정보를 Farm 클라이언트를 사용하는 객체에서 제공하기 위한 용도입니다.

\~english
It is for receive callback of farm client or provide additional information of farm client to object that using farm client

\~chinese
用于接收Farm客户端的回调或向使用Farm客户端的对象提供Farm客户额外信息。

\~japanese
Farmクライアントのコールバックを受け取ったり、Farmクライアントの追加情報をFarmクライアントを使うオブジェクトに提供するためです。

\~
*/
class IFarmClientDelegate
{
public:
	virtual ~IFarmClientDelegate() {}

	/**
	\~korean	
	Farm 클라가 Farm 서버로의 연결 시도가 끝났을 때 성공 또는 실패를 notify하는 handler입니다.

	\~english
	Handler that notify success or failure when farm client is finished connecting to farm server

	\~chinese
	提示Farm客户端连接Farm服务器成功与否的Handler。

	\~japanese
	FarmクライアントがFarmサーバへの接続の試みが終わった時、成功か失敗かをnotifyするhandlerです。

	\~
	*/
	virtual void OnJoinFarmServerComplete( ErrorInfo* info) = 0;

	/**
	\~korean
	Farm 서버와의 연결 해제의 handler입니다.

	\~english
	Handler of disconnecting with farm server

	\~chinese
	与Farm服务器断开连接的handler。

	\~japanese
	Farmサーバとの接続を解除するhandlerです。

	\~
	*/
	virtual void OnLeaveFarmServer(ErrorType reason) = 0;

	/**
	\~korean
	이 Farm 클라이언트(즉 게임 서버)에 접속한 총 게이머 수를 얻습니다.

	\~english
	Gets all number of gamer who connected at this farm client (game server).

	\~chinese
	获取连接该Farm客户端(即游戏服务器)的玩家总数。

	\~japanese
	このFarmクライアント（つまりゲームサーバ）に接続したゲーマーの総数を取得します。

	\~
	*/
	virtual int GetGamerCount() = 0;

	/**
	\~korean
	이 Farm 클라이언트(즉 게임 서버)에의 접속 가능한 인터넷 주소를 얻습니다.

	\~english
	Gets connectable inter address to this fram client (game server).

	\~chinese
	获取连接该Farm客户端(即游戏服务器)所需互联网地址。

	\~japanese
	このFarmクライアント（つまりゲームサーバ）への接続可能なインターネットアドレスを取得します。

	\~
	*/
	virtual NamedAddrPort GetServerAddr() = 0;

	/**
	\~korean
	이 Farm 클라이언트(즉 게임 서버)의 critical section을 얻습니다. CFarmClient는 자체적으로 critical section을 가지지 않고
	이 함수가 주는 객체를 사용해서 잠금을 합니다.

	\~english
	Gets critical section of this farm client (game server). CFarmClient does not have critical section itself and it does locking that using object from this function.

	\~chinese
	获取该Farm客户端(即游戏服务器)的critical section。CFarmClient自己没有critical section
	利用该函数的对象进行锁定。

	\~japanese
	このFarmクライアント（つまりゲームサーバ）のcritical sectionを取得します。 CFarmClientは自主的にcritical sectionを持たず
	この関数から与えられたオブジェクトを使ってロックします。

	\~
	*/
	virtual CriticalSection* GetCritSec() = 0;

	/**
	\~korean
	이 Farm 클라이언트(즉 게임 서버)가 연관된 로비 이름을 리턴합니다. 예를 들어 배틀 서버는 이 값을 리턴합니다.

	\~english
	Returns related lobby name with this farm client (game server). For example, battle server returns this value.

	\~chinese
	返回与该Farm客户端(即游戏服务器)有关的lobby名称。例如作战服务器返回该值。

	\~japanese
	このFarmクライアント（つまりゲームサーバー）に関わるロビーの名前を返します。例えばバトルサーバーはこの値を返します。

	\~
	*/
	virtual String GetRelatedLobbyName() {
		return String();
	}

	/**
	\~korean
	로비 서버에 의해 이 farm client가 게임방 생성 요청을 받았을 때의 handler입니다.
	배틀 서버가 아닌 이상, 즉 이 메서드를 override하지 않은 경우 게임방 생성을 실패시키도록 합니다.

	\~english
	Handler when this farm client receives room creation request by lobby server.
	It does not override this method, make it failed to room creation even if it is battle server.

	\~chinese
	该farm客户端从lobby服务器收到游戏房间创建请求时的handler。
	如果不是作战服务器，即不覆盖该method时游戏房间创建失败。

	\~japanese
	ロビーサーバによってこのfarm clientがゲームルーム生成の要請を受けた時のhandlerです。
	バトルサーバでない限り、つまりこのメソッドをoverrideしていない場合はゲームルーム生成が失敗するようにします。

	\~
	*/
	virtual CasualResult CreateGameRoomByLobbyServer(CGameRoomParameter& inoutParameter)
	{
		return Casual_Unexpected;
	}

	/**
	\~korean
	배틀 서버에 의해 이 farm client가 게임방 생성 결과를 받았을 때의 handler입니다.
	로비 서버가 아닌 이상 이 메서드는 아무것도 하지 않습니다.

	\~english
	Handler when this farm client receives game room creation result by battle server.
	It does not do anything even if it is lobby server

	\~chinese
	该farm客户端从作战服务器接收游戏房间创建结果时的handler。
	除lobby服务器，该method不做任何事情。
	
	\~japanese
	バトルサーバによってこのfarm clientがゲームルーム生成の要請を受けた時のhandlerです。
	ロビーサーバでない限り、このメソッドは何もしません。

	\~
	*/
	virtual void OnCreateGameRoomResult(HostID requestedLobbyClientID, CasualResult result, const CGameRoomParameter& createdGameRoomParam) {}

	/**
	\~korean
	로비 서버에 의해 이 farm client가 게임방 진입 요청을 받았을 때의 handler입니다.
	배틀 서버가 아닌 이상, 즉 이 메서드를 override하지 않은 경우 게임방 진입을 실패시키도록 합니다.

	\~english
	Handler when this farm client receives request about entering game room by lobby server.
	It does not override this method, make it failed to room creation even if it is battle server.

	\~chinese
	该farm客户端从lobby服务器收到游戏房间进入请求时的handler。
	如果不是作战服务器，即不覆盖该method时游戏房间进入失败。

	\~japanese
	ロビーサーバによってこのfarm clientがゲームルーム入場の要請を受けた時のhandlerです。
	バトルサーバでない限り、つまりこのメソッドをoverrideしていない場合はゲームルームへの入場を失敗させるようにします。

	\~
	*/
	virtual CasualResult JoinGameRoomByLobbyServer(const CJoinGameRoomParameter& joinParam, CGameRoomParameter& outJoinAllowedGameRoomInfo)
	{
		return Casual_Unexpected;
	}

	/**
	\~korean
	배틀 서버에 의해 이 farm client가 게임방 진입 결과를 받았을 때의 handler입니다.
	로비 서버가 아닌 이상 이 메서드는 아무것도 하지 않습니다.

	\~english
	Handler when this farm client receives game room entering result by battle server.
	It does not do anything even if it is lobby server.

	\~chinese
	该farm客户端从作战服务器接收游戏房间进入结果时的handler。
	除lobby服务器，该method不做任何事情。

	\~japanese
	バトルサーバによってこのfarm clientがゲームルーム入場の結果を受けた時のhandlerです。
	ロビーサーバでない限り、このメソッドは何もしません。

	\~
	*/
	virtual void OnJoinGameRoomResult(HostID requestedLobbyClientID, CasualResult result, const CGameRoomParameter& gameRoomParam) {}

	/**
	\~korean
	타 Farm 클라이언트의 동기화 메시지가 오는 순간의 handler입니다.

	\~english
	Handler that moment of coming synchronize message of other farm client.

	\~chinese
	收到其他Farm客户端同步消息时的handler。

	\~japanese
	他のFarmクライアントの同期化メッセージが来る瞬間のhandlerです。

	\~
	*/
	virtual void OnNotifyFarmClientInfo(const CFarmClientInfo& info) {}

	/**
	\~korean
	배틀 서버인 타 Farm 클라이언트에서 게임방 생성 notify가 도착하는 것을 처리하는 이벤트 handler입니다.
	로비 서버인 경우에 이 메서드를 override합니다.

	\~english
	Event handler that process arriving room creation notice from other farm client (battle server).
	If it is lobby server, it has to override this method.

	\~chinese
	从其他作战服务器即Farm客户端中接收游戏房间创建提示时进行处理的事件handler。
	如果为lobby服务器则覆盖该method。

	\~japanese
	バトルサーバである他のFarmクライアントからゲームルーム生成のnotifyが到着したのを処理するイベントhandlerです。
	ロビーサーバである場合、このメソッドをoverrideします。

	\~
	*/
	virtual void OnGameRoomAppear(const CGameRoomParameter& info)  {}

	/**
	\~korean
	배틀 서버인 타 Farm 클라이언트에서 게임방 변경 notify가 도착하는 것을 처리하는 이벤트 handler입니다.
	로비 서버인 경우에 이 메서드를 override합니다.

	\~english
	Event handler that process arriving room change notice from other farm client (battle server).
	If it is lobby server, it has to override this method.

	\~chinese
	从其他作战服务器即Farm客户端中接收游戏房间变更提示时进行处理的事件handler。
	如果为lobby服务器则覆盖该method。

	\~japanese
	バトルサーバである他のFarmクライアントからゲームルーム変更のnotifyが到着したのを処理するイベントhandlerです。
	ロビーサーバである場合、このメソッドをoverrideします。

	\~
	*/
	virtual void OnGameRoomShowState(const CGameRoomParameter& info)  {}

	/**
	\~korean
	배틀 서버인 타 Farm 클라이언트에서 게임방 제거 notify가 도착하는 것을 처리하는 이벤트 handler입니다.
	로비 서버인 경우에 이 메서드를 override합니다.

	\~english
	Event handler that process arriving room removal notice from other farm client (battle server).
	If it is lobby server, it has to override this method.

	\~chinese
	从其他作战服务器即Farm客户端中接收游戏房间删除提示时进行处理的事件handler。
	如果为lobby服务器则覆盖该method。

	\~japanese
	バトルサーバである他のFarmクライアントからゲームルーム削除のnotifyが到着したのを処理するイベントhandlerです。
	ロビーサーバである場合、このメソッドをoverrideします。

	\~
	*/
	virtual void OnGameRoomDisappear(Guid roomGuid) {}

	/**
	\~korean
	ILanClientEvent에서 일정 주기마다 호출하는 것을 처리해주는 이벤트 handler입니다.

	\~english
	Event handler that process every specific time at ILanClientEvent

	\~chinese
	处理ILanClientEvent每个一定周期调用的事件handler。

	\~japanese
	ILanClientEventで一定の周期ごとに呼び出すことを処理するイベントhandlerです。

	\~
	*/
	virtual void OnTick(void* context) {}

	virtual void NotifyCreatedCredential(Guid, HostID) {}

	/**
	\~korean
	status sever를 통하여 credential을 생성 확인합니다.
	status sever외의 모든 sever가 사용합니다.

	\~english
	Confirm credential creation through status sever.
	It uses all server except status sever.

	\~chinese
	通过status server确认credential的创建。
	Status server之外的所有server使用。

	\~japanese
	status severを介してcredential生成を確認します。
	status sever以外のすべてのseverが使います。

	\~
	*/
	virtual void NotifyCheckCredentialFail(HostID userHostID){}
	virtual void NotifyCheckCredentialSuccess(const Guid &gamerGuid, HostID userHostID){}

	/**
	\~korean
	status server에서만 사용 하는 이벤트 handler 인증입니다.
	인증키를 생성합니다.

	\~english
	Event handler authentication that use only as status server
	Creates authentication key.

	\~chinese
	只有在status server上使用的事件handler认证。
	生成认证key。

	\~japanese
	status serverでのみ使うイベントhandler認証です。
	認証キーを生成します。

	\~
	*/
	virtual bool RequestCreateCredential(const HostID remote, const Guid &gamerUUID, const HostID &UserHostID){return false;}

	/**
	\~korean
	status server에서만 사용 하는 이벤트 handler 인증입니다.
	인증키를 검사합니다.

	\~english
	Event handler authentication that use only as status server
	Check authentication key.

	\~chinese
	只有在status server上使用的事件handler认证。
	检查认证key。

	\~japanese
	status serverでのみ使うイベントhandler認証です。
	認証キーを検査します。

	\~
	*/
	virtual bool RequestCheckCredential(Proud::HostID remote, const Proud::Guid &credential,const HostID &userHostID,const String &ServerName){return false;}

	/**
	\~korean
	status server에서만 사용 하는 이벤트 handler 인증입니다.
	로그아웃 처리를 합니다.

	\~english
	Event handler authentication that use only as status server
	Do logout processing.

	\~chinese
	只有在status server上使用的事件handler认证。
	进行注销处理。

	\~japanese
	status serverでのみ使うイベントhandler認証です。
	ログアウト処理を行います。

	\~
	*/
	virtual bool UserLogOut(Proud::HostID remote, const Guid &gamerUUID){return false;}
};


/**
\~korean
Farm 서버로 접속하기 위한 파라메터 구조체입니다.

\~english
Parameter structure to connect farm server

\~chinese
连接Farm服务器的变量结构。

\~japanese
Farmサーバに接続するためのパラメータ構造体です。

\~
*/
class CFarmClientConnectParameter
{
public:

	/**
	\~korean
	Farm 클라이언트의 delegate

	\~english
	Delegate of farm client

	\~chinese
	指定Farm客户端

	\~japanese
	Farmクライアントのdelegate

	\~
	*/
	IFarmClientDelegate *m_delegate;

	/**
	\~korean
	Farm 서버의 이름. Farm 서버 시작시 지정되는 이름과 동일해야 합니다.

	\~english
	Name of farm server. It has to same name as name that assigned with starting farm server.

	\~chinese
	Farm服务器名称，应与Farm服务器开始时指定的名称一致。

	\~japanese
	Farmサーバの名前。 Farmサーバが起動する時に指定した名前と同じでなければならない。

	\~
	*/
	String m_farmName;

	/**
	\~korean
	Farm 서버와 타 Farm 클라이언트가 인식할, 로컬 클라이언트의 이름

	\~english
	Name of local client that identify with farm server and other farm client

	\~chinese
	Farm服务器和其他Farm客户端识别的本地客户端名称

	\~japanese
	Farmサーバと他のFarmクライアントが認識するローカルクライアントの名前

	\~
	*/
	String m_farmClientName;

	/**
	\~korean
	이 Farm 클라이언트가 어떤 역할을 하는 게임서버인지

	\~english
	This farm client dose what kind of role as game server

	\~chinese
	该Farm客户端的游戏服务器职责

	\~japanese
	このFarmクライアントがどのような役割をするゲームサーバなのか。

	\~
	*/
	ServerMode m_farmClientType;
};

/**
\~korean
Farm 클라이언트 메인 객체입니다.
- Farm 클라이언트는 게임 서버, 로비 서버 등 게이머가 접속 가능한 서버이기도 합니다.
- 모든 서버를 관장하는 하나의 도메인 서버가 Farm 서버입니다.

\~english
Farm client main object
- Farm client is server that possible to connect with gamer such as game server, lobby server.
- Farm server is one domain server which control all other servers.

\~chinese
Farm 客户端主对象。
- Farm 客户端也是游戏服务器、lobby服务器等玩家可连接的服务器。
- 控制所有服务器的域服务器为Farm服务器。

\~japanese
Farmクライアントのメインオブジェクトです。
- Farmクライアントはゲームサーバ、ロビーサーバなど、ゲーマーが接続可能なサーバでもあります。
- すべてのサーバを管理する単一のドメインサーバーがFarmサーバです。

\~
*/
class CFarmClient: public INetClientEvent, public FarmS2C::Stub, public FarmC2C::Stub
{
protected:

	/**
	\~korean
	Farm 클라이언트의 네트워크 엔진

	\~english
	Network engine of farm client

	\~chinese
	Farm客户端的网络引擎

	\~japanese
	Farmクライアントのネットワークエンジン

	\~
	*/
	CAutoPtr<CNetClient> m_lanClient;
	IFarmClientDelegate* m_dg;

	/**
	\~korean
	DB LogWriter 

	\~english
	DB LogWriter 

	\~chinese
	DB LogWriter

	\~japanese
	DB LogWriter

	\~
	*/
	CAutoPtr<CDbLogWriter> m_DbLogWriter;

	CHeldPtr<CTestLogWriterDelegate> m_logWriteDelegate;

	/**
	\~korean
	이 Farm 클라이언트가 인증이 끝났는지 여부

	\~english
	Check whether this farm client has authenticated or not

	\~chinese
	该 Farm客户端认证是否结束

	\~japanese
	このFarmクライアントが認証を終了したのかどうか

	\~
	*/
	bool m_authed;

	/**
	\~korean
	이 Farm 클라이언트가 Farm 서버로 접속 시도를 시작한 시간

	\~english
	Connecting attempt time to farm server from this farm client

	\~chinese
	该Farm客户端试图连接Farm服务器的开始时间

	\~japanese
	このFarmクライアントがFarmサーバに接続しようとした時間

	\~
	*/
	double m_connectStartedTime;

	/**
	\~korean
	이 서버 객체를 관장하는 어플리케이션 폼

	\~english
	Application form that control this server object

	\~chinese
	控制该服务器对象的应用表单

	\~japanese
	このサーバのオブジェクトを管理するアプリケーションフォーム

	\~
	*/
	CServerBase* m_owner;

	/**
	\~korean
	이 서버의 이름

	\~english
	Name of this server

	\~chinese
	该服务器名称

	\~japanese
	このサーバーの名前

	\~
	*/
	String m_ownerServerName;

public:

	/**
	\~korean
	Credential 키를 생성해주는 서버

	\~english
	Server that create Credential key

	\~chinese
	生成Credential Key的服务器

	\~japanese
	Credentialキーを生成してくれるサーバ

	\~
	*/
	HostID m_StatusServerHostID;

	FarmC2S::Proxy m_c2sProxy;
	FarmC2C::Proxy m_c2cProxy;
private:


	/**
	\~korean
	Farm 서버의 이름

	\~english
	Name of farm server

	\~chinese
	Farm服务器名称

	\~japanese
	Farmサーバの名前

	\~
	*/
	String m_farmName;

	/**
	\~korean
	이 Farm 클라의 이름

	\~english
	Name of this farm client

	\~chinese
	该Farm客户端的名称

	\~japanese
	このFarmクライアントの名前

	\~
	*/
	String m_farmClientName;

	/**
	\~korean
	 이 Farm 클라의 역할

	\~english
	 Role of this farm client

	\~chinese
	该Farm客户端的作用

	\~japanese
	このFarmクライアントの役割

	\~
	*/
	ServerMode m_farmClientType;

	CFarmClientInfo m_info;
	int64_t m_broadcastFarmClientInfoCoolTime;
	HostID m_p2pGroupID ;

	void RefreshFarmClientInfo();


public:

	/**
	\~korean
	Farm 서버에 접속해 있는 다른 Farm 클라의 개괄적 상태 데이터 리스트

	\~english
	General condition data list of other farm client who connected at farm server

	\~chinese
	连接Farm服务器的其他 Farm客户端的整体状态数据列表

	\~japanese
	Farmサーバーに接続している他のFarmクライアントの総括状態データのリスト

	\~
	*/
	FarmClientInfoMap m_remoteFarmClientInfoMap;

	CFarmClient(void);
	~CFarmClient(void);

	void DBLogWrite(String LogText, CPropNode* const pPropNode=NULL) 
	{
		//if(m_DbLogWriter)
		//	m_DbLogWriter->WriteLine(LogText, pPropNode); 
	}

	void Init( CFarmClientConnectParameter& param, CServerBase* pOwner, String &ServerName );

	/**
	\~korean
	Farm 서버에 연결 시도를 시작합니다.

	\~english
	Starts connecting attempt to farm server.

	\~chinese
	开始试图连接Farm服务器。

	\~japanese
	Farmサーバへの接続の試みを開始します。

	\~
	*/
	void Connect();

	/**
	\~korean
	일정 시간마다 호출해야 하는 프로세싱입니다.

	\~english
	Processing that calls every specific time

	\~chinese
	每个一定时间应调用的Processing。

	\~japanese
	一定時間ごとに呼び出す必要がある処理です。

	\~
	*/
	void FarmClientFrameMove();

	virtual void OnError(ErrorInfo *errorInfo) override;
	virtual void OnWarning(ErrorInfo *errorInfo) override;
	virtual void OnInformation(ErrorInfo *errorInf0) override;
	virtual void OnException(const Proud::Exception &e) override;
	virtual void OnNoRmiProcessed(RmiID rmiID) override {}

	virtual void OnJoinServerComplete(ErrorInfo *info, const ByteArray &replyFromServer) override;
	virtual void OnLeaveServer(ErrorInfo *errorInfo) override;
	virtual void OnP2PMemberJoin(HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField) override;
	virtual void OnP2PMemberLeave(HostID memberHostID, HostID groupHostID, int memberCount) override;
	virtual void OnChangeP2PRelayState(HostID remoteHostID, ErrorType reason) override {}
	virtual void OnSynchronizeServerTime() override {}

	virtual void OnTick(void* context) override;

	DECRMI_FarmS2C_NotifyFarmLogonFailed;
	DECRMI_FarmS2C_NotifyFarmLogonSuccess;
	DECRMI_FarmC2C_NotifyFarmClientInfo;

	DECRMI_FarmC2C_RequestCreateGameRoom;
	DECRMI_FarmC2C_NotifyCreateGameRoomResult;

	DECRMI_FarmC2C_GameRoom_Appear;
	DECRMI_FarmC2C_GameRoom_ShowState;
	DECRMI_FarmC2C_GameRoom_Disappear;

	DECRMI_FarmC2C_RequestJoinGameRoom;
	DECRMI_FarmC2C_NotifyJoinGameRoomResult;
	DECRMI_FarmC2C_NotifyStatusServer;

	DECRMI_FarmC2C_NotifyCreatedCredential;
	DECRMI_FarmC2C_NotifyCheckCredentialSuccess;
	DECRMI_FarmC2C_NotifyCheckCredentialFail;

	DECRMI_FarmC2C_RequestCreateCredential;
	DECRMI_FarmC2C_RequestCheckCredential;
	DECRMI_FarmC2C_UserLogOut;
};

#include "../GCCommon/Farm_Proxy.h"
#include "../GCCommon/Farm_Stub.h"

class CTestLogWriterDelegate : public ILogWriterDelegate
{
	virtual void OnLogWriterException(Proud::AdoException &Err) override;
};
