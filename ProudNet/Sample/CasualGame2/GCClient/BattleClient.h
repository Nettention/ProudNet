#pragma once

#include "../GCCommon/Vars.h"
#include "../GCCommon/Battle_proxy.h"
#include "../GCCommon/Battle_stub.h"
#include "clientbase.h"
#include "GameObject_C.h"
#include "Bullet_C.h"
#include "Item_C.h"




class CCasualClientDlg;


/**
\~korean
플레이 모드 혹은 대기실 모드에서 보여지는 자신과 타 플레이어 캐릭터의 정보 항목입니다.

\~english
Information list of other player character and itself which shows on play mode or waiting room mode.

\~chinese
游戏模式或等候室模式中显示的玩家自己和其他玩家人物信息。

\~japanese
プレイモード又は控室モードで表示される自分と他プレイヤーのキャラクターの情報項目です。

\~
*/
class CBattleHeroSlot
{
public:

	CHeroPublishInfo m_info;

	/**
	\~korean
	P2P 멤버로서 클라이언트의 Host ID

	\~english
	Host ID client as P2P member

	\~chinese
	作为P2P成员的客户端主机 ID

	\~japanese
	P2PメンバーとしてクライアントのHost ID

	\~
	*/
	HostID m_HostID;

	CBattleHeroSlot()
	{
		m_HostID = HostID_None;
	}
};

/**
\~korean
위 클래스의 스마트 포인터 선언

\~english
Declare smart pointer of above class

\~chinese
宣布上一级智能指针

\~japanese
上記クラスのスマートポインタの宣言

\~
*/
typedef RefCount<CBattleHeroSlot> CBattleHeroSlotPtr;
typedef CFastMap<Guid,CBattleHeroSlotPtr> BattleHeroSlots;


/**
\~korean
1개의 배틀 클라이언트 객체입니다.
배틀 클라이언트는 게임 시작 준비를 진행중인 대기실 및 게임 플레이가 진행중인 방 역할을 합니다.

\~english

\~chinese
1个作战客户端客体。
作战客户端起到准备游戏的等候室或正在玩游戏的玩家房间作用。

\~japanese
1つのバトルクライアントのオブジェクトです。
バトルクライアントはゲーム開始が準備中である控室及びゲームプレイ中である部屋の役割をします。

\~
*/
class CBattleClient: public CClientBase, public INetClientEvent, public BattleS2C::Stub, public BattleC2C::Stub	//public BattleC2C::Stub는 나중에 클라이언트들과 p2p로 묶을것...
{
public:
	/**
	\~korean
	서버로 보내는 RMI의 Proxy

	\~english
	RMI proxy that send to server

	\~chinese
	向服务器发送的RMI的Proxy

	\~japanese
	サーバに送るRMIのProxy

	\~
	*/
	BattleC2S::Proxy m_c2sProxy;

	/**
	\~korean
	P2P로 보내는 RMI의 Proxy

	\~english

	\~chinese
	向P2P发送的RMI的Proxy

	\~japanese
	P2Pに送るRMIのProxy

	\~
	*/
	BattleC2C::Proxy m_c2cProxy;

	/**
	\~korean
	게임방에 플레이중인 모든 플레이어 캐릭터의 리스트
	가시 영역 필터링을 하지 않습니다.

	\~english
	All player character list who play game in game room
	Do not visible area filtering.

	\~chinese
	游戏房间正在玩游戏的所有玩家人物列表
	不做可视区域过滤。
	
	\~japanese
	ゲームルームでプレイ中のすべてのプレイヤーキャラクターのリスト
	可視領域のフィルタリングは行いません。

	\~
	*/
	BattleHeroSlots m_heroSlots;

	/**
	\~korean
	이 클라이언트가 조종하는 캐릭터 객체
	클라이언트가 서버 연결이 성공한 후에야 이 객체가 만들어집니다.

	\~english
	Chracter object that control this client
	This object created once client succeed to connect with server.

	\~chinese
	该客户端操纵的人物对象
	客户端成功连接服务器后才能生成该对象。

	\~japanese
	このクライアントが操るキャラクターオブジェクト
	クライアントがサーバへの接続に成功した後、このオブジェクトが作成されます。
	
	\~
	*/
	CGameObject_C* m_localHero; // 이 로컬 클라이언트가 조종하는 캐릭터

	/**
	\~korean
	이 클라이언트에서 동기화되고 있는 다른 클라이언트의 플레이어 캐릭터들

	\~english

	\~chinese
	该客户端中同步的其他客户端的玩家人物

	\~japanese
	このクライアントで同期化されている他クライアントのプレイヤーキャラクター達

	\~
	*/
	typedef CFastMap<HostID,CGameObject_C*> RemoteHeroes;
	RemoteHeroes m_remoteHeroes;


