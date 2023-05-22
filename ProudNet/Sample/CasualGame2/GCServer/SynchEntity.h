#pragma once 

class CBattleClient_S;

class CSynchEntity
{
public:
	virtual ~CSynchEntity() {}
	CSynchEntity();

	/**
	\~korean
	이 캐릭을 감시하는 클라이언트들의 목록

	\~english
	List of clients which keep watch on this character

	\~chinese
	监事该人物的客户端目录

	\~japanese
	このキャラクターを監視するクライアントのリスト
	\~
	*/
	CFastSet<CBattleClient_S*> m_viewers;

	/**
	\~korean
	이 캐릭을 감시하는 클라이언트들의 모든 P2P group 목록

	\~english
	List of all P2P groups of clients which keep watch on this character

	\~chinese
	监督该人物的客户端所有P2P组目录

	\~japanese
	このキャラクターを監視するクライアントのすべてのP2P groupリスト
	\~
	*/
	HostID m_viewersGroupID;


};