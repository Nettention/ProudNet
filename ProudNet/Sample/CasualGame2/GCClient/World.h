#pragma once

#include "DXUTForm.h"
#include "ClientBase.h"
#include "../GCCommon/vars.h"
#include "LobbyClient.h"

class CEntryClient;
class CLobbyClient;
class CBattleClient;

/**
\~korean
로컬 클라이언트 입장에서, 게임 월드의 내부 상태를 가집니다.

\~english
Own internal state of game world at local client side.

\~chinese
具有本地客户端角度的游戏世界的内部状态。

\~japanese
ローカルクライアントの立場から、ゲームワールドの内部状態を持ちます。

\~
*/
class CWorld: public CSingleton<CWorld>
{
public:
	enum FormState
	{
		ConnectForm,		// Connection request form
		SelectHeroForm,		// Character select form
		NewHeroForm,		// New character adding form
		SelectChannelForm,	// Channel select form
		LobbyChannelForm,	// Lobby channel (waiting room) form
		NewRoomForm,
		BattleWaitingForm,	// Waiting room form that created lobby
		BattleForm,			// Game playing form
		ErrorForm,			// Error form
		JoinForm,           // Logon form
	};

/*
	enum FormState
	{
		ConnectForm,		// 서버에 연결을 묻는 폼
		SelectHeroForm,		// 캐릭터 선택 폼
		NewHeroForm,		// 새 캐릭터 추가 폼
		SelectChannelForm,	// 채널 선택 폼
		LobbyChannelForm,	// 로비 채널(대기실) 폼
		NewRoomForm,
		BattleWaitingForm,	// 로비 개설된 방 대기실 폼
		BattleForm,			// 게임 플레이 중 폼
		ErrorForm,			// 에러가 나서 더 이상 진행이 불능한 상태에서 표시되는 폼
		JoinForm,           //로그인 폼
	};
*/

	/**
	\~korean
	현재 활성화된 폼

	\~english
	Currently activated form

	\~chinese
	已激活表单

	\~japanese
	現在活性化されたフォーム

	\~
	*/
	FormState m_FormState;

	/**
	\~korean
	로컬 클라이언트

	\~english
	Local client

	\~chinese
	本地客户端

	\~japanese
	ローカルクライアント

	\~
	*/
	CAutoPtr<CClientBase> m_client;

	/**
	\~korean
	서버 연결 폼에서 받은 정보

	\~english
	Information that received from server connection

	\~chinese
	服务器连接表单提供的信息

	\~japanese
	サーバ接続のフォームから受け取った情報

	\~
	*/
	CStringW m_ServerAddr, m_UserName, m_Password,m_CheckedPassword;

	/**
	\~korean
	Gamer의 Guid

	\~english
	Guid of Gamer

	\~chinese
	玩家的Guid

	\~japanese
	Gamerの Guid

	\~
	*/
	Guid m_gamerguid;

	/**
	\~korean
	서버 이동시 필요한 인증 키

	\~english
	Authentication key for moving server

	\~chinese
	服务器转移所需的认证key

	\~japanese
	サーバを移動する時に必要な認証キー

	\~
	*/
	Guid m_credential;

	/**
	\~korean
	로컬 플레이어 캐릭터의 정보 중에서, 타 게이머와도 공유되어도 되는 정보를 담습니다.

	\~english
	Contains sharable information with other gamer from information of local player character.

	\~chinese
	包含在本地玩家人物信息中可以与其他玩家共享的信息。

	\~japanese
	ローカルプレイヤーのキャラクター情報の中から、他ゲーマーとも共有していい情報が入ります。

	\~
	*/
	CHeroPublishInfo m_localHeroInfo;

	/**
	\~korean
	각 폼 객체

	\~english
	Object of each form

	\~chinese
	各表单对象

	\~japanese
	各フォームのオブジェクト

	\~
	*/
	CDXUTDialog m_connectForm,
	m_selectHeroForm,
	m_newHeroForm,
	m_recoverHeroForm,
	m_selectChannelForm,
	m_lobbyChannelForm,
	m_newRoomForm,
	m_battleWaitingForm,
	m_battleForm,
	m_errorForm,
	m_joinForm;

	/**
	\~korean
	접속할 로비 서버의 주소
	배틀 세션 모드에서도 이 값은 유지되어야 하므로 CWorld의 멤버로 존재합니다.

	\~english
	Address of connecting lobby server
	This value exist as member of CWorld at battle session mode because this value has maintain.

	\~chinese
	需要连接的lobby服务器地址
	作战会话模式中应维持该值，因此为CWorld的成员。

	\~japanese
	接続するロビーサーバのアドレス
	バトルセッションモードでもこの値は維持されるべきなのでCWorldのメンバーとして存在します。

	\~
	*/
	NamedAddrPort m_lobbyServerAddr;
	

