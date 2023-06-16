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
#include "Exception.h"
#include "CriticalSect.h"
#include "atomic.h"
#include "Ptr.h"

#if defined __linux__
#include <unistd.h>
#endif

#include "PnThread.h"
#include "SysTime.h"


#ifdef _MSC_VER
#pragma pack(push,8)
#endif

#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif


namespace Proud
{




	/** \addtogroup util_group
	*  @{
	*/



	// just-in-time으로 객체를 생성한다. 스레드세이프하다.
	// 가령 싱글톤에서 이것을 사용한다.
	// 주의: 이 클래스는 ProudNet/include 폴더의 다른 헤더파일에서는 사용하지 마세요! DLL1에서 사용하는 경우 DLL2에서 인스턴스가 유일화되지 않는 문제점을 가질 수 있습니다.
	template<typename T>
	class JitObjectCreator
	{
		RefCount<T> m_object;

		RefCount<T> m_instancePtr;
		CriticalSection m_cs;

		void JitCreateObject()
		{
			// stale not 0이면 => 이미 생성된 것이 있으니까 그것을 리턴
			// stale 0이면 => 아래 lock 구문으로 이동
			if (0 != m_instancePtr)
				return;

			// 생성자가 두번 실행되는 것을 막는다.
			CriticalSectionLock lock(m_cs, true);

			// stale not 0이면 => 그 사이에 이미 만들어진 것이 있으니 그것을 리턴
			// stale 0이면 => 앞서 lock에서 이미 생성 후 m_instancePtr을 not 0을 하고 unlock을 한 상태이다. lock and unlock은 컴파일러와 CPU에 의해 fenced이다. 
			// 따라서 여기서의 stale 0은 fenced 0이기 때문에, 사실상 아직 생성자가 타 스레드에 의해 실행되고 m_instancePtr을 변경한 적 없음을 말한다.
			// 따라서 안전.
			if (0 != m_instancePtr)
				return;

			// 생성자 실행 및 변수 업데이트
			m_instancePtr = RefCount<T>(new T());
		}

	public:
		RefCount<T> GetJitCreatedObject()
		{
			JitCreateObject();

			// 한번 not 0이 됐으면 다시 0이 되는 일은 없다. 따라서 여기서 안전하게 리턴해도 된다.
			return m_instancePtr; // 여기서 shared_ptr 복사가 일어난다. 
		}

		// return value optimization을 못 믿겠으면 이것을 사용하자.
		T* GetJitCreatedObjectPtr()
		{
			JitCreateObject();
			return m_instancePtr.get();
		}


	};





