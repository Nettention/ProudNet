#pragma once

#include "cocos2d.h"
USING_NS_CC;


class MainLayer : public CCLayer
{
public:
	virtual bool init();

	// implement the "static node()" method manually
	CREATE_FUNC(MainLayer);

	// update
	void update(float deltaTime);

public:
	// ����ȭ���� ��������Ʈ.
	// SynchArea Sprite.
	CCSprite* m_synchAreaSprite;
};

