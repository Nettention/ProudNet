#include "stdafx.h"

#include "ConnectLayer.h"
#include "World.h"


// on "init" you need to initialize your instance
bool ConnectLayer::init()
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
	// 2. add your codes below...

	// add a label shows "SynchWorld"
	// create and initialize a label

	CCLabelTTF* synchWorldLabel = CCLabelTTF::create("SynchWorld", "Arial", CWorld::DefaultFontSize);

	// position the label on the center of the screen
	synchWorldLabel->setPosition(ccp(origin.x + visibleSize.width/2,
									 origin.y + visibleSize.height - synchWorldLabel->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(synchWorldLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* backgroudSprite = CCSprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	backgroudSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(backgroudSprite, 0);

	/////////////////////////////
	// 3. add gui objects.

	// add a editbox. it's an autorelease object
	CCEditBox* serverAddressEditBox = CCEditBox::create(CCSizeMake(290, 30), CCScale9Sprite::create("Textbox.png"));

	// position the editbox on the center of the screen
	serverAddressEditBox->setPosition(ccp(origin.x + visibleSize.width/2,
										  origin.y + visibleSize.height/2 + serverAddressEditBox->getContentSize().height));

	serverAddressEditBox->setDelegate(this);
	serverAddressEditBox->setPlaceHolder("Server Address");
	serverAddressEditBox->setTag(ServerAddress);
	serverAddressEditBox->setText(g_World->m_serverAddr.getCString());

	// add the editbox as a child to this layer
	this->addChild(serverAddressEditBox, 1);

	// add a editbox. it's an autorelease object
	CCEditBox* userNameEditBox = CCEditBox::create(CCSizeMake(290, 30), CCScale9Sprite::create("Textbox.png"));

	// position the editbox on the center of the screen
	userNameEditBox->setPosition(ccp(origin.x + visibleSize.width/2,
									 origin.y + visibleSize.height/2));

	userNameEditBox->setDelegate(this);
	userNameEditBox->setPlaceHolder("User Name");
	userNameEditBox->setTag(UserName);
	userNameEditBox->setText(g_World->m_userName.getCString());

	// add the editbox as a child to this layer
	this->addChild(userNameEditBox, 1);

	// add a "Connect" menu. it's an autorelease object
	CCMenuItemImage* connectMenuItemImage = CCMenuItemImage::create(
													"ButtonNormal.png",
													"ButtonSelected.png",
													"ButtonDisable.png",
													this,
													menu_selector(ConnectLayer::menuConnectCallback));

	connectMenuItemImage->setPosition(ccp(origin.x + visibleSize.width/2 - connectMenuItemImage->getContentSize().width/2,
										  origin.y + visibleSize.height/2 - connectMenuItemImage->getContentSize().height));

	// add a "Connect" label. it's an autorelease object
	m_connectMenuItemLabel = CCLabelTTF::create("Connect", "Arial", CWorld::DefaultFontSize);

	m_connectMenuItemLabel->setPosition(ccp(connectMenuItemImage->getContentSize().width/2,
											connectMenuItemImage->getContentSize().height/2));

	// add the label as a child to "Connect" menu
	connectMenuItemImage->addChild(m_connectMenuItemLabel, 1);

	// add a "Close" menu. it's an autorelease object
	CCMenuItemImage* closeMenuItemImage = CCMenuItemImage::create(
		"ButtonNormal.png",
		"ButtonSelected.png",
		this,
		menu_selector(ConnectLayer::menuCloseCallback));

	closeMenuItemImage->setPosition(ccp(origin.x + visibleSize.width/2 + closeMenuItemImage->getContentSize().width/2,
										origin.y + visibleSize.height/2 - closeMenuItemImage->getContentSize().height));

	// add a "Close" label. it's an autorelease object
	CCLabelTTF* closeMenuItemLabel = CCLabelTTF::create("Close", "Arial", CWorld::DefaultFontSize);

	closeMenuItemLabel->setPosition(ccp(closeMenuItemImage->getContentSize().width/2,
										closeMenuItemImage->getContentSize().height/2));

	// add the label as a child to "Close" menu
	closeMenuItemImage->addChild(closeMenuItemLabel, 1);

	// create menu, it's an autorelease object
	CCMenu* menu = CCMenu::create(connectMenuItemImage, closeMenuItemImage, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);

	/////////////////////////////
	// 4. update
	this->schedule(schedule_selector(ConnectLayer::update));

	return true;
}

void ConnectLayer::update(float deltaTime)
{
	g_World->FrameMove(deltaTime);
}

void ConnectLayer::menuConnectCallback(CCObject* pSender)
{
	// 중복 연결 시도를 방지하기 위해
	// To prevent repeat connection attempt
	CCMenuItemImage* menuItem = (CCMenuItemImage*)pSender;
	menuItem->setEnabled(false);

	// 클라 1개를 만든 후 서버에 연결 시도를 시작
	// Create 1 client then start connecting to server
	CClient* newCli = g_World->AddNewClient();

	bool ret = newCli->Connect();
	if (!ret)
	{
		g_World->GoToErrorScene("Wrong Server Address.");
	}
	else
	{
		m_connectMenuItemLabel->setString("Connecting...");
	}
}

void ConnectLayer::menuCloseCallback(CCObject* pSender)
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

void ConnectLayer::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	// 사용자가 EditBox에 입력한 내용을 변수에 저장한다.
	switch (editBox->getTag())
	{
	case ServerAddress:
		g_World->m_serverAddr = text;
		break;

	case UserName:
		g_World->m_userName = text;
		break;
	}
}