	/**
	\~korean
	JIT 인스턴싱만 thread safe하게 하는 singleton template
	- JIT 인스턴싱 과정만 CS lock을 해서 thread safe하게 하고, 생성된 이후부터는 thread unsafe하게 한다.
	- 생성만 thread 동기화하고 이후부터는 불필요한 경우, 예컨대 CItemTemplate 등에서 이 객체를 써야 드문 빈도의 서버 다운을
	예방 가능.

	일반적 용도
	- 이 클래스의 파생 클래스를 만든다. T에는 파생 클래스의 이름을 넣는다.
	- Instance()를 이용해서 싱글톤의 레퍼런스를 얻는다. Instance()를 최초로 호출하면 객체가 인스턴스화된다.
	- 싱글톤간 파괴 순서를 제어하려면 싱글톤의 생성자에서 의존 대상 싱글톤을 한번 접근해주면 된다.

	사용예
	\code
	class A:public CSingleton<A>
	{
	public:
	void Foo();
	};

	void Aoo()
	{
	A::Instance().Foo();
	}
	\endcode

	DLL과 EXE가 같은 Singleton을 쓰고자 하는 경우
	- CSingleton을 DLL이 제공하고 EXE가 그것을 쓰고자 하는 경우에는 본 클래스를 다음과 같이 구현해야 한다. 안그러면 EXE가 쓰는 singleton과
	DLL이 쓰는 singleton이 서로 다른 인스턴스로 존재하게 된다.
	\code
	class Goofie:protected CSingleton<Goofie> // protected로 선언해서 Instance를 직접 접근하지 못하게 한다.
	{
	public:
	__declspec(dllexport) Goofie& Instance() // CSingleton.Instance()를 오버라이드하되 DLL에서 export되는 함수로 만든다.
	{
	return __super::Instance(); // DLL 모듈 메모리 공간 내에 만들어진 instance를 리턴한다.
	}
	};
	\endcode

	\~english
	Singleton template that makes only JIT instancing to be thread safe.
	- Only CS locks JIT instancing process making it thread safe and makes thread unsafe after created.
	- Only thread synchronized then not needed afterwards, e.g. Server down with little frequency can be prevented by using this object at CItemTemplate ans so on.

	General usage
	- Creates derivative class to this class. The name of the derivative class is to enter at T.
	- Gets reference of Singleton using Instance(). Object is instanced when Instance() is called for the first time.
	- To control the destruction order of Singletons, constructor of Signleton should approach to dependent target Singleton just for once.

	Usage example
	\code
	class A:public CSingleton<A>
	{
	public:
	void Foo();
	};

	void Aoo()
	{
	A::Instance().Foo();
	}
	\endcode

	If DLL and EXE to use same Singleton,
	- This class must be realized as followings for the case that DLL provides CSingleton and EXE uses it. Otherwise, the singleton used by EXE and the singleton used by DLL will exist as different instance.
	\code
	class Goofie:protected CSingleton<Goofie> // Declared as protected to prevent direct access of Instance
	{
	public:
	__declspec(dllexport) Goofie& Instance() // Override CSingleton.Instance() but make it as a function exported from DLL.
	{
	return __super::Instance(); // Returns the instance that is created in DLL module memory space
	}
	};
	\endcode

	\~chinese
	只把JIT instancing进行成thread safe的singleton template。
	- 只把JIT instancing过程CS lock以后使其thread safe，生成以后开始让它thread unsafe。
	- 只同步生成thread，之后不必要的情况，例如要在 CItemTemplate%等使用此对象才能预防很少频率的服务器死机。

	一般用途
	- 创建此类的派生类。往T里输入派生类的名字。
	- 利用Instance()获得singleton的reference。初始呼叫Instance()的话对象会instance化。
	- 想要控制singleton之间的破坏顺序，要在singleton的生成者里接近一次依存对象的singleton。

	使用例
	\code
	class A:public CSingleton<A>
	{
	public:
	void Foo();
	};

	void Aoo()
	{
	A::Instance().Foo();
	}
	\endcode

	DLL 和EXE要使用相同的singleton的时候
	- DLL要提供 CSingleton%，EXE 要使用它的时候要把此类如下体现。否则EXE使用的singleton和DLL使用的singleton会存在成不同的instance。
	\code
	class Goofie:protected CSingleton<Goofie> // 用protected宣告，不让Instance直接接近。
	{
	public:
	__declspec(dllexport) Goofie& Instance() // 覆盖 CSingleton.Instance()，但是制造成在DLL能export的函数。
	{
	return __super::Instance(); // 返回在DLL模块内存空间内制造的instance。
	}
	};
	\endcode

	\~japanese
	JITインスタンシングのみthread safeするようにするsingleton template
	- JITインスタンシング過程のみCS lockをしてthread safeするようにして、生成後からはthread unsafeするようにします。
	- 生成のみthread同期化をして、以降からは不要な場合、例えばCItemTemplateなどでこのオブジェクトを使用すると、偶に発生するサーバーダウンを予防することができます。

	一般的な用途
	- このクラスの派生クラスを作ります。Tには派生クラスの名前を入れます。
	- Instance()を利用してシングルトーンのレファレンスを得ます。Instance()を初めて呼び出すとオブジェクトがインスタント化されます。
	- シングルトーン間の破壊手順を制御するためには、シングルトーンの生成子より依存対象シングルトーンを一度アクセスしてください。

	使用例
	\code
	class A:public CSingleton<A>
	{
	public:
	void Foo();
	};

	void Aoo()
	{
	A::Instance().Foo();
	}
	\endcode

	DLLとEXEが同じSingletonを使おうとする場合
	- CSingletonをDLLが提供してEXEがそれを使おうとする場合には、このクラスを次のように実現しなければなりません。それではなければ、EXEが使うsingletonとDLLが使うsingletonがお互い違うインスタンスで存在するようになります。
	\code
	class Goofie:protected CSingleton<Goofie> // protectedに宣言してInstanceを直接アクセスできないようにします。
	{
	public:
	__declspec(dllexport) Goofie& Instance() // CSingleton.Instance()をオーバーライドしますが、DLLよりexportされる関数に作ります。
	
	return __super::Instance(); // DLLモジュールメモリー空間内に作られたinstanceをリターンします。
	}
	};
	\endcode

	

	\~


	\~korean

	만약 싱글톤끼리 파괴 순서를 보장해야 하는 경우, shared pointer 객체를 얻어서 보관함으로써 파괴 순서를 정의할 수 있습니다.

	\~english

	You can define destruction order of singletons by keeping shared pointer object from singleton.

	\~chinese

	如果要在单例模式之间保障破坏顺序，可以获取并保存shared pointer对象以定义破坏顺序。

	\~japanese
	もし、シングルトーン間での破壊手順を保障する必要がある場合、shared pointerオブジェクトを得て保管箱まで破壊手順を定義することができます。

	\~



	\~korean

	예시 코드

	\~english

	Example code

	\~chinese

	编码例。

	\~japanese
	例示コード

	\~


	\~korean

	\code
	// 싱글톤 클래스입니다.
	class MySingleton:public CSingleton<MySingleton>
	{
	...
	Something();
	};

	// 싱글톤을 접근하는 사용자입니다.
	class MyGoo
	{
	// MyGoo 인스턴스가 존재하는 동안 싱글톤의 생존을 보장하려면 이 멤버를 갖고 있어야 합니다.
	// 물론, 싱글톤 접근을 위해 이 멤버를 사용하십시오.
	MySingleton::PtrType m_mySingleton;

	// 생성자 메서드
	User()
	{
	// 싱글톤 참조 카운트를 증가시키면서, 싱글톤의 생존을 보장시킵니다.
	m_mySingleton = MySingleton::GetInstanceAccessor();
	}

	Foo()
	{
	...
	// 싱글톤을 접근하려면 이미 갖고 있는 싱글톤 스마트포인터 객체를 통해 접근합니다.
	m_mySingleton->Something();
	}
	}

	\endcode

	\~english

	\code
	// Singleton Class
	class MySingleton:public CSingleton<MySingleton>
	{
	...
	Something();
	};

	// It is the user that has access to Singleton.
	class MyGoo
	{
	// While MyGoo instance exists, it must have this member in order to assure Singleton’s survival.
	// Use this member for Singleton access, of course.
	MySingleton::PtrType m_mySingleton;

	// constructor
	User()
	{
	// It guarantee survival of singleton with increasing singleton refer count.
	m_mySingleton = MySingleton::GetInstanceAccessor();
	}

	Foo()
	{
	...
	// To access singleton, use singleton smartpointer object that already has.
	// do not use MySingleton::Instance(), but use this member variable.
	m_mySingleton->Something();
	}
	}

	\endcode

	\~chinese

	\code
	// singleton类。
	class MySingleton:public CSingleton<MySingleton>
	{
	...
	Something();
	};

	// 接近singleton的使用者。
	class MyGoo
	{
	// MyGoo的instance存在期间，想保障singleton生存的话，要拥有此成员。
	// 为了singleton接近，使用此成员吧。
	MySingleton::PtrType m_mySingleton;

	// 生成者方法
	User()
	{
	// 想增加singleton参照count的话，可以保障singleton的生存。
	m_mySingleton = MySingleton::GetInstanceAccessor();
	}

	Foo()
	{
	...
	// 想接近singleton的话，通过已经拥有的singleton smartpointer对象来接近。
	m_mySingleton->Something();
	}
	}
	\endcode

	\~japanese
	\code
	// シングルトーンクラスです。
	class MySingleton:public CSingleton<MySingleton>
	{
	...
	Something();
	};
	// シングルトーンをアクセスするユーザーです。
	class MyGoo
	{
	// MyGoo 인스턴스가 존재하는 동안 싱글톤의 생존을 보장하려면 이 멤버를 갖고 있어야 합니다.
	// MyGooインスタンスが存在する間にシングルトーンの生存を保障するためにはこのメンバーを持っている必要があります。
	// 물론, 싱글톤 접근을 위해 이 멤버를 사용하십시오.
	// もちろん、シングルトーンのアクセスのためにこのメンバーを使ってください。
	MySingleton::PtrType m_mySingleton;

	// 生成子メソッド
	User()
	{

	// シングルトーン参照カウントを増加させながら、シングルトーンの生存を保障させます。
	m_mySingleton = MySingleton::GetInstanceAccessor();
	}

	Foo()
	{
	...

	// シングルトーンをアクセスするためには、既に持っているシングルトーンスマートポインターオブジェクトを通じアクセスします。
	m_mySingleton->Something();
	}
	}

	\endcode

	

	
	\~


	*/

