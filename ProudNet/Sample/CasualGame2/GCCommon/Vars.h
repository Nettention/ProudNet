#pragma once

#include "../../../include/d3dxmarshal.inl"


enum CasualGameErrorType
{
	CasualGameErrorType_Ok,
	CasualGameErrorType_InvalidCredential,
	CasualGameErrorType_InvalidHeroName,
};

/**
\~korean
 게임 클라이언트와 서버간 공유하는 데이터입니다.

\~english
 Shared data between game client and server

\~chinese
\~japanese
\~
*/
class CSettings: public CSingleton<CSettings>
{
	/**
	\~korean
	 entry 서버에 접속하기 위한 프로토콜 버전입니다.

	\~english
	 Protocol version to connect at entry server

	\~chinese
	\~japanese
	\~
	*/
	Guid m_EntryVersion;

	/**
	\~korean
	 entry 서버의 포트 번호입니다.

	\~english
	 Port number of entry server

	\~chinese
	\~japanese
	\~
	*/
	int m_EntryServerPort;
public:

	CSettings();

	Guid GetEntryVersion();
	int GetEntryServerPort();

	Guid GetLobbyVersion();
	int GetLobbyServerPort();

	Guid GetBattleVersion();
	int GetBattleServerPort();
};

/**
\~korean
 게임 서버간끼리만 공유하는 데이터입니다.

\~english
 Shared data between only game servers

\~chinese
\~japanese
\~
*/
class CFarmSettings: public CSingleton<CFarmSettings>
{
	/**
	\~korean
	 게임 서버들과 Farm 서버간의 프로토콜 버전입니다.

	\~english
	 Protocol version between game servers and farm server

	\~chinese
	\~japanese
	\~
	*/
	Guid m_FarmVersion;

	/**
	\~korean
	 게임 서버들이 Farm 서버에 접속하기 위한 포트 번호입니다.

	\~english
	 Port number for connecting game server to farm server

	\~chinese
	\~japanese
	\~
	*/
	int m_FarmServerPort;
public:
	/**
	\~korean
	 Farm 서버와 클라이언트가 공유하는 Farm 이름입니다.

	\~english
	 Shared farm name between farm server and client

	\~chinese
	\~japanese
	\~
	*/
	CStringW m_FarmName;


	/**
	\~korean
	 Farm 서버는 ProudDB cache server도 갖고 있습니다. 각 게임 서버(Farm 클라이언트)는
	ProudDB cache client를 갖고 있습니다.
	이 변수는 ProudDB cache client가 서버에 접속하기 위한 인증 값입니다.

	\~english
	 Farm server owned ProudDB cache server as well. Each game server (farm client) has ProudDB cache client
	This variable is for authentication value connection ProudDB cache client to server.

	\~chinese
	\~japanese
	\~
	*/
	CStringW m_dbAuthKey;

	/**
	\~korean
	 ProudDB cache server에서 DBMS에 접속하기 위한 connection string입니다.

	\~english
	 Connection string that connect DBMS from ProudDB cache server

	\~chinese
	\~japanese
	\~
	*/
	CStringW m_dbmsConnectionString;

	/**
	\~korean
	 Proud DbLogWriter가 DBMS에 접속하기 위한 connection string입니다.

	\~english
	 Connection string that Proud DbLogWriter connect to DBMS

	\~chinese
	\~japanese
	\~
	*/
	CStringW m_logdbConnectionString;

	Guid GetFarmVersion();
	int GetFarmServerPort();
	int GetDbServerPort();

	CFarmSettings();

};


/**
\~korean
 게임방의 현재 상태를 지칭합니다.

\~english
 It nameed current status of game room

\~chinese
\~japanese
\~
*/
enum RoomMode
{
	/**
	\~korean
	 None

	\~english
	 None

	\~chinese
	\~japanese
	\~
	*/
	RoomMode_None,

	/**
	\~korean
	 대기실 모드입니다.

	\~english
	 Waiting mode

	\~chinese
	\~japanese
	\~
	*/
	RoomMode_Waiting,

	/**
	\~korean
	 대기실에서 플레이 모드로 진입하는 사이에서의 데이터 파일 로딩 과정입니다.

	\~english
	 Data file loading process during enter play mode from waiting room

	\~chinese
	\~japanese
	\~
	*/
	RoomMode_Loading,

