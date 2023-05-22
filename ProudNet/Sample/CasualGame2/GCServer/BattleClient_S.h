#pragma once

#include "../gccommon/vars.h"
#include "SynchEntity.h"

class CBattleClient_S;
class CGameRoom_S;
class CBattleServer;


/**
\~korean
스마트 포인터 선언

\~english
Declare smart pointer

\~chinese
智能指针说明

\~japanese
スマートポインタの宣言

\~
*/
typedef RefCount<CBattleClient_S> CBattleClientPtr_S;
typedef RefCount<CGameRoom_S> CGameRoomPtr_S;


/**
\~korean
클라이언트가 조종하고 있는 캐릭터 클래스입니다.

\~english
Chracter which client is controlling

\~chinese
客户端操纵的人物类。

\~japanese
クライアントが操るキャラクターのクラスです。

\~
*/
class CGameObject_S:public CSynchEntity
{
public:
	GameObjectType m_type;

	/**
	\~korean
	캐릭터의 이름

	\~english
	Name of character

	\~chinese
	人物名称

	\~japanese
	キャラクターの名前

	\~
	*/
	CStringW m_name;

	/**
	\~korean
	캐릭터의 위치

	\~english
	Position of character

	\~chinese
	人物位置

	\~japanese
	キャラクターの位置
	\~
	*/
	D3DXVECTOR3 m_position;

	/**
	\~korean
	속도

	\~english
	Speed

	\~chinese
	速度

	\~japanese
	速度

	\~
	*/
	D3DXVECTOR3 m_velocity;

	/**
	\~korean
	바라보고 있는 방향. 캐릭터가 게걸음(strafe)도 가능하므로 별도로 있어야합니다.

	\~english
	Direction that the character is looking at It becomes additionally necessary as the character is available in strafing.

	\~chinese
	正在观望的方向，人物还可以横步(strafe)，因此也应具备。

	\~japanese
	見ている方向。キャラクターが横ばい（strafe）することも可能なので別途に必要です。

	\~
	*/
	float m_yaw;
	CGameObject_S();
};

/**
\~korean
배틀 서버에 접속한 각 배틀 클라이언트 객체입니다.

\~english
Each battle client object which joined in battle server

\~chinese
连接作战服务器的每个作战客户端对象。

\~japanese
バトルサーバに接続した各バトルクライアントのオブジェクトです。

\~
*/
class CBattleClient_S
{
public:
	/**
	\~korean
	클라이언트의 네트워크 ID

	\~english
	Network ID of client

	\~chinese
	客户端的网络ID

	\~japanese
	クライアントのネットワークID

	\~
	*/
	HostID m_HostID;

	/**
	\~korean
	로그온한 클라이언트의 게이머 정보를 DB cache server로부터 가져온 것입니다.
	클라이언트가 로그온이 성공하면 이 변수는 채워집니다.

	\~english
	Gamer information of loggedon client which took from DB cache server
	Once client has succeed to logon then this variable fills up.

	\~chinese
	从DB cache server的登录客户端玩家信息。
	客户端成功登录，则该变量被填充。
	
	\~japanese
	ログオンしたクライアントのゲーマー情報をDB cache serverから取得したものです。
	クライアントがログオンに成功するとこの変数は埋まります。

	\~
	*/
	CLoadedData2Ptr m_dbLoadedGamerData;

	/**
	\~korean
	로그온한 클라이언트의, 클라이언트들끼리 공유 가능한 데이터들

	\~english
	Sharable data between clients who loggedon

	\~chinese
	登录客户端之间可共享的数据

	\~japanese
	ログオンしたクライアントとクライアントの間で共有可能なデータ

	\~
	*/
	CHeroPublishInfo m_info;

	/**
	\~korean
	방 생성 요청을 가장 마지막에 한 시간

	\~english
	Lastest room creation request time

	\~chinese
	最有一次申请创建房间的时间

	\~japanese
	部屋作りの要求を一番最後にした時間

	\~
	*/
	int64_t m_lastRequestTimeOfNewGameRoomMs;

	/**
	\~korean
	배틀 클라이언트에서 로그온 요청을 할 때, 어떤 게임방에 들어갈 것인지를 요청받습니다.
	그 값이 여기에 저장됩니다. 이는 게이머 정보 로딩 결과를 DB cache server로부터 받을 때 사용됩니다.

	\~english
	When it send logon request from battle client, it gets request about join to which game room.
	That value save to here. It uses receiving result of gamer information loadong from DB cache server.

	\~chinese
	作战客户端请求登录时，接收目标游戏房间。
	该值保存到这里。用于从DB cache server接收玩家信息加载结果。

	\~japanese
	バトルクライアントからログオン要求をする時、どのゲームルームに入るかを求められます。
	その値はここに保存されます。これはゲーマー情報の読込み結果をDB cache serverから受け取った時に使われます。

	\~
	*/
	Guid m_joinIntendedGameRoomGuid;

