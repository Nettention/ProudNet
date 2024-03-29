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

#pragma once

#include "LambdaWrap.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/


	/** 비동기 실행의 종류입니다. */
	enum RunAsyncType
	{
		/** 1개의 스레드에서만 비동기 실행을 합니다. */
		RunAsyncType_Single,
		/** 모든 스레드에서 비동기 실행을 합니다. 예를 들어 thread pool 안에 스레드가 3개이면 각 스레드에서의 비동기 실행이 동시에 일어나서, 즉 3회의 비동기 실행이 됩니다. */
		RunAsyncType_Parallel,
	};

	/**
	\~korean
	CThreadPool 에서 발생하는 이벤트 콜백을 받아 처리하는 인터페이스 클래스입니다.

	자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#thread_pool_sharing" >서버에서 여러 호스트 모듈이 같은 Thread Pool를 공유하기</a> 을 참고하십시오.

	사용법
	- 객체를 상속받아 메서드를 구현합니다.
	- CThreadPool 에 이 객체를 파라메터로 전달합니다.

	\~english
	This is interface class that hand event callback occured from CThreadPool.

	Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#thread_pool_sharing" >Share Thread Pool that has same host module in a server.</a> for more detail.

	Usage
	- Realize this method to inherit object.
	- Pass this object to parameter at CThreadPool

	\~chinese
	接收并处理从 CThreadPool%发生的event回调的界面类。

	详细的请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#thread_pool_sharing" >在服务器中共享几个主机模块相同的Thread Pool</a>%。

	使用方法
	- 继承对象以后体现此方法。
	- 把此对象为参数传送给 CThreadPool%。

	\~japanese
	CthreadPoolで発生するイベントコールバックを受けて処理するインターフェースクラスです。

	詳しくは、\ref thread_pool_sharingをご参照ください。

	使用方法
	- オブジェクトを相続してもらいメソッドを実現します。
	- CThreadPoolへこのオブジェクトをパラメーターで伝達します。
	\~
	*/
	class IThreadPoolEvent
	{
	public:
		/**
		\~korean
		thread pool의 스레드가 시작할 때 이 메서드가 호출됩니다.

		\~english
		When thread pool started thread, call this method.

		\~chinese
		thread pool 线程开始的时候呼叫此方法。

		\~japanese
		thread poolのスレッドが開始する時、このメソッドが呼び出されます。
		\~
		*/
		virtual void OnThreadBegin() = 0;

		/**
		\~korean
		thread pool의 스레드가 종료할 때 이 메서드가 호출됩니다.

		\~english
		When thread pool is terminated, call this method.

		\~chinese
		thread pool 线程结束的时候呼叫此方法。

		\~japanese
		thread poolのスレッドが終了する時、このメソッドが呼び出されます。
		\~
		*/
		virtual void OnThreadEnd() = 0;
	};

#ifdef SUPPORTS_CPP11

	/** C++11을 사용하실 수 있으면, IThreadPoolEvent을 구현하는 것 대신에 이것을 쓰시는 것도 편리할 것입니다. */
	class CThreadPoolEventFunctional:public IThreadPoolEvent
	{
	public:
		/** IThreadPoolEvent의 같은 이름의 함수의 설명을 참고하십시오. */
		std::function<void()> OnThreadBeginFunction;
		/** IThreadPoolEvent의 같은 이름의 함수의 설명을 참고하십시오. */
		std::function<void()> OnThreadEndFunction;
	
	private:
		void OnThreadBegin() 
		{
			if (OnThreadBeginFunction)
				OnThreadBeginFunction();
		}

		void OnThreadEnd() 
		{
			if (OnThreadEndFunction)
				OnThreadEndFunction();
		}

	};

#endif

	typedef void (*RunAsyncFunction)(void*);


	/**
	\~korean
	서버사이드 호스트 모듈(NetServer,NetClient)을 위한 thread pool 객체입니다.

	자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#thread_pool_sharing" >서버에서 여러 호스트 모듈이 같은 Thread Pool를 공유하기</a> 을 참고하십시오.

	\~english
	This is thread pool object for host module(NetServer,NetClient)

	Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#thread_pool_sharing" >Share Thread Pool that has same host module in a server.</a> for more detail.

	\~chinese
	为了主机模型（NetServer,NetClient）的thread pool对象。

	详细请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#thread_pool_sharing" >在服务器中共享几个主机模块相同的Thread Pool</a>%。

	\~japanese
	ホストモジュール(NetServer,NetClient)のためのthread poolオブジェクトです。
	
	詳しくは、\ref thread_pool_sharingをご参照ください。
	\~

	\code

	- tp = CThreadPool.Create();
	- tp.SetEventSink(...); // (optinal)
	- tp.SetThreadCount(n); // n = 0~512
	- NetClient and NetServer references variable tp
	- delete tp;
	\endcode

	*/
	class CThreadPool
	{
	public:
		virtual ~CThreadPool();

		// 원하는 수의 스레드로 갯수를 조절한다.
		// 0~512까지 가능.
		// 이 함수는 즉시 리턴되고, 시간이 지나면서 목표치로 가게 된다.
		// 0이 될 경우 사용자는 CThreadPool.Process를 매번 호출해 주어야 한다.
		virtual void SetDesiredThreadCount(int threadCount) = 0;

		/**
		\~korean
		네트워킹 서버간 통신용 ThreadPool을 생성합니다.

		\~english
		Creates ThreadPool for server communications

		\~chinese
		生成网络服务器之间通信用的ThreadPool。

		\~japanese
		サーバー間通信用ThreadPoolを生成します。
		\~

		\param eventSink You can implement your own event handlers regarding to thread pool here.
		\param threadCount Specifies how many threads are in this thread pool. If you specify zero, this runs as 'zero thread pool mode'.
		*/
		PROUD_API static CThreadPool *Create(IThreadPoolEvent* eventSink, int threadCount);

		/** Zero thread pool mode란, 이 thread pool 객체가 별도의 스레드를 갖지 않고, 사용자가 갖고 있는 스레드 위에서 일을 하는 것을 말합니다.
		Create() 호출시 thread count 값을 0을 지정하면 이 thread pool 객체는 zero thread pool mode가 됩니다.
		
		Zero thread pool mode를 쓰는 경우, 이 thread pool 객체는 사용자가 명시적으로 일을 하라는 지시를 해야만 작동합니다.
		이 함수를 호출하면 이 thread pool 객체는 일을 합니다.
		Zero thread pool mode가 아니면 이 함수는 아무것도 하지 않습니다.
		여러분은 Zero thread pool mode이면 이 함수를 반복해서 호출하십시오.

		\timeOutMs 처리할 일이 있을 때까지 기다리는 최대 시간(밀리초)입니다. 0을 넣으면 대기를 하지 않습니다.
		*/
		virtual void Process(int timeoutMs) = 0;

		/** 함수 func 를 이 Thread pool 의 스레드에서 비동기로 실행합니다. 
		\param type 비동기 실행의 종류
		\param func 비동기로 실행할 함수
		\param context 함수에 전달할 데이터
		*/
		virtual void RunAsync(RunAsyncType type, RunAsyncFunction func, void* context) = 0;
	};

	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
