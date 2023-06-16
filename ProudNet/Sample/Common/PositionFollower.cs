/*
ProudNet

이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의: 저작물에 관한 위의 명시를 제거하지 마십시오.

ProudNet

This program is soley copyrighted by Nettention. 
Any use, correction, and distribution of this program are subject to the terms and conditions of the License Agreement.
Any violated use of this program is prohibited and will be cause of immediate termination according to the License Agreement.

** WARNING : PLEASE DO NOT REMOVE THE LEGAL NOTICE ABOVE. 

ProudNet

此程序的版权归Nettention公司所有。
与此程序的修改、使用、发布相关的事项要遵守此程序的所有权者的协议。
不遵守协议时要原则性的禁止擅自使用。
擅自使用的责任明示在与此程序所有权者的合同书里。

** 注意：不要移除关于制作物的上述明示。

*/
using System;
using System.Collections.Generic;
using System.Text;

namespace Nettention.Proud
{
	/** \addtogroup util_group
	*  @{
	*/

	/** 
	\~korean
	remote object의 jittering되는 position,velocity를 linear interpolation으로
	smoothing 처리하는 class이다.

	예를 들어, remote object를 dead reckoning을 할 때 유용하다.

	(나중에, 이 class를 보강해서, orientation을 quaternion 등으로 interpolation하는
	기능이 추가되어도 쓸만하겠다.)

	중력에 대한 고려 사항
	- 점프 등이 있는 게임에서는 캐릭터가 점프 중 직선 이동이 어색할 수밖에 없다.
	따라서 점프중에는 중력이 작용해서 계속해서 하향 곡선을 그려야 한다.
	이럴 때 SetGravity를 통해 중력 값을 미리 넣어둘 수 있다. 

	\~english
	Class that smoothing processes position,velocity of remote object that are 'jittering'ed into linear interpolation

	For an example, it is useful when dead reckoning remote object.

	(Later, it'd be useful if this class is strengthened to have interpolation function that converts orientation into quarternion.)
	
	Considerations on gravity
	- In a game that has jump motions, the character jumps while moving in linear fashion must be somewhat un-natural.
	So, during jump motion, there must be gravity acting to result downward curve motion.
	In that case, you can use SetGravity to pre-input the gravity value.

	\~chinese
	对由remote object进行jittering的position,velocity，用linear interpolation进行
	smoothing 处理的 class。

	如将remote object进行dead reckoning时会有用。

	(以后，补充此Class，添加用quaternion等对orientation进行interpolation的功能也可能会有所帮助。)

	对重力的考虑事项
	- 在有跳跃等功能的游戏中，角色在进行跳跃时直线移动必定会不自然。
	因此在进行跳跃时，要发挥重力的作用画出下向曲线。
	此时可以通过SetGravity提前输入重力值。 

	\~japanese

	\~
	*/
	public class PositionFollower
	{
		Vector3 targetPos;
		Vector3 targetVel;
		Vector3 followerPos;
		Vector3 followerVel;
		Vector3 gravity;
		double followDuration = 0.2;
		bool used = false;
		double targetPosOrVelSetElapsedTime = 0;
		bool autoFollowDuration = true;

		Vector3[] splineCoord = new Vector3[4];
		double splineTimeslice = 0;
		double splineTerm = 0;
		Vector3 splineFollowerVel;
		Vector3 splineFollowerPos;

		double warpThresholdSqu = 0;

		static readonly double veryLowAngleCos = Math.Cos(Sysutil.PROUDNET_PI / 180 * 1);

		private double SplineCoord23Term
		{
			get
			{
				return Math.Max(0.01, followDuration * .66);
			}
		}

		private double SplineCoord01Term
		{
			get { return Math.Max(0.01, followDuration * .66); }
		}

		private double SplineMaxTimeslice
		{
			get { return followDuration * 1.1; }
		}

		private Vector3 AdjustedSplineFollowerVel
		{
			get
			{
				double targetSpeed = targetVel.Length;

				Vector3 v1 = splineFollowerVel;
				Vector3 v2 = v1.Normal * targetSpeed;

				if (v1.Length > targetSpeed)
					return v2;

				return v1;
			}
		}

