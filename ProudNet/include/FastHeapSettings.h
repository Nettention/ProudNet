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

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud 
{
	/** \addtogroup util_group
	*  @{
	*/

	class CMemoryHeap;
	class CriticalSection;

	/** 
	\~korean
	Fast Heap을 접근하는 방식입니다.

	\~english
	This is how to access Fast Heap.

	\~chinese
	接近Fast Heap的方式。

	\~japanese
	Fast Heapをアクセスする方式です。
	\~
	*/
	enum FastHeapAccessMode
	{
		/** 
		\~korean
		단 한개의 스레드만이 Fast Heap을 접근하지만 Fast Heap 내부가 Thread safe하지 않습니다.
		이 설정은 단 한개의 스레드만이 Fast Heap을 접근한다는 것이 보장될 때만 사용하십시오!

		\~english
		Only 1 tread accesses to Fast Heap but the inside of Fast Heap is not Thread safe.
		This setting must be used when there is a guarantee that onl 1 thread accessed to Fast Heap.!

		\~chinese
		虽然只有一个线程接近Fast Heap，但Fast Heap内部不Thread safe。
		仅在保障只有一个线程接近Fast Heap时使用此设置！

		\~japanese
		たった一つのスレッドのみがFast Heapをアクセスしますが、Fast Heap内部がThread safeではありません。
		この設定はたった一つのスレッドのみがFast Heapをアクセスするということが保障される時のみ使ってください！
		\~
		*/
		FastHeapAccessMode_UnsafeSingleThread,
		
		/** 
		\~korean
		여러 스레드에서 Fast Heap을 접근해도 안전하게 작동합니다. 그리고 모든 CPU 코어를 활용하는 작동법을 구사합니다.

		\~english
		This operates safely even if multiple threads accee to Fast Heap. And has a way to utilize all of CPU cores.

		\~chinese
		尽管多个线程接近Fast Heap，其也能安全地运转。然后应用所有CPU核心的运转方法。

		\~japanese
		複数のスレッドでFast Heapをアクセスしても安全に作動します。そして、すべてのCPUコアを活用する作動法を駆使します。
		\~
		*/
		FastHeapAccessMode_MultiThreaded,
	};

	/** 
	\~korean
	Fast Heap과 Lookaside Allocator의 환경설정을 담고 있습니다.

	\~english
	Settings environment for Fast Heap and Lookaside Allocator

	\~chinese
	包含着Fast Heap和Lookaside Allocator的环境设置。

	\~japanese
	Fast HeapとLookaside Allocatorの環境設定を込めています。
	\~
	*/
	class CFastHeapSettings
	{
	public:
		/** 
		\~korean
		만약 메모리 관리자가 ProudNet에서 미리 생성한 전용 Heap이 아닌 다른 Heap을 사용하게 만들고자 한다면 여기에 CMemoryHeap 객체의 포인터를
		넣으면 됩니다. 
		- 이 설정과 관련된 메모리 매니저가 파괴시 여기서 지정한 CMemoryHeap 을 파괴하지는 않습니다.
		- NULL을 넣으면 ProudNet에서 미리 생성한 전용 Heap을 사용합니다.

		\~english
		If memory manager intends to let other Heap to be used rather than previously created custom Heap from ProudNet then this is where pointer of CMemoryHeap object to be entered.
		- When the memory manager related to this setting is destructed, CMemoryHeap designated in here is not to be destructed.
		- When NULL entered, the Heap previously created by ProudNet is used.
		
		\~chinese
		如果储存器管理员想制作不是ProudNet事先生成的专用Heap，而是其他Heap的话，在这里填入 CMemoryHeap%对象的指针。
		- 与此设置相关的储存器管理员被破坏时，并不会破坏在这里指定的 CMemoryHeap%。
		- 放入NULL的话使用在ProudNet事先生成的专用Heap。

		\~japanese
		もし、メモリー管理者がProudNetで既に生成した専用Heapではない他のHeapを使うようにしたい場合は、ここにCMemoryHeapオブジェクトのポインターを入れてください。
		- この設定に関連したメモリーマネージャーが破壊時にここで指定したCMemoryHeapを破壊することはありません。
		- NULLを入れればProudNetで既に生成した専用Heapを使います。
		\~
		*/
		CMemoryHeap* m_pHeap;

		/** 
		\~korean
		스레딩 모드입니다.

		\~english
		Threading mode 

		\~chinese
		Threading 模式。

		\~japanese
		スレッディングモードです。
		\~
		*/
		FastHeapAccessMode m_accessMode;

		/** 
		\~korean
		단 한개의 스레드에서만 이 fast heap을 사용하는지 검사하는 Critical section 객체입니다.
		- fast heap이 FastHeapAccessMode_UnsafeSingleThread 인 경우에만 유효합니다.
		- fast heap이 FastHeapAccessMode_UnsafeSingleThread 인 경우, 이 멤버를 사용자가 설정해 주어야 합니다.
		- 이 기능은 debug mode에서만 작동합니다.

		\~english
		This is Critical section object that checks if only 1 thread uses this fast heap.
		- Only valid if the fast heap is FastHeapAccessMode_UnsafeSingleThread
		- If the fast heap is FastHeapAccessMode_UnsafeSingleThread, then this member must be set by user.
		- This function only operates in debug mode. 

		\~chinese
		检查是否只在一个线程使用此fast heap的Critical section对象。
		- 只在fast heap是FastHeapAccessMode_UnsafeSingleThread的情况下有效。
		- fast heap 是FastHeapAccessMode_UnsafeSingleThread的情况时，用户要设置此成员。
		- 此功能只在debug mode运转。

		\~japanese
		たった一つのスレッドでのみこのFast Heapを使うのかを検査するCritical sectionオブジェクトです。
		- fast heap이 FastHeapAccessMode_UnsafeSingleThreadであｒｙ場合のみ有効です。
		- fast heap이 FastHeapAccessMode_UnsafeSingleThreadである場合、このメンバーをユーザーが設定する必要があります。
		- この機能はdebug modeでのみ作動します。
		\~
		*/
		CriticalSection* m_debugSafetyCheckCritSec;

		PROUD_API CFastHeapSettings();
	};

	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