	/**
	\~korean
	이 클라이언트에서 동기화되는 다른 클라이언트의 캐릭터

	\~english
	Character of other client which synchronize at this client

	\~chinese
	在此客户端中同步进行的其它客户端的字符

	\~japanese
	このクライアントで同期される他クライアントのキャラクター

	\~
	*/
	typedef CFastSet<CGameObject_C*> GameObjects;
	GameObjects m_gameObjects;

	/**
	\~korean
	이 클라이언트가 들어가 있는 게임방의 정보

	\~english
	Game room information where this client joined in

	\~chinese
	该客户端所在游戏房间信息

	\~japanese
	このクライアントが入っているゲームルームの情報

	\~
	*/
	CGameRoomParameter m_roomInfo;

	/**
	\~korean
	이 클라이언트가 들어가 있는 게임방에 들어와 있는 다른 클라이언트들을 묶은 P2P 그룹

	\~english
	Binded P2P gorup with other clients who joined same game room with this client

	\~chinese
	绑定该客户端所在游戏房间的其他客户端的P2P组

	\~japanese
	このクライアントが入っているゲームルームに入場した他のクライアントを結ぶP2Pグループ

	\~
	*/
	HostID m_p2pGroupID;

	/**
	\~korean
	이 클라이언트가 방장인 경우, [플레이 모드 시작] 버튼을 누르고 나서 그 결과가 아직 안와있을 때만 true입니다.

	\~english
	If this client is room master, it is only "true" when it does not receive result after click [Start Play Mode].

	\~chinese
	如果该客户端为房主，点击[游戏模式开始]后尚未有结果时才为true。

	\~japanese
	このクライアントが親の場合、[プレイモードの開始]ボタンを押した後、その結果が出て来ない場合にのみtrueです。

	\~
	*/
	bool m_startPlayModeUnderProgress;

	CBattleClient();
	~CBattleClient(void);

	/**
	\~korean
	매 프레임마다의 처리 루틴입니다.

	\~english
	Processing each frame

	\~chinese
	每个画面的处理规则。

	\~japanese
	毎フレームごとの処理ルーチンです。

	\~
	*/
	void FrameMove(float fElapsedTime) override;

	virtual void SetCamera();

	/**
	\~korean
	서버와의 연결을 시도합니다.

	\~english
	Try to connect with server.

	\~chinese
	尝试与服务器连接。

	\~japanese
	サーバーとの接続を試みます。

	\~
	*/
	bool Connect();

	/**
	\~korean
	서버와의 연결 시도 후 연결이 성공했는지 실패했는지 결과가 나오는 Handler 입니다.

	\~english
	Handler that notice result about success or fail of connecting after connecting attempt with server

	\~chinese
	显示与服务器连接成功与否结果的Handler。

	\~japanese
	サーバーとの接続を試みた後、接続が成功か失敗かの結果が出てくるHandlerです。

	\~
	*/
	virtual void OnJoinServerComplete(ErrorInfo *info, const ByteArray &replyFromServer) override;
	virtual void OnLeaveServer(ErrorInfo *errorInfo) override;

	/**
	\~korean
	P2P 그룹이 생성되거나 그룹에 새 멤버가 추가되면 오는 Handler입니다.

	\~english
	Handler when P2P group is created or added new member at group

	\~chinese
	生成P2P组或组里新增成员时的Handler。

	\~japanese
	P2Pグループが作成されるか、又はグループに新しいメンバーが追加されるたら来るHandlerです。

	\~
	*/
	virtual void OnP2PMemberJoin(HostID memberHostID, HostID groupHostID, int memberCount, const ByteArray &customField) override;
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
	異常状態の時、ErrorFormに進むため、warningとinformationは無視します。

	\~
	*/
	virtual void OnWarning(ErrorInfo *errorInfo) override{}
	virtual void OnInformation(ErrorInfo *errorInfo) override{}
	virtual void OnException(const Proud::Exception &e) override;
	virtual void OnNoRmiProcessed(RmiID rmiID) override {}

	/**
	\~korean
	화면에 표시할 캐릭터 정보를 문자열로 표현합니다.

	\~english
	Express character information which indicate on the screen with string.

	\~chinese
	以字符串表示画面中的人物信息。

	\~japanese
	画面に表示するキャラクター情報を文字列で表現します。

	\~
	*/
	CStringW GetHeroSlotText(const CHeroPublishInfo& heroSlot);

