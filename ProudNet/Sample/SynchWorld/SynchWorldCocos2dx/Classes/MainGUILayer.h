#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#include "CustomTableViewDataSource.h"


class MainGUILayer : public CCLayer, public CCEditBoxDelegate
{
	enum { ClientCount };

	CCLabelTTF* m_ipLabel;

public:
	virtual bool init();

	// implement the "static node()" method manually
	CREATE_FUNC(MainGUILayer);

	// update
	void update(float deltaTime);

	// a selector callback
	void menuApplyCallback(CCObject* pSender);

	// EditBoxDelegate
	virtual void editBoxReturn(CCEditBox* editBox) {}

public:
	CCMenuItemImage* m_upMenuItem;
	CCMenuItemImage* m_leftMenuItem;
	CCMenuItemImage* m_rightMenuItem;

	// 클라이언트 갯수 라벨.
	// Client count label.
	CCLabelTTF* m_clientCountLabel;

	// EventLog
	CCTableView* m_eventLogTableView;
	CEventLogTableViewDataSource m_eventLogTableViewDataSource;

	// ClientControl
	CCTableView* m_clientControlTableView;
	CClientControlTableViewDataSoruce m_clientControlTableViewDataSoruce;
};