	template<typename T>
	class CSingleton 
	{
		// 복사 방지 : 싱글턴 객체 특성상 복사 코드는 있어선 안됩니다.
		NO_COPYABLE(CSingleton)

	public:
		typedef RefCount<T> PtrType;

	public:
		virtual ~CSingleton() { }

	protected:
		// 생성 방지 : 싱글턴을 사용할 땐, 반드시 GetSharedPtr() or GetUnsafeRef() 로 제한합니다.
		CSingleton() { }

	private:
		static JitObjectCreator<T> m_jitObjectCreator;

	public:
		_Noinline static PtrType GetSharedPtr()
		{
			return m_jitObjectCreator.GetJitCreatedObject();
		}

		// 권장사항 : GetSharedPtr() 사용을 먼저 고려하시고, 속도에 민감하다면, GetUnsafeRef()를 사용하세요.
		static inline T& GetUnsafeRef()
		{
			static T* val = NULL;
			if (!val)
			{
				val = m_jitObjectCreator.GetJitCreatedObjectPtr();
			}
			return *val;
		}
		

		// 사용자가 이것의 변수를 멤버로 가지는 동안 사용자의 객체가 파괴되기 전에는 이 싱글톤이 파괴 안됨을 보장한다.
		class Holder
		{
			PtrType m_sharedPtr;
		public:
			Holder()
			{
				m_sharedPtr = CSingleton<T>::GetSharedPtr();
			}
			// 이 함수가 있어야.
			// 컴파일러가 알아서 파생 클래스의 디폴트 파괴자를 만들기는 하겠지만, 확인 사살을 위해.
			virtual ~Holder() {}
		};

	};

