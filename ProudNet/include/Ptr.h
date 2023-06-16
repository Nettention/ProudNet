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

#include "pnstdint.h" 

#include "BasicTypes.h"
#include "atomic.h"
#include "Enums.h"
#include "ProcHeap.h"
#include "ClassBehavior.h"


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

#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif

	class CLookasideAllocator;

	/** 
	\~korean
	reference count를 세지 않는 덜 smart한 pointer이다.
	- ATL CAutoPtr과 같은 역할을 한다.
	- 이 객체가 파괴될 때 hold하고 있는 pointer의 object를 delete한다. 

	\~english
	A less smart pointer that does not count reference count
	- Acts as ATL CAutoPtr
	- When this object is destructed, the object of the pointer that is held by this object is deleted.

	\~chinese
	不数reference cout的不太smart的pointer。
	- 起着与ATL CAutoPtr%一样的作用。
	- 这个对象被破坏的时候delete正在hold的pointer的object。

	\~japanese
	reference countを数えないあまりスマートではないpointerです。
	- ATL CautoPtrのような役割をします。
	- このオブジェクトが破壊される時にHoldしているpointerのオブジェクトを削除します。
	\~
	*/
	template<typename T>
	class CHeldPtr
	{
	public:
		T* m_p;

		inline CHeldPtr() : m_p(NULL) {}
		
		inline CHeldPtr(T* p_) : m_p(p_) {}
		
		inline CHeldPtr& operator=(T* p_)
		{
			m_p = p_;
			return *this;
		}

		inline ~CHeldPtr()
		{
			if (m_p)
				delete m_p;
		}

		/**
		\~korean 
		object를 제거한다. 

		\~english 
		Remove an object. 

		\~chinese
		删除object。

		\~japanese
		Objectを除去します。
		\~
		*/
		inline void Free()
		{
			if(m_p)
				delete m_p;
			m_p=0;
		}

		/**
		\~korean
		object를 attach한다.
		- 이미 다른 것을 attach하고 있는 경우 그것을 delete하지는 않는다.
		\param p_ attach할 객체의 포인터

		\~english 
		Attach an object
		- If something else is already attached, do not delete it. 
		\param p_ Pointer of the object to be attached. 

		\~chinese
		Attach object。
		- 正在attach其他项的时候不删除它。
		\param p_ 要attach的对象的指针。

		\~japanese
		Objectをattachします。
		- 既に他のものをAttachしている場合、それを削除することはありません。
		\param p_ attachするオブジェクトのポインター
		\~
		*/
		inline void Attach(T* p_)
		{
			m_p = p_;
		}

		/** 
		\~korean
		object를 detach한다. 즉, hold하고 있는 object를 NULL로 세팅한다.
		- 기존 attach된 object를 delete하지는 않는다.
		- 즉, Attach(NULL)과 같은 역할을 한다.

		\~english 
		Detach an object. In other words, the object being held is set to null. 
		- Do not delete the previously attached object. 
		- In other words, it performs the same role as Attach (NULL). 

		\~chinese
		Detach object。即将hold中的object设置成NULL。
		- 不delete之前attach的object。
		- 即起着与Attach(NULL)一样的作用。

		\~japanese
		Objectをdetachします。即ち、holdしているobjectをNULLに設定します。
		- 既存にattachされたobjectを削除することはありません。
		- 即ち、Attach(NULL)のような役割をします。
		\~
		*/
		inline void Detach()
		{
			m_p = NULL;
		}

		inline operator T*() const
		{
			return m_p;
		}

		inline T* get() const
		{
			return m_p;
		}

		inline T* operator->() const
		{
			return m_p;
		}

		NO_COPYABLE(CHeldPtr)
	};

	template<typename T>
	inline bool operator<(const CHeldPtr<T> a, const CHeldPtr<T> b)
	{
		return a.m_p < b.m_p;
	}