	/**
	\~korean
	Hero guid를 입력받아, 서버로부터 동기화되고 있는 hero 정보( hero slot)을 찾습니다.

	\~english
	Gets Hero guid, then find hero information (hero slot) which is synchronizing from server.

	\~chinese
	输入Hero guid时，搜索服务器同步的hero信息(hero slot)。

	\~japanese
	Hero guidを受け取り、サーバから同期化されているhero情報（hero slot）を検索します。

	\~
	*/
	CBattleHeroSlotPtr GetHeroSlotByGuid( Guid heroGuid );
	
	/**
	\~korean
	hero slot을 찾되 입력값으로 그 hero를 조종하는 클라이언트의 HostID를 씁니다.

	\~english
	Find out hero slot but use HostID of client who control that hero.

	\~chinese
	寻找hero slot，但输入值为操纵hero的客户端HostID。

	\~japanese
	hero slotを検索しますが、入力値としてはそのheroを操るクライアントのHostIDを使います。

	\~
	*/
	CBattleHeroSlotPtr GetHeroSlotByHostID( HostID clientID);

	/**
	\~korean
	이 클라의 플레이어에 대응하는 hero slot을 찾습니다.

	\~english
	Find out hero slot that opposite player of this client.

	\~chinese
	寻找对应该客户端玩家的hero slot。

	\~japanese
	このクライアントのプレイヤーに対応するhero slotを検索します。

	\~
	*/
	CBattleHeroSlotPtr GetLocalHeroSlot();

	/**
	\~korean
	이 클라이언트의 플레이어가 방장인지 확인 합니다.

	\~english
	Is player of this client room master?

	\~chinese
	确认该客户端的玩家是否为房主？

	\~japanese
	このクライアントのプレイヤーが親であることを確認します。

	\~
	*/
	bool IsLocalHeroRoomMaster();

	/**
	\~korean
	게임방 내 모든 게이머가 READY를 찍었나 체크합니다. 방장은 무조건 찍은걸로 간주합니다.

	\~english
	Check all gamer click "READY" in game room. Room master consider to clicked.

	\~chinese
	检查游戏房间内是否所有玩家都已点击READY，房主默认为已点击。

	\~japanese
	ゲームルーム内のすべてのゲーマーが「READY」を押しているかをチェックします。親は必ず押しているとみなします。

	\~
	*/
	bool IsEveryHeroPlayReady();

	/**
	\~korean
	타 클라이언트의 HostID를 갖고 타 클라이언트의 캐릭터 객체를 찾습니다.

	\~english
	Find out chracter object of other client with HostID of other client.

	\~chinese
	使用其他客户端的HostID寻找其他客户端的人物对象。

	\~japanese
	他クライアントのHostIDを持ちながら他クライアントのキャラクターオブジェクトを検索します。

	\~
	*/
	CGameObject_C* GetRemoteHeroByClientID( HostID remoteClientID );
	void ClearWorld();
	void Render();

	/**
	\~korean
	RMI 수신 함수 선언부

	\~english
	Declaratives of RMI receiving function

	\~chinese
	RMI接受函数说明部分

	\~japanese
	RMI受信関数の宣言部
	\~
	*/
	DECRMI_BattleS2C_NotifyUnauthedAccess;
	DECRMI_BattleS2C_ShowError;
	DECRMI_BattleS2C_NotifyNextLogonFailed;
	DECRMI_BattleS2C_NotifyNextLogonSuccess;
	DECRMI_BattleS2C_HeroSlot_Appear;
	DECRMI_BattleS2C_HeroSlot_Disappear;
	DECRMI_BattleS2C_HeroSlot_ShowState;
	DECRMI_BattleS2C_NotifyGameRoomInfo;
	DECRMI_BattleC2C_P2P_Chat;
	DECRMI_BattleS2C_NotifyStartPlayModeFailed;
	DECRMI_BattleS2C_GotoLoadingMode;
	DECRMI_BattleS2C_GotoPlayMode;
	DECRMI_BattleS2C_GotoWaitingMode;

	DECRMI_BattleC2C_P2P_LocalHero_Move;
	DECRMI_BattleS2C_NotifyLocalHeroViewersGroupID;
	DECRMI_BattleS2C_RemoteHero_Appear;
	DECRMI_BattleS2C_RemoteHero_Disappear;

	DECRMI_BattleS2C_NotifyHeroScore;
	DECRMI_BattleS2C_NotyfyGotoLobby;

};

typedef RefCount<CBattleClient> CClientPtr;