		private double SplineCoord23FutureTerm
		{
			get { return Math.Max(0.1, followDuration * 2); }
		}

		/** 
		\~korean
		FollowerVelocity,FollowerPosition을 한번에 모두 호출한다. 

		\~english
		Calls FollowerVelocity,FollowerPosition at the same time

		\~chinese
		一次性呼出所有FollowerVelocity,FollowerPosition。

		\~japanese

		\~
		*/
		public void SetFollower(Vector3 position, Vector3 velocity)
		{
			bool posChanged = false, velChanged = false;

			if (followerPos != position)
			{
				followerPos = position;
				posChanged = true;
			}

			if (followerVel != velocity)
			{
				followerVel = velocity;
				velChanged = true;
			}

			if (posChanged || velChanged)
			{
				splineCoord[0] = position;
				splineCoord[1] = position + velocity * SplineCoord01Term;
				splineTimeslice = 0;
				splineTerm = SplineMaxTimeslice;
			}
		}

		/** 
		\~korean
		TargetPosition과 TargetVelocity를 한번에 모두 호출한다.

		\~english
		Calls TargetPosition and TargetVelocity at the same time

		\~chinese
		一次性呼出所有的TargetPosition和TargetVelocity。

		\~japanese

		\~
		*/
		public void SetTarget(Vector3 position, Vector3 velocity)
		{
			bool posChanged = false, velChanged = false;
			if (targetPos != position)
			{
				posChanged = true;
			}
			if (targetVel != velocity)
			{
				velChanged = true;
			}

			// 처음 세팅되는 것이라면 follower도 변경하도록 한다.
			if (used == false)
			{
				followerPos = position;
				followerVel = velocity;
			}

			// duration을 필요시 재조정한다.
			if (autoFollowDuration && targetPosOrVelSetElapsedTime > 0)
			{
				double localtargetPosOrVelSetElapsedTime = this.targetPosOrVelSetElapsedTime;

				followDuration = Sysutil.Lerp(followDuration, localtargetPosOrVelSetElapsedTime * 1.3, 0.8);
			}
			targetPosOrVelSetElapsedTime = 0;

			if (posChanged || velChanged)
			{
				/* 수신 시간의 노이즈 보정: 자세한 것은 dead reckon 별도 노트 참조 */
				Vector3 futurePos0 = targetPos + targetVel * followDuration;
				Vector3 futurePos1 = position + velocity * followDuration;
				Vector3 futurePos01 = futurePos1 - futurePos0;
				double targetSpeed = targetVel.Length;

				if (targetSpeed > 0 &&
					Vector3.Dot(-futurePos01.Normal, velocity.Normal) > veryLowAngleCos &&
					futurePos01.Length / targetSpeed < 0.1)
				{
					position -= futurePos01;
				}
			}

			// 이제 세팅한다.
			targetPos = position;
			targetVel = velocity;

			if (posChanged || velChanged)
			{
				splineCoord[0] = splineFollowerPos;
				splineCoord[1] = splineFollowerPos + AdjustedSplineFollowerVel * SplineCoord01Term;
				splineCoord[2] = position + velocity * followDuration;
				splineCoord[3] = splineCoord[2] - velocity * SplineCoord23Term;

				splineTimeslice = 0;
				splineTerm = SplineMaxTimeslice;
			}
		}

