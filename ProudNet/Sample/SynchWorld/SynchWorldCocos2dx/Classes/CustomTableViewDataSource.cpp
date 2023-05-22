#include "stdafx.h"

#include "CustomTableViewDataSource.h"
#include "World.h"


CEventLogTableViewDataSource::CEventLogTableViewDataSource(void)
{
	m_eventLogList = CCArray::create();
	m_eventLogList->retain();
}

CEventLogTableViewDataSource::~CEventLogTableViewDataSource(void)
{
	if (m_eventLogList)
	{
		m_eventLogList->release();
		m_eventLogList = NULL;
	}
}

cocos2d::CCSize CEventLogTableViewDataSource::cellSizeForTable(CCTableView *table)
{
	// Table의 Cell의 크기를 반환한다.
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	return CCSizeMake(visibleSize.width/2, CWorld::DefaultFontSize/2);
}

CCTableViewCell* CEventLogTableViewDataSource::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCTableViewCell* tableViewCell = table->dequeueCell();
	CCString* txt = (CCString*)m_eventLogList->objectAtIndex(idx);

	// Cell이 생성되지 않은 경우에는 새롭게 생성한다.
	if (tableViewCell == NULL)
	{
		tableViewCell = new CCTableViewCell();
		tableViewCell->autorelease();

		CCLabelTTF* label = CCLabelTTF::create(
			txt->getCString(),
			"Arial",
			CWorld::DefaultFontSize/2,
			CCSizeMake(visibleSize.width/2, CWorld::DefaultFontSize/2),
			kCCTextAlignmentLeft,
			kCCVerticalTextAlignmentCenter);
		label->setPosition(ccp(label->getDimensions().width/2, 0));
		label->setTag(EventLog);

		tableViewCell->addChild(label);
	}
	else
	{
		// Cell이 존재하는 경우 해당 idx의 문자열을 가져와 설정한다.
		CCLabelTTF* label = (CCLabelTTF*)tableViewCell->getChildByTag(EventLog);
		label->setString(txt->getCString());
	}

	return tableViewCell;
}

unsigned int CEventLogTableViewDataSource::numberOfCellsInTableView(CCTableView *table)
{
	return m_eventLogList->count();
}


CClientControlTableViewDataSoruce::CClientControlTableViewDataSoruce(void)
{
	m_clientControlList = CCArray::create();
	m_clientControlList->retain();
}

CClientControlTableViewDataSoruce::~CClientControlTableViewDataSoruce(void)
{
	if (m_clientControlList)
	{
		m_clientControlList->release();
		m_clientControlList = NULL;
	}
}

void CClientControlTableViewDataSoruce::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	int clinetUnderUserControl = g_World->m_clientUnderUserControl;

	// 이전의 Index와 현재 선택된 Index가 다른 경우에만 처리한다.
	if (clinetUnderUserControl != cell->getIdx())
	{
		CCTexture2D* checkBoxNormal	= CCTextureCache::sharedTextureCache()->addImage("CheckboxNormal.png");
		CCTexture2D* checkBoxSelected = CCTextureCache::sharedTextureCache()->addImage("CheckboxSelected.png");	

		CCSprite* checkBoxSprite;

		// 이전에 선책하였던 Cell의 정보를 가져온다.
		CCTableViewCell* previousCell = table->cellAtIndex(clinetUnderUserControl);

		if (previousCell)
		{
			checkBoxSprite = (CCSprite*)previousCell->getChildByTag(ClientControlCheckBox);

			if (checkBoxSprite)
			{
				// 이전에 체크되어있던 Cell의 스프라이트를 일반 스프라이트로 변경한다.
				checkBoxSprite->setTexture(checkBoxNormal);
			}
		}

		// 사용자가 Client를 변경하였음을 알려준다.
		g_World->SetClientUnderUserControl(cell->getIdx());

		checkBoxSprite = (CCSprite*)cell->getChildByTag(ClientControlCheckBox);

		if (checkBoxSprite)
		{
			// 현재 선택된 Cell의 스프라이트를 선택된 스프라이트로 변경한다.
			checkBoxSprite->setTexture(checkBoxSelected);
		}
	}
}

CCSize CClientControlTableViewDataSoruce::cellSizeForTable(CCTableView *table)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	return CCSizeMake(visibleSize.width/5, CWorld::DefaultFontSize/2);
}

CCTableViewCell* CClientControlTableViewDataSoruce::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCTableViewCell* tableViewCell = table->dequeueCell();
	CCString* txt = (CCString*)m_clientControlList->objectAtIndex(idx);

	// Cell이 생성되지 않은 경우에는 새롭게 생성한다.
	if (tableViewCell == NULL)
	{
		tableViewCell = new CCTableViewCell();
		tableViewCell->autorelease();

		CCSprite* checkBoxSprite = CCSprite::create("CheckboxNormal.png");
		checkBoxSprite->setPosition(ccp(checkBoxSprite->getContentSize().width/2, 0));
		checkBoxSprite->setScale(0.5);
		checkBoxSprite->setTag(ClientControlCheckBox);

		CCLabelTTF* label = CCLabelTTF::create(
			txt->getCString(),
			"Arial",
			CWorld::DefaultFontSize/2,
			CCSizeMake(visibleSize.width/5, CWorld::DefaultFontSize/2),
			kCCTextAlignmentLeft,
			kCCVerticalTextAlignmentCenter);
		label->setPosition(ccp(checkBoxSprite->getContentSize().width + label->getDimensions().width/2, 0));
		label->setTag(ClientControl);

		tableViewCell->addChild(checkBoxSprite);
		tableViewCell->addChild(label);
	}
	else
	{
		CCTexture2D* textureCache;

		// 사용자가 컨트롤하는 Client의 인덱스와 Cell의 인덱스의 일치 여부에 따라서 스프라이트를 가져온다.
		if (g_World->m_clientUnderUserControl == idx)
		{
			textureCache = CCTextureCache::sharedTextureCache()->addImage("CheckboxSelected.png");
		}
		else
		{
			textureCache = CCTextureCache::sharedTextureCache()->addImage("CheckboxNormal.png");
		}

		CCSprite* checkBoxSprite = (CCSprite*)tableViewCell->getChildByTag(ClientControlCheckBox);

		if (checkBoxSprite)
		{
			checkBoxSprite->setTexture(textureCache);
		}

		CCLabelTTF* label = (CCLabelTTF*)tableViewCell->getChildByTag(ClientControl);
		label->setString(txt->getCString());
	}

	return tableViewCell;
}

unsigned int CClientControlTableViewDataSoruce::numberOfCellsInTableView(CCTableView *table)
{
	return m_clientControlList->count();
}
