#include "StdAfx.h"
#include "ITem_S.h"

CItem_S::CItem_S(void)
{
	m_savingTime = 0;
}

CItem_S::~CItem_S(void)
{
}

bool CItem_S::FrameMove(double m_ElapsedTime)
{
	m_savingTime += m_ElapsedTime;
	if(m_type == LIFEDECREASE && m_savingTime >=5)
	{
		m_savingTime = 0;
		return true;
	}
	else if(m_savingTime >=10)
	{
		m_savingTime = 0 ;
		return true;
	}
	return false;
}