// 	 void* RefCount_LookasideAllocator_Alloc(size_t length);
// 	 void RefCount_LookasideAllocator_Free(void* ptr);

	/** 
	\~korean
	Smart pointer 클래스. 자세한 내용은 <a target="_blank" href="http://guide.nettention.com/cpp_ko#smart_ptr" >스마트 포인터</a>에 있다.

	RefCount를 사용시 다음을 주의해야 한다.
	- RefCount가 최초로 객체를 참조할 때 Tombstone이라는 것을 만든다. 그리고 이후의 공동 참조 객체가 등장시 이 tombstone이 공유된다.
	즉 tombstone을 최초에 생성하는 시간 비용(메모리 할당 1회)이 든다. 
	- 서로 다른 연관된 클래스 타입으로 캐스팅이 안된다. 예컨대 RefCount<CFile>을 RefCount<CObject>에 복사할 수 없다.
	이런 경우는 RefCount의 class type으로 가급적 상위 base class를 넣고 코딩시 type cast를 매번 해야 한다. 
	\code
	class Base {};
	class A:public Base {};

	void Foo()
	{
	   RefCount<A> a;
	   RefCount<Base> b;
	   a = b; // 에러
	}

	void Foo2()
	{
	   RefCount<Base> a;
	   RefCount<Base> b(new A);
	   a = b; // OK
	   A* p = (A*)a.get(); // 타입 A를 참조하려면 이렇게 해야 한다.
	}
	\endcode
	
	\param T 스마트 포인터가 다룰 객체의 타입
	\param AllocT AllocType 값 중 하나. 
	\param IsThreadSafe <안씀!> true이면 이 스마트 포인터 변수를 여러 스레드가 동시에 다루어도 안전하다. 그러나 성능 저하가 있다.

	\~english
	Smart pointer class. Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#smart_ptr" >Smart Pointer</a>.

	When using RefCount, the following must be considered.
	- When RefCount refers object for the first time, it creates Tombstone. And when joint reference object appears, this tombstone will be shared.
	  In other words, there is a time cost(memory allocation, 1 time) to create tombstone initially. 
	- Cannot cast to related class types that are different to each other. Which means, RefCount<CFile> cannot be copied to RefCount<CObject>.  
	  Type cast must be performed each time when coding as class type of RefCount entered higher possible base class.
	\code
	class Base {};
	class A:public Base {};

	void Foo()
	{
	   RefCount<A> a;
	   RefCount<Base> b;
	   a = b; // Error
	}

	void Foo2()
	{
	   RefCount<Base> a;
	   RefCount<Base> b(new A);
	   a = b; // OK
	   A* p = (A*)a.get(); // This must be done to refer type A.
	}
	\endcode
	
	\param T Type of object that is to be handled by smart pointer
	\param AllocT one of the values of AllocType 
	\param IsThreadSafe <NEVER USED!> If true then it is safe that many threads handle the variable of this smart pointer. But it reduces overall performance.

	\~chinese
	Smart pointer类。详细内容在<a target="_blank" href="http://guide.nettention.com/cpp_zh#smart_ptr" >Smart指针</a>%。

	使用RefCount的时候要注意以下几点。
	- RefCount最初参考对象的时候创建叫Tombstone的东西。然后如有共同参考对象的登场时此tombstone被共享。
	即产生初次生成tombstone的时间费用（内存分配一次）。
	- 不能用互不相同的关联类类型进行cast。例如不能把RefCount<CFile>复制给RefCount<CObject>。
	这种情况用RefCount的class type尽可能输入上位base class，编程时每次都要进行type cast。
	\code
		class Base {};
		class A:public Base {};

		void Foo()
		{
		   RefCount<A> a;
		   RefCount<Base> b;
		   a = b; // 错误
		}

		void Foo2()
		{
		   RefCount<Base> a;
		   RefCount<Base> b(new A);
		   a = b; // OK
		   A* p = (A*)a.get(); // 想参考A的话要这样做。
		}
	\endcode

	\param T smart指针要处理的对象类型。
	\param AllocT AllocType值中之一。
	\param IsThreadSafe <不用！> true的话姐线程同时处理此smart指针变数也安全。但是性能会下降。

	\~japanese
	Smart pointer クラス。詳しくは、\ref smart_ptrにあります。

	RefCountを使用時に次を注意しなければなりません。
	- RefCountが初めてオブジェクトを参照時にTombstoneということを作ります。そして、以降の共同参照オブジェクトが登場時にこのtombstoneが共有されます。
	即ち、tombstoneを初めて生成する時間コスト(メモリー割当1回)がかかります。 
	- お互い違う関連したクラスタイプでキャスティングがされません。例えば、RefCount<CFile>をRefCount<CObject>にコピーすることはできません。
	このような場合はRefCountのclass typeでなるべく上位のbase classを入れてコーディング時にtype castを毎回しなければなりません。 
\code
	class Base {};
	class A:public Base {};

	void Foo()
	{
	   RefCount<A> a;
	   RefCount<Base> b;
	   a = b; // エラー　
	}

	void Foo2()
	{
	   RefCount<Base> a;
	   RefCount<Base> b(new A);
	   a = b; // OK
	   A* p = (A*)a.get(); // タイプAを参照するためにはこのようにしなければなりません。
	}
	\endcode

	\param T スマートポインターが扱うオブジェクトのタイプ
	\param AllocT AllocType 値のうち一つ。 
	\param IsThreadSafe <使わない!> trueであればこのスマートポインター変数を複数のスレッドが同時に扱っても安全です。しかし、性能低下があります。

	\~
	*/
	template<typename T>
	class RefCount 
	{
	public:
		typedef RefCount<T> Type;
	private:
		struct Tombstone 
		{
			T* m_ptr;

			/* 낮은 확률일 것 같지? 64bit에서는 reference count가 2^32를 넘어가지 말란 법 없다.
			여러 스레드에서 동시에 assign이나 암묵적인 scope out이 있을 경우 얼마든지 thread unsafe는 있을 수 있다. 
			따라서 이 값은 atomic op으로만 다루어져야 한다. */
			volatile intptr_t m_count; 
 
			inline Tombstone() {}
			inline ~Tombstone() {
				delete m_ptr;
			}
#ifdef _WIN32
 #pragma push_macro("new")
 #undef new
		private:
			// new,delete oper 금지시킴
			void* operator new(size_t size);
			void operator delete(void* ptr, size_t size);
		public:
 #pragma pop_macro("new")
#endif
		public:
			/**
			\~korean
			CFirstHeap은 CSingleton 참조. 
			- CSingleton은 RefCount를 참조. 
			- 따라서 CSingleton은 CFirstHeap 사용 금지.
			- object를 생성 해줌
			- 자체적으로 new를 할 수 없게끔 되어있음

			\~english TODO:translate needed
			CFirstHeap refers to CSingleton
			- CSingleton refers to RefCount
			- Therefore, CSingleton is forbidden to use CFirstHeap.
			- It generates objects. 
			- It is made not to do new on its own. 

			\~chinese
			CFirstHeap 参考 CSingleton%。
			- CSingleton%参考RefCount。
			- 因此 CSingleton%禁止使用 CFirstHeap%。
			- 生成object。
			- 不让自己本身做new。

			\~japanese
			CFirstHeapはCSingletonをご参照ください。 
			- CSingletonはRefCountをご参照ください。 
			- よって、CSingletonはCFirstHeap使用禁止
			- objectを生成します。
			- 自体的にnewができなくなっています。
			\~
			*/
			inline static Tombstone* NewInstance()
			{
				Tombstone* ret = (Tombstone*)CProcHeap::Alloc(sizeof(Tombstone));  // 반드시 CProcHeap을 써야 한다. CFirstHeap은 프로세스 종료중 사용 불가이므로.				
				if(ret == NULL)
				 ThrowBadAllocException();
 
				CallConstructor<typename Type::Tombstone>(ret);// <Type::Tombstone>를 붙여야 컴파일러가 엉뚱한 타입 매칭을 하는 버그를 피함
 
				return ret;
			}
 
			/**
			\~korean
			 object를 제거함.
			- 자체적 delete를 할 수 없음
			\param object 제거할 객체

			\~english 
			Removes objects. 
			- It cannot delete on its own. 
			\param object Object to be removed. 

			\~chinese
			删除object。
			- 不能进行本身的delete。
			\param object 要删除的对象。

			\~japanese
			Objectを除去します。
			- 自体的にdeleteを行うことができません。
			\param object 除去するオブジェクト
			\~
			*/
			inline static void DeleteInstance(Tombstone* object)
			{
				CallDestructor<typename Type::Tombstone>(object);// <Type::Tombstone>를 붙여야 컴파일러가 엉뚱한 타입 매칭을 하는 버그를 피함
 
				CProcHeap::Free(object);
			}

			NO_COPYABLE(Tombstone)
		};
 
		Tombstone* m_tombstone;
 
	public:
		/** 
		\~korean
		이 객체를 참조하고 있는 다른 변수 객체들의 갯수를 리턴한다. 즉 참조 횟수다. 

		\~english
		Returns the number of other variable objects that refer this object. In other words, the number of referring.

		\~chinese
		返回参考此对象的其他变数对象个数。即参考次数。

		\~japanese
		このオブジェクトを参照している他の変数オブジェクトの数をリターンします。即ち、参照回数です。
		\~
		*/
		inline intptr_t GetRefCount() 
		{
			if (!m_tombstone)
				return 0;
 
			return m_tombstone->m_count;
		}
 
		/** 
		\~korean
		생성자. 포인터 p가 가리키는 객체의 소유권을 가져온다. 

		\~english
		Constructor. Brings the ownership of the object that pointer p points.

		\~chinese
		生成者。取指针p所指的对象的所有权。

		\~japanese
		生成子。ポインターpが指すオブジェクトの所有権を取り込みます。
		\~
		*/
		inline explicit RefCount(T* p = 0)			
		{
			if(p)
			{
				m_tombstone = Tombstone::NewInstance();
				
				m_tombstone->m_count = 1;
				m_tombstone->m_ptr = p;
			}
			else
				m_tombstone = 0;
		}
		/** 
		\~korean
		복사 대입 연산자 

		\~english
		Copy assignment operator

		\~chinese
		复制赋值运算符。

		\~japanese
		コピー代入演算子
		\~
		*/
		inline RefCount(const Type& other)
		{
			m_tombstone = 0;
			AssignFrom(other);
		}

		inline ~RefCount()
		{
			Reset();
		}

		inline void Reset()
		{
			if (m_tombstone)
			{
				intptr_t result = AtomicDecrementPtr(&m_tombstone->m_count);
				if (result == 0)
				{
					Tombstone::DeleteInstance(m_tombstone);
				}
			}
			m_tombstone = NULL;
		}

		// shared_ptr.reset과 동일
		inline void reset()
		{
			Reset(); 
		}
 
	private:
		inline void AssignFrom(const Type& other)
		{
			// 이미 같은거면 스킵
			if(other.m_tombstone == m_tombstone)
				return;
 
			// 상대측 RefCount 증가
			if(other.m_tombstone)
			{
				AtomicIncrementPtr(&other.m_tombstone->m_count);
			}
 
			// 이쪽 RefCount 감소
			Tombstone* deletePendee = 0;
			if(m_tombstone)
			{
				intptr_t result = AtomicDecrementPtr(&m_tombstone->m_count);
				if(result == 0)
				{
					deletePendee = m_tombstone;
				}
			}
 
			// 본격
			m_tombstone = other.m_tombstone;
 
			// 파괴해야 하는 객체의 뒤늦은 파괴
			if(deletePendee != NULL)
				Tombstone::DeleteInstance(deletePendee);
		}

	public:
		inline Type& operator=(const Type& other)
		{
			AssignFrom(other);
			return *this;
		}
 
		inline operator T* () const
		{
			return m_tombstone ? m_tombstone->m_ptr : 0;
		}
		inline T* operator->() const
		{
			return m_tombstone ? m_tombstone->m_ptr : 0;
		}
		inline T* get() const
		{
			return m_tombstone ? m_tombstone->m_ptr : 0;
		}
 
	};

	/**
	\~korean
	memory page를 통째로 alloc/free할 때 유용하다.

	\~english 

	\~chinese
	把整个memory page进行alloc/free的时候有用。

	\~japanese
	memory pageを丸ごとでalloc/freeする時に有用です。
	\~
	*/
