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

#if defined _WIN32 || defined _WIN64
#include <winbase.h>
#endif
#include "ProcHeap.h"
#include "Singleton.h"
#include "Event.h"
#include "PnThread.h"
#include "sysutil.h"
#include "BasicTypes.h"
#include "SysTime.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/*	이 함수들은 Windows Vista 이후에나 쓸 수 있음. 따라서 막음.

	// 변수 하나 갖고 critical section으로 보호되는 read/write는 오버액션.
	// 32bit OS에서 64bit 값을 volatile keyword에만 의존해서 i/o하는 것은 non-atomic. 즉 tearing의 위험이 있음.
	// 이를 예방하고자 이 함수들이 있음
	// 인자 값은, align(8)이 되어 있어야 함
	inline double InterlockedRead(volatile double* src)
	{
	LONGLONG *src2 = (LONGLONG*)src;
	LONGLONG ret0 = InterlockedOr64(&src2, 0);
	double* pRet = (double*)&ret0; // raw data cast
	return *pRet;
	}

	inline void InterlockedWrite(volatile double* dest, double newValue)
	{
	LONGLONG* dest2 = (LONGLONG*)dest;
	LONGLONG* pNewValue2 = (LONGLONG*)&newValue; // raw cast
	InterlockedExchange64(&dest2, *pNewValue);
	}*/

	/** \addtogroup util_group
	*  @{
	*/

	/**
	\~korean
	CTimerQueue::NewTimer 를 사용시 넘겨주는 파라메터 입니다.

	\~english
	Parameter that pass CTimerQueue::NewTimer when it needs

	\~chinese
	使用 CTimerQueue::NewTimer%时传交的参数。

	\~japanese
	CTimerQueue::NewTimer を使用時に引き渡すパラメーターです。
	\~
	*/
