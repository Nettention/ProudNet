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

#include "ErrorInfo.h"
#include "Enums.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/


	class RmiContext;

	/** user worker thread callback context. */
	class CUserWorkerThreadCallbackContext
	{
	};



	/** 
	\~korean
	ProudNet용 event sink
	- 파생 클래스에서 사용됨.

	\~english
	Event sink for ProudNet
	- Use it with derivation class

	\~chinese
	ProudNet用event sink。
	- 在派生类里使用。

	\~japanese
	ProudNet用 event sink
	- 派生クラスで使われます。
	\~
	*/
	class INetCoreEvent
	{
	public:

	public:
		 PROUD_API virtual ~INetCoreEvent();

		/** 
		\~korean
		내부 에러 발생시 호출됨
		\param errorInfo 에러 정보. ErrorInfo::ToString() 을 통하여 필요한 정보를 쉽게 얻으실 수 있습니다. 자세한 것은 ErrorInfo 도움말에 있습니다.

		\~english TODO:translate needed.
		It is called when an internal error is called. 
		\param errorInfo Error information. You can easily obtain the necessary information through ErrorInfo::ToString(). More details are in the ErrorInfo help. 

		\~chinese
		发生内部错误时被呼叫。
		\param errorInfo 错误信息。通过 ErrorInfo::ToString()%可以轻松获得需要的信息。详细的在ErrorInfo帮助里。

		\~japanese
		内部エラーが発生時に呼び出されます。
		\param errorInfo エラー情報。ErrorInfo::ToString()を通じ必要な情報を簡単に手に入れることができます。詳しくは、ErrorInfoのヘルプをご参照ください。
		\~
		*/
		virtual void OnError(ErrorInfo *errorInfo) = 0;

		/** 
		\~korean
		내부 경고 발생시 호출됨. 에러보다는 낮은 수위의 문제를 보고함.
		\param errorInfo 에러 정보. ErrorInfo::ToString() 을 통하여 필요한 정보를 쉽게 얻으실 수 있습니다. 자세한 것은 ErrorInfo 도움말에 있습니다. 

		\~english TODO:translate needed.
		It is called when an internal warning is called. This reports a problem on a lower level than an error. 
		\param errorInfo Error information. You can easily obtain the necessary information through ErrorInfo::ToString(). More details are in the ErrorInfo help.

		\~chinese
		发生内部警告时被呼叫。报告比错误低级的问题
		\param errorInfo 错误信息。通过 ErrorInfo::ToString()%可以轻松获得需要的信息。详细的在ErrorInfo帮助里。

		\~japanese
		内部警告が発生時に呼び出されます。エラーよりは低いレベルの問題を報告します。
		\param errorInfo エラー情報。ErrorInfo::ToString()を通じ必要な情報を簡単に手に入れることができます。詳しくは、ErrorInfoのヘルプをご参照ください。
		\~
		*/
		virtual void OnWarning(ErrorInfo *errorInfo) = 0;

		/** 
		\~korean
		내부 알림 발생시 호출됨. 내부 상황 추적 등을 위한 것으로 문제 상황을 말하는 것은 아님.
		\param errorInfo 에러 정보. ErrorInfo::ToString() 을 통하여 필요한 정보를 쉽게 얻으실 수 있습니다.

		\~english TODO:translate needed.
		It is called when an internal notification is called. This is for internal status tracing, etc. and does not mean a problem situation. 
		\param errorInfo Error information. You can easily obtain the necessary information through ErrorInfo::ToString(). More details are in the ErrorInfo help.

		\~chinese
		发生内部告知时被呼叫。是为了内部情况追踪等，并不是说对问题的情况。
		\param errorInfo 错误信息。通过 ErrorInfo::ToString()%可以轻松获得需要的信息。

		\~japanese
		内部アラームが発生時に呼び出されます。内部状況追跡などのためのことで、問題状況を言うのではありません。
		\param errorInfo エラー情報。ErrorInfo::ToString()を通じ必要な情報を簡単に手に入れることができます。
		\~
		*/
		virtual void OnInformation(ErrorInfo *errorInfo) = 0;

		/** 
		\~korean
		Proud.Exception 으로 통합
		- 일반적으로 유저 코드에서 나는 에러를 처리한다.
		- 유저는 필요한 것들을 구현하면 된다.
		- ExceptionType 으로 해당 Exception 객체를 얻어서 사용하면 된다.
		- _com_error, void*, std::exception 등을 모두 처리한다.
		또한 catch(...)를 처리하는 OnUnhandledException()도 있다.
		- void*는 예외 객체의 타입을 캐스팅해서 받을 수 있다. 가령 MFC CException 을 받는데 사용된다.

		\~english
		Merge to Proud.Exception
		- It handles error that occur from user code. 
		- User realize things that user needed. 
		- You can use Exceptiontype that obtain Exception object. 
		- It process _com_error, void*, std::exception, etc. Also there is OnUnhandledException() that handle catch(...) 
		- void* can obtain exceptional object type by casting. For example, it uses obtaining MFC CException. 

 		\~chinese
		合并为 Proud.Exception%。
		- 一般处理在用户代码发生的错误。
		- 用户体现所需要的即可。
		- 用ExceptionType获得相关Exception对象并全部处理。
		- 处理全部的_com_error, void*, std::exception。
		也有处理catch(...)的OnUnhandledException()。
		- void*可以对例外对象类型选择接收。例如用于接收MFC CException%。

		\~japanese
		Proud.Exception に統合
		- 一般的にユーザーコードより出るエラーを処理します。
		- ユーザーは必要なことを実現すれば良いです。
		- ExceptionTypeで該当Exceptionオブジェクトを得て使えば良いです。
		- _com_error, void*, std::exception などを全て処理します。
		また、catch(...)を処理するOnUnhandledException()もあります。
		- void*は例外オブジェクトのタイプをキャスティングして受けられます。仮にMFC CExceptionを受けることに使われます。
		\~
		*/
		virtual void OnException(const Exception &e) = 0;

		/** 
		\~korean
		RMI가 호출이 들어왔으나 Attach된 Stub 중에 대응하는 RMI가 전혀 없으면 이것이 콜백된다.

		\~english
		When RMI is called but there is no RMI in Stub that attached then this will callback.

		\~chinese
		有RMI呼叫，但是没有对应于Attach的Stub的RMI的话，这个被回调。

		\~japanese
		RMIに呼び出しが入りましたが、AttachされたStubのうち対応するRMIが全く無ければこれがコールバックされます。
		\~
		*/
		virtual void OnNoRmiProcessed(RmiID rmiID) = 0;

		/** 
		\~korean
		사용자가 정의한 메시지를 수신하면 이 함수가 콜백됩니다. 사용자는 이 함수를 오버라이드해서 원하는 처리를 할 수 있습니다.
		자세한 것은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#send_user_message" >RMI를 전혀 안쓰고 사용자 정의 메시지를 주고 받기</a> 를 참고하십시오.
		- Visual Studio 2005 이상의 컴파일러에서는 override 키워드 사용을 권장합니다. 
		
		\param sender 메시지를 송신한 호스트입니다.
		\param rmiContext 수신 부가 정보입니다.
		\param payload 수신된 메시지 본문입니다.
		\param payloadLength 수신된 메시지 본문의 길이(바이트)입니다.

		\~english
		This function will callback once user received defined message. Then user can process as overriding this function.
		Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#send_user_message" >Exchanging user-defined message without using RMI at all</a> for more detail.
		- We recommend to use override keyword on Visual Studio 2005 or above.

		\param sender Host that sent message.
		\param rmiContext Additional information of receiving
		\param payload Main text of received message.
		\param payloadLength Length(byte) of received main text.

		\~chinese
		收到用户定义的信息的话，此函数被回调。用户可以覆盖此函数后进行想要的处理。
		详细的请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#send_user_message" >完全不使用RMI交换用户定义信息</a>%。
		- 在Visual Studio 2005以上的生成器里建议使用override关键字。

		\param sender 传送信息的主机。
		\param rmiContext 收信附加信息。
		\param payload 收信的信息原文。.
		\param payloadLength 收信的信息原文的长度（byte）。

		\~japanese
		ユーザーが定義したメッセージを受信すればこの関数がコールバックされます。ユーザーはこの関数をオーバーライドして願い処理をすることができます。
		詳しくは、\ref send_user_messageをご参照ください。
		- Visual Studio 2005 以上のコンパイラーではoverrideキーワードの使用をお勧めします。 

		\param sender メッセージを送信したホストです。
		\param rmiContext 受信付加情報です。
		\param payload 受信されたメッセージの本文です。
		\param payloadLength 受信されたメッセージ本文の長さ（バイト）です。
		\~
		*/
		 PROUD_API virtual void OnReceiveUserMessage(HostID sender, const RmiContext &rmiContext, uint8_t* payload, int payloadLength);

		/** 
		\~korean
		일정 시간마다 user worker thread pool에서 콜백되는 함수입니다. <a target="_blank" href="http://guide.nettention.com/cpp_ko#server_timer_callback" >서버에서 타이머 루프 RMI 이벤트 처리하기</a> 기능입니다. 		
		\param context CStartServerParameter.m_timerCallbackContext 에서 입력된 값과 동일합니다.

		\~english
		Function called back at user worker thread pool periodically. This is a <a target="_blank" href="http://guide.nettention.com/cpp_en#server_timer_callback" >Managing Timer loop RMI Event from server</a> function.		
		\param context same as the value entered at CStartLanServerParameter.m_timerCallbackContext

		\~chinese
		每隔一段时间在user worker thread pool回调的函数。<a target="_blank" href="http://guide.nettention.com/cpp_zh#server_timer_callback" >在服务器处理Timer Loop，RMI，Event</a> 功能。
		\param context 与在 CStartLanServerParameter.m_timerCallbackContext%输入的值相同。

		\~japanese
		所定時間ごとにuser worker thread poolでコールバックされる関数です。\ref server_timer_callback機能です。
		\param context CStartServerParameter.m_timerCallbackContextで入力された値と同じです。
		\~
		*/
		virtual void OnTick(void* /*context*/){}	
		
		/** 
		\~korean
		RMI 호출 또는 이벤트 발생으로 인해 user worker에서 callback이 호출되기 직전에 호출됩니다.
		프로그램 실행 성능 측정을 위해 사용하셔도 됩니다.

		\~english
		It calls before calling callback at user worker because of RMI calling or event occuring.
		You can use it for measuring program running performance.

		\~chinese
		RMI 呼叫或者因event的发生，在user worker中callback被呼叫之前呼叫。
		也可以为了测定程序执行性能而使用。

		\~japanese
		RMI呼び出しまたはイベント発生によってuser workerでcallbackが呼び出される直前に呼び出されます。
		プログラム実行性能測定のために使っても良いです。
		\~
		*/
		virtual void OnUserWorkerThreadCallbackBegin(CUserWorkerThreadCallbackContext* /*ctx*/){}
		/** 
		\~korean
		RMI 호출 또는 이벤트 발생으로 인해 user worker에서 callback이 리턴한 직후에 호출됩니다.
		프로그램 실행 성능 측정을 위해 사용하셔도 됩니다.

		\~english
		It calls right after calling callback at user worker because of RMI calling or event occuring.
		You can use it for measuring program running performance.

		\~chinese
		RMI 呼叫或者因event发生，在user worker中callback返回之后呼叫。
		也可以用在测定程序执行性能方面。

		\~japanese
		RMI呼び出しまたはイベント発生によってuser workerでcallbackがリターンした直後に呼び出されます。
		プログラム実行性能測定のために使っても良いです。
		\~
		*/
		virtual void OnUserWorkerThreadCallbackEnd(CUserWorkerThreadCallbackContext* /*ctx*/){}

	};

	/**  @} */



	/** Add your event handler to these member objects. 
	
	Example:
	\code
	CNetClient* c = CNetClient::Create();
	c->OnError = [&](ErrorInfo* e)->{
		// Your event handler code here.
	};
	\endcode
	*/
	class HasCoreEventFunctionObjects
	{
	public:
		/// See Proud.INetCoreEvent.OnError for details.
		LambdaBaseHolder_Param1<void, ErrorInfo *> OnError;
		/// See Proud.INetCoreEvent.OnException for details.
		LambdaBaseHolder_Param1<void, const Exception&> OnException;
		/// See Proud.INetCoreEvent.OnInformation for details.
		LambdaBaseHolder_Param1<void, ErrorInfo*> OnInformation;
		/// See Proud.INetCoreEvent.OnNoRmiProcessed for details.
		LambdaBaseHolder_Param1<void, RmiID> OnNoRmiProcessed;
		/// See Proud.INetCoreEvent.OnReceiveUserMessage for details.
		LambdaBaseHolder_Param4<void, HostID, const RmiContext&, uint8_t*, int> OnReceiveUserMessage;
		/// See Proud.INetCoreEvent.OnWarning for details.
		LambdaBaseHolder_Param1<void, ErrorInfo*> OnWarning;
		/// See Proud.INetCoreEvent.OnTick for details.
		LambdaBaseHolder_Param1<void, void*> OnTick;
		/// See Proud.INetCoreEvent.OnUserWorkerThreadCallbackBegin for details.
		LambdaBaseHolder_Param0<void> OnUserWorkerThreadCallbackBegin;
		/// See Proud.INetCoreEvent.OnUserWorkerThreadCallbackEnd for details.
		LambdaBaseHolder_Param0<void> OnUserWorkerThreadCallbackEnd;
	};
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
