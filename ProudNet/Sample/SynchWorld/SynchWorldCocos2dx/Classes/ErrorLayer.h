#pragma once

#include "cocos2d.h"
USING_NS_CC;


class ErrorLayer : public CCLayer
{
public:
	virtual bool init();

	// implement the "static node()" method manually
	CREATE_FUNC(ErrorLayer);

	// update
	void update(float deltaTime);

	// a selector callback
	void menuCloseCallback(CCObject* pSender);
};

