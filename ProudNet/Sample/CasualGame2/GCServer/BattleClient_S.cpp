#include "stdafx.h"
#include "BattleClient_S.h"
#include "BattleServer.h"
#include "CasualGamer.h"

const int64_t UpdateHeroViewerInterval = 1000;


CGameObject_S::CGameObject_S()
{
	m_yaw = 0;
	m_velocity = m_position = D3DXVECTOR3(0, 0, 0);
}


CBattleClient_S::CBattleClient_S(CBattleServer* owner) : m_owner(owner)
{
	m_hero = NULL;

	m_lastRequestTimeOfNewGameRoomMs = 0;

	m_updateHeroViewerCoolTimeMs = CRandom::StaticGetInt64() * UpdateHeroViewerInterval;
}

/// If this client is not loggedon then return "null" otherwise return guid of gamer.
/// 이 클라가 로그온하지 않았으면 null을, 로그온했으면 게이머의 guid를 리턴합니다.
Proud::Guid CBattleClient_S::GetGamerGuid()
{
	if (m_dbLoadedGamerData)
		return m_dbLoadedGamerData->GetRootUUID();
	else
		return Guid();
}

/// If this client is not loggedon then return "null" otherwise return guid of player character which currently using.
/// 이 클라가 로그온하지 않았으면 null을, 로그온했으면 게이머가 사용중인 플레이어 캐릭터의 guid를 리턴합니다.
Proud::Guid CBattleClient_S::GetSelectedHeroGuid()
{
	if (!m_dbLoadedGamerData)
		return Guid();

	return CCasualGamer::GetSelectedHeroGuid(m_dbLoadedGamerData);
}

CPropNodePtr CBattleClient_S::GetSelectedHero()
{
	// Currently loaded gamer data at DB
	//현재 db에 로딩된 게이머 데이터
	if (!m_dbLoadedGamerData)
		return CPropNodePtr();

	return CCasualGamer::GetSelectedHero(m_dbLoadedGamerData);
}

String CBattleClient_S::GetSelectedHeroName()
{
	if (!m_dbLoadedGamerData)
		return String();

	return CCasualGamer::GetSelectedHeroName(m_dbLoadedGamerData);
}

void CBattleClient_S::FrameMove()
{
	// 가시 영역의 변화를 처리합니다.
	// 이 샘플에서는 캐릭의 중심점으로부터 일정 반경이 캐릭을 조종하는 유저의 가시 영역으로 간주됩니다.
	// Process changing of visible area.
	// In this sample, it consider specific area from center mark as visible area of user who control character.
	if (m_updateHeroViewerCoolTimeMs < GetPreciseCurrentTimeMs())
	{
		m_updateHeroViewerCoolTimeMs = UpdateHeroViewerInterval + GetPreciseCurrentTimeMs();

		FrameMove_DoSynchFilter();



	}
}

void CBattleClient_S::FrameMove_DoSynchFilter()
{
	if (m_hero != NULL)
		{
		for each(RemoteBattleClients::value_type iRC in m_owner->m_remoteClients)
			{
			CBattleClient_S* rc = iRC.second;
				if (m_owner->IsHeroVisibleToClient(this, rc))
				{
				if (!m_hero->m_viewers.ContainsKey(rc)) 
					{
						// Adds to list, adds to P2P group, Receive appearance report message.
						// 목록에 추가, P2P 그룹에도 추가, 출현 신고 메시지 송신.
						m_hero->m_viewers.Add(rc);
						rc->m_tangibles.Add(m_hero);
						m_owner->m_netServer->JoinP2PGroup(rc->m_HostID, m_hero->m_viewersGroupID, ByteArray());
						m_owner->m_s2cProxy.RemoteHero_Appear(rc->m_HostID, RmiContext::ReliableSend,
							m_HostID, m_hero->m_name, m_hero->m_position, m_hero->m_velocity, m_hero->m_yaw);
					}
				}
			else // 없으면
				{
				if(m_hero->m_viewers.ContainsKey(rc))
					{
						// Remove from the list. Remove from P2P group. Receive disappearance report message.
						// 목록에서 제거. P2P 그룹에서도 제거, 사라짐 신고 메시지 송신.
					m_hero->m_viewers.Remove(rc);
					rc->m_tangibles.Remove(m_hero);

					m_owner->m_netServer->LeaveP2PGroup(rc->m_HostID, m_hero->m_viewersGroupID);
						m_owner->m_s2cProxy.RemoteHero_Disappear(rc->m_HostID, RmiContext::ReliableSend, m_HostID);
					}
				}
			}
		}
	}

CGameRoom_S::~CGameRoom_S()
{
	for each(CGameObject_S* obj in m_gameObjects)
	{
		delete obj;
	}
}