#if defined(_WIN32)
	template<typename T>
	class CVirtualAllocPage
	{
	public:
		T* m_p;
		size_t m_dwSizeOnFree;
		uint32_t m_dwTypeOnFree;

		CVirtualAllocPage()
		{
			m_p=NULL;
			m_dwSizeOnFree=0;
			m_dwTypeOnFree=0;
		}

		~CVirtualAllocPage()
		{
			if(m_p!=NULL)
				::VirtualFree(m_p,m_dwSizeOnFree,m_dwTypeOnFree);
		}

		/** 
		\~korean 
		memory page를 할당하되 자동 해제시 사용될 값도 미리 입력해둔다. 
		
		\~english 
		
		\~chinese
		分配memory page，而且预先输入自动解除时将被使用的值。

		\~japanese
		memory pageを割り当てるが自動解除時に使用される値も前もって入力して置きます。 
		\~
		*/
		void VirtualAlloc(void* lpAddress,size_t dwSize,uint32_t flAllocationType,uint32_t flProtect,size_t dwSizeOnFree,uint32_t dwFreeType)
		{
			void* ret = ::VirtualAlloc(lpAddress,dwSize,flAllocationType,flProtect);
			if(ret == NULL)
				ThrowBadAllocException();

			m_p=(T*)ret;
			m_dwSizeOnFree=dwSizeOnFree;
			m_dwTypeOnFree=dwFreeType;
		}

	public:
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
	};
