#include "stdafx.h"

#include "RemoteClient.h"
#include "Server.h"


const double UpdateHeroViewerInterval = 1;

CRemoteClient_S::CRemoteClient_S(CSynchWorldServer* owner)
{
	m_hostID = HostID_None;
	m_owner = owner;
	m_updateHeroViewerCoolTime = CRandom::StaticGetFloat() * UpdateHeroViewerInterval;
}

CRemoteClient_S::~CRemoteClient_S(void)
{
}

// 매 프레임마다의 처리
// Processing every frames
void CRemoteClient_S::FrameMove()
{
	// 가시 영역의 변화를 처리한다.
	// 이 샘플에서는 캐릭의 중심점으로부터 일정 반경이 캐릭을 조종하는 유저의 가시 영역으로 간주된다.
	// Process changes of visible area.
	// In this sample, it considers specific radius from character as visible area of user who control character.
	if (m_updateHeroViewerCoolTime < GetPreciseCurrentTimeMs())
	{
		m_updateHeroViewerCoolTime = UpdateHeroViewerInterval + GetPreciseCurrentTimeMs();

		if (m_hero)
		{
			for (Proud::Position iRC = m_owner->m_remoteClients.GetStartPosition();iRC;)
			{
				CRemoteClientPtr_S rc = m_owner->m_remoteClients.GetNextValue(iRC);
				if (m_owner->IsHeroVisibleToClient(this, rc))
				{
					if (m_hero->m_viewers.FindByValue(rc) < 0)
					{
						// 목록에 추가, P2P 그룹에도 추가, 출현 신고 메시지 송신.
						// Add to list, Add to P2P group, Send appearing report message.
						m_hero->m_viewers.Add(rc);
						rc->m_tangibles.Add(m_hero);

						m_owner->m_server->JoinP2PGroup(rc->m_hostID, m_hero->m_viewersGroupID, ByteArray());
						m_owner->m_S2CProxy.RemoteHero_Appear(rc->m_hostID, RmiContext::ReliableSend,
						                                    m_hostID, m_hero->m_name, m_hero->m_position, m_hero->m_velocity, m_hero->m_direction);
					}
				}
				else
				{
					int delIndex = m_hero->m_viewers.FindByValue(rc);
					if (delIndex >= 0)
					{
						// 목록에서 제거. P2P 그룹에서도 제거, 사라짐 신고 메시지 송신.
						// Remove from list. Remove from P2P group, Send disappearing report message.
						m_hero->m_viewers[delIndex]->m_tangibles.RemoveOneByValue(m_hero);
						m_hero->m_viewers.RemoveAt(delIndex);

						m_owner->m_server->LeaveP2PGroup(rc->m_hostID, m_hero->m_viewersGroupID);
						m_owner->m_S2CProxy.RemoteHero_Disappear(rc->m_hostID, RmiContext::ReliableSend, m_hostID);
					}
				}
			}
		}
	}
}

CRemoteHero_S::CRemoteHero_S()
{
	m_direction = 0;
	m_velocity = m_position = Vector3(0, 0, 0);
	m_viewersGroupID = HostID_None;
}
