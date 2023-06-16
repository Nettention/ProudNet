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

#include "FakeClrBase.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud 
{
#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif
#ifdef _WIN32

	/** \addtogroup util_group
	*  @{
	*/

	/** 
	\~korean
	일정 시간마다 특정 루틴이 실행되게 하고자 한다면 CTimerThread 나 CTimerQueue 를 사용하는 방법이 있습니다. 
	- CTimerThread 는 일정 시간마다 타이머 콜백 함수가 실행됩니다. 하지만 단 한개의 스레드에서만의 콜백이 일어납니다. 
	- CTimerQueue 는 과거 타이머 콜백이 리턴하지 않은 상태에서 타이머 시간이 되었을 때에는 또 다른 스레드에서 콜백이 일어납니다.

	\~english
	 If you want to have a specific routine to run at a specific time, use  CTimerThread or CTimerQueue.
	- CTimerThread runs a timer callback function at specific times. But callback only happens in a single thread. 
	- CTimerQueue allows callback to start in a different thread at specific times even thoud the previous timer callback hasn't been returned.

	\~chinese
	如果想在每一定时间进行特定routine的话，有使用 CTimerThread%或者 CTimerQueue%的方法。
	- CtimerThread在每一定时间实行timer回拨函数。但是只发生一个线程的回拨。
	- CtimerQueue在过去timer回拨没有返回的状态下到了timer的时间，或者在其他线程产生回拨。

	\~japanese
	所定時間ごとに特定ルーチンが実行されるようにしたい場合は、CTimerThreadまたはCTimerQueueを使う方法があります。
	- CTimerThreadは所定時間ごとにタイマーコールバック関数が実行されます。しかし、たった1個のスレッドでのみコールバックが発生します。
	- CTimerQueueは過去のタイマーコールバックがリターンしていない状態でタイマー時間になった時は、また違うスレッドでコールバックが発生します。
	\~
	*/
	class CTimerThread
	{
		Event m_tickEvent;
		Timer m_timer;
		Thread m_thread;
		Thread::ThreadProc m_proc;
		void* m_procCtx;
		volatile bool m_stopThread;
		CriticalSection m_startStopMethodLock;

		static void OuterThreadProc(void* ctx);

	public:
		// true이면 CoInitialize가 스레드 시작시 호출된다.
		bool m_useComModel; 

		/** 
		\~korean
		생성자. 타이머 스레드가 시작하지는 않습니다. 
		\param threadProc 일정 시간마다 호출되는 함수입니다. 사용자가 정의해야 합니다.
		\param interval 타이머 주기. 밀리초입니다.
		\param ctx threadProc이 실행될 때 던져지는 인자입니다. 사용자가 지정해야 합니다. 

		\~english
		Creator and timer thead do not start here. 
		\param threadProc a function that gets called at an user-defined specific time.
		\param interval Timer cycle in millisecond.
		\param ctx A factor tht gets passed on when threadProc is running. This needs to be defined by user. 

		\~chinese
		生成者。Timer 线程并不会开始。
		\param threadProc 每一定时间呼叫的函数。要用户定义。
		\param interval timer周期。毫秒。
		\param ctx threadProc实行的时候的因子。要用户指定。

		\~japanese
		生成子。タイマースレッドが開始されることはありません。
		\param threadProc 所定時間ごとに呼び出される関数です。ユーザーが定義しなければなりません。
		\param interval タイマー周期。ミリ秒です。
		\param ctx threadProc が実行される時に使われる因子です。ユーザーが指定する必要があります。
		\~
		*/
		PROUD_API CTimerThread(Thread::ThreadProc threadProc, uint32_t interval, void *ctx);
		
		/** 
		\~korean
		파괴자. 파괴되기 전에 타이머 스레드를 종료시킵니다. 

		\~english
		Shuts down timer thread before destruction.

		\~chinese
		破坏者。在破坏之前终止timer线程。

		\~japanese
		破壊者。破壊される前にタイマースレッドを終了させます。
		\~
		*/
		PROUD_API ~CTimerThread();

		/** 
		\~korean
		타이머 스레드의 작동을 시작합니다.

		\~english
		Starts the operation of timer thread.

		\~chinese
		开始timer线程的运转。

		\~japanese
		タイマースレッドの作動を開始します。
		\~
		*/
		PROUD_API void Start();

		/** 
		\~korean
		타이머 스레드의 작동을 중지하고, 스레드 파괴가 완료될 때까지 기다립니다. 

		\~english
		Stops the operation of timer thread and wait until a thread gets destructed.

		\~chinese
		终止timer线程的运转，等到线程破坏结束。

		\~japanese
		タイマースレッドの作動を中止して、スレッド破壊が完了する時まで待ちます。
		\~
		*/
		PROUD_API void Stop();
	};

	/**  @} */
#endif
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