	/**
	\~korean
	 실제 게임 플레이 모드입니다.

	\~english
	 Actual game play mode

	\~chinese
	\~japanese
	\~
	*/
	RoomMode_Playing,
};

PROUDNET_SERIALIZE_ENUM(RoomMode)

enum CasualResult
{
	Casual_Ok,
	Casual_CheckCredentialFail,
	Casual_Unexpected,
	Casual_TooShortRequest,
	Casual_BadGamerCount,
	Casual_BadMasterHeroGuid,
	Casual_NoGameRoom,
	Casual_NotAllowedHero,
};

PROUDNET_SERIALIZE_ENUM(CasualResult)

const BYTE DisConnectLogout = 0x01;
const BYTE DisConnectServerChange = 0x02;


/**
\~korean
 게임방 1개의 개괄적 정보입니다.
- 만약 게임방의 개괄적 정보에 더 추가할 것이 있으면 이 클래스를 사용하면 됩니다.

\~english
 General information of 1 game room.
- If you need to add more information rather than general one then modify this class.

\~chinese
\~japanese
\~
*/
class CGameRoomParameter
{
public:
	Guid m_guid;
	CStringW m_name;
	int m_type;
	int m_gamerCount;
	int m_maxGamerCount;
	Guid m_masterHeroGuid;

	/**
	\~korean
	 로비 클라이언트는 m_masterHeroGuid만 갖고 방장 이름을 얻을 수 없으므로 이게 별도로 존재합니다.

	\~english
	 Lobby client owned only m_masterHeroGuid without room master name so it additionally exist.

	\~chinese
	\~japanese
	\~
	*/
	CStringW m_masterHeroName;

	/**
	\~korean
	 이 게임방을 관리하고 있는 배틀 서버의 주소와 포트입니다.

	\~english
	 Address and port of battle server which this game room has controlled

	\~chinese
	\~japanese
	\~
	*/
	NamedAddrPort m_serverAddr;

	/**
	\~korean
	 이 게임방 생성을 요청했던 로비 서버의 HostID입니다.

	\~english
	 HostID of lobby server which request creation of this game room

	\~chinese
	\~japanese
	\~
	*/
	HostID m_creatorLobbyServerHostID;

	RoomMode m_mode;

	CGameRoomParameter()
	{
		m_creatorLobbyServerHostID = HostID_None;
		m_type = 0;
		m_maxGamerCount = 0;
		m_gamerCount = 0;
		m_mode = RoomMode_None;
	}
};

namespace Proud
{
inline CMessage& operator<<(CMessage& a, const CGameRoomParameter& b)
{
	a << b.m_guid;
	a << b.m_name;
	a << b.m_type;
	a << b.m_gamerCount;
	a << b.m_maxGamerCount;
	a << b.m_masterHeroGuid;
	a << b.m_masterHeroName;
	a << b.m_serverAddr;
	a << b.m_mode;

	return a;
}

inline CMessage& operator>>(CMessage& a, CGameRoomParameter& b)
{
	a >> b.m_guid;
	a >> b.m_name;
	a >> b.m_type;
	a >> b.m_gamerCount;
	a >> b.m_maxGamerCount;
	a >> b.m_masterHeroGuid;
	a >> b.m_masterHeroName;
	a >> b.m_serverAddr;
	a >> b.m_mode;


	return a;
}

inline void AppendTextOut(String& a, const CGameRoomParameter& b)
{
	String xx;
	xx.Format(L"{Name=%s ...}", b.m_name);
	a += xx;
}
}

/**
\~korean
 게임 클라이언트가 게임방에 들어가기 위해 입력하는 정보입니다.
- 게임 클라이언트가 게임방에 들어가기 위해 입력하는 정보에 더 추가할 게 있으면
(예: 비밀번호, 팀 소속 등) 여기에 파라메터를 추가하면 됩니다.

\~english
 Entering information for game client to enter game room.
- If game client need to enter more information (e.g. password, team, etc) then add parameter to here.

\~chinese
\~japanese
\~
*/
class CJoinGameRoomParameter
{
public:
	Guid m_guid;
	Guid m_joinRequestedHeroGuid;

	CJoinGameRoomParameter()
	{
	}

