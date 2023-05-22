#include "stdafx.h"

#include "MainGUILayer.h"
#include "World.h"


// on "init" you need to initialize your instance
bool MainGUILayer::init()
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
	// 2. add gui objects.

	// add a "Up" menu. it's an autorelease object
	m_upMenuItem = CCMenuItemImage::create(
		"ArrowNormal.png",
		"ArrowSelected.png");

	m_upMenuItem->setOpacity(127);
	m_upMenuItem->setPosition(ccp(origin.x + visibleSize.width/2,
								  origin.y + visibleSize.height - m_upMenuItem->getContentSize().height));

	// add a "Left" menu. it's an autorelease object
	m_leftMenuItem = CCMenuItemImage::create(
		"ArrowNormal.png",
		"ArrowSelected.png");

	m_leftMenuItem->setOpacity(127);
	m_leftMenuItem->setRotation(-90.0);
	// 회전되었기 때문에 width가 아닌 height로 위치를 조정한다.
	m_leftMenuItem->setPosition(ccp(origin.x + m_leftMenuItem->getContentSize().height,
									origin.y + visibleSize.height/2));

	// add a "Right" menu. it's an autorelease object
	m_rightMenuItem = CCMenuItemImage::create(
		"ArrowNormal.png",
		"ArrowSelected.png");

	m_rightMenuItem->setOpacity(127);
	m_rightMenuItem->setRotation(90.0);
	// 회전되었기 때문에 width가 아닌 height로 위치를 조정한다.
	m_rightMenuItem->setPosition(ccp(origin.x + visibleSize.width - m_rightMenuItem->getContentSize().height,
									 origin.y + visibleSize.height/2));

	// create menu, it's an autorelease object
	CCMenu* menu = CCMenu::create(m_upMenuItem, m_leftMenuItem, m_rightMenuItem, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);

	// add a "ClientCount" label. it's an autorelease object
	m_clientCountLabel = CCLabelTTF::create("ClientCount:", "Arial", CWorld::DefaultFontSize/2);

	m_clientCountLabel->setPosition(ccp(origin.x + 3 * visibleSize.width/4 + m_clientCountLabel->getContentSize().width/2,
										origin.y + visibleSize.height/3));

	// add the label as a child to this layer
	this->addChild(m_clientCountLabel, 1);

	// add a "ClientCount" editbox. it's an autorelease object
	CCEditBox* clientCountEditBox = CCEditBox::create(CCSizeMake(40, 20), CCScale9Sprite::create("Textbox.png", CCRectMake(7, 7, 40, 20)));

	clientCountEditBox->setPosition(ccp(origin.x + 3 * visibleSize.width/4 + clientCountEditBox->getContentSize().width/2,
										m_clientCountLabel->getPositionY() - clientCountEditBox->getContentSize().height));

	clientCountEditBox->setDelegate(this);
	clientCountEditBox->setTag(ClientCount);
	clientCountEditBox->setText("1");

	// add the "ClientCount" editbox as a child to this layer
	this->addChild(clientCountEditBox, 1);

	// it's an autorelease object
	CCMenuItemImage* applyMenuItemImage = CCMenuItemImage::create(
		"ButtonNormal.png",
		"ButtonSelected.png",
		this,
		menu_selector(MainGUILayer::menuApplyCallback));

	applyMenuItemImage->setScale(0.5);
	applyMenuItemImage->setPosition(ccp(clientCountEditBox->getPositionX() + applyMenuItemImage->getContentSize().width/2,
										clientCountEditBox->getPositionY()));

	// add the "Apply" menu as a child to this layer
	menu->addChild(applyMenuItemImage, 1);

	// add a "Apply" label. it's an autorelease object
	CCLabelTTF* applyMenuItemLabel = CCLabelTTF::create("Apply", "Arial", CWorld::DefaultFontSize/2);

	applyMenuItemLabel->setPosition(ccp(applyMenuItemImage->getContentSize().width/2,
										applyMenuItemImage->getContentSize().height/2));

	// add the "Apply" label as a child to apply menu
	applyMenuItemImage->addChild(applyMenuItemLabel, 1);

	// add a "ClientControl" label. it's an autorelease object
	CCLabelTTF* controlClientLabel = CCLabelTTF::create("Clients under your control", "Arial", CWorld::DefaultFontSize/2);

	controlClientLabel->setPosition(ccp(origin.x + 3 * visibleSize.width/4 + controlClientLabel->getContentSize().width/2 -3,
										clientCountEditBox->getPositionY() - controlClientLabel->getContentSize().height * 2));

	// add the "ClientControl" label as a child to this layer
	this->addChild(controlClientLabel, 1);

	// add a "EventLong" label. it's an autorelease object
	CCLabelTTF* eventLogLabel = CCLabelTTF::create("Event Log", "Arial", CWorld::DefaultFontSize/2);

	eventLogLabel->setPosition(ccp(origin.x + eventLogLabel->getContentSize().width/2,
								   origin.y + visibleSize.height/3));

	// add the "EventLog" label as a child to this layer
	this->addChild(eventLogLabel, 1);

	// add a "EventLog" tableview. it's an autorelease object
	m_eventLogTableView = CCTableView::create(&m_eventLogTableViewDataSource, CCSizeMake(visibleSize.width/3, visibleSize.height/3));

	m_eventLogTableView->setPosition(CCPointZero);
	m_eventLogTableView->setDelegate(&m_eventLogTableViewDataSource);
	m_eventLogTableView->setDirection(kCCScrollViewDirectionVertical);
	m_eventLogTableView->setVerticalFillOrder(kCCTableViewFillBottomUp);

	// add the "EventLog" tableview as a child to this layer
	this->addChild(m_eventLogTableView, 1);

	// add a "ClientControl" tableview. it's an autorelease object
	m_clientControlTableView = CCTableView::create(&m_clientControlTableViewDataSoruce, CCSizeMake(visibleSize.width/5, visibleSize.height/5));

	m_clientControlTableView->setPosition(ccp(3* visibleSize.width/4, 0));
	m_clientControlTableView->setBounceable(false);
	m_clientControlTableView->setDelegate(&m_clientControlTableViewDataSoruce);
	m_clientControlTableView->setDirection(kCCScrollViewDirectionVertical);
	m_clientControlTableView->setVerticalFillOrder(kCCTableViewFillBottomUp);

	// add the "ClientControl" tableview as a child to this layer
	this->addChild(m_clientControlTableView, 1);

	// add a "ID" label to "ClientControl" tableview
	CCString* txt = CCString::createWithFormat("%d", g_World->GetClientUnderUserControl()->m_ID);

	m_clientControlTableViewDataSoruce.m_clientControlList->addObject(txt);
	m_clientControlTableView->reloadData();

	CClient* client = g_World->GetClientUnderUserControl();
	m_ipLabel = CCLabelTTF::create(client->GetLocalAddress().GetString(), "Consolas", 18);
	m_ipLabel->setPositionX(visibleSize.width / 2);
	m_ipLabel->setPositionY(visibleSize.height - 80);
	
	this->addChild(m_ipLabel);

	/////////////////////////////
	// 3. update
	this->schedule(schedule_selector(MainGUILayer::update));

	return true;
}

