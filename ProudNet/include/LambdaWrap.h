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

#include "CallbackContext.h"
#include <functional>
#include "Ptr.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

//////////////////////////////////////////////////////////////////////////
// 람다식 지원 컴파일러와 미지원 컴파일러 모두를 위한 함수 객체 관련 정의들 
// 프넷 내부 모듈이므로. 번역하지 마세요.

namespace Proud
{
	/* 파라메터 갯수가 몇개든 상관없이 처리 가능한 일반적인 람다식 객체의 베이스 클래스.
	내부적으로 std::function이 없으므로, 항상 고정 크기 클래스임을 보장한다.
	그리고 C++11을 끄거나 람다식 미지원 컴파일러에서도 이 클래스는 존재가 가능하다.
	따라서 미리 빌드된 라이브러리에서의 이식성을 보장한다.
	RETURN에는 람다식의 리턴 타입을 넣자. void도 가능.

	사용 예:
	shared_ptr<LambdaBase_Param0<void>> p1;

	std::function<void()> f = [](){...}; // 함수 객체
	p1 = new Lambda_Param0<void()>(f); // 함수 객체를 넣는다.


	Q: 람다식 미지원 컴파일러에서도 이것을 살려야 하나요?
	A: 실체 멤버이기 때문에 없으면 찾기 힘든 버그로 이어질 수 있습니다.
	자세한 것은 Sole/doc 내 Lambda support.pptx를 참고하시길.
	*/


	// 함수 객체를 실제로 가지는 클래스의 인터페이스 클래스.
	// 람다식 미지원 컴파일러에서도 생성 가능하다.
	template<typename RETURN>
	class LambdaBase_Param0
	{
	public:
		virtual RETURN Run() = 0;
		virtual ~LambdaBase_Param0() {}
	};

#ifdef SUPPORTS_LAMBDA_EXPRESSION
	// 함수 객체를 실제로 가지는 클래스.
	// RETURN: void() or bool() 등이 들어가면 된다. 파라메터 0개.
	template<typename RETURN>
	class Lambda_Param0 :public LambdaBase_Param0 < RETURN >
	{
		// thread and context
		std::function<RETURN()> m_lambda;
	public:
		Lambda_Param0(const std::function<RETURN()> &lambda) { m_lambda = lambda; }
		void Run() { m_lambda(); }
	};

#endif

	/*

	사용예

	LambdaBaseHolder_Param0 p;
	std::function<...> f = [](){...};
	p = f;
	*/

	template<typename RETURN>
	class LambdaBaseHolder_Param0
	{
		typedef LambdaBase_Param0<RETURN> BaseType;
#ifdef SUPPORTS_LAMBDA_EXPRESSION
		typedef Lambda_Param0<RETURN> LambdaType;
#endif

	public:
		// 함수 객체 본체.
		// 람다 미지원 컴파일러에서도 멤버로 존재해야. 실존 멤버이므로.
		// We don't use shared_ptr as this is public in include/ folder.
		RefCount<BaseType> m_functionObject;

#ifdef SUPPORTS_LAMBDA_EXPRESSION

		LambdaBaseHolder_Param0() {}
		LambdaBaseHolder_Param0(const std::function<RETURN()>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
		}

		LambdaBaseHolder_Param0& operator=(const std::function<RETURN()>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
			return *this;
		}
#endif
	};



	//////////////////////////////////////////////////////////////////////////
	// 아래 클래스들은 위에와 같고, 단지 파라메터 갯수만 다르다.

	template<typename RETURN, typename PARAM1>
	class LambdaBase_Param1
	{
	public:
		virtual RETURN Run(PARAM1 param1) = 0;
		virtual ~LambdaBase_Param1() {}
	};

#ifdef SUPPORTS_LAMBDA_EXPRESSION
	template<typename RETURN, typename PARAM1>
	class Lambda_Param1 :public LambdaBase_Param1 < RETURN, PARAM1 >
	{
		// thread and context
		std::function<RETURN(PARAM1)> m_lambda;
	public:
		Lambda_Param1(const std::function<RETURN(PARAM1)> &lambda) { m_lambda = lambda; }
		void Run(PARAM1 param1) { m_lambda(param1); }
	};
#endif

	template<typename RETURN, typename PARAM1>
	class LambdaBaseHolder_Param1
	{
		typedef LambdaBase_Param1<RETURN, PARAM1> BaseType;
#ifdef SUPPORTS_LAMBDA_EXPRESSION
		typedef Lambda_Param1<RETURN, PARAM1> LambdaType;
#endif

	public:
		// 람다 미지원 컴파일러에서도 멤버로 존재해야. 실존 멤버이므로.
		RefCount<BaseType> m_functionObject;

#ifdef SUPPORTS_LAMBDA_EXPRESSION
		LambdaBaseHolder_Param1() {}
		LambdaBaseHolder_Param1(const std::function<RETURN(PARAM1)>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
		}
		
		LambdaBaseHolder_Param1& operator=(const std::function<RETURN(PARAM1)>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
			return *this;
		}
#endif
	};

	template<typename RETURN, typename PARAM1, typename PARAM2>
	class LambdaBase_Param2
	{
	public:
		virtual RETURN Run(PARAM1 param1, PARAM2 param2) = 0;
		virtual ~LambdaBase_Param2() {}
	};

#ifdef SUPPORTS_LAMBDA_EXPRESSION
	template<typename RETURN, typename PARAM1, typename PARAM2>
	class Lambda_Param2 :public LambdaBase_Param2 < RETURN, PARAM1, PARAM2 >
	{
		// thread and context
		std::function<RETURN(PARAM1, PARAM2)> m_lambda;
	public:
		Lambda_Param2(const std::function<RETURN(PARAM1, PARAM2)> &lambda) { m_lambda = lambda; }
		void Run(PARAM1 param1, PARAM2 param2) { m_lambda(param1, param2); }
	};
#endif

	template<typename RETURN, typename PARAM1, typename PARAM2>
	class LambdaBaseHolder_Param2
	{
		typedef LambdaBase_Param2<RETURN, PARAM1, PARAM2> BaseType;
#ifdef SUPPORTS_LAMBDA_EXPRESSION
		typedef Lambda_Param2<RETURN, PARAM1, PARAM2> LambdaType;
#endif

	public:
		// 람다 미지원 컴파일러에서도 멤버로 존재해야. 실존 멤버이므로.
		RefCount<BaseType> m_functionObject;

#ifdef SUPPORTS_LAMBDA_EXPRESSION
		LambdaBaseHolder_Param2() {}

		LambdaBaseHolder_Param2(const std::function<RETURN(PARAM1, PARAM2)>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
		}

		LambdaBaseHolder_Param2& operator=(const std::function<RETURN(PARAM1, PARAM2)>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
			return *this;
		}
#endif
	};

	template<typename RETURN, typename PARAM1, typename PARAM2, typename PARAM3>
	class LambdaBase_Param3
	{
	public:
		virtual RETURN Run(PARAM1 param1, PARAM2 param2, PARAM3 param3) = 0;
		virtual ~LambdaBase_Param3() {}
	};

#ifdef SUPPORTS_LAMBDA_EXPRESSION
	template<typename RETURN, typename PARAM1, typename PARAM2, typename PARAM3>
	class Lambda_Param3 :public LambdaBase_Param3 < RETURN, PARAM1, PARAM2, PARAM3 >
	{
		// thread and context
		std::function<RETURN(PARAM1, PARAM2, PARAM3)> m_lambda;
	public:
		Lambda_Param3(const std::function<RETURN(PARAM1, PARAM2, PARAM3)> &lambda) { m_lambda = lambda; }
		void Run(PARAM1 param1, PARAM2 param2, PARAM3 param3) { m_lambda(param1, param2, param3); }
	};
#endif

