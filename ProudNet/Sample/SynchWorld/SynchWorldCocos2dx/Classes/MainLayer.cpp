#include "stdafx.h"

#include "MainLayer.h"
#include "World.h"


// on "init" you need to initialize your instance
bool MainLayer::init()
{
	/////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	/////////////////////////////
	// 2. add your codes below...
	
	// add "Background" sprite. it's an autorelease object
	CCSprite* backgroundSprite = CCSprite::create("cellwall.png");

	// position the sprite on the center of the screen
	backgroundSprite->setPosition(ccp(origin.x + backgroundSprite->getContentSize().width/2,
									  origin.y + backgroundSprite->getContentSize().height/2));

	// add the sprite as a child to this layer
	this->addChild(backgroundSprite, 0);

	// add "Circle" sprite. it's an autorelease object
	m_synchAreaSprite = CCSprite::create("circle.png");

	m_synchAreaSprite->setOpacity(127);

	// add the sprite as a child to "MainLayer" node
	g_World->m_mainLayerObjects->addChild(m_synchAreaSprite, 0);

	// add the node as a child to this layer
	this->addChild(g_World->m_mainLayerObjects);
	
	/////////////////////////////
	// 3. update
	this->schedule(schedule_selector(MainLayer::update));

	return true;
}

void MainLayer::update(float deltaTime)
{
	g_World->FrameMove(deltaTime);

	// 사용자가 선택한 LocalHero에 SynchArea를 보여준다.
	CCSprite* localHeroSprite = g_World->GetLocalHeroUnderUserControl()->m_localHeroSprite;

	if (localHeroSprite)
	{
		m_synchAreaSprite->setPosition(localHeroSprite->getPosition());
	}
}
