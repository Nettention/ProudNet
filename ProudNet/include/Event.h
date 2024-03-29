/*
ProudNet 1.8.58849-master


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



/*
LICENSE

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/



#pragma once

#include "BasicTypes.h"

#if !defined(_WIN32)
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
	멀티스레드 프로그래밍에서의 이벤트 객체이다.

	\~english
	Event object in multi-thread programming

	\~chinese
	多线程编程里的事件对象。

	\~japanese
	マルチスレッドプログラミングでのイベントオブジェクトです。
	\~
	*/
	class Event
	{
	public:

#if defined(_WIN32)
		HANDLE m_event;

#elif defined(__linux__) || defined(__MACH__) || defined(__FreeBSD__)

		pthread_mutex_t	m_mutex;
		pthread_cond_t m_condVar;

		bool	m_manualReset;
		bool	m_isSignaled;
#endif

		/**
		\~korean
		생성자
		\param manualReset ::WaitForSingleObject 또는 WaitAny, WaitAll 등을 통해 이 객체의 시그널을 대기하는 스레드가 있는 상태에서
		시그널이 발생하면 대기가 끝나게 된다. 이때 이벤트 객체가 자동으로 non-singal 상태(event reset)이 되게 할 것이라면 true를 넣도록 한다.
		\param initialState 이 이벤트 객체가 시그널을 처음부터 받은 상태로 생성되게 하려면 true

		\~english
		Constructor
		\param manualReset Waiting will end when signal occurs while there exists thread waiting for this object's signal through ::WaitForSingleObject or WaitAny or WaitAll.
		Enter true to make event object be non-signal state (event reset) automatically.
		\param initialState True in order to create this event object as to make it received signal initially.

		\~chinese
		生成者
		\param manualReset 通过::WaitForSingleObject 或者WaitAny, WaitAll等，等待此个体的信号的线程存在的状态下，发生信号的话会结束等待。这时候想让事件对象自动变成non-signal 状态（event reset）的话输入true。
		\param initialState 想让此事件对象生成为从头接收信号的状态的话true。

		\~japanese
		生成子
		\param manualReset ::WaitForSingleObject または WaitAny, WaitAll などを通じ、このオブジェクトのシグナルを待機するスレッドがある状態でシグナルが発生すると待機が終わります。この時、イベントオブジェクトが自動にnon-signal状態(event reset)になるようにしたい場合はtrueを入れてください。
		\param initialState このイベントオブジェクトがシグナルを最初から受けた状態で生成されるようにしたい場合は true
		\~
		*/
		 PROUD_API Event(bool manualReset, bool initialState);

		/**
		\~korean
		기본 옵션으로 생성한다.
		- auto reset, non signaled로 시작한다.

		\~english
		Creates with default option
		- Begins as auto reset and non signaled

		\~chinese
		生成基本选项。
		- 从auto reset, non signaled 开始。

		\~japanese
		基本オプションとして生成します。
		- auto reset, non signaledで開始します。
		\~
		*/
		 PROUD_API Event();

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
		 PROUD_API ~Event();

		/**
		\~korean
		이벤트에 시그널을 넣는다.

		\~english
		Enters signal into event

		\~chinese
		在事件当中加入信号。

		\~japanese
		イベントにシグナルを入れます。
		\~
		*/
		inline void Set()
		{
#if defined(_WIN32)
			::SetEvent(m_event);
#elif defined(__linux__) || defined(__FreeBSD__)
			int ret = 0;

			// mutex lock 잡고 작업 해야함
			ret = pthread_mutex_lock(&m_mutex);
			assert(ret == 0);

			m_isSignaled = true;

			// windows event 는 manual reset 일때 모든 쓰레드가 깨어남
			// (reset 을 안시키니 계속 signal 상태라...)
			if (m_manualReset)
			{
				ret = pthread_cond_broadcast(&m_condVar);
				assert(ret == 0);
			}
			else  // Auto Reset...
			{
				// WFSO 중인 쓰레드에게 signal 날림
				ret = pthread_cond_signal(&m_condVar);
				assert(ret == 0);

				// !! 여기서 state 값을 변경하면 안됨
				// 깨어난 쓰레드가 바꿔야함
				// m_isSignaled = false;
			}

			ret = pthread_mutex_unlock(&m_mutex);
			assert(ret == 0);
#endif
		}

		/**
		\~korean
		시그널을 제거한다.

		\~english
		Revmoes signal

		\~chinese
		移除信号。

		\~japanese
		シグナルを除去します。
		\~
		*/
		inline void Reset()
		{
#if defined(_WIN32)
			::ResetEvent(m_event);
#elif defined(__linux__)
			int ret = 0;

			ret = pthread_mutex_lock(&m_mutex);
			assert(ret == 0);

			m_isSignaled = false;

			ret = pthread_mutex_unlock(&m_mutex);
			assert(ret == 0);
#endif
		}

		/**
		\~korean
		시그널이 올 때까지 기다린다.
		\param timeOut (밀리초) 시그널 대기 최대 시간. INFINITE를 넣으면 무한정 기다린다.

		\~english
		Waits until signal arrives
		\param timeOut (in milliseconds) signal waiting max time. It waits permanantly when infinite entered.

		\~chinese
		等到信号到达。
		\param timeOut （毫妙）等待信号最大时间。加入INFINITE的话是无限等待。

		\~japanese
		シグナルが来る時まで待ちます。
		\param timeOut (ミリ秒) シグナル待機最大時間。INFINITEを入れると無限定で待ちます。
		\~
		*/
		 PROUD_API bool WaitOne(uint32_t timeOut);

		/**
		\~korean
		시그널이 올 때까지 무한정 기다린다.

		\~english
		Waits indefinitely until signal arrives
		
		\~chinese
		无限等至信号到达。

		\~japanese
		シグナルが来る時まで無限定で待ちます。
		\~
		*/
		 PROUD_API bool WaitOne();

		/**
		\~korean
		입력된 이벤트 객체들 중 하나라도 시그널이 발생할 때까지 기다린다.
		\return -1 또는 완료 성공한 이벤트 객체의 배열 항목

		\~english
		Waits until at least one of event objects entered occurs
		\return -1 or allocation clause of event object that has been successfully completed

		\~chinese
		等待输入的事件个体中至少一个事件发生信号。
		\return -1或者完毕成功的事件对象的排列项目。

		\~japanese
		入力されたイベントオブジェクトのうち一つでもシグナルが発生する時まで待ちます。
		\return -1 または完了に成功したイベントオブジェクトの配列項目
		\~
		*/
#if defined(_WIN32)
		inline static int WaitAny(Event** events, int count)
		{
			return WaitAny(events, count, PN_INFINITE);
		}
#endif

		/**
		\~korean
		입력된 이벤트 객체들 중 하나라도 시그널이 발생할 때까지 기다린다.
		\return -1 또는 완료 성공한 이벤트 객체의 배열 항목

		\~english
		Waits until at least one of event objects entered occurs
		\return -1 or allocation clause of event object that has been successfully completed

		\~chinese
		等到输入的事件个体中至少一个发生信号。
		\return -1或者完毕成功的事件对象的排列项目。

		\~japanese
		入力されたイベントオブジェクトのうち一つでもシグナルが発生する時まで待ちます。
		\return -1 または完了に成功したイベントオブジェクトの配列項目
		\~
		*/
#if defined(_WIN32)
		static int WaitAny(Event** events, int count, uint32_t timeOut);
#endif

		/**
		\~korean
		입력된 이벤트 객체이 모두 시그널이 발생할 때까지 기다린다.

		\~english
		Waits until all of event objects entered occur

		\~chinese
		等到事件的对象全部发生信号。

		\~japanese
		入力されたイベントオブジェクト全てにおいてシグナルが発生する時まで待ちます。
		\~
		*/
#if defined(_WIN32)
		inline static int WaitAll(Event** events, int count)
		{
			return WaitAll(events, count, PN_INFINITE);
		}
#endif

		/**
		\~korean
		입력된 이벤트 객체이 모두 시그널이 발생할 때까지 기다린다.

		\~english
		Waits until all of event objects entered occur

		\~chinese
		等到事件的对象全部发生信号。

		\~japanese
		入力されたイベントオブジェクト全てにおいてシグナルが発生する時まで待ちます。
		\~
		*/
#if defined(_WIN32)
		static int WaitAll(Event** events, int count, uint32_t timeOut);
#endif
	private:
#if defined(_WIN32)
		static int WaitForSignal(Event** events, int count, uint32_t timeOut, bool waitForAll);
#endif

	public:
		// called by CObjectPool. does nothing.
		void SuspendShrink() {}
		void OnRecycle() {}
		void OnDrop() {
			// handle을 지우지 않는다. 커널 리소스 자체를 재사용하니까.
		}
	};
	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
