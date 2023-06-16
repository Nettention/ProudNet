/* ProudNet

이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의: 저작물에 관한 위의 명시를 제거하지 마십시오.
*/

// xcode는 STL을 cpp에서만 include 가능 하므로
#include <stack>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <cstddef>
#include <iosfwd>
#include <algorithm>

#include "PositionFollower.h"

#include "NumUtil.h"

#ifdef max
#undef max
#endif

#define PROUDNET_PI    (3.14159265358979323846)

namespace Proud
{
	class CPositionFollower_Interior
	{
		friend class CPositionFollower;
	public:
		Vector3 m_targetPos, m_targetVel;  // follower가 따라가는 목적지 pos,vel
		Vector3 m_followerPos, m_followerVel; // follower pos,vel
		Vector3 m_gravity; // 중력. 어느 방향으로든 설정 가능. 점프 등 중력이 관여되는 부분을 위함
		double m_followDuration;	// follower가 target까지 도달하는 시간
		bool m_used;	// FrameMove가 사용된 적이 있는가?
		double m_targetPosOrVelSetElapsedTime;	// 마지막으로 taget pos and vel이 설정된지 지연된 시간
		bool m_autoFollowDuration;	// true이면 follow duration값이 자동으로 설정된다. taget pos and vel값이 자주 바뀔수록 follow duration을 짧게 바꿔주는 효과를 낸다.

		Vector3 m_splineCoord[4];
		double m_splineTimeslice;
		double m_splineTerm;
		Vector3 m_splineFollowerVel,m_splineFollowerPos;

		double m_warpThresholdSqu;

		CPositionFollower_Interior()
			: m_targetPos(0, 0, 0)
			, m_targetVel(0, 0, 0)
			, m_followerPos(0, 0, 0)
			, m_followerVel(0, 0, 0)
			, m_gravity(0, 0, 0)
			, m_followDuration(0.2f)
			, m_used(false)
			, m_targetPosOrVelSetElapsedTime(0)
			, m_autoFollowDuration(true)
			, m_splineTimeslice(0)
			, m_splineTerm(0)
			, m_warpThresholdSqu(0)
		{
		}


		double GetSplineCoord23Term() const
		{
			return std::max(0.01,m_followDuration*.66);
		}

		double GetSplineCoord01Term() const
		{
			return std::max(0.01,m_followDuration*.66);
		}

		double GetSplineCoord23FutureTerm() const
		{
			return std::max(0.1,m_followDuration*2);
		}

		double GetSplineMaxTimeslice() const
		{
			return m_followDuration*1.1;//max(GetSplineCoord23FutureTerm(),m_followDuration);
		}

		Vector3 GetAdjustedSplineFollowerVel() const
		{
			double targetSpeed=m_targetVel.GetLength();

			Vector3 v1=m_splineFollowerVel;
			Vector3 v2=v1.GetNormal() * targetSpeed;

			if(v1.GetLength() > targetSpeed)
				return v2;
			else
				return v1;
		}

		void SetFollower( const Vector3 &position, const Vector3 &velocity) 
		{
			bool posChanged=false,velChanged=false;

			if(m_followerPos!=position)
			{
				m_followerPos = position;
				posChanged=true;
			}

			if(m_followerVel != velocity)
			{
				m_followerVel=velocity;
				velChanged=true;
			}

			if(posChanged || velChanged)
			{
				m_splineCoord[0]=position;
				m_splineCoord[1]=position+GetSplineCoord01Term()*velocity;
				m_splineTimeslice=0;
				m_splineTerm=GetSplineMaxTimeslice();
			}

		}

