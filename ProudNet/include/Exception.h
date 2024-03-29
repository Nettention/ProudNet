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

#include <exception>
#include "BasicTypes.h"
#include "PNString.h"
#include "Enums.h"
#include "Ptr.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4324)
#endif

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{

	/** \addtogroup util_group
	*  @{
	*/
	class ErrorInfo;

	/**
	\~korean
	Exception Type 객체
	- type으로 구분하여 해당 exception 객체의 포인터 및 정보를 얻을수 있습니다.

	\~english
	Exception Type object
	- You can get pointer and information of exception object by classifying type.

	\~chinese
	Exception Type对象
	- 由type区分后可以获得相关exception对象的指针及信息。

	\~japanese
	Exception Type オブジェクト
	- typeで区分して該当exceptionオブジェクトのポインター及び情報を得ることができます。
	\~
	*/
	enum ExceptionType
	{
		/**
		\~korean
		없음

		\~english
		None

		\~chinese
		无

		\~japanese
		なし
		\~
		*/
		ExceptionType_None = 0,

		/**
		\~korean
		 Exception(LPCWSTR pFormat, ...); 으로 생성된 Exception

		\~english
		Exception that created by Exception(LPCWSTR pFormat, ...);

		\~chinese
		生成为Exception(LPCWSTR pFormat, ...);的Exception。

		\~japanese
		Exception(LPCWSTR pFormat, ...); から生成された Exception
		\~
		*/
		ExceptionType_String,

		/**
		\~korean
		Exception(std::exception& src); 으로 생성된 Exception

		\~english
		Exception that created by Exception(std::exception& src);

		\~chinese
		生成为Exception(std::exception& src);的Exception。

		\~japanese
		Exception(std::exception& src); から生成された Exception
		\~
		*/
		ExceptionType_Std,

		/**
		\~korean
		Exception(_com_error& src); 으로 생성된 Exception

		\~english
		Exception that created by Exception(_com_error& src);

		\~chinese
		生成为Exception(_com_error& src);的Exception。

		\~japanese
		Exception(_com_error& src); から生成された Exception
		\~
		*/
		ExceptionType_ComError,

		/**
		\~korean
		Exception(void* src); 으로 생성된 Exception

		\~english
		Exception that created by Exception(void* src);

		\~chinese
		生成为Exception(void* src);的Exception。

		\~japanese
		Exception(void* src); から生成された Exception
		\~
		*/
		ExceptionType_Void,

		/**
		\~korean
		Exception(ErrorInfo* src); 으로 생성된 Exception

		\~english
		Exception that created by Exception(ErrorInfo* src);

		\~chinese
		生成为Exception(ErrorInfo* src);的Exception。

		\~japanese 
		Exception(ErrorInfo* src); から生成された Exception
		\~
		*/
		ExceptionType_ErrorInfo,

		/**
		\~korean
		Exception() 으로 생성된 UnhandleException

		\~english
		UnhandleException that created by Exception()

		\~chinese
		生成为Exception()的UnhandleException。

		\~japanese
		Exception() から生成された UnhandleException
		\~
		*/
		ExceptionType_Unhandled,

		/**
		\~korean
		CLR을 사용하는 경우 Managed code에서 발생한 Exception

		\~english
		Exception occurred in the managed code in case of using a CLR. 

		\~chinese
		使用CLR时在Managed code中发生的Exception

		\~japanese
		CLRを使用する場合、Managed codeより発生したException

		\~
		*/
		ExceptionType_Clr
	};

	/**
	\~korean
	printf()처럼 에러 메시지를 받아 보낼 수 있는 exception 객체.
	통상 다음과 같이 쓴다.

	\~english
	An exception object that can receive and send an error message as printf().
	It's commonly used as followed.

	\~chinese
	像printf()一样能接收并发送错误信息的exception对象。
	一般写入如下。

	\~japanese
	printf()のようにエラーメッセージを受けて送ることができるexceptionオブジェクト
	通常、次のように使用します。

	\~

	\code
		throw Exception("error=%d",error);
	\endcode

	*/
	class Exception : public std::exception // std::exception은 defacto C++ 표준임. 지우지 말 것!
	{
	private:
		void operator= (const Exception& src);

	public:
		/**
		\~korean
		Exception 을 유발시킨 remote 의 hostID

		\~english
		hostID of remote that occured Exception

		\~chinese
		诱发Exception的remote的host ID。

		\~japanese
		Exception を誘発されたremoteのhostID
		\~
		*/
		HostID m_remote;

		/**
		\~korean
		void*형 Exception 객체의 포인터를 저장한다.

		\~english
		Save pointer of void*type Exception object

		\~chinese
		储存void*型Exception对象的指针。

		\~japanese
		void*型Exceptionオブジェクトのポインターを保存します。
		\~
		*/
		void* m_pVoidSource;

		/**
		\~korean
		std::exception 객체의 포인터를 저장한다.

		\~english
		Save pointer of std::exception object

		\~chinese
		储存 std::exception%对象的指针。

		\~japanese
		std::exception オブジェクトのポインターを保存します。
		\~
		*/
		std::exception *m_pStdSource;

		/**
		\~korean
		Exception Type 객체

		\~english
		Exception Type object

		\~chinese
		Exception Type对象。

		\~japanese
		Exception Type オブジェクト
		\~
		*/
		ExceptionType m_exceptionType;

		/**
		\~korean
		ErrorInfo 객체의 복제본을 저장한다.

		\~english 
		Save the copy of ErrorInfo object. 

		\~chinese 
		保存ErrorInfo对象的副本。

		\~japanese 
		ErrorInfo オブジェクトのコピーを保存します。
		\~
		*/
		RefCount<ErrorInfo> m_errorInfoSource;

		/**
		\~korean
		- 사용자 정의 콜백에서 Exception이 발생한 경우 해당 함수명이 기록됩니다.
		- 사용자 정의 RMI Stub에서 Exception이 발생한 경우 해당 RMI ID가 기록됩니다.

		\~english
		- The relevant name of function will be recorded in case exception occurs at user definition callback.
		- The relevant RMI ID will be recorded in case exception occurs at user definition RMI Stub.

		\~chinese
		- 在自定义Callback中发生Exception时，该函数名将被记录。
		- 在自定义RMI Stub中发生Exception时，该RMI ID将被记录。

		\~japanese
		- ユーザー定義CallbackからExceptionが発生した場合、該当関数名が記録されます。
		- ユーザー定義RMI StubからExceptionが発生した場合、該当RMI IDが記録されます。
		\~
		*/
		String m_userCallbackName;

		/**
		\~korean
		- 사용자 정의 콜백에서 Exception이 발생한 경우 해당 콜백이 구현된 객체(SetEventSink로 등록한 객체)의 주소가 입력됩니다.
		- 사용자 정의 RMI Stub에서 Exception이 발생한 경우 해당 Stub이 구현된 IRmiStub객체(AttachStub으로 등록한 객체)의 주소가 입력됩니다.
		- m_userCallbackName으로 문제의 함수를 판별하기 어려운 경우 이 변수의 값이 힌트가 될 수 있습니다.

		\~english
		- In case exception occurs at user definition callback, the address of object that has been registered by SetEventSink will be inputted.
		- In case exception occurs at user definition RMI Stub, the address of IRmiStub object that has been registered by AttachStub will be inputted.
		- This variable value could be the hint in case it is difficult to find out the problematic function by m_userCallbackName.

		\~chinese
		- 在自定义Callback中发生Exception时， 该Callback所体现的对象（以SetEventSink注册的对象）的地址将被输入。
		- 在自定义RMI Stub中发生Exception时， 该Stub所体现的IRmiStub对象（以AttachStub注册的对象）的地址将被输入。
		- 以m_userCallbackName很难判断问题函数时，该变数值也许是提示。


		\~japanese
		- ユーザー定義CallbackからExceptionが発生した場合、該当Callbackが具現されたオブジェクト(SetEventSinkと登録されたオブジェクト)のアドレスが入力されます。
		- ユーザー定義RMI StubからExceptionが発生した場合、該当Stubが具現されたIRmiStubオブジェクト(AttachStubと登録されたオブジェクト)のアドレスが入力されます。
		- m_userCallbackNameで問題の関数を判別しにくい場合、この変数の値がヒントになります。

		\~
		*/
		void* m_delegateObject;

		StringA chMsg;// public because of Exception_UpdateFromComError

	public:
		 PROUD_API Exception(void);
		 PROUD_API Exception(const char* text);
		 PROUD_API Exception(const wchar_t* text);
		 PROUD_API Exception(std::exception& src);
		 PROUD_API Exception(ErrorInfo* src);
		 PROUD_API Exception(const Exception& src);

		 PROUD_API ~Exception(void) throw();

		/**
		\~korean
		Exception 에 대한 문자열 포인터를 리턴합니다.

		\~english 
		Return the string pointer to the exception. 

		\~chinese
		返回对Exception的字符串指针。

		\~japanese
		Exception に対する文字列ポインターをリターンします。
		\~
		*/
		 PROUD_API const char *what( ) const throw();

	};

	 void XXXHeapChkImpl(int index = 1); // PN DLL이면 이거 세다!

#if defined(CHECK_HEAP_CORRUPTION) && defined(_WIN32)
	#define XXXHeapChk XXXHeapChkImpl
#else
	#if (defined(_MSC_VER) && _MSC_VER>=1500) // 2005 버젼 이하에서는 가변인자 지원안됨.
		#define XXXHeapChk(...)
	#else
		#if defined(_WIN32)
			#define XXXHeapChk __noop
		#else
			#define XXXHeapChk(...)
		#endif
	#endif
#endif

	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