	void From(const CGameRoomParameter& roomInfo, Guid joinRequestedHeroGuid)
	{
		m_guid = roomInfo.m_guid;
		m_joinRequestedHeroGuid = joinRequestedHeroGuid;
	}
};

namespace Proud
{
inline CMessage& operator<<(CMessage& a, const CJoinGameRoomParameter& b)
{
	a << b.m_guid;
	a << b.m_joinRequestedHeroGuid;

	return a;
}

inline CMessage& operator>>(CMessage& a, CJoinGameRoomParameter& b)
{
	a >> b.m_guid;
	a >> b.m_joinRequestedHeroGuid;

	return a;
}

inline void AppendTextOut(String& a, const CJoinGameRoomParameter& b)
{
	String xx;
	xx.Format(L"{Guid=%s ...}", b.m_guid);
	a += xx;
}
}

/**
\~korean
 게이머가 현재 어떤 상태인지를 가리킵니다.

\~english
 Pointing gamer's current state

\~chinese
\~japanese
\~
*/
enum GamingState
{
	/**
	\~korean
	 None

	\~english
	 None

	\~chinese
	\~japanese
	\~
	*/
	GamingState_None,

	/**
	\~korean
	 방의 멤버인 경우, READY 를 안찍은 경우입니다.

	\~english
	 Room member without click "READY"

	\~chinese
	\~japanese
	\~
	*/
	GamingState_NotReady,

	/**
	\~korean
	 방의 멤버인 경우, READY 를 찍은 경우입니다.

	\~english
	 Room member with click "READY"

	\~chinese
	\~japanese
	\~
	*/
	GamingState_Ready,

	/**
	\~korean
	 방장이 게임 시작을 승인한 후, 플레이를 위한 데이터 파일을 로딩하고 있는 중인 상태입니다.

	\~english
	 Once room master accept starting game and loading data file for play

	\~chinese
	\~japanese
	\~
	*/
	GamingState_Loading,

	/**
	\~korean
	 로딩은 끝났으나 타 게이머가 아직 데이터 파일 로딩이 끝나지 않아 대기하는 중입니다.

	\~english
	 Loading is finished but other gamer still load data file so waiting for them.

	\~chinese
	\~japanese
	\~
	*/
	GamingState_PlayWaiting,

	/**
	\~korean
	 게임 플레이 중입니다.

	\~english
	 Playing game

	\~chinese
	\~japanese
	\~
	*/
	GamingState_Playing
};


PROUDNET_SERIALIZE_ENUM(GamingState)

enum GameObjectType
{
	GameObjectType_Hero,
	GameObjectType_Obstacle,
};



LPCWSTR GamingStateToString(GamingState s);


/**
\~korean
 플레이어 캐릭터 정보 중 클라이언트-서버간 공유가 가능한 정보입니다.
 \todo 보안상 주의점을 기재하자. 특히 타 클라이언트에게는 공유되면 안되는 정보는 여기있으면 안됩니다.

\~english
 Shared information between client and server from player character information.
 \todo Write security notice. Do not put information that never share with other client to here.

\~chinese
\~japanese
\~
*/
class CHeroPublishInfo
{
public:

	/**
	\~korean
	 플레이어 캐릭터의 guid입니다.

	\~english
	 Guid of player character

	\~chinese
	\~japanese
	\~
	*/
	Guid m_guid;

	/**
	\~korean
	 플레이어 캐릭터의 이름입니다.

	\~english
	 Name of player character

	\~chinese
	\~japanese
	\~
	*/
	CStringW m_name;

	/**
	\~korean
	 타입

	\~english
	 Type

	\~chinese
	\~japanese
	\~
	*/
	int m_type;


	/**
	\~korean
	 플레이어 캐릭터가 소지한 점수입니다.
	\todo 이런 식으로, 타 게이머에게도 노출될 수 있는 플레이어 캐릭터 정보(예: 종족,성별,외모 등)은 이런 식으로
	추가하면 됩니다.

	\~english
	 Score which player character had
	\todo Add exposable player character information (e.g: race, gender, appearance, etc) like this.

	\~chinese
	\~japanese
	\~
	*/
	LONGLONG m_score;


	/**
	\~korean
	 이 플레이어 캐릭터의 상태입니다.

	\~english
	 State of this player character

	\~chinese
	\~japanese
	\~
	*/
	GamingState m_gamingState;