		void SetTarget( Vector3 position, Vector3 velocity ) 
		{
			bool posChanged=false,velChanged=false;
			
			if(m_targetPos != position)
			{
				posChanged=true;
			}
			
			if(m_targetVel != velocity)
			{
				velChanged=true;
			}

			// 처음 세팅되는 것이라면 follower도 변경하도록 한다.
			if (m_used == false)
			{
				m_followerPos = position;
				m_followerVel = velocity;
			}

			// duration을 필요시 재조정한다.
			if(m_autoFollowDuration && m_targetPosOrVelSetElapsedTime>0)
			{
				double targetPosOrVelSetElapsedTime=m_targetPosOrVelSetElapsedTime;

				m_followDuration = Lerp(m_followDuration, targetPosOrVelSetElapsedTime*1.3,0.8);
			}

			m_targetPosOrVelSetElapsedTime=0;

			if(posChanged || velChanged)
			{
				/* 수신 시간의 노이즈 보정: 자세한 것은 dead reckon 별도 노트 참조 */
				Vector3 futurePos0 = m_targetPos + m_targetVel*m_followDuration;
				Vector3 futurePos1 = position + velocity* m_followDuration;
				Vector3 futurePos01= futurePos1-futurePos0;
				double targetSpeed = m_targetVel.GetLength();
				static double veryLowAngleCos=cos(PROUDNET_PI/180*1);
				if(targetSpeed>0 && 
					Vector3::Dot(-futurePos01.GetNormal(),velocity.GetNormal())>veryLowAngleCos &&
					futurePos01.GetLength()/targetSpeed<0.1)
				{
					position-=futurePos01;
					//  #ifdef _DEBUG
					//  				static int xxx=0;
					//  				xxx++;
					//  				NTTNTRACE("%d!\n",xxx);
					//  #endif
				}
			}

			// 이제 세팅한다.
			m_targetPos = position;
			m_targetVel = velocity;

			if(posChanged || velChanged)
			{
				m_splineCoord[0]=m_splineFollowerPos;
				m_splineCoord[1]=m_splineFollowerPos+ GetAdjustedSplineFollowerVel()*GetSplineCoord01Term();
				m_splineCoord[2]=position+m_followDuration*velocity;
				m_splineCoord[3]=m_splineCoord[2]-GetSplineCoord23Term()*velocity;

				m_splineTimeslice=0;
				m_splineTerm=GetSplineMaxTimeslice();
			}
		}

		void FrameMove(double elapsedTime)
		{
			Vector3 diff = m_targetPos - m_followerPos; // 현재 target, follower의 pos 차이
			double diff_squ = diff.GetLengthSq();

			// target과 follower의 거리의 제곱이 warpThresholdSqu를 넘으면 warp를 시킨다.
			if (m_warpThresholdSqu && diff_squ > m_warpThresholdSqu)
			{
				EqualizeFollowerToTarget();
			}
			else
			{
				// 백업
				Vector3 targetPos_prev = m_targetPos;
				Vector3 followerPos_prev = m_followerPos;

				// 부드럽게 target pos의 예측 위치로 선형 이동을 시킨다.
				m_targetVel += m_gravity * elapsedTime;
				m_targetPos += m_targetVel * elapsedTime;

				m_targetPosOrVelSetElapsedTime+=elapsedTime;

				// follower duration까지의 잔여 시간에 follower target에 도착할 수 있는 속도를 구한다.
				// 단, 잔여 시간이 너무 작으면 follower가 지나치게 멀리 뛰어버릴 수 있으므로 한 프레임
				// 시간이라는 최소 시간을 보장하도록 한다.
				double durationRemaining=m_followDuration-m_targetPosOrVelSetElapsedTime;
				durationRemaining=PNMAX(durationRemaining,elapsedTime);

				Vector3 targetPredictPos=m_targetPos+m_targetVel*durationRemaining;

				m_followerVel = (targetPredictPos - m_followerPos)/durationRemaining;

				// target speed보다는 커야 '정지되는 remote object가 급 서서히 목적지에 가 버리는 현상'을 막는다.
				double targetSpeed = m_targetVel.GetLength();
				if(m_followerVel.GetLength() < targetSpeed)
				{
					m_followerVel.SetLength(targetSpeed);
				}

				m_followerVel += m_gravity * elapsedTime;
				m_followerPos += m_followerVel * elapsedTime;

				/* 이미 follower가 target을 교차해 지나가 버린 경우, FrameMove 하기 전보다 서로간 거리가 
				더 벌어지게 된다. 이러한 경우 follower는 target과 강제 일치시켜 버리자. */
				double prevDistanceSq = (targetPos_prev-followerPos_prev).GetLength();
				double currDistanceSq = (m_targetPos-m_followerPos).GetLength();
				if(currDistanceSq>prevDistanceSq)
				{
					EqualizeFollowerToTarget();
				}
			}

			m_used = true;

			if(m_splineTimeslice >=m_splineTerm)
			{
				m_splineFollowerPos=m_targetPos;	
				m_splineFollowerVel=m_targetVel;	
			}
			else
			{
				double t=m_splineTimeslice / m_splineTerm;

				// http://www.saltire.com/applets/advanced_geometry/spline/spline.htm
				// AE = t AB ; BF = t BC; CG = t CD; EH = t EF; FI = t FG; HJ = t HI
				Vector3 A,B,C,D,E,F,G,H,I,J;
				A=m_splineCoord[0];
				B=m_splineCoord[1];
				C=m_splineCoord[2];
				D=m_splineCoord[3];
				E=Vector3::Lerp(A,B,t);
				F=Vector3::Lerp(B,C,t);
				G=Vector3::Lerp(C,D,t);
				H=Vector3::Lerp(E,F,t);
				I=Vector3::Lerp(F,G,t);
				J=Vector3::Lerp(H,I,t);

				Vector3 prevSplineFollowerPos=m_splineFollowerPos;
				m_splineFollowerPos=J;

				if(elapsedTime>0)
				{
					m_splineFollowerVel=(m_splineFollowerPos-prevSplineFollowerPos)/elapsedTime;
				}
				m_splineTimeslice+=elapsedTime;
			}

		}