	/**
	\~korean
	이 클라이언트가 들어가 있는 게임방 객체

	\~english
	Game room object which this client has joined in

	\~chinese
	该客户端加入的游戏房间对象

	\~japanese
	このクライアントが入っているゲームルームのオブジェクト

	\~
	*/
	CGameRoomPtr_S m_joinedGameRoom;

	CBattleServer* m_owner;

	/**
	\~korean
	소유하고 있는 캐릭터의 가시 가능한 클라이언트들 수집하는 처리를 하는 데 까지 남은 시간

	\~english
	It is time left to finish collecting visible client of owned character.

	\~chinese
	自有人物可视客户端收集完成所剩余时间

	\~japanese
	所有しているキャラクターが可視可能なクライアントを収集し、処理までの残り時間

	\~
	*/
	int64_t m_updateHeroViewerCoolTimeMs;

	/**
	\~korean
	소유하고 있는 캐릭터. weak pointer입니다.

	\~english
	Owned character information. weak pointer

	\~chinese
	自有人物的weak pointer。

	\~japanese.
	所有しているキャラクター。 weak pointerです。

	\~
	*/
	CGameObject_S* m_hero;

	CFastSet<CSynchEntity*> m_tangibles;

	CBattleClient_S(CBattleServer* owner);

	/**
	\~korean
	이 클라이언트가 로그온하지 않았으면 null을, 로그온했으면 게이머가 사용중인 플레이어 캐릭터의 guid를 리턴합니다.

	\~english
	If this client is not loggedon then return "null" otherwise return guid of player character which currently using.

	\~chinese
	如该客户端未登录则返回null，如果为登录状态则返回玩家正在适用的玩家人物guid。

	\~japanese
	このクライアントがログオンしていない場合はnullを、ログオンした場合はゲーマーが使っているプレイヤーキャラクターのguidを返します。

	\~
	*/
	Guid GetSelectedHeroGuid();
	String GetSelectedHeroName();
	CPropNodePtr GetSelectedHero();

	/**
	\~korean
	이 클라이언트가 로그온하지 않았으면 null을, 로그온했으면 게이머의 guid를 리턴합니다.

	\~english
	If this client is not loggedon then return "null" otherwise return guid of gamer.

	\~chinese
	如该客户端未登录则返回null，如果为登录状态则返回玩家guid。

	\~japanese
	このクライアントがログオンしていない場合はnullを、ログオンした場合はゲーマーのguidを返します。

	\~
	*/
	Guid GetGamerGuid();

	void FrameMove();

	void FrameMove_DoSynchFilter();

};

/**
\~korean
각 클라이언트의 HostID를 키로 두는 클라이언트 맵

\~english
Client map which has HostID of each client as key

\~chinese
将各客户端的HostID作为Key的客户端映射

\~japanese
各クライアントのHostIDをキーとしておくクライアントマップ

\~
*/
typedef CFastMap<HostID,CBattleClientPtr_S> RemoteBattleClients;


class CJoinAllowedGamer
{
public:
	int64_t m_addedTimeMs;
	CJoinGameRoomParameter m_joinParameter;

	CJoinAllowedGamer()
	{
		m_addedTimeMs = 0;
	}
};

typedef RefCount<CJoinAllowedGamer> CJoinAllowedGamerPtr;

/**
\~korean
배틀 서버에 존재하는 1개의 게임방 클래스입니다.

\~english
1 game room that exists at battle server

\~chinese
作战服务器上存在的一个游戏房间类。

\~japanese
バトルサーバに存在する1つのゲームルームのクラスです。

\~
*/
class CGameRoom_S
{
public:
	/**
	\~korean
	여기에 전반적인 게임방 정보가 들어가 있습니다.

	\~english
	It contains general information of game room.

	\~chinese
	含有游戏房间的一般信息。

	\~japanese
	ここで全体的なゲームルーム情報が入っています。

	\~
	*/
	CGameRoomParameter m_info;

	/**
	\~korean
	loading mode를 시작한 시간. 즉, 방장이 게임 시작을 Confirm한 시간

	\~english
	Starting time of loading mode. Time that room master confirms game start.

	\~chinese
	Loading mode的开始时间，即房主确认开始游戏的时间

	\~japanese
	loading modeを開始した時間。つまり親がゲームの開始をConfirmした時間。

	\~
	*/
	int64_t m_loadingModeStartedTimeMs;

