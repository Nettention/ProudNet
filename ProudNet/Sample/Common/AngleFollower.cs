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
	특정한 각도를 향해 부드럽게 각도가 변화하도록 한다.
	각도의 부드러운 움직임을 구현할 때 사용하는 클래스이다.
	예를 들어, remote character의 방향 전환이 부드럽게 이루어지는 처리 등에 효과적이다.

	일반적인 사용법:
	- FollowerAngle로, follower angle을 넣는다. (보통 초기 1회)
	- TargetAngle로, target angle을 지정한다. 통상 매 framemove마다 한다.
	- FrameMove()을 매 프레임마다 호출한다.
	- 매 프레임미다 TargetAngle로 follower angle을 얻는다.
     
	\~english
	This method helps changing an angle more smoothly.
	This class is helpful and effective in handling smooth direction change of remote character.

	General Usage:
	- Insert a follower angle with FollowerAngle (Normally at one inital time)
	- Assign a target angle with TargetAngle. Normally set a target at every each framemove.
	- Calll for FrameMove() at each frame.
	- Get a follower angle by TargetAngle at each frame.
      
	\~chinese
	使其向一个特定角度进行流畅的角度变换。
	是在体现流畅的角度变换时所使用的类。
	如在处理流畅的remote character方向变换时，此方法将会有效。

	一般使用方法：
	- 向FollowerAngle插入 follower angle。 (一般在第一次开始时的一次)
	- 向TargetAngle指定 target angle。一般每在进行 framemove时进行。
	- 在每个Frame中呼出FrameMove()。
	- 在每个Frame中用 TargetAngle获取 follower angle。

	\~japanese
    
	\~
	 */
	public class AngleFollower
	{
		double followerAngle = 0;
		double targetAngle = 0;
		double followerAngleVelocity = 0;

		public void FrameMove(double elapsedTime)
		{
			// target에 가장 가까운 follower 위치를 재조정한다.
			followerAngle = followerAngle % (2 * Sysutil.PROUDNET_PI);
			if (targetAngle + Sysutil.PROUDNET_PI < followerAngle)
				followerAngle -= 2 * Sysutil.PROUDNET_PI;
			if (targetAngle - Sysutil.PROUDNET_PI > followerAngle)
				followerAngle += 2 * Sysutil.PROUDNET_PI;

			// 이제 이동시킨다
			double moveAmount = elapsedTime * followerAngleVelocity;
			double diff = targetAngle - followerAngle;

			if (diff < 0)
				diff = Math.Max(-moveAmount, diff);
			else
				diff = Math.Min(moveAmount, diff);

			followerAngle += diff;
		}

		public double FollowerAngle
		{
			set { followerAngle = value; }
			get { return followerAngle; }
		}

		public double TargetAngle
		{
			set { targetAngle = value % (Sysutil.PROUDNET_PI * 2); }
			get { return targetAngle; }
		}


		public double FollowerAngleVelocity
		{
			set { followerAngleVelocity = Math.Max(Sysutil.PROUDNET_PI / 180 * 0.001f, value); }
			get { return followerAngleVelocity; }
		}
	}
	/**  @} */
}