	/**
	\~korean
	배틀 게임방에서 표시되는, 게임방 내 모든 유저들의 플레이어 캐릭터 리스트

	\~english
	Player character list of all users in game room that indicates at battle game room

	\~chinese
	战役游戏房间中显示的游戏房间内所有用户的玩家人物列表
	
	\~japanese
	バトルゲームルームで表示される、ゲームルーム内の全ユーザのプレイヤーキャラクターのリスト
	
	\~
	*/
	LobbyHeroSlots m_battleHeroSlots;

	/**
	\~korean
	뷰포트 카메라

	\~english
	Viewport camera

	\~chinese
	视图摄像机

	\~japanese
	ビューポートカメラ

	\~
	*/
	CPositionFollower		m_CameraPosition;
	
	/**
	\~korean
	로비 서버로부터 받은 게임방 정보

	\~english
	Game room information that receives from lobby server

	\~chinese
	从lobby服务器收到的游戏房间信息

	\~japanese
	ロビーサーバから受け取ったゲームルームの情報

	\~
	*/
	CGameRoomParameter m_roomToJoin;
public:
	
	/**
	\~korean
	Entry 서버와 connect되어있는지에 대한 여부
	
	\~english
	Whether it connect with Entry server or not

	\~chinese
	Entry服务器连接与否

	\~japanese
	Entryサーバとconnectされているかの可否

	\~
	*/
	bool m_isConnectEntryServer;

	/**
	\~korean
	FirstLogon시 ConnectForm에서 온건지 로비서버에서 엔트리서버로 이동한건지 판별하기위함입니다.

	\~english
	Distinct that move entry server from lobby server or ConnectForm when it does FirstLogon.

	\~chinese
	判断FirstLogon时是否从ConnectFrom还是Lobby服务器转移至entry服务器。

	\~japanese
	FirstLogon時にConnectFormからか、ロビーサーバからエントリサーバーに移動したのかを判別するためで

	\~
	*/
	bool m_isComebackFromLobby;

	CWorld(void);
	~CWorld(void);

public:
	/**
	\~korean
	UI 메시지 Handler입니다.

	\~english
	UI message handler

	\~chinese
	UI消息Handler。

	\~japanese
	UIメッセージのHandlerです。

	\~
	*/
	bool FormMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	/**
	\~korean
	1회 프레임 처리입니다.

	\~english
	Once frame process

	\~chinese
	一次frame处理。
	
	\~japanese
	1回のフレーム処理です。

	\~
	*/
	void FrameMove(float fElapsedTime);

	/**
	\~korean
	현재 활성화된 폼을 렌더링 합니다.
	CWorld.Render와 같이 하지 않는 이유는, 폼과 3D 월드는 렌더 순서를 명확히 나눠야 하기 때문입니다.

	\~english
	Render current activated form.
	Reason that why do not like CWorld.Render because render order of form and 3D world has to separate clearly.

	\~chinese
	提供目前已激活的表单。
	之所以与CWorld.Render不同，是因为表单和3D World的提交顺序应明确区分。

	\~japanese
	現在活性化されているフォームをレンダリングします。
	CWorld.Renderのようにしない理由として、フォームと3Dワールドはレンダリング順を明確に分けなければならないからです。

	\~
	*/
	void RenderForm(float elapsedTime);

	void GotoConnectForm(); 
	void GotoErrorForm(LPCWSTR text);
	CStringW LoadString(int stringResourceNum);
	void GotoSelectHeroForm();
	void GotoNewHeroForm();
	void GotoSelectChannelForm();
	void GotoLobbyChannelForm(bool withDisconnectingOldServer);
	void GotoNewRoomForm();
	void GotoBattleWaitingForm();
	void GotoBattleForm();
	void GotoJoinForm();

	CEntryClient* GetEntryClient();
	CLobbyClient* GetLobbyClient();
	CBattleClient* GetBattleClient();

	void Render();

	/**
	\~korean
	카메라 위치를 이동시킵니다.

	\~english
	Moves camera position.

	\~chinese
	变化摄像机位置。

	\~japanese
	カメラの位置を移動させます。

	\~
	*/
	void MoveCamera(float fElapsedTime);
	void AdjustDesiredClientCountOnNeed(float fElapsedTime);
	void SetCamera();
	
	
	/**
	\~korean
	추가 클라이언트들을 생성할 때, 한꺼번에 생성하면 과부하가 걸리므로, 천천히 클라이언트를 증가시키며 서버에 접속시키거나
	서버로부터 접속 해지를 시키기 위해서 쓰이는 변수

	\~english
	It is variable when you create additional client, add clients slowly or disconnect from server because it overloads if you create clients at once

	\~chinese
	生成新的客户端时，如果一次一起生成会发生超负荷，通过不该变量可缓慢增加客户端连接服务器
	或解除服务器连接
	
	\~japanese
	追加クライアントを作成する際、すべてを作成すると過負荷がかかるため、ゆっくりとクライアントを増加させ、サーバに接続させたり、
	サーバから接続解除をさせるために使われる変数
	
	\~
	*/
	int64_t m_adjustDesiredClientCountCoolTimeMs;

};