#endif

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif

	/**  @} */
}

/**
\~korean
RefCount 객체를 CFastMap이나 CFastSet의 key로 쓰려면 이 타입을 Traits로써 사용하면 편하다.

\~english
It is convenient to use this type as Traits in order to use RefCount object as key of CFastMap or CFastSet.

\~chinese
想把RefCount对象用于 CHashMap%或者 CFastSet%的key，把此类使用成Traits会更方便。

\~japanese
RefCount オブジェクトをCFastMapとかCFastSetのキーで使うためには、このタイプをTraitsとして使えば楽です。
\~
*/
template<typename T>
class CPNElementTraits<Proud::RefCount<T> >
{
public:
	typedef Proud::RefCount<T> Type;
	typedef const Type& INARGTYPE;
	typedef Type& OUTARGTYPE;

	inline static uint32_t Hash(INARGTYPE a)
	{
		uintptr_t v = (uintptr_t)a.get();
		return CPNElementTraits<uintptr_t>::Hash(v);
	}

	/**
	\~korean
	두 Element를 비교한다.
	\param element1 비교할 첫번째 element
	\param element2 비교할 두번째 element
	\return 같으면 true 같지 않으면 false

	\~english '
	Compare two elements
	\param element1 First element to be compared
	\param element2 Second element to be compared
	\return True if the same, otherwise false. 


	\~chinese
	比较两个Element。
	\param element1 要比较的第一个element。
	\param element2 要比较的第二个element。
	\return 相同的话true，不同的话false。

	\~japanese
	両Elementを比較します。
	\param element1 比較する1番目のelement
	\param element2 比較する2番目のelement
	\return 同じであればtrue、同じではなければfalse
	\~
	*/
	inline static bool CompareElements(INARGTYPE element1, INARGTYPE element2)
	{
		return element1.get() == element2.get();
	}

	/**
	\~korean
	두 Element를 비교한다.
	\param element1 비교할 첫번째 element
	\param element2 비교할 두번째 element
	\return element1이 더 크면 1, 작으면 -1, 같으면 0

	\~english 
	Compare two elements
	\param element1 First element to be compared
	\param element2 Second element to be compared
	\return 1 if element 1 is bigger, -1 if smaller, 0 if equal. 


	\~chinese
	比较两个Element。
	\param element1 要比较的第一个element。
	\param element2 要比较的第二个element。
	\return element1更大的话1，小的话-1，相同的话0。

	\~japanese
	両Elementを比較します。
	\param element1 比較する1番目のelement
	\param element2 比較する1番目のelement
	\return element1がもっと大きければ1、小さければ-1、同じであれば0
	\~
	*/
	inline static int CompareElementsOrdered(INARGTYPE element1, INARGTYPE element2)
	{
		if (element1.get() < element2.get())
			return -1;
		if (element1.get() == element2.get())
			return 0;
		return 1;
	}

};


#ifdef _MSC_VER
#pragma pack(pop)
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif 
