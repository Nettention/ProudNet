#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;


class CEventLogTableViewDataSource : public CCTableViewDataSource, public CCTableViewDelegate
{
	enum { EventLog = 1 };
public:
	CEventLogTableViewDataSource(void);
	~CEventLogTableViewDataSource(void);

	// CCScrollViewDelegate
	virtual void scrollViewDidScroll(CCScrollView* view) {}
	virtual void scrollViewDidZoom(CCScrollView* view) {}

	// CTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell) {}
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell) {}
	virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell) {}

	CCArray* m_eventLogList;
};

class CClientControlTableViewDataSoruce : public CCTableViewDataSource, public CCTableViewDelegate
{
	enum { ClientControlCheckBox = 1, ClientControl };
public:
	CClientControlTableViewDataSoruce(void);
	~CClientControlTableViewDataSoruce(void);

	// CCScrollViewDelegate
	virtual void scrollViewDidScroll(CCScrollView* view) {}
	virtual void scrollViewDidZoom(CCScrollView* view) {}

	// CTableViewDelegate
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);

	virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell) {}
	virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell) {}
	
	CCArray* m_clientControlList;
};