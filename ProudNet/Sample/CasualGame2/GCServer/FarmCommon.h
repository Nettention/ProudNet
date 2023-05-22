#pragma once

#include "../GCCommon/Vars.h"

enum ServerMode
{
	ServerMode_None,
	ServerMode_Farm,
	ServerMode_Status,
	ServerMode_Entry,
	ServerMode_Lobby,
	ServerMode_Battle,
};

PROUDNET_SERIALIZE_ENUM(ServerMode)

class CFarmClientInfo
{
public:
	 
	/**
	\~korean
	어떤 역할을 하는 Farm 클라이언트인지 지정

	\~english
	Designate role of farm client

	\~chinese
	指定Farm客户端的角色

	\~japanese
	どのような役割をするFarmクライアントであるかを指定

	\~
	*/
	ServerMode m_farmClientType;

	/**
	\~korean
	Farm 클라이언트의 이름

	\~english
	Designate name of farm client

	\~chinese
	Farm客户端的名称

	\~japanese
	Farmクライアントの名前

	\~
	*/
	String m_farmClientName;

	 
	/**
	\~korean
	이 Farm 클라가 로비 서버나 배틀 서버인 경우 어느 로비에 종속된 것인지를 구별하는 이름

	\~english
	Distincted name that if this farm client is lobby server or battle server then subordinate to which lobby.

	\~chinese
	该Farm客户端为lobby服务器或作战服务器时区分所属lobby的名称

	\~japanese
	このFarmクライアントがロビーサーバやバトルサーバである場合、どのロビーに属しているかを区別する名前

	\~
	*/
	String m_relatedLobbyName;

	/**
	\~korean
	Farm 클라이언트에 접속중인 게임 클라의 갯수

	\~english
	Number of game client that connected to farm client

	\~chinese
	连接Farm客户端的游戏客户端数量

	\~japanese
	Farmクライアントに接続されているゲームクライアントの数

	\~
	*/
	int m_gamerCount;

	/**
	\~korean
	Farm 클라이언트에 게임 클라이언트가 접속할 수 있는 인터넷 주소

	\~english
	Internet address that farm client can connect to game client
	
	\~chinese
	Farm客户端中游戏客户端可连接的互联网地址

	\~japanese
	Farmクライアントにゲームクライアントがアクセスできるインターネットアドレス

	\~
	*/
	NamedAddrPort m_serverAddr;
	
	/**
	\~korean
	 Farm 클라/서버 사이에서, Farm Client의 Host ID입니다.
	이 값은 게임 클라에서는 인식되지 않는 것이 좋습니다. 

	\~english
	Host ID of Farm Client between Farm client/server.
	This value is better to not recognize at game client.

	\~chinese
	Farm客户端/服务器之间，Farm Client的Host ID。
	该值在游戏客户端不被识别为好。
	
	\~japanese
	Farmクライアント/サーバの間で、Farm ClientのHost IDです。
	この値はゲームクライアントで認識されないのをお勧めします。

	\~
	*/
	HostID m_farmClientHostID;

	CFarmClientInfo()
	{
		m_farmClientType = ServerMode_None;
		m_gamerCount = 0;
		m_farmClientHostID = HostID_None;
	}
};

namespace Proud
{
inline CMessage& operator<<(CMessage& a, const CFarmClientInfo& b)
{
	a << b.m_farmClientHostID ;
	a << b.m_farmClientName ;
	a << b.m_relatedLobbyName;
	a << b.m_farmClientType ;
	a << b.m_gamerCount ;
	a << b.m_serverAddr ;
	return a;
}
inline CMessage& operator>>(CMessage& a, CFarmClientInfo& b)
{
	a >> b.m_farmClientHostID ;
	a >> b.m_farmClientName ;
	a >> b.m_relatedLobbyName;
	a >> b.m_farmClientType ;
	a >> b.m_gamerCount ;
	a >> b.m_serverAddr ;
	return a;
}

inline void AppendTextOut(String& a, const CFarmClientInfo& b)
{
	a += L"<CFarmClientInfo>";
}
}

typedef RefCount<CFarmClientInfo> CFarmClientInfoPtr;
typedef CFastMap<HostID,CFarmClientInfoPtr> FarmClientInfoMap;
