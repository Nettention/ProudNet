#pragma once
#include "..\GCCommon\Vars.h"


class CBattleClient;

/**
\~korean
게임 월드를 구성하는 오브젝트 통칭 
객체 지향 프로그래밍을 일부러 하지 않고 단일 클래스에서 모든 타입의 게임 캐릭터 정보를 담습니다.
김성익(noerror)의 super class 철학에 기반합니다.

\~english
Common name of the object which forms a game world
Information of all types of game characters is contained in a single class, not doing the object-oriented programming on purpose, 
which is based on the philosophy of a so-called ‘super class’ of Kim Sung-Ik(noerror).

\~chinese
构成游戏世界的对象总称
刻意不用面向对象的程序设计,在单一类中加载所有类型的游戏人物信息.
这基于김성익(noerror)的super class哲学。

\~japanese
ゲームワールドを構成するオブジェクトの通称
あえてオブジェクト指向プログラミングを使わず、単一のクラス内にすべてのタイプのゲームキャラクターの情報を入れます。
ギムソンイク（noerror）のsuper class哲学に基づいています。

\~
*/
class CGameObject_C
{
public:
	CBattleClient* m_owner;

	GameObjectType m_type;

	/**
	\~korean
	캐릭터의 위치. CPositionFollower이므로, 네트워크로 받은 실제 위치와 화면에 그리기 위해 보정되는 위치를 모두 갖습니다.

	\~english
	Location of the character  Being C Position Follower, it contains both an actual location received from the network and location which is revised to draw on the screen.

	\~chinese
	人物位置，CPositionFollower，因此包含从网络收到的实际位置和画面中矫正的位置。

	\~japanese
	キャラクターの位置。 CPositionFollowerなのでネットワークから受け取った実際の位置と画面に描画するため補正される位置をすべて持ちます。

	\~
	*/
	CPositionFollower m_positionFollower;

	/**
	\~korean
	캐릭터의 바라보는 방향. 네트워크로 받은 실제 위치와 화면에 그리기 위해 보정되는 위치를 모두 갖습니다.

	\~english
	Direction that the character is looking at It contains both an actual location received from the network and location which is revised to draw on the screen.

	\~chinese
	人物观望方向，包含从网络收到的实际位置和画面中矫正的位置。

	\~japanese
	キャラクターが見ている方向。ネットワークで受け取った実際の位置と画面に描画するため補正される位置をすべて持っています。

	\~
	*/
	CAngleFollower m_yawFollower;

	/**
	\~korean
	캐릭터 이름

	\~english
	Name of the character

	\~chinese
	人物名称

	\~japanese
	キャラクターの名前

	\~
	*/
	CStringW m_name;

	/**
	\~korean
	캐릭터를 조종하는 클라이언트의 HostID

	\~english
	Host ID of the client which controls the character

	\~chinese
	操纵人物的客户端HostID

	\~japanese
	キャラクターを操るクライアントのHostID
	\~
	*/
	HostID m_ownerHostID;

	/**
	\~korean
	가장 마지막으로 받은 캐릭터의 위치 정보가, direct P2P 인지 Relay P2P인지 확인합니다.

	\~english
	Verify if the location of the character received at the very last time is direct P2P or Relay P2P.

	\~chinese
	确认最近一次收到的人物位置信息时direct P2P还是Relay P2P。

	\~japanese
	一番最後に受け取ったキャラクターの位置情報が、direct P2PかRelay P2Pかを確認します。

	\~
	*/
	bool m_synchedThruDirectP2P;

	/**
	\~korean
	m_synchedThruDirectP2P 상태를 화면에 표시하되, 일정 시간이 지나면 표시하지 않게 하기 위함

	\~english
	It displays the status of m_synchedThruDirectP2P on the screen, however aims not to display it after a certain amount of time.

	\~chinese
	在画面中显示m_synchedThruDirectP2P的状态，但一定时间后自动消失。

	\~japanese
	m_synchedThruDirectP2P状態を画面に表示するが、一定時間が経過すると表示しないようにするためである。

	\~
	*/
	int64_t m_synchedThruDirectP2PChangeShowCoolTimeMs;

