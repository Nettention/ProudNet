#include "stdafx.h"

#include "RemoteHero_C.h"
#include "World.h"


const float CRemoteHero_C::SynchedThruDirectP2PChangeShowInterval = 1.5f;

CRemoteHero_C::CRemoteHero_C(void)
{
	m_synchedThruDirectP2P = false;
	m_synchedThruDirectP2PChangeShowCoolTime = SynchedThruDirectP2PChangeShowInterval;

	// create sprite, it's an autorelease object
	m_remoteHeroSprite = CCSprite::create("Airplane.png");
	m_remoteHeroSprite->setVisible(false);
	g_World->m_mainLayerObjects->addChild(m_remoteHeroSprite, 1);

	m_remoteHeroLabel = CCLabelTTF::create("", "Arial", CWorld::DefaultFontSize/2);
	m_remoteHeroLabel->setVisible(false);
	g_World->m_mainLayerObjects->addChild(m_remoteHeroLabel, 1);
}


CRemoteHero_C::~CRemoteHero_C(void)
{
	if (g_World->m_mainLayerObjects)
	{
		g_World->m_mainLayerObjects->removeChild(m_remoteHeroSprite);
		g_World->m_mainLayerObjects->removeChild(m_remoteHeroLabel);
	}
}

// 캐릭터를 1 프레임만큼의 처리를 한다.
// Process character equal to 1.
void CRemoteHero_C::FrameMove(float fElapsedTime)
{
	if (m_synchedThruDirectP2PChangeShowCoolTime >= 0)
		m_synchedThruDirectP2PChangeShowCoolTime -= fElapsedTime;
	m_positionFollower.FrameMove(fElapsedTime);
	m_directionFollower.FrameMove(fElapsedTime);

	// 캐릭터 자체의 좌표를 구한다.
	// Obtain the coordinates of the character itself.
	m_remoteHeroSprite->setRotation(m_directionFollower.GetFollowerAngle() * 180.0 / M_PI);
	m_remoteHeroSprite->setPosition(ccp(
		m_positionFollower.GetFollowerPosition().x,
		m_positionFollower.GetFollowerPosition().z));

	m_remoteHeroLabel->setPosition(m_remoteHeroSprite->getPosition());
	m_remoteHeroLabel->setString(m_name.getCString());

	ccColor3B redTone  = ccc3(255, 127, 127);
	ccColor3B blueTone = ccc3(127, 127, 255);

	// 캐릭터 위에 이름을 표시한다.
	// Indicate name on the character.
	if (m_synchedThruDirectP2PChangeShowCoolTime > 0)
	{
		CCString stateString;
		if (m_synchedThruDirectP2P)
		{
			stateString = "Direct P2P";
			m_remoteHeroSprite->setColor(blueTone);
		}
		else
		{
			stateString = "Relay P2P";
			m_remoteHeroSprite->setColor(redTone);
		}

		static CCString text;
		text.initWithFormat("%s : %s", m_name.getCString(), stateString.getCString());
		m_remoteHeroLabel->setString(text.getCString());
	}
}
