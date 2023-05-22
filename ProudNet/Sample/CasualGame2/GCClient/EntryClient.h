#pragma once

#include "../GCCommon/Vars.h"
#include "../GCCommon/Entry_proxy.h"
#include "../GCCommon/Entry_stub.h"
#include "clientbase.h"

class CCasualClientDlg;

/**
\~korean
채널 선택 폼에서 보여줄 각 채널 클래스입니다.

\~english
Each channel that shows at channel selection form

\~chinese
Channel选择表单中显示的各Channel的类。

\~japanese
チャンネル選択フォームで表示される各チャンネルのクラスです。

\~
*/
class CEntryLobbySlot
{
public:
	CStringW m_name;
	NamedAddrPort m_serverAddr;
	int m_gamerCount;

	CEntryLobbySlot();
};

typedef RefCount<CEntryLobbySlot> CEntryLobbySlotPtr;

/**
\~korean
캐릭터 선택 폼에서 보여줄, 로컬 게이머가 소유하고 있는 각 캐릭터 클래스입니다.

\~english
Each character that owned by local character and shows at character selection form

\~chinese
在人物选择表单里显示的本地玩家所拥有的各种人物类。

\~japanese
キャラクター選択フォームで表示されるローカルゲーマーが所有している各キャラクターのクラスです。

\~
*/
class CEntryLocalHeroSlot
{
public:
	CStringW m_name;

	/**
	\~korean
	guid of hero

	\~english
	guid of hero

	\~chinese
	Hero的guid

	\~japanese
	guid of hero

	\~
	*/
	Guid m_guid;

	/**
	\~korean
	type of hero

	\~english
	type of hero

	\~chinese
	Hero的类别

	\~japanese
	type of hero

	\~
	*/
	int m_type;		// type of hero

	/**
	\~korean
	hero가 보유하고 있는 누적 점수

	\~english
	Accumulated score that hero had

	\~chinese
	Hero的累积分数

	\~japanese
	heroが保有している累積点数

	\~
	*/
	LONGLONG m_score;

	CEntryLocalHeroSlot();
};

/**
\~korean
위 클래스의 스마트 포인터입니다.

\~english
Smart Pointer

\~chinese
智能指针

\~japanese
スマートポインタ

\~
*/
typedef RefCount<CEntryLocalHeroSlot> CEntryLocalHeroSlotPtr;

