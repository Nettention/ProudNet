#include "stdafx.h"

#include "ErrorLayer.h"
#include "World.h"


// on "init" you need to initialize your instance
bool ErrorLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	/////////////////////////////
	// 2. add gui objects.

	// add a "Close" menu. it's an autorelease object
	CCMenuItemImage* closeMenuItemImage = CCMenuItemImage::create(
		"ButtonNormal.png",
		"ButtonSelected.png",
		this,
		menu_selector(ConnectLayer::menuCloseCallback));

	closeMenuItemImage->setPosition(ccp(origin.x + visibleSize.width/2,
										origin.y + closeMenuItemImage->getContentSize().height));

	// add a "Close" label. it's an autorelease object
	CCLabelTTF* closeMenuItemLabel = CCLabelTTF::create("Close", "Arial", CWorld::DefaultFontSize);

	closeMenuItemLabel->setPosition(ccp(closeMenuItemImage->getContentSize().width/2,
										closeMenuItemImage->getContentSize().height/2));

	// add the label as a child to "Close" menu
	closeMenuItemImage->addChild(closeMenuItemLabel, 1);

	// create menu, it's an closeMenuItemImage object
	CCMenu* menu = CCMenu::create(closeMenuItemImage, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);

	// add a "ErrorMessage" label. it's an autorelease object
	CCLabelTTF* errorMessageLabel = CCLabelTTF::create(
		g_World->m_errorMsg.getCString(),
		"Arial",
		CWorld::DefaultFontSize,
		CCSizeMake(visibleSize.width, visibleSize.height/2),
		kCCTextAlignmentLeft,
		kCCVerticalTextAlignmentTop);

	// position the label on the center of the screen
	errorMessageLabel->setPosition(ccp(origin.x + visibleSize.width/2,
									   origin.y + visibleSize.height/2));
	
	// add the label as a child to this layer
	this->addChild(errorMessageLabel, 1);

	/////////////////////////////
	// 3. update
	this->schedule(schedule_selector(ErrorLayer::update));
	
	return true;
}

void ErrorLayer::update(float deltaTime)
{
	g_World->FrameMove(deltaTime);
}

void ErrorLayer::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}