#if defined(_WIN32)
	struct NewTimerParam {
		/**
		\~korean
		매 시간마다 콜백 되어질 함수를 설정합니다.

		\~english
		Set function that will callback every hours

		\~chinese
		设置每段时间被回调的函数。

		\~japanese
		毎時間ごとにコールバックされる関数を設定します。
		\~
		*/
		WaitOrTimerCallbackProc m_callback;

		/**
		\~korean
		Callback 되어지는 포인터값을 지정합니다.

		\~english
		Designate pointer value that can callback

		\~chinese
		指定callback的指针值。

		\~japanese
		コールバックされるポインター値を指定します。
		\~
		*/
		void *m_pCtx;

		/**
		\~korean
		매번 Callback 되어지는 시간을 입력합니다.

		\~english
		Enter the time to callback

		\~chinese
		输入每次callback的时间。

		\~japanese
		毎回コールバックされる時間を入力します。
		\~
		*/
		uint32_t m_period;

		/**
		\~korean
		DueTime 으로 지정된 시간 이후 부터 Callback 이 시작됩니다.

		\~english
		Callback will start time which is set by Duetime

		\~chinese
		从指定为DueTime时间以后开始进行callback。

		\~japanese
		DueTimeで指定された時間以降からCallbackが開始されます。
		\~
		*/
		uint32_t m_DueTime;

		PROUD_API NewTimerParam() : m_callback(0), m_pCtx(0), m_period(0), m_DueTime(0) {}
		PROUD_API NewTimerParam(WaitOrTimerCallbackProc callback, void *pCtx, uint32_t period, uint32_t DueTime);
	};

	class CTimerQueueTimer;

	/**
	\~korean
	스레드에 이름을 지정한다. 이렇게 하면 디버거 스레드 뷰에서 각 스레드에 이름이 표시된다.
	- 디버깅이 편해질 것이다.
	- 보다 자세한 내용은
	ms-help://MS.VSCC.2003/MS.MSDNQTR.2003APR.1033/vsdebug/html/vxtskSettingThreadName.htm 참고

	\~english
	Designates a name to thread. By doing so, each thread will have their names shown at debugger thread view.
	- Debugging will be easier.
	- Please refer
	ms-help://MS.VSCC.2003/MS.MSDNQTR.2003APR.1033/vsdebug/html/vxtskSettingThreadName.htm

	\~chinese
	给线程指定名字。这样的话在调试线程view里会显示每个线程的名字。
	- 调试会更加方便。
	- 详细内容请参考
	ms-help://MS.VSCC.2003/MS.MSDNQTR.2003APR.1033/vsdebug/html/vxtskSettingThreadName.htm 。

	\~japanese
	スレッドに名前を指定します。このようにすると、デバッガースレッドビューで各スレッドに名前が表示されます。
	- デバッギングが容易くなります。
	- より詳しくは、
	ms-help://MS.VSCC.2003/MS.MSDNQTR.2003APR.1033/vsdebug/html/vxtskSettingThreadName.htm をご参照ください。

	\~
	*/
	PROUD_API void SetThreadName(uint32_t dwThreadID, const char* szThreadName);

	/**
	\~korean
	이 스레드의 실제 핸들을 구한다.

	\~english
	Gets the real handle of this thread

	\~chinese
	求此线程的实际handle。

	\~japanese
	このスレッドの実際ハンドを求めます。
	\~
	*/
	PROUD_API HANDLE GetCurrentRealThread();

	/**
	\~korean
	운영체제에서 제공하는 Thread Pool에서 일정 시간마다의 tick event를 실행할 수 있게 하는
	싱글톤 클래스이다.
	- CTimerQueueTimer 와 혼용된다.
	- 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#timer_queue" >Timer Queue (Thread pool에서 tick event를 수행하는 모듈)</a> 를 참고바람
	- Windows 98 이하 버전에서는 쓸 수 없다.

	\~english
	Singleton class that can execute periodic tick event at thread pool provided by OS
	- Regarded as CTimerQueueTimer
	- Please refer <a target="_blank" href="http://guide.nettention.com/cpp_en#timer_queue" >Timer Queue (A module that operates tick event from thread pool)</a>
	- Cannot be used Windows 98 or older version

	\~chinese
	从运行体系提供的Thread Pool里每一定时间能实行tick event的singleton类。
	- 与 CTimerQueueTimer%混用。
	- 详细的请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#timer_queue" >Timer Queue (在Thread pool中执行tick event的模块)</a>%。
	- 在Windows 98版本以下不能使用。

	\~japanese
	OSが提供するThreadPoolで所定時間ごとのtick eventを実行できるようにするシングルトンクラスです。
	- CTimerQueueTimer と混用されます。
	- 詳しくは、\ref timer_queueをご参照ください。
	- Windows 98 以下のバージョンでは使えません。
	\~
	*/
	class CTimerQueue : public DllSingleton<CTimerQueue>
	{
		friend CTimerQueueTimer;
		HANDLE m_timerQueue;
		Event m_endEvent;

	public:
		PROUD_API CTimerQueue();
		PROUD_API 	~CTimerQueue();

		/**
		\~korean
		일정 시간마다 유저 함수의 호출을 등록한다.
		- 유저 함수 호출을 등록하면 CTimerQueueTimer 객체가 리턴된다.
		- 유저 함수 호출은 CTimerQueue 싱글톤에 여럿을 중복해서 등록해도 된다.

		\param callback 일정 시간마다 호출될 유저 함수
		\param ctx callback에 전달될 유저 파라메터
		\param period 유저 함수가 호출될 주기(밀리초)

		\~english
		Periodically registeres user function calling
		- Once registered then CTimerQueueTimer object is returned.
		- Calling user function can be repeatedly registered at CTimerQueue singleton.

		\param callback User function to be called periodically
		\param ctx user parameter to be delivered to callback
		\param period the period that user function to be called in millisecond

		\~chinese
		每一定时间登录用户函数的呼叫。
		- 登录用户函数呼叫的话返回 CTimerQueueTimer%对象。
		- 用户函数的呼叫可以在 CTimerQueue singleton重复几个后登录。

		\param callback 每一定时间要呼叫的用户函数。
		\param ctx 传达给callback的用户参数。
		\param period 要呼叫用户函数的周期（毫秒）。

		\~japanese
		所定時間ごとのユーザー関数の呼び出しを登録します。
		- ユーザー関数の呼び出しを登録すると CTimerQueueTimer オブジェクトがリターンされます。
		- ユーザー関数の呼び出しはCTimerQueue シングルトンに複数を重複して登録しても良いです。

		\param callback 所定時間ごとに呼び出されるユーザー関数
		\param ctx callbackに伝われるユーザーパラメーター
		\param period ユーザー関数が呼び出される周期（ミリ秒）
		\~
		*/
		PROUD_API  CTimerQueueTimer* NewTimer(NewTimerParam& Param);
	};

	DECLARE_DLL_SINGLETON(PROUD_API, CTimerQueue);

	/**
	\~korean
	일정 시간마다 스레드 풀에서 주어진 함수를 호출하는 타이머 객체
	- 자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#timer_queue" >Timer Queue (Thread pool에서 tick event를 수행하는 모듈)</a> 를 참고바람
	- CTimerQueue.NewTimer 에 의해서 리턴되는 객체의 타입이다. 이 객체를 파괴하기 전까지 CTimerQueue.NewTimer 에 의해
	등록된 함수가 일정 시간마다 스레드 풀에서 실행된다.
	- 서버 용도로 쓰는 것을 권장한다. Windows 98 이하 버전에서는 쓸 수 없다.

	\~english
	Timer object that periodicaly calls the functions provided by thread pool
	- Please refer <a target="_blank" href="http://guide.nettention.com/cpp_en#timer_queue" >Timer Queue (A module that operates tick event from thread pool)</a>.
	- Type of object that is returned by CTimerQueue.NewTimer. Until this object is destructed, the function registered by CTimerQueue.NewTimer is executed periodically in thread pool.
	- Recommended to use for server. Cannot be used for Windows 98 or older version.

	\~chinese
	每一定时间解开线程并呼叫所提供函数的timer对象。
	- 详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#timer_queue" >Timer Queue (在Thread pool中执行tick event的模块)</a>%。
	- 被 CTimerQueue.NewTimer%返回的对象类型。到破坏此对象为止，被 CTimerQueue.NewTimer%登录的函数每一定时间在线程槽实行。
	- 建议用于服务器用途。在Windows 98以下版本无法使用。

	\~japanese
	所定時間ごとにスレッドプールより与えられた関数を呼び出すタイマーオブジェクト
	- 詳しくは\ref timer_queueをご参照ください。
	- CTimerQueue.NewTimer によってリターンされるオブジェクトのタイプです。このオブジェクトを破壊する前までCTimerQueue.NewTimerによって登録された関数が予定時間ごとにスレッドプールで実行されます。
	- サーバー用途で使うことをお勧めします。 Windows 98以下のバージョンでは使えません。
	\~
	*/
	class CTimerQueueTimer
	{
		friend CTimerQueue;

		CTimerQueue *m_manager;
		HANDLE m_timer;
		Event m_endEvent;

		CTimerQueueTimer() {} // 명시적 생성 금지
	public:

		PROUD_API  ~CTimerQueueTimer();

#ifdef _WIN32
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif
	};

#endif // _WIN32
	/**  @} */

	// win32 or not을 위해
	inline void* TlsGetValue(uint32_t tlsIndex)
	{
#ifdef _WIN32
		return ::TlsGetValue(tlsIndex);
#else
		return pthread_getspecific(tlsIndex);
#endif
	}

	// win32 or not을 위해
	inline bool TlsSetValue(uint32_t tlsIndex, void* tlsValue)
	{
#ifdef _WIN32
		return ::TlsSetValue(tlsIndex, tlsValue) != 0;
#else
		return pthread_setspecific(tlsIndex, (const void*)tlsValue) == 0;
#endif
	}

	uint32_t TlsAlloc();
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