		/** 
		\~korean
		매 타임 프레임마다 이 메서드를 호출해야 한다.
		\param elapsedTime 한 프레임의 시간(초) 

		\~english
		This method must be called by each time frame.
		\param elapsedTime length of a frame(in second)

		\~chinese
		需要在每Time Frame中呼出此方法。
		\param elapsedTime Frame的时间(秒) 

		\~japanese

		\~
		*/
		public void FrameMove(double elapsedTime)
		{
			Vector3 diff = targetPos - followerPos; // 현재 target, follower의 pos 차이
			double diff_squ = diff.LengthSq;

			// target과 follower의 거리의 제곱이 warpThresholdSqu를 넘으면 warp를 시킨다.
			if (warpThresholdSqu > 0 && diff_squ > warpThresholdSqu)
			{
				EqualizeFollowerToTarget();
			}
			else
			{
				// 백업
				Vector3 targetPos_prev = targetPos;
				Vector3 followerPos_prev = followerPos;

				// 부드럽게 target pos의 예측 위치로 선형 이동을 시킨다.
				targetVel = targetVel + gravity * elapsedTime;
				targetPos = targetPos + targetVel * elapsedTime;

				targetPosOrVelSetElapsedTime += elapsedTime;

				// follower duration까지의 잔여 시간에 follower target에 도착할 수 있는 속도를 구한다.
				// 단, 잔여 시간이 너무 작으면 follower가 지나치게 멀리 뛰어버릴 수 있으므로 한 프레임
				// 시간이라는 최소 시간을 보장하도록 한다.
				double durationRemaining = followDuration - targetPosOrVelSetElapsedTime;
				durationRemaining = Math.Max(durationRemaining, elapsedTime);

				Vector3 targetPredictPos = targetPos + targetVel * durationRemaining;

				followerVel = (targetPredictPos - followerPos) / durationRemaining;


				// target speed보다는 커야 '정지되는 remote object가 급 서서히 목적지에 가 버리는 현상'을 막는다.
				double targetSpeed = targetVel.Length;
				if (followerVel.Length < targetSpeed)
				{
					followerVel.Length = targetSpeed;
				}


				followerVel = followerVel + gravity * elapsedTime;
				followerPos = followerPos + followerVel * elapsedTime;


				/* 이미 follower가 target을 교차해 지나가 버린 경우, FrameMove 하기 전보다 서로간 거리가 
				더 벌어지게 된다. 이러한 경우 follower는 target과 강제 일치시켜 버리자. */
				double prevDistanceSq = (targetPos_prev - followerPos_prev).Length;
				double currDistanceSq = (targetPos - followerPos).Length;
				if (currDistanceSq > prevDistanceSq)
				{
					EqualizeFollowerToTarget();
				}
			}

			used = true;

			if (splineTimeslice >= splineTerm)
			{
				splineFollowerPos = targetPos;
				splineFollowerVel = targetVel;
			}
			else
			{
				double t = splineTimeslice / splineTerm;

				// http://www.saltire.com/applets/advanced_geometry/spline/spline.htm
				// AE = t AB ; BF = t BC; CG = t CD; EH = t EF; FI = t FG; HJ = t HI
				Vector3 A, B, C, D, E, F, G, H, I, J;
				A = splineCoord[0];
				B = splineCoord[1];
				C = splineCoord[2];
				D = splineCoord[3];
				E = Vector3.Lerp(A, B, t);
				F = Vector3.Lerp(B, C, t);
				G = Vector3.Lerp(C, D, t);
				H = Vector3.Lerp(E, F, t);
				I = Vector3.Lerp(F, G, t);
				J = Vector3.Lerp(H, I, t);

				Vector3 prevSplineFollowerPos = splineFollowerPos;
				splineFollowerPos = J;

				if (elapsedTime > 0)
				{
					splineFollowerVel = (splineFollowerPos - prevSplineFollowerPos) / elapsedTime;
				}
				splineTimeslice += elapsedTime;
			}
		}

		private void EqualizeFollowerToTarget()
		{
			followerPos = targetPos;
			followerVel = targetVel;

			splineCoord[0] = targetPos;
			splineCoord[1] = targetPos + targetVel * SplineCoord01Term;
			splineTerm = splineTimeslice = SplineMaxTimeslice;
		}

		/** 
		\~korean
		follower의 position, velocity를 구한다. 

		\~english
		Gets position, velocity of follower

		\~chinese
		求follower的 position, velocity。

		\~japanese

		\~
		*/
		public void GetFollower(ref Vector3 position, ref Vector3 velocity)
		{
			position = followerPos;
			velocity = followerVel;
		}


		/** 
		\~korean
		follower가 따라가는 target의 pos, vel을 구한다. 

		\~english
		Gets pos, vel of target that follower follows

		\~chinese
		求follower跟随的target的pos,vel。

		\~japanese

		\~
		*/
		public void GetTarget(ref Vector3 position, ref Vector3 velocity)
		{
			position = targetPos;
			velocity = targetVel;
		}

		/** 
		\~korean
		follower가 target까지 다다르는 시간을 설정하거나 얻습니다.
		- EnableAutoFollowDuration(true)를 호출한 상태이면 이 메서드는 실행이 무시된다. 대신, 내부적으로 적절한
		duration을 자동으로 찾는다.
		- 이 값은 일반적으로 SetTargetPosition,SetTarget,SetTargetVelocity가 호출되는 주기보다 약간 길게 잡는 것이 좋다.
		지나치게 작은 값을 넣으면 움직임이 너무 튀며, 너무 큰 값을 넣으면 움직임의 정확성이 떨어진다. 
		\param duration follower가 target까지 다다르는 시간(초단위) 

		\~english
		Sets or Gets the time taken that follower gets to target
		- This method will be ignored if EnableAutoFollowDuration(true) is called. Instead, it automatically finds suitable duration internally.
		- Usually, it is ideal to set this value a little bit larger than the period that calls SetTargetPosition,SetTarget,SetTargetVelocity.
		If the value is too small then the movement will be jumpy but if too large then the precision of movement will be lost.
		\param duration time taken that follower gets to target (in second)

		\~chinese
		设置或获取follower接近target时所需要的时间。.
		- 如果是要呼出EnableAutoFollowDuration(true)的状态，此方法会被无视。并且在内部自动查找适当的duration。
		- 此值最好设置成比SetTargetPosition,SetTarget,SetTargetVelocity被呼出的周期更长的时间。
		如果设置得过小，移动可能会频繁跳跃，如果设置过大移动的准确性将会下降。 
		\param duration follower接近 target时的所需时间（毫秒单位）。 

		\~japanese

		\~
		*/
		public double FollowDuration
		{
			get { return followDuration; }
			set
			{
				if (!autoFollowDuration)
					followDuration = value;
			}
		}

		/** 
		\~korean
		FrameMove()가 1회 이상 사용되었는가? 

		\~english
		FrameMove() used more than once?

		\~chinese
		使用了一次以上的 FrameMove()吗? 

		\~japanese

		\~
		*/
		public bool IsFirstUse
		{
			get { return !used; }
		}

		/** 
		\~korean
		만약 새로 받은 target position이 follower와의 거리차가 지나치게 큰 경우, 의도적 워프, 리스폰
		혹은 지나친 랙으로 인한 위치 오차일 수 있다. 이런 경우 위치 보정은 오히려 이상한 결과를 야기한다.
		warp threshold를 정해놓으면 지나치게 큰 위치 오차의 경우 보정을 하지 않고 강제로 follower를 target 위치와
		일치시킨다.
		\param warpThreshold 위치 보정을 무시할 수 있는 임계값으로서의 거리

		\~english
		If newly received target position is too far from the location of follower, it may be location error due to intentional warp, resapwn or extreme lags. In this case, performing location correction will make the situation worse.	
		if warp threshold is set then it does not perform correction but forcefully synchronizes follower with target location.
		\param warpThreshold distance as critical value that can ignore location correction

		\~chinese
		如果重新获得的target position与follower的距离差过大，则可能会是因意图性Warp，Resapwn或过大Lag而发生的位置误差。此时位置情报可能会引发异常结果。
		如果指定warp threshold，在发生过大位置误差时不会对此进行修改，而是强制将follower的位置统一到target的位置。
		\param warpThreshold 可以无视修改位置情报的临界值距离。

		\~japanese

		\~
		*/
		public double WarpThreshold
		{
			set { warpThresholdSqu = Math.Pow(value, 2); }
		}

		/** 
		\~korean
		position follower 자체의 위치를 지정하거나 얻습니다.
		- remote object의 초기 상태를 세팅할 때에 쓰인다.

		\~english
		Sets or Gets the location of position follower itself
		- Used to set the initial condition of remote object

		\~chinese
		指定或获取position follower本身位置。
		- 可以在设置remote object的初始状态时使用。 

		\~japanese

		\~
		*/
		public Vector3 FollowerPosition
		{
			get { return followerPos; }
			set { SetFollower(value, FollowerVelocity); }
		}