	CHeroPublishInfo();
};


namespace Proud
{
inline CMessage& operator<<(CMessage& a, const CHeroPublishInfo& b)
{
	a << b.m_guid ;
	a << b.m_name ;
	a << b.m_score ;
	a << b.m_type;
	a << b.m_gamingState;

	return a;
}

inline CMessage& operator>>(CMessage& a, CHeroPublishInfo& b)
{
	a >> b.m_guid ;
	a >> b.m_name ;
	a >> b.m_score ;
	a >> b.m_type;
	a >> b.m_gamingState;

	return a;
}

inline void AppendTextOut(String& a, const CHeroPublishInfo& b)
{
	String xx;
	xx.Format(L"{Name=%s ...}", b.m_name);
	a += xx;
}
}

enum DirectionState{
	NORTH,
	SOUTH,
	WEST,
	EAST,
	TOP
};
PROUDNET_SERIALIZE_ENUM(DirectionState)


/**
\~korean
 Bullet class. 서버와 클라이언트 각각 상속받습니다.
 서버는 시뮬레이션하고 클라이언트는 Render만 합니다.

\~english
 Bullet class. Inheit server and client separatley.
 Server does simulation and client does only Render.

\~chinese
\~japanese
\~
*/
class CBullet
{
public:

	D3DXVECTOR3 m_position; // Position
	D3DXVECTOR3 m_velocity;
	float m_direction; // Watching direction
	float m_speed;// Speed (Scalar)
	float m_maxSpeed;// Maximum speed
	float m_accelspeed;// Accelation Speed
	int m_id;// Index

/*
	float m_direction; //바라보는위치
	float m_speed;//속력(스칼라)
	float m_maxSpeed;//최대속력
	float m_accelspeed;//가속도
	int m_id;//인덱스
*/

	bool m_active;

	DirectionState m_directionState;

	float m_broadcastStateCoolTime;

	float m_BroadCastBulletStateInterval;

	CBullet();
	~CBullet(void);

};
namespace Proud{
	inline void AppendTextOut(String &a, const CBullet &b)
	{
		String f;
		f.Format(L"<CBullst_s_INFO");
		a+=f;
	}

	inline CMessage& operator>>(CMessage &a, CBullet &b)
	{
		a>>b.m_position.x;
		a>>b.m_position.y;
		a>>b.m_position.z;
		a>>b.m_direction;
		a>>b.m_speed;
		a>>b.m_maxSpeed;
		a>>b.m_accelspeed;
		a>>b.m_directionState;
		a>>b.m_id;
		a>>b.m_broadcastStateCoolTime;
		a>>b.m_BroadCastBulletStateInterval;

		return a;
	}
	inline CMessage& operator<<(CMessage &a, const CBullet &b)
	{
		a<<b.m_position.x;
		a<<b.m_position.y;
		a<<b.m_position.z;
		a<<b.m_direction;
		a<<b.m_speed;
		a<<b.m_maxSpeed;
		a<<b.m_accelspeed;
		a<<b.m_directionState;
		a<<b.m_id;
		a<<b.m_broadcastStateCoolTime;
		a<<b.m_BroadCastBulletStateInterval;
		return a;

	}
}

enum ItemType{
	LIFEINCREASE,//score++
	LIFEDECREASE,//score--
	DISAPPEARBULLET,
	RAPID,
};
PROUDNET_SERIALIZE_ENUM(ItemType)
class CItem{
public:
	D3DXVECTOR3 m_position;
	ItemType m_type;
	int m_id;

	CItem();
	~CItem();
};

namespace Proud{
	inline void AppendTextOut(String &a, const CItem &b)
	{
		String f;
		f.Format(L"<CItem_INFO");
		a+=f;
	}

	inline CMessage& operator>>(CMessage &a, CItem &b)
	{
		a>>b.m_position.x;
		a>>b.m_position.y;
		a>>b.m_position.z;
		a>>b.m_type;

		return a;
	}
	inline CMessage& operator<<(CMessage &a, const CItem &b)
	{
		a<<b.m_position.x;
		a<<b.m_position.y;
		a<<b.m_position.z;
		a<<b.m_type;
		return a;

	}
}

extern CStringW NoRoomNameText ;

extern const float SynchAreaRange;
extern const int distanceHeroFromToBullet,distanceHeroFromToItem;