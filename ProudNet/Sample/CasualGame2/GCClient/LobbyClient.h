#pragma once

#include "../GCCommon/Vars.h"
#include "../GCCommon/Lobby_proxy.h"
#include "../GCCommon/Lobby_stub.h"
#include "clientbase.h"

class CCasualClientDlg;

/**
\~korean
로비 서버로부터 동기화받는 각 게임방 정보입니다.

\~english
Information of each game room that synchronize from lobby server

\~chinese
Lobby服务器进行同步操作的各游戏房间信息。

\~japanese
ロビーサーバから同期化される各ゲームルームの情報です。

\~
*/
class CLobbyGameRoom
{
public:
	CGameRoomParameter m_info;
};

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
typedef RefCount<CLobbyGameRoom> CLobbyGameRoomPtr;

/**
\~korean
각 게임방은 guid를 가지고 있습니다. 그것의 맵 타입 선언

\~english
Each game room has guid. Declare map type of that

\~chinese
各游戏房间都有guid，其映射类型的说明

\~japanese
各ゲームルームはguidを持っています。そのマップ型の宣言

\~
typedef Dictionary<Guid, CLobbyGameRoomPtr> LobbyGameRooms;
*/
typedef CFastMap<Guid,CLobbyGameRoomPtr> LobbyGameRooms;

/**
\~korean
로비 폼에서 보여줄 자신 + 타 유저들의 캐릭터 클래스입니다.

\~english
Character of itself + other users that shows on lobby form

\~chinese
Lobby表单中显示的本人+其他用户的人物类。

\~japanese
ロビーフォームで表示される自分+他ユーザーたちのキャラクタークラスです。

\~
*/
class CLobbyHeroSlot
{
public:
	CHeroPublishInfo m_info;
};

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
typedef RefCount<CLobbyHeroSlot> CLobbyHeroSlotPtr;

/**
\~korean
각 플레이어 캐릭터는 guid를 통해 구별합니다. 그것의 맵 타입 선언

\~english
Each player character distinguish by guid. Declare map type of that

\~chinese
每个玩家人物都用guid区分，其映射类型说明

\~japanese
各プレイヤーのキャラクターはguidを介して区別します。そのマップ型の宣言

\~
typedef Dictionary<Guid, CLobbyHeroSlotPtr> LobbyHeroSlots;
*/
typedef CFastMap<Guid,CLobbyHeroSlotPtr> LobbyHeroSlots;

/**
\~korean
로비 클라이언트입니다.
- 로비 클라이어언트는 로비 채널, 즉 공개 대화창의 역할을 합니다.
- 또한 방 만들기, 방 들어가기 등을 하는 공간입니다.

\~english
Lobby client
- Lobby client is lobby channel, so it does role of open chatting window.
- Also it is place where create room, enter room.

\~chinese
Lobby客户端。
-lobby客户端起到lobby channel，即公开对话框的作用。
-并且还是生成房间，进入房间等的空间。

\~japanese
ロビークライアントです。
- ロビークライアントはロビーチャンネル、つまり公開チャットボックスの役割をします。
- また部屋作り、部屋入りなどを行う空間です。

\~
*/
class CLobbyClient: public CClientBase, public INetClientEvent, public LobbyS2C::Stub
{
public:
	/**
	\~korean
	서버로 보내는 RMI의 Proxy입니다.

	\~english
	RMI proxy that send to server

	\~chinese
	向服务器发送的RMI的Proxy。

	\~japanese
	サーバに送るRMIのProxyです。

	\~
	*/
	LobbyC2S::Proxy m_c2sProxy;

	/**
	\~korean
	로비에서 표시되는, 로비 채널 내 모든 유저들의 플레이어 캐릭터 리스트

	\~english
	Player character list of all users in lobby channel also shows on lobby

	\~chinese
	Lobby中显示的lobby channel内所有用户的玩家人物列表

	\~japanese
	ロビーで表示されているロビーチャンネル内のすべてのユーザのプレイヤーキャラクターのリスト

	\~
	*/
	LobbyHeroSlots m_heroSlots;

	/**
	\~korean
	로비에서 표시되는 게임방 리스트

	\~english
	Game room list that shows on lobby

	\~chinese
	Lobby显示的游戏房间列表

	\~japanese
	ロビーで表示されるゲームルームのリスト

	\~
	*/
	LobbyGameRooms m_gameRooms;

	CLobbyClient();
	~CLobbyClient(void);

	/**
	\~korean
	매 프레임마다의 처리 루틴입니다.

	\~english
	Processing each frame

	\~chinese
	每个Frame的处理规则。

	\~japanese
	毎フレームごとの処理ルーチンです。

	\~
	*/
	void FrameMove(float fElapsedTime) override;
	virtual void SetCamera() {}

	/**
	\~korean
	서버와의 연결을 시도합니다.

	\~english
	Try to connect with server.

	\~chinese
	尝试与服务器连接。

	\~japanese
	サーバとの接続を試みます。

	\~
	*/
	bool Connect();

