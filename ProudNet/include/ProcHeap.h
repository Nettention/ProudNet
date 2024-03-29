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

#include "BasicTypes.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/

	class CMemoryHeap;

	/**
	\~korean
	ProudNet 자체에서 보유하는 메모리 Heap. Process Heap이 아니다!
	- 이 클래스는 ProudNet 내부에서 쓴다. 과거에는 CFastArray 등 많은 곳에서 malloc,free를 썼지만
	이것이 debug CRT에서 redefine되기 때문에 debug 빌드에서 다르게 작동한다. 특히
	ProudNet이 DLL 버전인 경우 malloc,free는 msvcr90d.dll!_free_dbg 등에서 런타임 에러를 내기 때문에
	이 클래스의 사용이 불가피하다.
	- 그러나 CSingleton 을 내부적으로 사용하고 있다. CSingleton 은 CFirstHeap 이 아닌 CProcHeap 을 의존한다.

	\~english
	This is a memory heap that is owned by ProudNet. This is not 'Process Heap' at all!
	- This class is used inside of ProudNet. In the past, malloc and free used by many such as CFastArray but since this is re-defined at debug CRT and works differently in debug build.
	Especially when ProudNet is DLL version, malloc and free cause runtime error at msvcr90d.dll!_free_dbg therefore it is necessary to use this class.
	- However, this uses CSingleton internally. CSingleton is depended on CProcHeap, not CFirstHeap.

	\~chinese
	在ProudNet本身保留的储存器Heap.不是Process Heap！
	- 此类在ProudNet内部使用。过去在 CFastArray%等许多地方用了malloc,free，但是这个在debug CRT里redefine，所以在debug build中的运转不同。特别是ProudNet是DLL版本时，malloc,free 在msvcr90d.dll!_free_dbg 等发生运行事件错误，此类的使用是不可避免的。
	- 但是在内部使用 CSingleton%。 CSingleton 依靠 CProcHeap%而不是 CFirstHeap%。

	\~japanese
	ProudNet の自体が保有するメモリーHeap. Process Heapではありません！
	- このクラスはProudNetの内部で使います。過去にはCFastArrayなど様々な所で malloc、freeを使っていましたが、これがdebug CRTでredefineになるため、debugビルドで違う作動をします。特に、ProudNetがDLLバージョンである場合、malloc、freeはmsvcr90d.dll!_free_dbgなどでランタイムエラーを出しますので、このクラスの使用は仕方ありません。
	- しかし、CSingletonを内部的に使っています。CSingletonはCFirstHeapではないCProcHeapに依存します。
	\~
	*/
	class CFirstHeap
	{
	public:
		 PROUD_API static void* Alloc(size_t size);
		 PROUD_API static void* Realloc(void* ptr, size_t size);
		 PROUD_API static void Free(void* ptr);

		 static CMemoryHeap* GetHeap();
	};

	/**
	\~korean
	CFirstHeap 과 달리 이것은 Process Heap을 직접 다룬다.
	- 이것은 CSingleton 이 의존하는 Heap 클래스이다.

	\~english
	Not as CFirstHeap, this directly manages Process Heap.
	- This is Heap class that CSingleton is depended on.

	\~chinese
	与 CFirstHeap%不同，这个直接处理Process Heap。
	- 这是 CSingleton%依靠的Heap类。

	\~japanese
	CFirstHeapと違ってこれは Process Heapを直接取り扱います。
	- これはCSingletonが依存するHeapクラスです。
	\~
	*/
	class CProcHeap
	{
	public:
		PROUD_API static void* Alloc(size_t size);
		PROUD_API static void* Realloc(void* ptr, size_t size);
		PROUD_API static void Free(void* ptr);
	};

	class CMemoryHeap
	{
	public:
#if !defined(_WIN32)
		 static CMemoryHeap* NewFromHeapHandle();
		 static CMemoryHeap* NewHeap();
#else
		 static CMemoryHeap* NewFromHeapHandle(HANDLE hHeap, bool autoDestroy);
		 static CMemoryHeap* NewHeap(bool autoDestroy);
#endif

		virtual void* Alloc(size_t size) = 0;
		virtual void* Realloc(void* ptr, size_t size) = 0;
		virtual void Free(void* ptr) = 0;
		virtual int DebugCheckConsistency() = 0;

		virtual ~CMemoryHeap() {}
	};

	/**  @} */

}

#define DECLARE_NEW_AND_DELETE // 사실상 CProcHeap::Alloc() = malloc()이기 때문에, 그냥 막아버렸다.
// #define DECLARE_NEW_AND_DELETE					\
// 	void* operator new(size_t size)					\
// 		{															\
// 		void *ret = CProcHeap::Alloc(size);						\
// 		if (ret == NULL)										\
// 			ThrowBadAllocException();							\
// 		return ret;												\
// 		}															\
// 	void operator delete(void* ptr, size_t)			\
// 		{															\
// 		return CProcHeap::Free(ptr);							\
// 		}
// 

/* UE4 등은 operator new, delete를 override하는데, 이미 자체적으로 obj-pool 등을 쓰는 경우 꼬일 수 있다.
이 때문에 obj-pool로 다뤄지는 것들은 아래 매크로를 안에서 쓰도록 하자.
UE4는 throw keyword를 못 쓰므로 ThrowBadAllocException를 쓴다.
*/
#define DECLARE_NEW_AND_DELETE_THROWABLE \
 	void* operator new(size_t size)					\
 		{															\
 		void *ret = CProcHeap::Alloc(size);						\
 		if (ret == NULL)										\
 			ThrowBadAllocException();							\
 		return ret;												\
 		}															\
 	void operator delete(void* ptr, size_t)			\
 		{															\
 		return CProcHeap::Free(ptr);							\
 		}
 
#ifdef _MSC_VER
#pragma pack(pop)
#endif