	/**  @} */


	template<typename T>
	JitObjectCreator<T> CSingleton<T>::m_jitObjectCreator;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 전방선언
	template<typename T>
	void GetDllSingletonSharedPtr(RefCount<T>* output);
	template<typename T>
	void GetDllSingletonRawPtr(T** output);

	/** CSingleton은 executable이나 static library에서는 안전하게 사용할 수 있습니다.
	그러나 DLL에서는 CSingleton은 문제가 있습니다.
	
	1. DLL1에 싱글톤을 둡니다.
	2. DLL2는 DLL1의 싱글톤을 액세스합니다.
	3. 그러나 싱글톤은 실제로 두개가 존재해 버립니다.

	이러한 문제를 없애려면 CSingleton대신에 DllSingleton을 사용하십시오.

	사용법:
	
	1. 여러분의 헤더 파일에 DllSingleton<X>을 정의합니다.
	2. namespace Proud를 하나 만들고 그 안에 `DECLARE_DLL_SINGLETON(X_API, X)`를 추가합니다. X_API는 여러분의 DLL에 대한 dllexport, dllimport 매크로입니다.
	3. 여러분의 DLL의 소스 파일에 namespace Proud를 하나 만들고 그 안에 `IMPLEMENT_DLL_SINGLETON(X)`를 추가합니다. 싱글톤의 생성과 파괴순서를 정하고 싶으시면 이 매크로를 여러분의 1개의 소스파일에서만 사용하십시오.

	\code

	////////////////
	// Your header file
	class X: DllSingleton<X> {}

	DECLARE_DLL_SINGLETON(MY_DLL_API, X);

	////////////////
	// Your source file
	IMPLEMENT_DLL_SINGLETON(X);

	\endcode
	*/
	template<typename T>
	class DllSingleton
	{
	public:
		typedef RefCount<T> PtrType;

		// 사용자가 이것의 변수를 멤버로 가지는 동안 사용자의 객체가 파괴되기 전에는 이 싱글톤이 파괴 안됨을 보장한다.
		class Holder
		{
			PtrType m_sharedPtr;
		public:
			Holder()
			{
				m_sharedPtr = DllSingleton<T>::GetSharedPtr();
			}
			// 이 함수가 있어야.
			// 컴파일러가 알아서 파생 클래스의 디폴트 파괴자를 만들기는 하겠지만, 확인 사살을 위해.
			virtual ~Holder() {}
		};

		// 권장사항 : GetSharedPtr() 사용을 먼저 고려하시고, 속도에 민감하다면, GetUnsafeRef()를 사용하세요.
		static inline T& GetUnsafeRef()
		{
			T* output;
			GetDllSingletonRawPtr(&output);   // GetDllSingletonSharedPtr을 쓰면 너무 느려진다. 그래서 이 함수를 쓴다.
			return *output;
		}

		inline static PtrType GetSharedPtr()
		{
			PtrType output;
			GetDllSingletonSharedPtr(&output);
			return output;
		}
	};

}

// 이 매크로는 namespace Proud 안에서 사용하세요.
#define DECLARE_DLL_SINGLETON(DLLSPEC, CLASS) \
	DLLSPEC void GetDllSingletonSharedPtr(RefCount<CLASS>* output); \
	DLLSPEC void GetDllSingletonRawPtr(CLASS** output)

// 이 매크로는 namespace Proud 안에서 사용하세요.	
#define IMPLEMENT_DLL_SINGLETON(CLASS) \
JitObjectCreator<CLASS> g_singleton_##CLASS; \
void GetDllSingletonSharedPtr(RefCount<CLASS>* output) \
{ \
	*output = g_singleton_##CLASS.GetJitCreatedObject(); \
} \
void GetDllSingletonRawPtr(CLASS** output) \
{ \
	*output = g_singleton_##CLASS.GetJitCreatedObjectPtr(); \
}


#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif

#ifdef _MSC_VER
#pragma pack(pop)
#endif
