#pragma once
#include "ServerBase.h"
#include "farmclient.h"


/**
\~korean
전 서버에 접속되어 있는 유저들에 대한 Credential값과, DB에서 유저 정보를 로드하기 위해 필요한 정보들을 관리합니다.

\~english
Manage Credential value about connected user at all servers and information for loading user information at DB.

\~chinese
管理所有服务器连接用户的Credential值和DB中加载用户数据所需的信息。

\~japanese
全サーバに接続しているユーザーに対するCredential値とDBからユーザ情報をロードするために必要な情報を管理します。

\~
*/
class CGamerInfo_S
{
public:

	/**
	\~korean
	게이머가 어느 서버에 로그온 되어 있는지

	\~english
	Is gamer logged on which server?

	\~chinese
	玩家登录的服务器是？

	\~japanese
	ゲーマーがどのサーバにログオンしているか

	\~
	*/
	HostID m_serverHostID;
	
	/**
	\~korean
	 게이머가 로그온 된 서버의 이름

	\~english
	 Name of server that gamer has logged on

	\~chinese
	玩家登录的服务器名称

	\~japanese
	ゲーマーがログオンしているサーバの名前

	\~
	*/
	String m_serverName;

	Guid m_credential;
	Guid m_gamerUUID; 

	/**
	\~korean
	현재 로그인 되어있는지 여부

	\~english
	Current logon state

	\~chinese
	目前是否为登录状态

	\~japanese
	現在ログインしているかどうか

	\~
	*/
	bool m_isLoggedOn;

	/**
	\~korean
	 Credential 소멸하지 전의 남은 시간 - 로그아웃 후에만 사용

	\~english
	 Time that left for disappearing Credential - Use only after logout

	\~chinese
	Credential小时之前所剩时间-只能在注销之后使用

	\~japanese
	Credential消滅する前の残り時間 - ログアウト後のみ使用

	\~
	*/
	__int64 m_clientLogOutTime;
};

typedef RefCount<CGamerInfo_S> CGamerInfoPtr_S;
typedef CFastMap<Guid,CGamerInfoPtr_S> GuidToGamerInfoMap;


/**
\~korean
이 시간이 끝날 때까지 인증키를 보유하게 됩니다. 
잦은 재접속 유저가 있을 수 있기 때문에 나갔다고 하여 바로 제거하는 것은 효율적이지 못합니다.

\~english
Own authentication key until this time is end. 
It is not good to remove it instantly because there is user who often does connect and disconnet.

\~chinese
认证key保留至该时间结束。
因为有可能会有频繁重新连接的用户，因此断开之后立即删除为非有效方案。

\~japanese
この時間が終わるまで認証キーを保有することになります。
頻繁に再接続するユーザがいる可能性があるため、退出したとしてもすぐに削除することは効率的ではない。

\~
*/
const double WaitLogOutTime=600000;

class CStatusServer: public CServerBase, public IFarmClientDelegate
{
	CAutoPtr<CFarmClient> m_farmClient;

	/**
	\~korean
	이 스탯 서버에 보고된 클라이언트 리스트
	첫 로그인에는 GamerUUID 를 사용하여 사용자를 검색하고 이루 NextLogOn부터는 오로지 Credential값으로만 사용자를 검색합니다.
	때문에 동시에 추가와 제거가 되며같은 CriticalSection 으로 묶여야만 합니다.

	\~english
	Client list that reported to this state server
	Searching user wiht GamerUUID when it first logon then search user with only Credential value from NextLogOn.
	So it has to bind as CriticalSection that does add and remove.

	\~chinese
	上报到该state服务器上的客户端列表
	第一次登录时使用GamerUUID搜索用户，下一次NextLogOn开始只用Credential值搜索用户。
	因此同时增加和删除的应捆绑为一个CriticalSection。
	
	\~japanese
	このステータスサーバに報告されたクライアントのリスト
	最初のログインにはGamerUUIDを使ってユーザーを検索し、その後NextLogOnからはCredential値のみでユーザーを検索します。
	従って同時に追加、又は削除され、同じCriticalSectionに結ばれていｓるべきです。

	\~
	*/
	GuidToGamerInfoMap m_gamerGuidToGamerMap;
	GuidToGamerInfoMap m_credentialToGamerMap;

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