void MainGUILayer::update(float deltaTime)
{
	// Cocos2d-x-2.2.2에서는 키보드 입력에 대한 지원을 하지 않는다. (Cocos2d-x-3부터는 지원)
#ifndef _WIN32
	// Win32가 아닌 플랫폼에서는 화면 상의 방향 메뉴를 선택해서 움직이도록 구현하였다
	g_World->m_upMenuItemSelected = m_upMenuItem->isSelected();
	g_World->m_leftMenuItemSelected = m_leftMenuItem->isSelected();
	g_World->m_rightMenuItemSelected = m_rightMenuItem->isSelected();
#else
	// Win32에서는 화면 상의 방향 메뉴 뿐만아니라 키보드 입력 시에도 움직이도록 구현하였다
	g_World->m_upMenuItemSelected = m_upMenuItem->isSelected() || GetAsyncKeyState(VK_UP);
	g_World->m_leftMenuItemSelected = m_leftMenuItem->isSelected() || GetAsyncKeyState(VK_LEFT);
	g_World->m_rightMenuItemSelected = m_rightMenuItem->isSelected() || GetAsyncKeyState(VK_RIGHT);
#endif

	CClient* client = g_World->GetClientUnderUserControl();
	m_ipLabel->setString(client->GetLocalAddress().GetString());
}

void MainGUILayer::menuApplyCallback(CCObject* pSender)
{
	// 입력한 클라 갯수의 값을 얻는다.
	// Get value of entered client number.
	CCEditBox* editBox = (CCEditBox*)getChildByTag(ClientCount);
	int desiredCount = atoi(editBox->getText());
	desiredCount = kmMax(1, desiredCount);

	g_World->m_desiredClientCount = desiredCount;
}