		void EqualizeFollowerToTarget()
		{
			m_followerPos = m_targetPos;
			m_followerVel = m_targetVel;

			m_splineCoord[0]=m_targetPos;
			m_splineCoord[1]=m_targetPos+m_targetVel*GetSplineCoord01Term();
			m_splineTerm=m_splineTimeslice=GetSplineMaxTimeslice();
		}
	};


	void CPositionFollower::SetFollower(Vector3 position, Vector3 velocity)
	{
		m_interior->SetFollower(position,velocity);
	}

	void CPositionFollower::SetTarget(Vector3 position, Vector3 velocity)
	{
		m_interior->SetTarget(position,velocity);
	}
	
	// 이 함수는 수학에 관련된 것이고 elapsed time을 다루므로, int64 ms로 바꾸지 말 것. 
	void CPositionFollower::FrameMove(double elapsedTime)
	{
		m_interior->FrameMove(elapsedTime);
	}

	void CPositionFollower::GetFollower(Vector3 &position, Vector3 &velocity) const
	{
		position = m_interior->m_followerPos;
		velocity = m_interior->m_followerVel;
	}


	void CPositionFollower::GetTarget(Vector3 &position, Vector3 &velocity) const
	{
		position = m_interior->m_targetPos;
		velocity = m_interior->m_targetVel;
	}


	void CPositionFollower::SetFollowDuration(double duration)
	{
		if(!m_interior->m_autoFollowDuration)
			m_interior->m_followDuration = duration;
	}


	bool CPositionFollower::IsFirstUse()
	{
		return !m_interior->m_used;
	}

	CPositionFollower::CPositionFollower()
	{
		m_interior = new CPositionFollower_Interior;
	}

	CPositionFollower::~CPositionFollower()
	{
		delete m_interior;
	}

	void CPositionFollower::SetWarpThreshold(double warpThreshold)
	{
		// Mac or iOS or android math.h에는 pow template 함수가 없습니다.
		m_interior->m_warpThresholdSqu = pow(warpThreshold, 2);
	}

	Vector3 CPositionFollower::GetFollowerPosition() const
	{
		return m_interior->m_followerPos;
	}

	Vector3 CPositionFollower::GetFollowerVelocity() const
	{
		return m_interior->m_followerVel;
	}

	void CPositionFollower::SetFollowerPosition(const Vector3 &value)
	{
		SetFollower(value,GetFollowerVelocity());
	}

	void CPositionFollower::SetFollowerVelocity(const Vector3 &value)
	{
		SetFollower(GetFollowerPosition(),value);
	}

	Vector3 CPositionFollower::GetTargetPosition() const
	{
		//	return followerPosition;
		return m_interior->m_targetPos;
	}

	Vector3 CPositionFollower::GetTargetVelocity() const
	{
		return m_interior->m_targetVel;
	}

	Vector3 CPositionFollower::GetGravity() const
	{
		return m_interior->m_gravity;
	}

	void CPositionFollower::SetTargetPosition(const Vector3 &value)
	{
		SetTarget(value,GetTargetVelocity());

	}

	void CPositionFollower::SetTargetVelocity(const Vector3 &value)
	{
		SetTarget(GetTargetPosition(),value);
	}

	void CPositionFollower::SetGravity(const Vector3 &value)
	{
		m_interior->m_gravity = value;
	}

	void CPositionFollower::EnableAutoFollowDuration(bool val)
	{
		m_interior->m_autoFollowDuration=val;
	}


	double CPositionFollower::GetFollowDuration()
	{
		return m_interior->m_followDuration;
	}

	Vector3 CPositionFollower::GetSplineFollowerPosition() const
	{
		return m_interior->m_splineFollowerPos;
	}

	Vector3 CPositionFollower::GetSplineFollowerVelocity() const
	{
		return m_interior->m_splineFollowerVel;
	}

}
