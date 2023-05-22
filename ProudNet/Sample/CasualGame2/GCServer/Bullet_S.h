#pragma once
#include "../gccommon/vars.h"


class CBattleServer;

class CBullet_S :public CBullet
{
public:
	CBullet_S(CBattleServer* owner);
	~CBullet_S(void);
public:
	void FrameMove(float m_ElapsedTime);
	CBattleServer* m_owner;
	double m_AccumTimeForMoveBullet;
};
typedef RefCount<CBullet_S> Bullet_S_Ptr;