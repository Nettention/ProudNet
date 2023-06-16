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

#include <new>
#include "BasicTypes.h"
#include "FastHeapSettings.h"
#include "Exception.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/

	/**
	\~korean
	빠른 속도로 heap alloc을 하지만 제한 사항이 있는 할당자 클래스.

	사용법
	- <a target="_blank" href="http://guide.nettention.com/cpp_ko#fast_heap" >Fast heap</a>  참조

	장점
	- 할당 속도가 빠르다

	단점
	- 이 할당자로 할당한 인스턴스는 main() 종료 전에 파괴되어야 한다. 즉 전역 변수로 둘 때 주의해야 한다.
	- 특정 이상 크기는 가속을 받지 못한다.
	- 메모리 풀 방식이므로 장시간 사용시 잉여 메모리가 남는다. 따라서 자주 할당/파괴를 하는 경우에만 쓰도록 하자.

	\~english
	Though this allocation class allocates heap alloc with high speed but there are some restrictions.

	Usage
	- Please refer to <a target="_blank" href="http://guide.nettention.com/cpp_en#fast_heap" >Fast heap</a>

	Strength
	- High allocation speed

	Weakness
	- The instance allocated by this allocator must be destructed before main() terminates. In other words, must be carefu when set this as global variable.
	- A certain size or bigger cannot be accelerated.
	- This leaves some unused memory when used for a long time since this is memory pool way. Therefore, this can be used only when there are needs to perform frequent allocation/destruction.

	\~chinese
	虽然能快速heap alloc，不过有着限制事象的分配者类。

	使用方法
	- 请参考<a target="_blank" href="http://guide.nettention.com/cpp_zh#fast_heap" >Fast heap</a>%。

	优点
	- 分配速度快。

	缺点
	- 用此分配者分配的实例要在main()终止之前破坏。即，用于全程变量的时候需注意。
	- 特定以上的大小不能被加速。
	- 因为内存是pool方式，长时间使用会产生残余的内存容量。所以请只在经常进行分配/破坏的状况使用。时候使用吧。

	\~japanese
	速い速度でheap allocをしますが、制限事項のある割当子クラスです。

	使用方法
	- \ref fast_heap  参照

	長所
	- 割当速度が速いです。

	短所
	- この割当子で割り当てたインスタンスはmain()終了の前に破壊しなければなりません。即ち、全域変数に置く時は注意しなければなりません。
	- 特定以上のサイズは加速を受けられません。
	- メモリープール方式なので、長時間使用時には剰余メモリーが残ります。よって、よく割当/破壊をする場合のみ使うようにしましょう。
	\~
	*/
	class CFastHeap
	{
	protected:
		CFastHeap(); // 직접 생성 금지
	public:
		enum { DefaultAccelBlockSizeLimit = 2048 };

		 virtual ~CFastHeap();

		/**
		\~korean
		메모리를 할당한다. 할당한 메모리는 Realloc이나 Free로 조정될 수 있다.
		\param size 할당받을 메모리 크기

		\~english 
		Allocate the memory. The allocated memory may be adjusted by Realloc or Free. 
		\param size Size of the memory to be allocated. 


		\~chinese
		分配内存。分配的内存可以用Realloc或者Free调整。
		\param size 可以分配的内存大小。

		\~japanese
		メモリーを割り当てます。割り当てたメモリーはReallocとかFreeに調整されることができます。
		\param size 割り当てられるメモリーサイズ
		\~
		*/
		virtual void* Alloc(size_t size) = 0;

		/**
		\~korean
		메모리를 재할당한다. 재할당한 메모리는 위치가 바뀔 수 있기 때문에 리턴되는 포인터 값을 반드시 참조할 것.
		\param ptr 재할당할 메모리 포인터
		\param size 재할당할 메모리 사이즈

		\~english 
		Reallocate the memory. As reallocated memory may have its position switched, you must refer the returned pointer value. 
		\param ptr Memory pointer to be reallocated.
		\param size Memory size to be reallocated. 


		\~chinese
		重新分配内存。重新分配的内存的位置可能会改变，必须参考返回的指针值。
		\param ptr 要重新分配的内存指针。
		\param size 要重新分配的内存大小。

		\~japanese
		メモリーを再割り当てします。再割り当てしたメモリーは位置が変わることがありますので、リターンされないポインター値を必ず参照してください。
		\param ptr 再割り当てするメモリーポインター 
		\param size 再割り当てするメモリーサイズ
		\~
		*/
		virtual void* Realloc(void* ptr, size_t size) = 0;

		/**
		\~korean
		할당했던 메모리를 해제한다.
		\param ptr 할당 받았던 메모리의 포인터

		\~english 
		Clear the memory that was allocated. 
		\param ptr Pointer of the memory that was allocated. 

		\~chinese
		解除分配过的内存。
		\param ptr 被分配的内存指针。

		\~japanese
		割り当てたメモリーを解除します。
		\param ptr 割り当てたメモリーのポインター
		\~
		*/
		virtual void Free(void* ptr) = 0;

		/**
		\~korean
		CFastHeap 객체를 생성한다.
		\param AccelBlockSizeLimit 이 값보다 큰 크기의 메모리 블럭을 할당할 때마다 lookaside allocator 방식을
		쓰지 않는 통상적인 메모리 블럭을 할당한다. 이 값이 클 수록 더 많은 메모리를 소모하지만 빠른 속도이 heap
		할당 확률이 커진다. (그렇다고 지나치게 크면 page fault 비용으로 인한 성능 저하가 되레 발생한다.)
		\param pHeap 만약 이 Fast Heap이 ProudNet에서 미리 생성한 전용 Heap이 아닌 다른 Heap을 사용하게 만들고자 한다면 여기에 CMemoryHeap 객체의 포인터를
		넣어야 한다. 단, 이 클래스가 CMemoryHeap 을 자동 제거하지는 않는다.
		NULL을 넣으면 ProudNet에서 미리 생성한 전용 Heap을 사용한다.
		\param safeMode CLookasideAllocator.New() 참고
		\param debugSafetyCheckCritSec 디버그용이다. safeMode가 false인 경우에만 유효하다.
		이 fast heap을 접근할 때마다 debugSafetyCheckCritSec 가 가리키는 critical section이 현재 스레드에 의해 잠금 상태인지 체크하고,
		잠금 상태가 아니면 에러를 발생시키는 알람을 한다.

		\~english
		Creates CFastHeap object
		\param AccelBlockSizeLimit Whenever allocating memory block bigger than this value, this allocates a usual memory block that does not follow lookaside allocator.
		Bigger this value becomes, occupies more memory but also increased chance of high speed heap allocation probability. (But if it is too bug then it lowers performance due to page fault cost.)
		\param pHeap In order to let this Fast Heap use other Heap that is not previously created by ProudNet, CMemoryHeap object pointer must be entered in here.
		But, this class does not automatically remove CMemoryHeap.
		If NULL is entered in here then uses previously created Heap by ProudNet.
		\param safeMode Please refer CLookasideAllocator.New()
		\param debugSafetyCheckCritSec for debugging use. Only valid when safeMode=false
		Whenever access to this fast heap, must check if critical section pointed by debugSafetyCheckCritSec is in its locked status by current thread, if not locked then this alarms that error occurs.

		\~chinese
		生成 CFastHeap%对象。
		\param AccelBlockSizeLimit 每当分配比此值大的内存BLOCK的时候，分配不用lookaside allocator方式的一般内存block。这个值越大便会消耗更多的内存，但是快速分配heap概率会增加。（尽管如此，过大的话也会发生因page fault费用的性能低下。）
		\param pHeap 如果这个Fast Heap想创建不是在ProudNet事先生成的专用Heap的其他heap或是想要使用其他heap的话，这里要添加 CMemoryHeap%对象的指针。但是，此类不会自动删除 CMemoryHeap%。
		输入NULL会使用在ProudNet之前生成的专用Heap。
		\param safeMode 请参考 CLookasideAllocator.New()。
		\param debugSafetyCheckCritSec 是调试用。只在safeMode是false时有效。
		每当访问这个fast heap的时候，要检查debugSafetyCheckCritSe所指向的critical section是否是被现在线程锁定的状态，如果不是便设定警报错误。

		\~japanese
		CFastHeapオブジェクトを生成します。
		\param AccelBlockSizeLimit この値より大きいサイズのメモリーブロックを割り当てる時ごとにlookaside allocator方式を使わない通常のメモリーブロックを割り当てます。この値が大きいほどもっと多いメモリーを消耗しますが、速い速度がheap割当確率が大きくなります。(しかし、あまりにも大き過ぎたらpage faultコストによる性能低下のため逆に性能低下が発生します。) 
		\param pHeap もし、このFast HeapがProudNetで前もって生成した専用のHeapではない他のHeapを使用するようにしたい場合は、ここにCMemoryHeapオブジェクトのポインターを入れなければなりません。但し、このクラスがCMemoryHeapを自動除去することはありません。
		NULLを入れるとProudNetで前もって生成した専用のHeapを使用します。
		\param safeMode CLookasideAllocator.New() 参照	
		\param debugSafetyCheckCritSec デバッグ用です。safeModeがfalseである場合のみ有効です。 
		このfast heapをアクセスする時ごとにdebugSafetyCheckCritSecが指すcritical sectionが現在のスレッドによってロック状態なのかをチェックして、ロック状態ではなければエラーを発生させるアラームをします。 
		
		\~
		*/
		 PROUD_API static CFastHeap* New( size_t AccelBlockSizeLimit = DefaultAccelBlockSizeLimit , const CFastHeapSettings& settings = CFastHeapSettings());

		/**
		\~korean
		block이 fast heap에 의해 만들어진 것이 아니면 에러창을 내거나 크래시가 발생한다.

		\~english
		If block is not created by fast heap then it will either produce an error window or cause a crash.

		\~chinese
		如果block不是被fast heap创建的话，会出现错误窗或者发生崩溃。

		\~japanese
		ブロックがfast heapによって作られたのではなければエラー画面を表示するかクラッシュが発生します。
		\~
		*/
		 static void AssureValidBlock(void* block);

		virtual int DebugCheckConsistency() = 0;
	};

	template<typename T>
	inline T* FastHeap_NewInstance(CFastHeap* heap)
	{
		T* ret = (T*)heap->Alloc(sizeof(T));

		if (ret == NULL)
			ThrowBadAllocException();

		CallConstructor<T>(ret);

		return ret;
	}

	template<typename T>
	inline void FastHeap_DeleteInstance(CFastHeap* heap, T* obj)
	{
		CallDestructor<T>(obj);
		heap->Free(obj);
	}

	/**  @} */

}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
