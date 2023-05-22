#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;


class ConnectLayer : public CCLayer, public CCEditBoxDelegate
{
	enum { ServerAddress = 1, UserName };
public:
	virtual bool init();

	// implement the "static node()" method manually
	CREATE_FUNC(ConnectLayer);

	// update
	void update(float deltaTime);

	// a selector callback
	void menuConnectCallback(CCObject* pSender);
	void menuCloseCallback(CCObject* pSender);

	// EditBoxDelegate
	virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
	virtual void editBoxReturn(CCEditBox* editBox) {}

public:
	CCLabelTTF* m_connectMenuItemLabel;
};

