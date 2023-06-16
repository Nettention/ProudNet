/*
ProudNet HERE_SHALL_BE_EDITED_BY_BUILD_HELPER


이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의 : 저작물에 관한 위의 명시를 제거하지 마십시오.


This program is soley copyrighted by Nettention.
Any use, correction, and distribution of this program are subject to the terms and conditions of the License Agreement.
Any violated use of this program is prohibited and will be cause of immediate termination according to the License Agreement.

** WARNING : PLEASE DO NOT REMOVE THE LEGAL NOTICE ABOVE.


此程序的版权归Nettention公司所有。
与此程序的修改、使用、发布相关的事项要遵守此程序的所有权者的协议。
不遵守协议时要原则性的禁止擅自使用。
擅自使用的责任明示在与此程序所有权者的合同书里。

** 注意：不要移除关于制作物的上述明示。


このプログラムの著作権はNettentionにあります。
このプログラムの修正、使用、配布に関する事項は本プログラムの所有権者との契約に従い、
契約を遵守しない場合、原則的に無断使用を禁じます。
無断使用による責任は本プログラムの所有権者との契約書に明示されています。

** 注意：著作物に関する上記の明示を除去しないでください。

*/

#pragma once

#include "BasicTypes.h"
#include "Vector3D.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
#if (_MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif
	/** \addtogroup util_group
	*  @{
	*/

	class CPositionFollower_Interior;

	/**
	\~korean
	remote object의 jittering되는 position,velocity를 linear interpolation으로
	smoothing 처리하는 class이다.

	예를 들어, remote object를 dead reckoning을 할 때 유용하다.

	(나중에, 이 class를 보강해서, orientation을 quaternion 등으로 interpolation하는
	기능이 추가되어도 쓸만하겠다.)

	일반적인 사용법
	- <a target="_blank" href="http://guide.nettention.com/cpp_ko#dead_reckoning" >추측 항법(dead reckoning)</a>  참고

	중력에 대한 고려 사항
	- 점프 등이 있는 게임에서는 캐릭터가 점프 중 직선 이동이 어색할 수밖에 없다.
	따라서 점프중에는 중력이 작용해서 계속해서 하향 곡선을 그려야 한다.
	이럴 때 SetGravity를 통해 중력 값을 미리 넣어둘 수 있다.

	\~english
	Class that smoothing processes position,velocity of remote object that are 'jittering'ed into linear interpolation

	For an example, it is useful when dead reckoning remote object.

	(Later, it'd be useful if this class is strengthened to have interpolation function that converts orientation into quarternion.)

	General usage
	- Please refer <a target="_blank" href="http://guide.nettention.com/cpp_en#dead_reckoning" >Dead Reckoning</a>

	Considerations on gravity
	- In a game that has jump motions, the character jumps while moving in linear fashion must be somewhat un-natural.
	So, during jump motion, there must be gravity acting to result downward curve motion.
	In that case, you can use SetGravity to pre-input the gravity value.

	\~chinese
	把remote object的jittering的position,velocity 用linear interpolation进行smoothing处理的class。

	例如把remote object进行dead reckoning的时候有用。

	（以后加强此类，把orientation用quaternion等进行interpolation的功能添加进去也不错嘛。）

	一般使用方法
	- 参考<a target="_blank" href="http://guide.nettention.com/cpp_  #dead_reckoning" >推测方法 (dead reckoning)</a>%。

	对重力的考虑事项。
	- 有跳跃等的游戏中角色跳跃中的直线移动必定会不自然。因此在跳跃中重力要起作用使其继续画出向下曲线。这时候可以通过SetGravity，预先输入重力值。

	\~japanese
	remote objectのjitteringされるposition、velocityをlinear interpolationでsmoothing処理するclassです。
	例えば、 remote objectでdead reckoningをする時に有用です。
	(後で、このclassを補強して、orientationをquaternionなどでinterpolationする機能が追加されても良いと思います)
	一般的な使用方法
	- \ref dead_reckoning  参考

	重力に対する考慮事項
	- ジャンプなどがあるゲームでは、キャラクターがジャンプ中に直線移動はぎこちないしかありません。
	よって、ジャンプ中には重力が作用して続けて下向き曲線を描く必要があります。
	このような時、SetGravityを通じ重力値を前もって入れて置く必要があります。
	\~
	*/
	class CPositionFollower
	{
		CPositionFollower_Interior* m_interior;

	public:
		 CPositionFollower();
		 ~CPositionFollower();

		/**
		\~korean
		중력을 세팅한다.
		- 중력을 세팅하게 되면, follower와 target이 모두 영향을 받는다.
		예를 들어, 점프를 하는 캐릭터라면 이 메서드가 필요할 것이다.

		\~english
		Sets gravity
		- Once gravity is set then both follower and taget are affected.
		For an example, if it is a jumping character then it will need this method.

		\~chinese
		设置重力。
		- 设置重力的话follower和target都会受到影响。
		例如，如果是需要跳跃的角色可能会需要此方法。

		\~japanese
		重力を設定します。
		- 重力を設定することになると、followerとtargetが全て影響を受けます。
		例えば、ジャンプするキャラクターならこのメソッドが必要です。
		\~
		*/
		 void SetGravity(const Vector3 &value);

		/**
		\~korean
		position follower가 쫓아갈 투명 오브젝트의 속도를 세팅한다.
		- 타 호스트로부터 상대방 캐릭터의 위치를 받으면 이 메서드를 호출할 필요가 있을 것이다.

		\~english
		Sets the speed of invisible object that position follower is to follow
		- When the location of opponent character is received from other host, there is a need to call this method.

		\~chinese
		设置position follower追随的透明object的速度。
		- 从其他主机收到对方角色的位置的话，会有必要呼叫此方法。

		\~japanese
		position followerがついて行く透明オブジェクトの速度を設定します。 
		- 他のホストから相手のキャラクターの位置を受けると、このメソッドを呼び出す必要があります。
		\~
		*/
		 void SetTargetVelocity(const Vector3 &value);

		/**
		\~korean
		position follower가 쫓아갈 투명 오브젝트의 위치를 세팅한다.
		- 타 호스트로부터 상대방 캐릭터의 위치를 받으면 이 메서드를 호출할 필요가 있을 것이다.

		\~english
		Sets the location of invisible object that position follower is to follow
		- When the location of opponent character is received from other host, there is a need to call this method.

		\~chinese
		设置position follower追随的透明object的位置。
		- 从其他主机收到对方角色的位置的话，会有必要呼叫此方法。

		\~japanese
		position followerがついて行く透明オブジェクトの位置を設定します。 
		- 他のホストから相手のキャラクターの位置を受けると、このメソッドを呼び出す必要があります。
		\~
		*/
		 void SetTargetPosition(const Vector3 &value);

		/**
		\~korean
		spline 형태로 쫓아가는 follower의 위치를 얻는다.

		\~english
		Gets the location of follower that moves in spline shape

		\~chinese
		获得以spline形式追随的follwer的位置。

		\~japanese
		Spline形態で付いて行くfollowerの位置を得ます。
		\~
		*/
		 Vector3 GetSplineFollowerPosition() const;

		/**
		\~korean
		spline 형태로 쫓아가는 follower의 현 속도를 얻는다.

		\~english
		Gets the current speed of follower moves in spline shape

		\~chinese
		获得以spline形式追随的follwer的现速度。

		\~japanese
		Spline形態で付いて行くfollowerの現在速度を得ます。
		\~
		*/
		 Vector3 GetSplineFollowerVelocity() const;

		/**
		\~korean
		마지막으로 입력한 중력 값을 얻는다.

		\~english
		Gets the last input value of gravity

		\~chinese
		获得最后输入的重力值。

		\~japanese
		最後に入力した重力値を得ます。
		\~
		*/
		 Vector3 GetGravity() const;

		/**
		\~korean
		position follower가 쫓아가는 목표 지점의 현재 속도를 얻는다.

		\~english
		Gets the current speed of target location that position follower is following

		\~chinese
		获得position follower追随去的目标地点的现速度。

		\~japanese
		position followerがついて行く目標地点の現在速度を得ます。
		\~
		*/
		 Vector3 GetTargetVelocity() const;

		/**
		\~korean
		position follower가 쫓아가는 목표 지점의 현재 위치를 얻는다.
		- SetTargetPosition에서 입력한 값과 다를 수 있다. 왜냐하면 FrameMove에 의해 지속적으로 갱신되기 때문이다.

		\~english
		Get the current location of target location that position follower is following
		- Can be different to the value entered in SetTargetPosition since it is continuously updated by FrameMove

		\~chinese
		获得position follower追随去的目标地点的现位置。
		- 可能与在SetTargetPosition输入的值不同。因为被FrameMove持续更新。

		\~japanese
		position followerがついて行く目標地点の現在位置を得ます。
		- SetTargetPositionで入力した値と違うことがあります。なぜなら、FrameMoveによって持続的に更新されるからです。
		\~
		*/
		 Vector3 GetTargetPosition() const;

		/**
		\~korean
		position follower 자체의 속도를 지정한다.
		- remote object의 초기 상태를 세팅할 때에 쓰인다.

		\~english
		Sets the speed of position follower itself
		- Used to set the initial condition of remote object

		\~chinese
		指定position follower本身的速度。
		- 使用于设置remote object的初始状态时。

		\~japanese
		position follower 自体の速度を指定します。
		- remote objectの初期状態を設定する時に使われます。
		\~
		*/
		 void SetFollowerVelocity(const Vector3 &value);

		/**
		\~korean
		position follower 자체의 위치를 지정한다.
		- remote object의 초기 상태를 세팅할 때에 쓰인다.

		\~english
		Sets the location of position follower itself
		- Used to set the initial condition of remote object

		\~chinese
		指定position follower本身的位置。
		- 使用于设置remote object的初始状态时。

		\~japanese
		position follower 自体の位置を指定します。
		- remote objectの初期状態を設定する時に使われます。
		\~
		*/
		 void SetFollowerPosition(const Vector3 &value);

		/**
		\~korean
		position follower 자체의 현재 속도를 얻는다.
		- SetFollowerVelocity에서 입력한 값과 다를 수 있다. 왜냐하면 FrameMove에 의해 지속적으로 갱신되기 때문이다.

		\~english
		Gets the current speed of position follower itself
		- Can be different to the value entered in SetFollowerVelocity since it is continuously updated by FrameMove

		\~chinese
		获得position follower本身的现速度。
		- 可能与在SetFollowerVelocity输入的值不同。因为被FrameMove持续更新。

		\~japanese
		position follower 自体の現在速度を得ます。
		- SetFollowerVelocityで入力した値と異なる場合があります。なぜなら、FrameMoveによって持続的に更新されるからです。
		\~
		*/
		 Vector3 GetFollowerVelocity() const;

		/**
		\~korean
		position follower 자체의 현재 위치를 얻는다.
		- SetFollowerPosition에서 입력한 값과 다를 수 있다. 왜냐하면 FrameMove에 의해 지속적으로 갱신되기 때문이다.

		\~english
		Gets the current location of position follower itself
		- Can be different to the value entered in SetFollowerPosition since it is continuously updated by FrameMove

		\~chinese
		获得position follower本身的现位置。
		- 可能与在SetFollowerPosition输入的值不同。因为被FrameMove持续更新。

		\~japanese
		position follower 自体の現在位置を得ます。
		- SetFollowerVelocityで入力した値と異なる場合があります。なぜなら、FrameMoveによって持続的に更新されるからです。
		\~
		*/
		 Vector3 GetFollowerPosition() const;

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
		如果新接收的target position与follower的距离之差过大的话，可能是因为warp，resapwn，或者过分的lag的位置误差。这时候位置校正反而引起奇怪的结果。
		确定warp threshold的话，出现过大的位置误差的情况时，不进行校正，而是强制性的把follower与target的位置进行适配。
		\param warpThreshold 作为可以无视位置校正临界值的距离。

		\~japanese
		もし、新たに受けたtarget positionがfollowerとの距離差があまりにも大きい場合、意図的なワープ、レスポーンまたはラックによる位置誤差であるかも知れません。このよな場合、位置補正はむしろおかしい結果を起こします。
		warp thresholdを決めて置けばあまりにも大きい位置誤差の場合、補正をしなく強制にfollower target位置と一致させます。
		\param warpThreshold 位置補正を無視できる臨界値としての距離
		\~
		*/
		 void SetWarpThreshold(double warpThreshold);

		/**
		\~korean
		FrameMove()가 1회 이상 사용되었는가?

		\~english
		FrameMove() used more than once?

		\~chinese
		FrameMove()被使用过一次以上吗？

		\~japanese
		FrameMove()が1回以上使われたのか？
		\~
		*/
		 bool IsFirstUse();

		/**
		\~korean
		follower가 target까지 다다르는 시간을 설정한다.
		- EnableAutoFollowDuration(true)를 호출한 상태이면 이 메서드는 실행이 무시된다. 대신, 내부적으로 적절한
		duration을 자동으로 찾는다.
		- 이 값은 일반적으로 SetTargetPosition,SetTarget,SetTargetVelocity가 호출되는 주기보다 약간 길게 잡는 것이 좋다.
		지나치게 작은 값을 넣으면 움직임이 너무 튀며, 너무 큰 값을 넣으면 움직임의 정확성이 떨어진다.
		\param duration follower가 target까지 다다르는 시간(초단위)

		\~english
		Sets the time taken that follower gets to target
		- This method will be ignored if EnableAutoFollowDuration(true) is called. Instead, it automatically finds suitable duration internally.
		- Usually, it is ideal to set this value a little bit larger than the period that calls SetTargetPosition,SetTarget,SetTargetVelocity.
		  If the value is too small then the movement will be jumpy but if too large then the precision of movement will be lost.
		\param duration time taken that follower gets to target (in second)

		\~chinese
		设置follower到达target的时间。
		- 如果是呼叫EnableAutoFollowDuration(true)的状态的话此方法的实行会被无视。不过在内部会自动寻找合适的duration。
		- 一般把此值设置成比SetTargetPosition,SetTarget,SetTargetVelocity 被呼叫的周期稍微长为好。
		输入过小值的话动作会太显眼，过大值的话动作的正确性会降低。
		\param duration follower到达target的时间（秒单位）。

		\~japanese
		Followerがtargetまで至る時間を設定します。
		- EnableAutoFollowDuration(true)を呼び出した状態であれば、このメソッドは実行が無視されます。代わりに、内部的に適切なdurationを自動に探します。
		- この値は一般的にSetTargetPosition,SetTarget,SetTargetVelocityが呼び出される周期より少し長く取った方が良いです。
		あまりにも小さい値を入れると動きがとても目立って、あまりにも大きい値を入れると動きの正確性が落ちます。
		\param duration followerがtargetまで至る時間(秒単位) 
		\~
		*/
		 void SetFollowDuration(double duration);
		/**
		\~korean
		마지막으로 SetFollowDuration으로 입력한 값을 출력한다.

		\~english
		Outputs the last value entered to SetFollowDuration

		\~chinese
		输出最后用SetFollowDuration输入的值。

		\~japanese
		最後にSetFollowDurationで入力した値を出力します。 
		\~
		*/
		 double GetFollowDuration();
		/**
		\~korean
		true로 세팅하면 follower가 target까지 다다르는 시간을 자동으로 조절한다.

		\~english
		If set as true then it automatically controls the time taken for follower to get to taget.

		\~chinese
		设置为true的话自动调整follower到达target的时间。

		\~japanese
		Trueで設定するとfollowerがtargetまで至る時間を自動調節します。 
		\~
		*/
		 void EnableAutoFollowDuration(bool val);

		/**
		\~korean
		follower가 따라가는 target의 pos, vel을 구한다.
		\param position 얻어올 타겟의 위치 vector3
		\param velocity 얻어올 타겟의 속도 vector3

		\~english TODO:translate needed.
		Calculate the pos and vel of the target followed by the follower. 
		\param position Vector 3 location of the target to be obtained for the position. 
		\param velocity Vector 3 speed of the target to be obtained for the velocity.

		\~chinese
		求follower追随的target的pos, vel。
		\param position 要获取的target位置vector3。
		\param velocity 要获取的target速度vector3。

		\~japanese
		Followerがついて行くtargetのpos, velを求めます。
		\param position 得て来るターゲットの位置 vector3
		\param velocity 得て来るターゲットの速度 vector3
		\~
		*/
		 void GetTarget(Vector3 &position, Vector3 &velocity) const;

		/**
		\~korean
		follower의 position, velocity를 구한다.
		\param position 얻어올 Follower의 위치 vector3
		\param velocity 얻어올 Follower의 속도 vector3

		\~english TODO:translate needed.
		Gets position, velocity of follower

		\~chinese
		求follower追随的target的position, velocity。
		\param position 要获取的target位置vector3。
		\param velocity 要获取的target速度vector3。

		\~japanese
		Followerのposition, velocityを求めます。
		\param velocity 得て来るFollowerの速度 vector3 
		\~
		*/
		 void GetFollower(Vector3 &position, Vector3 &velocity) const;

		/**
		\~korean
		SetFollowerVelocity,SetFollowerPosition을 한번에 모두 호출한다.

		\~english
		Calls SetFollowerVelocity,SetFollowerPosition at the same time

		\~chinese
		一次性全部呼叫SetFollowerVelocity,SetFollowerPosition。

		\~japanese
		SetFollowerVelocity,SetFollowerPositionを一度に全てを呼び出します。 
		\~
		*/
		 void SetFollower(Vector3 position, Vector3 velocity);

		/**
		\~korean
		매 타임 프레임마다 이 메서드를 호출해야 한다.
		\param elapsedTime 한 프레임의 시간(초)

		\~english
		This method must be called by each time frame.
		\param elapsedTime length of a frame(in second)

		\~chinese
		要在每个time frame呼叫此方法。
		\param elapsedTime 一个frame的时间（秒）。

		\~japanese
		タイムフレームごとにこのメソッドを呼び出さなければなりません。
		\param elapsedTime 一フレームの時間(秒) 
		\~
		*/
		 virtual void FrameMove(double elapsedTime);

		/**
		\~korean
		SetTargetPosition과 SetTargetVelocity를 한번에 모두 호출한다.

		\~english
		Calls SetTargetPosition and SetTargetVelocity at the same time

		\~chinese
		一次性全部呼叫SetTargetPosition和SetTargetVelocity。

		\~japanese
		SetTargetPositionとSetTargetVelocityを一度に全てを呼び出します。
		\~
		*/
		 virtual void SetTarget(Vector3 position, Vector3 velocity);
	};

	/**  @} */
#if (_MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