	/**
	\~korean
	이 시간을 넘게 되면 아직 로딩이 덜 끝난 게이머는 추방되고 게임 플레이가 강제로 시작됩니다.
	따라서 충분한 값을 잡아야 합니다.

	\~english
	If it overs this time then kicks out gamer who still loading and then starts game.
	Therefore you should set enough value.

	\~chinese
	超过这个时间则尚未加载成功的玩家会被驱逐。
	因此应设置足够的时间。
	
	\~japanese
	この時間を超えるとまだロードが終わっていないゲーマーは追放され、ゲームプレイが強制的に開始されます。
	したがって十分な値を取ってください。

	\~
	*/
	int64_t m_clientPlayLoadingTimeoutMs;

	/**
	\~korean
	게임방에 들어있는 모든 게이머들을 묶은 P2P 그룹

	\~english
	P2P group that binded all gamers in game room

	\~chinese
	绑定游戏房间内所有玩家的P2P组

	\~japanese
	ゲームセンターに入っているすべてのゲーマーを結ぶP2Pグループ

	\~
	*/
	HostID m_p2pGroupID;

	/**
	\~korean
	이 게임방에 진입함이 허락된 플레이어 캐릭터의 ID들.
	방장 또는 로비 서버에 의해 진입이 허락된 것들만 들어가며, 일단 진입이 성공하면 이 명단에서 제거해버립니다.
	Key=Hero GUID, Value = 추가된 시간

	\~english
	IDs of player character who have permission of entering this game room.
	Only enter who have permission by lobby server or room master then remove it once succeed entery.
	Key=Hero GUID, Value = Added time

	\~chinese
	允许进入该游戏房间的玩家人物ID
	只有房主或lobby服务器允许的玩家才能进入，成功进入后从该名单中删除。
	Key=Hero GUID, Value = 增加的时间

	\~japanese
	このゲームルームの入場が許されたプレイヤーキャラクターのID。
	親又はロビーサーバによって入場が許されたものだけが入り、いったん入場が成功するとこのリストから削除してしまいます。
	Key= Hero GUID、Value=追加された時間

	\~
	*/
	typedef CFastMap<Guid,CJoinAllowedGamerPtr> JoinAllowedHeroGuidToObjMap;
	JoinAllowedHeroGuidToObjMap m_joinAllowedHeroGuidToObjMap;

	/**
	\~korean
	이 게임방에 이미 들어와 있는 게임 클라이언트 리스트

	\~english
	Game client list who alreay joined in this game room

	\~chinese
	已进入该游戏房间的游戏客户端列表

	\~japanese
	このゲームルームの中に既に入場したゲームクライアントのリスト

	\~
	*/
	typedef CFastMap<HostID,CBattleClient_S*> JoinedGamers;
	JoinedGamers m_joinedGamers;

	CFastSet<CGameObject_S*> m_gameObjects;

	CGameRoom_S();
	~CGameRoom_S();

	/**
	\~korean
	gamerGuid가 가리키는 게이머를 제외한 나머지 room member 중에서 방장을 선발합니다.

	\~english
	Select room master from room member except gamer who pointed by gamerGuid.

	\~chinese
	除gamerGuild所指玩家之外在剩余的room member中选拔房主。

	\~japanese
	gamerGuidが指すゲーマーを除いた残りのroom memberの中で親を選びます。

	\~
	*/
	void ChangeMasterGamerOtherThan(Guid gamerGuid);

	/**
	\~korean
	게임방 내의 모든 room member들의 READY 상태인지 검사합니다.

	\~english
	Are all room members clicked ready in game room?

	\~chinese
	检查游戏房间内的room member是否全部为READY状态。

	\~japanese
	ゲームルーム内のすべてのroom memberのREADY状態を確認します。

	\~
	*/
	bool IsEveryHeroPlayReady();

	/**
	\~korean
	모든 게이머가 플레이를 위한 데이터 로딩 과정이 끝났는지 검사합니다.

	\~english
	Has all gamers finished data loading process?

	\~chinese
	检查所有玩家是否已完成数据加载。

	\~japanese
	すべてのゲーマーがプレイのためのデータの読込み処理が終了しているかを確認します

	\~
	*/
	bool IsEveryGamerLoadFinished();
};

/**
\~korean
각 게임방은 guid를 갖습니다. 게임방 guid를 키로 두는 게임방 맵

\~english
Each game room has guid. Game room map has game room guid as a key.

\~chinese
每个游戏房间都有guid。以游戏房间guid作为key的游戏房间映射

\~japanese
各ゲームルームはguidを持ちます。ゲームルームguidをキーとしておくゲームルームマップ

\~
*/
typedef CFastMap<Guid,CGameRoomPtr_S> BattleGameRooms;