	/**
	\~korean
	게임방  guid를 입력받아 대응하는 게임방 객체를 찾습니다.

	\~english
	Find out opposite game room object by entering guid of game room.

	\~chinese
	根据输入的游戏房间guid寻找对应的游戏房间对象。

	\~japanese
	ゲームルームguidの入力を受け取り、対応するゲームルームのオブジェクトを検索します。

	\~
	*/
	CLobbyGameRoomPtr GetLobbyGameRoomByGuid( Guid roomGuid );

	/**
	\~korean
	hero guid를 입력받아 대응하는 플레이어 캐릭터 정보 항목(hero slot)를 찾습니다.

	\~english
	Find out opposited player character information item (hero slot) by entering hero guid.

	\~chinese
	根据输入的hero guid寻找对应的玩家人物信息项(hero slot)。

	\~japanese
	hero guidを受け取り、対応するプレイヤーキャラクターの情報項目（hero slot）を検索します。

	\~
	*/
	CLobbyHeroSlotPtr GetHeroSlotByGuid( Guid heroGuid );

	/**
	\~korean
	화면에 표시할 게임방 문자열을 만듭니다.

	\~english
	Creates game room string which indicate on screen.

	\~chinese
	生成页面上显示的游戏房间字符串。

	\~japanese
	画面に表示するゲームルームの文字列を作成します。

	\~
	*/
	CStringW GetGameRoomText(const CGameRoomParameter &info);

	/**
	\~korean
	서버와의 연결 시도 후 연결이 성공했는지 실패했는지 결과가 나오는 Handler입니다.

	\~english
	Handler notice result whether it has succeed to connect with server ot not

	\~chinese
	显示尝试服务器连接成功与否结果的Handler。

	\~japanese
	サーバとの接続を試みた後、接続が成功か失敗かの結果が出てくるHandlerです。

	\~
	*/
	virtual void OnJoinServerComplete(ErrorInfo *info, const ByteArray &replyFromServer) override;

	/**
	\~korean
	서버와의 접속이 종료되었을 경우 콜백 됩니다.

	\~english
	When it closed connection with server

	\~chinese
	服务器连接终止时回调。

	\~japanese
	サーバとの接続が終了した場合、コールバックされます。

	\~
	*/
	virtual void OnLeaveServer(ErrorInfo *errorInfo) override;
	virtual void OnP2PMemberJoin(HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField) override {}
	virtual void OnP2PMemberLeave(HostID memberHostID, HostID groupHostID, int memberCount) override {}
	virtual void OnChangeP2PRelayState(HostID remoteHostID, ErrorType reason) override {}
	virtual void OnSynchronizeServerTime() override {}
	virtual void OnError(ErrorInfo *errorInfo) override;

	/**
	\~korean
	이상 상태시 ErrorForm으로 넘어가기 때문에 warning과 information은 무시하도록 합니다.

	\~english
	If it is wrong, it moves to ErrorForm so ignore warning and information 

	\~chinese
	状态异常时直接转到ErrorForm，因此可忽略warning和information。

	\~japanese
	異常状態の時、ErrorFormに進むのでwarningとinformationは無視されます。

	\~
	*/
	virtual void OnWarning(ErrorInfo *errorInfo) override{}
	virtual void OnInformation(ErrorInfo *errorInfo) override{}
	virtual void OnException(const Proud::Exception &e) override;
	virtual void OnNoRmiProcessed(RmiID rmiID) override {}

	/**
	\~korean
	RMI 수신 함수 선언부입니다.

	\~english
	Declaratives of RMI receiving function

	\~chinese
	RMI接收函数说明部分。

	\~japanese
	RMI受信関数の宣言部です。

	\~
	*/
	DECRMI_LobbyS2C_NotifyUnauthedAccess;
	DECRMI_LobbyS2C_ShowError;
	DECRMI_LobbyS2C_NotifyNextLogonFailed;
	DECRMI_LobbyS2C_NotifyNextLogonSuccess;
	DECRMI_LobbyS2C_ShowChat;
	DECRMI_LobbyS2C_HeroSlot_Appear;
	DECRMI_LobbyS2C_HeroSlot_Disappear;
	DECRMI_LobbyS2C_GameRoom_Appear;
	DECRMI_LobbyS2C_GameRoom_Disappear;
	DECRMI_LobbyS2C_GameRoom_ShowState;
	DECRMI_LobbyS2C_NotifyCreateRoomSuccess;
	DECRMI_LobbyS2C_NotifyCreateRoomFailed;
	DECRMI_LobbyS2C_LocalHeroSlot_Appear;

	DECRMI_LobbyS2C_NotifyJoinRoomSuccess;
	DECRMI_LobbyS2C_NotifyJoinRoomFailed;

	void ServerErrorTest();
	void ErrorTest();
};


