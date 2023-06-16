#include <math.h>
#include "AngleFollower.h"

#define PROUDNET_PI    (3.14159265358979323846)

namespace Proud
{
	CAngleFollower::CAngleFollower(void)
	{
		m_followerAngle = 0;
		m_targetAngle = 0;
		m_followerAngleVelocity = 0;
	}

	CAngleFollower::~CAngleFollower(void)
	{
	}

	/**
	매 프레임마다 이 메서드를 호출해야 follower가 값이 계속 변한다.
	\param elapsedTime 프레임 처리 시간(in sec)
	*/
	void CAngleFollower::FrameMove(double elapsedTime)
	{
		// target에 가장 가까운 follower 위치를 재조정한다.
		m_followerAngle = fmod(m_followerAngle, 2 * PROUDNET_PI);
		if (m_targetAngle + PROUDNET_PI < m_followerAngle)
			m_followerAngle -= 2 * PROUDNET_PI;
		if (m_targetAngle - PROUDNET_PI > m_followerAngle)
			m_followerAngle += 2 * PROUDNET_PI;

		// 이제 이동시킨다
		double moveAmount = elapsedTime * m_followerAngleVelocity;
		double diff = m_targetAngle - m_followerAngle;

		if (diff < 0)
			diff = PNMAX(-moveAmount, diff);
		else
			diff = PNMIN(moveAmount, diff);

		m_followerAngle += diff;
	}

	/**
	* follower angle을 세팅한다.
	* \param value 각도(in radian)
	*/
	void CAngleFollower::SetFollowerAngle(double value)
	{
		m_followerAngle = value;
	}

	/**
	* follower angle을 구한다.
	* \return 각도(in radian)
	*/
	double CAngleFollower::GetFollowerAngle()
	{
		return m_followerAngle;
	}

	/**
	* target angle을 세팅한다.
	* \param value 각도(in radian)
	*/
	void CAngleFollower::SetTargetAngle(double value)
	{
		m_targetAngle = fmod(value, PROUDNET_PI * 2);
	}

	/**
	* target angle을 구한다.
	* \return 각도(in radian)
	*/
	double CAngleFollower::GetTargetAngle()
	{
		return m_targetAngle;
	}

	/**
	* follower의 이동 각속도를 넣는다.
	* \param value 각속도(in radian/sec)
	*/
	void CAngleFollower::SetFollowerAngleVelocity(double value)
	{
		// 360도에 정확히 맞아 떨어지는 값이 입력된 경우라도 커버해야 하므로
		//m_followerAngleVelocity=fmodf(value,PROUDNET_PI*2);
		m_followerAngleVelocity = PNMAX(PROUDNET_PI / 180 * 0.001f, value);
	}

	/**
	* follower의 이동 각속도를 구한다.
	* \return 각속도(in radian/sec)
	*/
	double CAngleFollower::GetFollowerAngleVelocity()
	{
		return m_followerAngleVelocity;
	}

}