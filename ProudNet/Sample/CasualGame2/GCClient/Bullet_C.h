#pragma once

#include "../GCCommon/Vars.h"

class CBullet_C : public CBullet
{
public:
	CBullet_C(void);
	~CBullet_C(void);

	
	/**
	\~korean
	클라이언트에서는 총알을 그리는 역할만 합니다. 시뮬레이션은 서버에서 합니다.

	\~english
	It just draws bullet at client (simulation process at server).

	\~chinese
	客户端只负责画子弹，模拟在服务器上进行。

	\~japanese
	クライアントでは弾丸を描く役割だけをします。シミュレーションはサーバで行います。

	\~
	*/
	void Render();
	
	void FrameMove(float ElapsedTime);
	CPositionFollower m_PositionFollower;
};
typedef RefCount<CBullet_C> Bullet_C_Ptr;