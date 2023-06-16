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

#include <vector>

#if defined(_WIN32)
	#include <WinSock2.h>
	#include <MMSystem.h>
#else
	#include <pthread.h>
	#include <sys/types.h>
#endif

#include "BasicTypes.h"
#include "Deque.h"
#include "FastArrayPtr.h"
#include "Ptr.h"
#include "Event.h"
#include "Exception.h"
#include "TimeUtil.h"
#include "PnThread.h"
#include "Singleton.h"
#include "StackNew.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4324)
#endif // _MSC_VER


#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/


	// CLR에 대등 클래스 참고
	class IClonable
	{
	public:
		virtual void Clone() = 0;
		virtual ~IClonable() {}
	};

#ifdef _WIN32
	/** 
	\~korean
	타이머 클래스
	- 일정 시간마다 이벤트 객체(Proud.Event 참고)를 SetEvent, 즉 시그널한다.
	- .Net framework의 System.Timer와 같은 역할을 한다.

	\~english
	Timer class
	- Periodically lets SetEvent evenrt object(Please refer Proud.Event), in other words, this lets it signal.
	- Acts similar to System.Timer of .NET framework

	\~chinese
	计时器类
	- 每隔一段时间把事件对象（ 参考 Proud.Event ）SetEvent，即发信号通知。
	- 与Net framework的 System.Timer%起着一样的作用。

	\~japanese
	タイマークラス
	- 所定時間ごとにイベントオブジェクト（Proud.Eventを参考）をSetEvent、即ちシグナルします。
	- .Net frameworkのSystem.Timerのような役割をします。
	\~
	*/
	class Timer
	{
		TimerEventHandle m_timerID;
		HANDLE m_eventHandle; // 이 객체에 대해서 set이 된다.

		static void EventSetter(void* ctx);

	public:
		/** 
		\~korean
		생성자
		\param eventHandle 일정 시간마다 시그널을 넣을 이벤트 객체. Proud.Event를 파라메터로 넣어도 된다.
		\param interval 시그널을 넣을 주기. 밀리초이다.

		\~english
		 Constructor
		\param eventHandle Evnet object where signal to be entered periodically. It is possible to use Proud.event as parameter.
		\param interval The period to enter signal. Unit is millisecond.

		\~chinese
		生成者
		\param eventHandle 每一定时间放入信号的事件对象。可以把 Proud.Event%放入为参数。
		\param interval 放入信息的周期。微妙。

		\~japanese
		生成子
		\param eventHandle 所定時間ごとにシグナルを入れるイベントオブジェクト。Proud.Eventをパラメーターとして入れても良いです。
		\param interval シグナルを入れる周期。ミリ秒です。
		\~
		*/
		 Timer(HANDLE eventHandle, uint32_t interval, DWORD_PTR dwUser = 0);

		/** 
		\~korean
		파괴자
		- 객체를 파괴한 직후부터는 이벤트 객체에 시그널을 넣지 않는다.

		\~english
		Destructor
		- From right after destructing objct, it is not to enter signal into event object.

		\~chinese
		破坏者
		- 破坏对象后开始，不把信息放入事件对象。

		\~japanese
		破壊者
		- オブジェクトを破壊した直後からはイベントオブジェクトにシグナルを入れません。
		\~
		*/
		 ~Timer();
	};
#endif
	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER
