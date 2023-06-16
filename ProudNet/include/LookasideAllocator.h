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
#include "Ptr.h"
#include "FastHeapSettings.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif
	/** \addtogroup util_group
	*  @{
	*/
	class CLookasideAllocatorInternal;

	/** 
	\~korean
	매우 빠른 메모리 할당,해제를 하지만 몇 가지 제약이 있다.
	- 항상 같은 크기의 메모리만 할당할 수 있다.
	- 이걸로 할당한 객체는 전역 변수이어서는 안된다. 즉 할당한 객체의 폐기 시간보다 이 allocator의 폐기 시간보다 앞이어야 한다.

	특징
	- 내부적으로 memory page 단위의 메모리 블럭을 미리 할당하고 사용한다. 따라서 너무 많은 갯수의 CLookasideAllocator 인스턴스를 만드는 것은 권장하지 않는다.
	- memory page는 VirtualAlloc으로 할당된다. 따라서 internal fragmentation을 만들지 않으므로 win32 working set의 크기도 절감하는
	효과를 준다. 

	\~english
	Performs very high speed memory allocation/disengagement but there are some restrictions.
	- Memory of same size can only be allocated at all time.
	- The object allocated by this cannot be global variable. In other words, the destruction time of this allocator must come prior to the desruction time of allocated object.

	Characteristics
	- Allocate memory block of memory page unit in advance and then use it, so it is recommended not to make too many CLookasideAllocator instances. 
	- Memory page is allocated to VirtualAlloc. Since it will not create internal fragmentation, it brings an effect of reducing the size of win32 working set.

	\~chinese
	非常快的内存分配，但有几种制约。
	- 只能分配同样大小的内存。
	- 用此分配的客体不能是全域变数，即分配的客体废弃时间要在llocator前面。

	特征
	- 在内部会预先分配memory page单位的内存Block后使用，因此不建议创建过多的 CLookasideAllocator%实例。
	- memory page 分配成VirtualAlloc。因为不创建internal fragmentation，所以也会带来减小win32 working set的大小的效果。

	\~japanese
	とても速いメモリー割当、解除をしますが、いくつかの制約があります。
	- いつも同じサイズのメモリーのみを割り当てることができます。
	- これで割り当てたオブジェクトは全域変数ではなければなりません。即ち、割り当てたオブジェクトの廃棄時間よりこのAllocatorの廃棄時間より先ではなければなりません。

	特徴
	- 内部的にmemory page単位のメモリーブロックを前もって割り当てて使用します。よって、とても多い個数のCLookasideAllocatorインスタンスを作ることはお勧めしません。
	- memory pageはVirtualAllocに割り当てます。よって、internal fragmentationを作らないので、win32 working setのサイズも節減する効果を与えます。 
	\~
	*/
	class CLookasideAllocator
	{
	protected:
		CLookasideAllocator(); // 생성자를 막아버림. 파생 클래스에서나 허용함.
	public:
		virtual ~CLookasideAllocator();

		/** 
		\~korean
		메모리를 할당합니다. 

		\~english
		Allocate memory.

		\~chinese
		分配内存。

		\~japanese
		メモリーを割り当てます。
		\~
		*/
		virtual void* Alloc(size_t size) = 0;
		/** 
		\~korean
		할당했던 메모리를 해제합니다.

		\~english
		Disengage the allocated memory.

		\~chinese
		解除分配的内存。

		\~japanese
		割り当てたメモリーを解除します。
		\~
		*/
		virtual void Free(void* ptr) = 0;
		/** 
		\~korean
		기본적으로 thread safe입니다. 하지만 thread unsafe를 해서 실행 가속화를 하려면 thread unsafe하게 옵션을 바꿀 수 있습니다.
		- 주의해서 사용하셔야 합니다. 

		\~english
		This is thread safe by default. the option can change to thread unsafe in order to accelerate.
		- Must be careful when use this.

		\~chinese
		一般是thread safe，但想用thread unsafe进行实行加速则可以将选项变换成thread unsafe。
		- 要注意使用。

		\~japanese
		基本的にthread safeです。しかし、thread unsafeによって実行加速化をするためには、thread unsafeにオプションを変えることができます。
		- 注意して使ってください。
		
		\~
		*/
		virtual int DebugCheckConsistency() = 0;

		/** 
		\~korean
		새 Lookaside allocator를 생성합니다.
		\param settings 초기 속성을 설정할 수 있습니다. 생략 가능한 파라메터입니다.  

		\~english
		Creates a new Lookaside allocator
		\param settings Can set the initial attributes. This is a neglectable parameter.

		\~chinese
		生成新的Lookaside allocator。
		\param settings 可以设置初期属性。是可以省略的参数。

		\~japanese
		新しいLookaside allocatorを生成します。
		\param settings 初期属性を設定することができます。省略可能なパラメーターです。  
		\~
		*/
		 static CLookasideAllocator* New(const CFastHeapSettings& settings = CFastHeapSettings());
	};
	typedef RefCount<CLookasideAllocator> CLookasideAllocatorPtr;

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
