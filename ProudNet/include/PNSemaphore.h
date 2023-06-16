/* 106 228 203 199 203 251 69 164 171 249 223 147 102 233 72 249 25 46 47 89 4 75 196 233 166 154 197 5 225 93 96 137 109 76  */
/*
ProudNet 1.8.00002-master


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
#ifndef _WIN32
    #include "CriticalSect.h"
    #include <pthread.h>
#endif

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/

	/** 
	\~korean
	멀티스레드 프로그래밍에서 semaphore 객체이다.
	- .Net Framework의 System.Threading.Semaphore와 같은 역할을 한다.

	\~english
	Semaphore object of multi-thread programming.
	- This works same as System.Threading.Semaphore of .Net Framework.

	\~chinese
	多线程编程中的semaphore对象。
	- 起着与.Net Framework 的System.Threading.Semaphore%相同的作用。

	\~japanese
	マルチスレッドプログラミングでsemaphoreオブジェクトです。
	- .Net FrameworkのSystem.Threading.Semaphoreのような役割をします。
	\~
	*/
	class Semaphore
	{
#ifdef _WIN32
		HANDLE m_sema;
#else
		// m_cs에 의해 보호됨. 이 값이 0이 될때까지 스레드를 awake한다.
		int32_t m_count, m_maxCount; 

		// semaphore 대신 condition variable을 사용한다. ios에서는 무조건 named object를 요구함 ㅎㄷㄷ
		pthread_cond_t m_cond;

		// m_cond가 필요로 함
		CriticalSection m_cs;
#endif 

	public:

		/** 
		\~korean
		생성자
		\param initialCount semaphore 객체가 가질 초기 내부 값
		\param maxCount semaphore 객체가 가질 수 있는 최대값

		\~english
		Creator
		\param initialCount the initial value that semaphore object will have.
		\param maxCount the maximum value that semaphore object can have.

		\~chinese
		生成者
		\param initialCount semaphore对象将会拥有的初始内部值。
		\param maxCount semaphore可以拥有的最大值。

		\~japanese
		生成子
		\param initialCount semaphore オブジェクトが持つ初期内部値
		\param maxCount semaphore オブジェクトが持てる最大値
		\~
		*/
		 PROUD_API Semaphore(int initialCount, int maxCount);

		/** 
		\~korean
		파괴자 

		\~english
		Destructor

		\~chinese
		破坏者

		\~japanese
		破壊子
		\~
		*/
		 PROUD_API ~Semaphore();

		/** 
		\~korean
		내부 카운트가 1 이상이 될 때까지 무한정 기다린다. 
		내부 카운트가 1 이상인 경우 카운트를 1 감소시키면서 리턴한다.

		\~english
		It waits endlessly untill the internal counte becomes higher than 1. 
		If the count becomes higher than 1, it reduces the count by 1 and returns.

		\~chinese
		无限等到内部count为1以上。
		内部count为1以上的时候，把count减少1并返回。

		\~japanese
		内部カウントが1以上になる時まで限りなく待ちます。
		内部カウントが1以上である場合、カウントを1減少させながらリターンします。
		\~
		*/
		inline bool WaitOne()
		{
			return WaitOne(PN_INFINITE);
		}
		/** 
		\~korean
		내부 카운트가 1 이상이 될 때까지 일정 시간 기다린다. 
		내부 카운트가 1 이상인 경우 카운트를 1 감소시키면서 리턴한다. 
		\param timeOut (밀리초) 시그널 대기 최대 시간. INFINITE를 넣으면 무한정 기다린다.

		\~english
		It waits for a specific time until the internal count becomes higher than 1. 
		If the count becomes higher than 1, it reduces the count by 1 and returns. 
		\param timeOut (millisecond) The maximum waiting time for signal standby. Input INFINITE if you want it to wait endlessly.

		\~chinese
		等待一定时间至内部count变成1以上。
		内部count为1以上的时候，把count减少1并返回。
                \param timeOut （毫秒）等待信号的最大时间。输入INFINITE的话是无限期等待。

		\~japanese
		内部カウントが1以上になる時まで所定時間を待ちます。
		内部カウントが1以上である場合、カウントを1減少させながらリターンします。
		\param timeOut（ミリ秒）シグナル待ち最大時間。INFINITEを入れれば限りなく待ちます。
		\~
		*/
		 PROUD_API bool WaitOne(uint32_t timeOut);

		/** 
		\~korean
		내부 카운트를 특정 값만큼 증가시킨다.
		\param releaseCount 증가시킬 값 
		\return 증가시키기 전의 semaphore의 내부 값

		\~english TODO:translate needed.
		Increase the internal count with a certain value. 
		\param releaseCount Value to be increased
		\return Internal value of semaphore before the increase. 

		\~chinese
		增加内部count相当于特定值。
		\param releaseCount 要添加的值。
		\return 增加之前的semaphore内部值。

		\~japanese
		内部カウントを特定値だけ増加させます。
		\param releaseCount 増加させる値
		\return 増加させる前の semaphoreの内部値
		\~
		*/
		 PROUD_API void Release(int releaseCount = 1);
	};

	/** @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
