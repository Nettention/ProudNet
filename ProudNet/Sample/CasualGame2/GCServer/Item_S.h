#pragma once
#include "../gccommon/vars.h"

class CItem_S : public CItem
{
public:
	CItem_S(void);
	~CItem_S(void);
public:
	double m_savingTime;
	bool FrameMove(double m_ElapsedTime);
};
typedef RefCount<CItem_S> Item_S_Ptr;