	template<typename RETURN, typename PARAM1, typename PARAM2, typename PARAM3>
	class LambdaBaseHolder_Param3
	{
		typedef LambdaBase_Param3<RETURN, PARAM1, PARAM2, PARAM3> BaseType;
#ifdef SUPPORTS_LAMBDA_EXPRESSION
		typedef Lambda_Param3<RETURN, PARAM1, PARAM2, PARAM3> LambdaType;
#endif

	public:
		// 람다 미지원 컴파일러에서도 멤버로 존재해야. 실존 멤버이므로.
		RefCount<BaseType> m_functionObject;

#ifdef SUPPORTS_LAMBDA_EXPRESSION
		LambdaBaseHolder_Param3() {}
		
		LambdaBaseHolder_Param3(const std::function<RETURN(PARAM1, PARAM2, PARAM3)>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
		}
		LambdaBaseHolder_Param3& operator=(const std::function<RETURN(PARAM1, PARAM2, PARAM3)>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
			return *this;
		}
#endif
	};

	template<typename RETURN, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4>
	class LambdaBase_Param4
	{
	public:
		virtual RETURN Run(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) = 0;
		virtual ~LambdaBase_Param4() {}
	};

#ifdef SUPPORTS_LAMBDA_EXPRESSION
	template<typename RETURN, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4>
	class Lambda_Param4 :public LambdaBase_Param4 < RETURN, PARAM1, PARAM2, PARAM3, PARAM4 >
	{
		// thread and context
		std::function<RETURN(PARAM1, PARAM2, PARAM3, PARAM4)> m_lambda;
	public:
		Lambda_Param4(const std::function<RETURN(PARAM1, PARAM2, PARAM3, PARAM4)> &lambda) { m_lambda = lambda; }
		void Run(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { m_lambda(param1, param2, param3, param4); }
	};
#endif


	template<typename RETURN, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4>
	class LambdaBaseHolder_Param4
	{
		typedef LambdaBase_Param4<RETURN, PARAM1, PARAM2, PARAM3, PARAM4> BaseType;
#ifdef SUPPORTS_LAMBDA_EXPRESSION
		typedef Lambda_Param4<RETURN, PARAM1, PARAM2, PARAM3, PARAM4> LambdaType;
#endif

	public:
		// 람다 미지원 컴파일러에서도 멤버로 존재해야. 실존 멤버이므로.
		RefCount<BaseType> m_functionObject;

#ifdef SUPPORTS_LAMBDA_EXPRESSION
		LambdaBaseHolder_Param4() {}

		LambdaBaseHolder_Param4(const std::function<RETURN(PARAM1, PARAM2, PARAM3, PARAM4)>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
		}

		LambdaBaseHolder_Param4& operator=(const std::function<RETURN(PARAM1, PARAM2, PARAM3, PARAM4)>& lambda)
		{
			m_functionObject = RefCount<BaseType>(new LambdaType(lambda));
			return *this;
		}
#endif
	};


#ifdef SUPPORTS_LAMBDA_EXPRESSION
	/* 로컬 변수로 이것을 사용해서 scope out시 특정 코드가 실행되게 한다.
	타 언어의 try-finally 구문 대용이다.

	추후 C++ 미래 버전에서 std::finally가 도입되면 얘기가 달라지겠지만, 일단은 이걸 자체구현해서 사용하자. 
	
	사용 예:
	\code
	int a;
	a++;
	finally dec_it([&]
	{ 
		a--; 
	});
	...;
	\endcode
	*/
	template<typename T>
	class finally
	{
#ifdef __GNUC__
		T m_lambda;			// gcc에서 발생하는 문제 회피
#else
		const T& m_lambda;	// &가 있어야 copy 로 인한 성능낭비가 없다.
#endif
	public:
		finally(const T & lambda)
			: m_lambda(lambda)
		{
		}
		inline ~finally()
		{
			m_lambda();
		}
	};
#endif


} // namespace

#ifdef _MSC_VER
#pragma pack(pop)
#endif