	/**
	\~korean
	캐릭터의 m_synchedThruDirectP2P 상태가 바뀔 때마다 화면에 보여주는 한계 시간

	\~english
	Limited time which is displayed on the screen whenever the status of m_synchedThruDirectP2P for the character changes.

	\~chinese
	每次人物m_synchedThruDirectP2P变更时在画面的显示时间

	\~japanese
	キャラクターのm_synchedThruDirectP2P状態が変わるたびに画面に表示される限界時間

	\~
	*/
	static const int64_t SynchedThruDirectP2PChangeShowInterval;

	/**
	\~korean
	위치

	\~english
	Location

	\~chinese
	位置

	\~japanese
	位置

	\~
	*/
	D3DXVECTOR3 m_position;

	/**
	\~korean
	캐릭터가 바라보는 방향(좌우)

	\~english
	Direction that the character is looking at (left and right)

	\~chinese
	人物观望的方向(左右)

	\~japanese
	キャラクターが見ている方向（左右）

	\~
	*/
	float m_yaw;

	/**
	\~korean
	캐릭터가 바라보는 방향(위아래)

	\~english
	Direction that the character is looking at (up and down)

	\~chinese
	人物观望的方向(上下)

	\~japanese
	キャラクターが見ている方向（上下）

	\~
	*/
	float m_pitch;

	/**
	\~korean
	속력(스칼라)

	\~english
	Speed (scala)

	\~chinese
	速度(标量)

	\~japanese
	速度（スカラー）

	\~
	*/ 
	float m_speed;

	/**
	\~korean
	최대 낼 수 있는 속력

	\~english
	Max. speed

	\~chinese
	最高速度

	\~japanese
	最大に出せる速度

	\~
	*/
	float m_maxSpeed;

	/**
	\~korean
	선회속도(라디안)

	\~english
	Turning speed (radian)

	\~chinese
	旋转速度(弧度)

	\~japanese
	旋回速度（ラジアン）

	\~
	*/
	float m_turnSpeed;

	/**
	\~korean
	가속시 가속도

	\~english
	Acceleration when gaining speed

	\~chinese
	加速时的加速度

	\~japanese
	加速時の加速度

	\~
	*/
	float m_accelSpeed;

	/**
	\~korean
	감속시 가속도

	\~english
	Acceleration when slowing down

	\~chinese
	减速时的加速度

	\~japanese
	減速時の加速度

	\~
	*/
	float m_brakeSpeed;

	LONGLONG m_score;

	/**
	\~korean
	이 로컬 캐릭을 가시하는 타 클라이언트들을 묶은 P2P group ID

	\~english
	P2P group ID that collects other clients which can watch this local character

	\~chinese
	可看到该本地人物的绑定其他客户端P2P group ID

	\~japanese
	このローカルキャラクターを可視する他のクライアントを結んだP2P group ID

	\~
	*/
	HostID m_viewersGroupID;

	/**
	\~korean
	이 로컬 캐릭의 위치를 BroadCast하기까지 남은 시간

	\~english
	Remaining time until broadcasting the location of this local character

	\~chinese
	该本地人物位置的BroadCast所剩时间

	\~japanese
	このローカルキャラクターの位置をBroadCastするまでの残り時間

	\~
	*/
	int64_t m_broadcastStateCoolTimeMs;

	void Accel(float elapsedTime);
	void Brake(float elapsedTime);
	void FrameMove(float elapsedTime);

	void FrameMove_RemoteHero( float fElapsedTime );

	void FrameMove_LocalHero(float elapsedTime);

	void Render();
	void Render_LocalHero();
	void Render_RemoteHero();

	CGameObject_C(void);
	~CGameObject_C(void);

	/**
	\~korean
	객체를 파괴하기 전에 콜 해야 하는 함수입니다.
	m_gameObjects 외의 인덱스에서 지우는 역할도 합니다.

	\~english
	It is a function which needs to be called before destroying an object. 
	It also plays a role which erases at index other than m_gameObjects

	\~chinese
	破坏对象之前应调用的函数。
	还可以在m_gameObjects以外的Index中删除。

	\~japanese
	オブジェクトを破壊する前にコールする必要がある関数です。
	m_gameObjectsの他にインデックスで消す役割も果たします。

	\~
	*/
	void Dispose();
	bool IsLocalHero();
};

//typedef RefCount<CGameObject_C> CGameObjectPtr_C;