		/** 
		\~korean
		position follower 자체의 속도를 지정하거나 얻습니다.
		- remote object의 초기 상태를 세팅할 때에 쓰인다.

		\~english
		Sets or Gets the speed of position follower itself
		- Used to set the initial condition of remote object 

		\~chinese
		指定或获取position follower本身速度。
		- 可以在设置remote object的初始状态时使用。

		\~japanese

		\~
		*/
		public Vector3 FollowerVelocity
		{
			get { return followerVel; }
			set { SetFollower(FollowerPosition, value); }
		}

		/** 
		\~korean
		position follower가 쫓아갈 투명 오브젝트의 위치를 세팅하거나 얻습니다. 
		- 타 호스트로부터 상대방 캐릭터의 위치를 받으면 이 메서드를 호출할 필요가 있을 것이다. 

		\~english
		Sets or Gets the location of invisible object that position follower is to follow
		- When the location of opponent character is received from other host, there is a need to call this method.

		\~chinese
		设置或获取position follower要追踪的透明Object的位置。 
		- 如果从其他主机接收对方角色位置，则需要呼出此方法。 

		\~japanese

		\~
		*/
		public Vector3 TargetPosition
		{
			get { return targetPos; }
			set { SetTarget(value, TargetVelocity); }
		}

		/** 
		\~korean
		position follower가 쫓아갈 투명 오브젝트의 속도를 세팅하거나 얻습니다. 
		- 타 호스트로부터 상대방 캐릭터의 위치를 받으면 이 메서드를 호출할 필요가 있을 것이다. 

		\~english
		Sets or Gets the speed of invisible object that position follower is to follow 
		- When the location of opponent character is received from other host, there is a need to call this method.

		\~chinese
		设置或获取position follower要追踪的透明Object的速度。 
		- 如果从其他主机接收对方角色位置，则需要呼出此方法。

		\~japanese

		\~
		*/
		public Vector3 TargetVelocity
		{
			get { return targetVel; }
			set { SetTarget(TargetPosition, value); }
		}

		/** 
		\~korean
		중력을 세팅하거나 얻습니다.
		- 중력을 세팅하게 되면, follower와 target이 모두 영향을 받는다.
		예를 들어, 점프를 하는 캐릭터라면 이 메서드가 필요할 것이다. 

		\~english
		Sets or Gets gravity
		- Once gravity is set then both follower and taget are affected.
		For an example, if it is a jumping character then it will need this method.

		\~chinese
		设置或获取重力。
		- 如果设置重力，follower和 target也都会受到影响。
		如果是跳跃的角色，可能需要此方法。 

		\~japanese

		\~
		*/
		public Vector3 Gravity
		{
			get { return gravity; }
			set { gravity = value; }
		}

		/** 
		\~korean
		true로 세팅하면 follower가 target까지 다다르는 시간을 자동으로 조절한다. 

		\~english
		If set as true then it automatically controls the time taken for follower to get to taget.

		\~chinese
		设置为true的话，自动调节follower接近target时的所需时间。 

		\~japanese

		\~
		*/
		public bool EnableAutoFollowDuration
		{
			set { autoFollowDuration = value; }
		}

		/** 
		\~korean
		spline 형태로 쫓아가는 follower의 위치를 얻는다. 

		\~english
		Gets the location of follower that moves in spline shape

		\~chinese
		获取以spline形式追踪的follower位置。

		\~japanese

		\~
		*/
		public Vector3 SplineFollowerPosition
		{
			get { return splineFollowerPos; }
		}

		/** 
		\~korean
		spline 형태로 쫓아가는 follower의 현 속도를 얻는다. 

		\~english
		Gets the current speed of follower moves in spline shape

		\~chinese
		获取以spline形式追踪的follower现速度。

		\~japanese

		\~
		*/
		public Vector3 SplineFollowerVelocity
		{
			get { return splineFollowerVel; }
		}
	}

	/**	 @} */
}