/**
\~korean
게임을 처음 시작할 때 사용하는 클라이언트입니다.
- 로비 서버에 접속하기 직전까지의 과정, 즉 인증,캐릭터 선택&만들기&제거하기, 로비 채널 선택 등을 하는 클라이언트입니다.

\~english
Client that use for starting game at first time
- Process just before connect to lobby server, This client does authentication, select & create & remove character, select lobby channel.

\~chinese
最初玩游戏使用的客户端。
-连接lobby服务器之前的完成认证、人物选择&生成&删除、lobby channel 选择等过程的客户端。

\~japanese
ゲームを初めて起動したときに使うクライアントです。
- ロビーサーバに接続する直前までの過程、すなわち認証、キャラクターの選択、作成、削除、ロビーチャンネルの選択などを行うクライアントです。

\~
*/
class CEntryClient: public CClientBase, public INetClientEvent, public EntryS2C::Stub
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
	EntryC2S::Proxy m_c2sProxy;

	/**
	\~korean
	서버로부터 받은 플레이어 캐릭터 리스트.
	현재 클라이언트가 EntryClient에 접속하고 있는 경우에만 유효합니다.

	\~english
	Player character list that received from server.
	It is only available when current client is connected at EntryClient.

	\~chinese
	从服务器收到的玩家人物列表
	只有在当前客户端已连接EntryClient的情况下才有效。

	\~japanese
	サーバから受け取ったプレイヤーキャラクターのリスト。
	現在のクライアントがEntryClientに接続している場合にのみ有効です。

	\~
	*/
	CFastArray<CEntryLocalHeroSlotPtr> m_entryLocalHeroSlots, m_entryRemovedHeroSlots;

	/**
	\~korean
	서버로부터 받은 로비 채널 리스트
	현재 클라이언트가 EntryClient에 접속하고 있는 경우에만 유효합니다.

	\~english
	Lobby channel list that received from server
	It is only available when current client is connedted at EntryClient.

	\~chinese
	从服务器收到的lobby channel列表
	只有在当前客户端已连接EntryClient的情况下才有效。
	
	\~japanese
	サーバから受け取ったロビーチャンネルのリスト
	現在のクライアントがEntryClientに接続している場合にのみ有効です。
	
	\~
	*/
	CFastArray<CEntryLobbySlotPtr> m_entryLobbySlots;

	CEntryClient();
	~CEntryClient(void);

	/**
	\~korean
	매 프레임마다의 처리 루틴입니다.

	\~english
	Processing each frame

	\~chinese
	每个frame的处理规则。

	\~japanese
	毎フレームごとの処理ルーチンです

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
	서버와의 연결 시도 후 연결이 성공했는지 실패했는지 결과가 나오는 Handler입니다.

	\~english
	Handler notice result whether it has succeed to connect with server ot not

	\~chinese
	显示尝试连接服务器成功与否结果的Handler。

	\~japanese
	サーバとの接続を試みた後、接続が成功か失敗かの結果が出てくるHandlerです。

	\~
	*/
	virtual void OnJoinServerComplete(ErrorInfo *info, const ByteArray &replyFromServer) override;

	/**
	\~korean
	서버와의 접속이 종료된 경우입니다.

	\~english
	When it ends connection with server

	\~chinese
	与服务器的连接终止的情况。

	\~japanese
	サーバとの接続が終了した場合です。

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
	異常状態の時にErrorFormに引き渡すため、warningとinformationは無視します。

	\~
	*/
	virtual void OnWarning(ErrorInfo *errorInfo) override{}
	virtual void OnInformation(ErrorInfo *errorInfo) override{}
	virtual void OnException(const Proud::Exception &e) override;
	virtual void OnNoRmiProcessed(RmiID rmiID) override {}

	/**
	\~korean
	RMI 수신 함수 선언부

	\~english
	Declaratives of RMI receiving function

	\~chinese
	RMI接收函数说明部分

	\~japanese
	RMI受信関数の宣言部

	\~
	*/
	DECRMI_EntryS2C_NotifyUnauthedAccess;
	DECRMI_EntryS2C_ShowError;
	DECRMI_EntryS2C_NotifyFirstLogonFailed;
	DECRMI_EntryS2C_NotifyFirstLogonSuccess;
	DECRMI_EntryS2C_NotifySelectHeroFailed;
	DECRMI_EntryS2C_NotifySelectHeroSuccess;
	DECRMI_EntryS2C_HeroList_Begin;
	DECRMI_EntryS2C_HeroList_Add;
	DECRMI_EntryS2C_HeroList_End;
	DECRMI_EntryS2C_RemovedHeroList_Begin;
	DECRMI_EntryS2C_RemovedHeroList_Add;
	DECRMI_EntryS2C_RemovedHeroList_End;
	DECRMI_EntryS2C_LobbyList_Begin;
	DECRMI_EntryS2C_LobbyList_Add;
	DECRMI_EntryS2C_LobbyList_End;
	DECRMI_EntryS2C_NotifyAddHeroFailed;
	DECRMI_EntryS2C_NotifyAddHeroSuccess;
	DECRMI_EntryS2C_NotifySelectedHero;
	DECRMI_EntryS2C_NotifyRemoveHeroSuccess;
	DECRMI_EntryS2C_NotifyCreateNewGamerSuccess;
	DECRMI_EntryS2C_NotifyCreateNewGamerFailed;
	DECRMI_EntryS2C_NotifyReturnToEntryFailed;
};