	__int64 m_lastGuiUpdateTime;

public :


	/**
	\~korean
	이 인식 가능한 이름

	\~english
	Identifiable name

	\~chinese
	可识别名称

	\~japanese
	認識可能な名前

	\~
	*/
	String m_statusName;

	CStatusServer(void);
	virtual ~CStatusServer(void);

	void Start() override;
	String GetDescription();
	CGamerInfoPtr_S GetClientByCredentialUUID(Guid credentialUUID);
	CGamerInfoPtr_S GetClientByGamerUUID(Guid gamerUUID);


	/**
	\~korean
	Farm 클라이언트가 Farm 서버로의 연결 시도가 끝났을 때 성공 또는 실패를 notify하는 handler입니다.

	\~english
	Handler that notify success or failure when farm client has finished connectiong to farm server

	\~chinese
	提示Farm客户端连接Farm服务器时成功与否的handler。

	\~japanese
	FarmクライアントがFarmサーバへの接続試みが終わった時、成功又は失敗をnotifyするhandlerです。

	\~
	*/
	virtual void OnJoinFarmServerComplete( ErrorInfo* info) override;

	/**
	\~korean
	Farm 서버와의 연결 해제의 handler입니다.

	\~english
	Disconnecting handler with farm server

	\~chinese
	断开与Farm服务器连接的handler。

	\~japanese
	Farmサーバとの接続を解除するhandlerです。

	\~
	*/
	virtual void OnLeaveFarmServer(ErrorType reason) override;

	/**
	\~korean
	이 Farm 클라이언트(즉 게임 서버)에 접속한 총 게이머 수를 얻습니다.

	\~english
	Gets number of all gamers who connected farm client (game server)

	\~chinese
	获取连接该Farm客户端(即游戏服务器)的玩家总数。

	\~japanese
	このFarmクライアント（つまりゲームサーバ）に接続したゲーマーの総数を取得します。

	\~
	*/
	virtual int GetGamerCount() override;

	/**
	\~korean
	이 Farm 클라이언트(즉 게임 서버)에의 접속 가능한 인터넷 주소를 얻습니다.

	\~english
	Gets vaild internet address which is able to connect from this fram client (game server)

	\~chinese
	获取连接该Farm客户端(即游戏服务器)所需互联网地址。

	\~japanese
	このFarmクライアント（つまりゲームサーバ）に接続可能なインターネットアドレスを取得します。

	\~
	*/
	virtual NamedAddrPort GetServerAddr() override;

	/**
	\~korean
	이 Farm 클라이언트(즉 게임 서버)의 critical section을 얻습니다. CFarmClient는 자체적으로 critical section을 가지지 않고
	이 함수가 주는 객체를 사용해서 잠금을 합니다.

	\~english
	Gets critical selection of this farm client (game server). CFarmClient는 does not own critical section itself and lock by object that receive from this function.

	\~chinese
	获取该Farm客户端(即游戏服务器)的critical section。CFarmClient自身没有critical section，
	利用从该函数收到的对象进行锁定。

	\~japanese
	このFarmクライアント（つまりゲームサーバー）のcritical sectionを取得します。 CFarmClientは自主的にcritical sectionを持たず
	この関数から与えられたオブジェクトを使ってロックします。

	\~
	*/
	virtual CriticalSection* GetCritSec() override { return &m_cs; };

	virtual void OnTick(void* context) override;
	virtual void FrameMove() override;

	virtual bool RequestCreateCredential(const HostID remote, const Guid &gamerUUID, const HostID &UserHostID) override;
	virtual bool RequestCheckCredential(Proud::HostID remote, const Proud::Guid &credential,const HostID &userHostID,const String &ServerName) override;
	virtual bool UserLogOut(Proud::HostID remote, const Guid &gamerUUID) override;

};
