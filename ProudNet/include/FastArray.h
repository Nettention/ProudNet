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
#include "UnsafeMem.h"
#include "Exception.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/


	extern const char* CannotUseFastHeapForFilledCollectionErrorText;


	/**
	\~korean
	이 객체의 배열 크기가 증가할 때 가중치 타입

	\~english
	The weight type when the size of the array of this object.

	\~chinese
	此对象的数组大小增加的时候加重值类型。

	\~japanese
	このオブジェクトの配列サイズが増加する時の加重値タイプ
	\~
	 */
	enum GrowPolicy
	{
		/**
		\~korean
		균형

		\~english
		Balance

		\~chinese
		均衡

		\~japanese
		バランス
		\~
		*/
		GrowPolicy_Normal,
		/**
		\~korean
		고성능. 대신 메모리 사용량이 많음

		\~english
		High performance but use more memory

		\~chinese
		高性能。不过内存使用量多。

		\~japanese
		高性能。代わりにメモリー使用量が多いです。
		\~
		*/
		GrowPolicy_HighSpeed,
		/**
		\~korean
		메모리 절감. 대신 배열 크기가 증가시 복사 부하로 인한 저성능.

		\~english
		Save memory but when array size is increased, it shows low performance because of overload of copying

		\~chinese
		节省内存。但是会产生数组大小增加时复制负荷引起的低性能。

		\~japanese
		メモリー節減。代わりに配列サイズが増加時にコピー負荷によって性能が落ちます。
		\~
		*/
		GrowPolicy_LowMemory,
	};

	// byval인지 byref인지에 따라 선택되어 사용되는, 특성화 템플릿 클래스

	template<typename T, bool T_IN_REF>
	struct FastArray_Traits;

	template<typename T>
	struct FastArray_Traits< T, true >
	{
		typedef const T& type;
	};

	template<typename T>
	struct FastArray_Traits< T, false >
	{
		typedef T type;
	};

	/**
	\~korean
	배열 클래스

	Primitive type을 위한 성능 향상 기능
	- RAWTYPE=true로 세팅하면, 배열 요소는 int 등의 primitive 타입으로 간주되어 배열 다루는 성능이 향상됩니다. (성능 극대화를 위해 내부적으로 memcpy 등이 사용됩니다.)
	- 단, int, bool, byte 등 primitive type만 사용 가능합니다. 생성자, 파괴자, 복사 대입 연산자를 따로 가지고 있는 타입이 배열 항목으로 쓰이는 경우 위험합니다.

	주요 특징
	- heap 접근을 최소화하기 위해 auto memory shrink를 하지 않습니다.
	즉 capacity는 절대 줄지 않습니다. 따라서 메모리 과다 사용의 문제점을 고려해야 한다면 이 객체의 사용을 피하는 것이 좋습니다.
	- 내부적으로 <a target="_blank" href="http://guide.nettention.com/cpp_ko#fast_heap" >Fast heap</a> 을 써서 heap 접근으로 인한 부하를 절약할 수 있습니다.
	기본적으로 이 기능은 꺼져있습니다. 그러나, UseFastHeap으로 변경 가능합니다.
	- .Net framework의 List<>, STL.vector, CAtlArray의 메서드 이름을 모두 쓸 수 있습니다. 그리고 STL.vector.iterator와 같은 역할의
	iterator class도 제공하고 있습니다.

	\param T 배열의 항목 타입입니다.
	\param T_IN_REF true이면, by reference로 입력값이 전달됩니다. false이면 by value로 전달됩니다. int 처럼 크기가 8 byte 이하인 것은 false를 쓰는 것이 권장됩니다.
	\param RAWTYPE 배열 항목이 raw memory copy를 해도 안전한 타입인지에 대한 여부입니다. int는 안전하지만 std.string은 안전하지 않습니다.
	배열의 항목 타입이 생성자, 파괴자, 복사 대입 연산자가 사용되지 않아도 되는 타입인 경우 true로 지정할 수 있습니다.
	이를 true로 지정하면 배열에 삽입, 삭제, 크기 변경 등을 할 때 내부적으로 발생하는 배열 항목의 생성, 파괴, 복사 과정을 위해
	생성자, 파괴자, 복사 대입 연산자를 호출하지 않습니다. 따라서 처리 속도가 향상됩니다. 기본값은 false 입니다.
	\param INDEXTYPE 배열 최대 크기 및 index의 type. int32,int64,intPtr 중 하나를 쓰는 것을 권장합니다. 32-64bit int간 casting 부하도 무시 못하므로 적절한 것을 쓸 것을 권합니다.
	예를 들어 패킷 크기의 경우 웬만하면 2GB를 넘는 것을 안 다루므로 int32를 권장합니다. 로컬 프로세스에서만 다루는 것이면 intPtr을 권합니다. 네트워크 통계 등 32bit로는 불충분한 값을 다루면
	int64를 권합니다.

	\~english
	Array class

	Performance increase function for primitive type
	- The performance that manages array increases when RAWTYPE=true due to array factors are regarded as primitive type such as int. (In order to maximize the performance, sometimes memcpy can be used internally.)
	- However, only specific primitive types such as int, bool and byte can be used. It can be dangerous that the type with separate constructor, destructor and duplicator is used as array clause.

	Major characteristics
	- Does not perform auto memory shrink in order to minimize heap access
	  In other words, capacity never decreases. Therefore, if the circumstances must consider the side-effects of using too much memory then this object must not be used.
	- Internally, by using <a target="_blank" href="http://guide.nettention.com/cpp_en#fast_heap" >Fast heap</a>, it is possible to decrease the load cuased by heap access.
	  By default, this feature is off. But it can be changed by UseFastHeap.
	- Can use the metohd names of List<> of .Net framework, STL.vector and CAtlArray. And provides interator class that acts as STL.vector.iterator

	\param T Clause type of array
	\param T_IN_REF Set to true if you want the value to be passed by reference, or false for by value. Type of 8 or less of bytes such as int is passed faster if by value is selected.
	\param RAWTYPE To check if the type is safe even if array element is processed as “raw memory copy”. int is safe but std.string is not safe.
	In case element type of array is not related to constructor, destructor and copy assignment operator, you can set it as “true”.
	If setting it as “true”, constructor, destructor and copy assignment will not be called for progression of construction, destruction and copy of array element that internally occurs when inserting & deleting & changing size.
	Therefore, processing speed will be improved.
	Default is “false”.
	\param INDEXTYPE It is strongly recommended to use the maximum size of array and one of index types like int32,int64 and intPtr.
	Casting load between int32 and int64 should be considered, so using the appropriate one is recommended.
	For example, packet size should be int32 because it does not exceed 2GB and if it is only for local process, intPtr is recommended.
	int64 is appropriate when int32 cannot deal with accurate value like network statistics.

	\~chinese
	数组类

	为了Primitive type的性能提高功能。
	- 设置成RAWTYPE=true 的话数组因素会看作int等primitive类型，导致使用数组的性能提高（为了性能的极大化，内部使用memcpy等。）
	- -但只能使用int, bool, byte等Primitive type。如果另拥有Constructor、Destructor、复制赋值运算符的类型被用为排列项目则可能会存在危险。

	主要特征
	- 为了最小化heap访问，不做auto memory shrink。
	即capacity绝对不会减少。因此要考虑内存使用过多问题的话，最好避免此对象的使用。
	- 内部可以使用<a target="_blank" href="http://guide.nettention.com/cpp_zh#fast_heap" >Fast heap</a>%而节约heap访问照成的负荷。
	此功能基本为关闭状态。但是可以用UseFastHeap进行修改。
	- 可以使用.Net framework 的List<>, STL.vector, CAtlArray%所有的方法名称。还提供像STL.vector.iterator%的iterator class。

	\param T 数组的项目类型。
	\param T_IN Type name for input parameter.
	Use T/const T& for passing values by value/reference.
	byval is preferred if data type is same or little than 8 bytes.
	\param RAWTYPE 数组项目进行raw memory copy是否是安全的类型。Int 虽然安全，但 std.string%不安全。
	如果排列的项目类型可以不使用Constructor、Destructor、复制赋值运算符（Copy Assignment Operator）那么可以指定为true。
	如果将其指定为true，在对排列进行插入、删除、变更大小等操作时，为了进行在内部发生的排列项目的生成、破坏、复制过程，不呼叫Constructor、Destructor、复制赋值运算符。
	这会提高处理速度。其初始值为false。
	\param INDEXTYPE 建议在数组最大大小及index的type. int32,int64,intPtr 之中使用一个。32-64bit int 之间的casting负荷也不能忽视，所以建议使用恰当的方法。
	例如数据包大小基本都不使用超过2GB的，所以建议使用int32。使用本地程序的话建议intPtr。如互联网统计等即使利用32bit也不充分的话，建议用int64。

	\~japanese
	配列クラス
	Pritimitive typeのための性能向上機能
	- RAWTYPE=trueで設定すると、配列要素はintなどのprimitiveタイプで見なされて配列性能が向上されます。(性能極大化のために内部的にmemcpyなどが使われます)
	- 但し、int, bool, byteなどprimitive typeのみ使用可能です。生成子、破壊子、コピー代入演算子を別に持っているタイプが配列項目で使われる場合危険です。

	主要特徴
	- heap アクセスを最小化するために auto memory shrinkをしません。
	即ち、capacityは絶対減らしません。よって、メモリー過多使用の問題点を考慮すれば、このオブジェクトの使用は避けた方が良いです。
	- 内部的に\ref fast_heapを使ってheapアクセスによる負荷を節約することができます。 
	基本的にこの機能はオフです。しかし、UseFastHeapに変更可能です。
	- .Net frameworkのList<>、STL.vector、CAtlArrayのメソッド名を全て使うことができます。そして、STL.vector.iteratorのような役割のiterator classも提供しています。
	\param T 配列の項目タイプです。
	\param T_IN_REF trueであれば、by referenceに入力値が伝達されます。Falseであればby valueに伝達されます。Intのようにサイズが8 byte以下のものはfalseを使うことをお勧めします。
	\param RAWTYPE 配列項目が raw memory copyをしても安全なタイプなのかどうかです。Intは安全ですが、std.stringは安全ではありません。
	配列の項目タイプが生成子、破壊子、コピー代入演算子が使われなくても良いタイプである場合はtrueで指定することができます。
	これをtureで指定すると配列に挿入、削除、サイズ変更などをする時、内部的に発生する配列項目の生成、破壊、コピー過程のために、生成子、破壊子、コピー代入演算子を呼び出しません。よって、処理速度が向上されます。基本値はfalseです。
	\param INDEXTYPE 配列最大サイズおよびindexのtype。int32,int64,intPtrの中で一つを使うことをお勧めします。32-64bit int間のcasting負荷も無視できませんので、適切なものを使うことをお勧めします。
	例えば、パケットサイズの場合、できれば2GBを超えるものは扱わないのでint32をお勧めします。ローカルプロセスでのみ扱うならintPtrをお勧めします。ネットワーク統計など32bitでは不十分な値を扱えばint64をお勧めします。

	\~
	 */
	template<typename T, bool T_IN_REF = true, bool RAWTYPE = false, typename INDEXTYPE = intptr_t>
	class CFastArray
	{
		typedef typename FastArray_Traits<T, T_IN_REF>::type T_IN;

	public:
		// ikpil.choi 2016-11-29 : T의 타입 사이즈 추가, 		
		static const size_t TYPE_SIZE = sizeof(T);

	protected:
		/* 이 값들은 상속된 클래스에서 직접 조작할 수 있다.
		Q: 패치워크 아닌가요?
		A: 있을 것은 있어야 하고 없을 것은 없어야 한다는 구현 정책에 위배되지만,
		min capacity나 capacity 등이 여기서 자주 액세스되기 때문에 이들 상태 변수가 따로 있는 것이 차라리 직관적입니다.
		*/

		// 배열 데이터
		T* m_Data;
		// 배열의 현재 크기
		INDEXTYPE m_Length;
		// 배열 데이터 블럭의 실제 할당된 크기
		INDEXTYPE m_Capacity;
		// 배열 데이터 블럭의 최소 크기. 단 1개의 배열 항목을 잡아도 이것 이상은 잡히게 되어 있다.
		INDEXTYPE m_minCapacity;

		// if true, capacity never shrinks.
		// for example, this is flagged if this object is being object-pooled.
		bool m_suspendShrink;

		// 메모리 우선? CPU 우선?
		GrowPolicy m_growPolicy;

		inline void InitVars()
		{
			m_Data = NULL;
			m_Length = 0;
			m_Capacity = 0;
			m_minCapacity = 0;
		}

		// gets recommended capacity for required length actualCount.
		virtual INDEXTYPE GetRecommendedCapacity(INDEXTYPE actualCount)
		{
			INDEXTYPE nGrowBy = INDEXTYPE(); // prevent compile LV4 warning
			switch (m_growPolicy)
			{
			case GrowPolicy_LowMemory:
				// 사용자가 늘리려한 크기 그대로만 늘린다.
				return PNMAX(m_minCapacity, actualCount);
			default:
				nGrowBy = m_Length / 8;
				nGrowBy = PNMIN(nGrowBy, 1024); // 최대 증가분
				nGrowBy = PNMAX(nGrowBy, 4); // 최소 증가분
				break;
			case GrowPolicy_HighSpeed:
				// 원래 길이의 약 12% 정도를 추가로 늘린다.
				nGrowBy = m_Length / 8;
				nGrowBy = PNMAX(nGrowBy, 16); // 최소 증가분
				nGrowBy = PNMAX(nGrowBy, (INDEXTYPE)(64 / sizeof(T))); // for ByteArray and CMessage.
				nGrowBy = PNMIN(nGrowBy, 1024); // 최대 증가분
				break;
			}

			// change capacity only if there is much gap between current capacity and new capacity.
			INDEXTYPE v1 = m_Capacity;
			INDEXTYPE v2 = actualCount + nGrowBy;
			INDEXTYPE diff;
			if (v1 > v2) // INDEXTYPE might be unsigned.
				diff = v1 - v2;
			else
				diff = v2 - v1;

			// never return 0 which clears all capacity.
			if(!m_suspendShrink)
			{
				// shrink를 허용하는 상황.
				if (diff > nGrowBy)
				{
					return PNMAX(m_minCapacity, v2);
				}
				else
					return PNMAX(m_minCapacity, v1);
			}
			else
			{
				// shrink를 절대 하지 않는 상황.
				if (diff > nGrowBy)
				{
					return PNMAX(m_Capacity, v2);
				}
				else
					return PNMAX(m_Capacity, v1);
			}
		}

		inline void BoundCheck(INDEXTYPE index) const
		{
			if (index < 0 || index >= m_Length)
				ThrowArrayOutOfBoundException();
		}
	public:

		/**
		\~korean
		기본 생성자

		\~english
		Default constructor

		\~chinese
		默认生成者

		\~japanese
		基本生成子
		\~
		*/
		inline CFastArray()
		{
			m_growPolicy = GrowPolicy_Normal;
			m_suspendShrink = false;

			InitVars();
		}

		/**
		\~korean
		외부 데이터를 복사해오는 생성자
		\param data 데이터 배열의 포인터 입니다.
		\param count 배열의 Data수 입니다.

		\~english 
		Constructor that copies external data
		\param src This is the pointer of the data array
		\param count This is the number of data in the array		



		\~chinese
		复制外部数据的生成者。
		\param data 数据数组的指针。
		\param count 是数组的Data数。

		\~japanese
		外部データをコピーしてくる生成子
		\param data データ配列のポインターです。
		\param count 配列のデータ数です。

		\~
		*/
		inline CFastArray(const T* data, INDEXTYPE count)
		{
			m_growPolicy = GrowPolicy_Normal;
			m_suspendShrink = false;

			InitVars();
			InsertRange(0, data, count);
		}

		/**
		\~korean
		외부 데이터를 복사해오는 생성자
		\param src 복사할 CFastArray

		\~english 
		Constructor that copies external data
		\param src CFastArray to be copied
		

		\~chinese
		复制外部数据的生成者。
		\param src 要复制的 CFastArray%。

		\~japanese
		外部データをコピーしてくる生成子
		\param src コピーする CFastArray 

		\~
		*/
		inline CFastArray(const CFastArray &src)
		{
			m_suspendShrink = src.m_suspendShrink;
			m_growPolicy = src.m_growPolicy;
			InitVars();
			src.CopyTo(*this);
		}

		virtual ~CFastArray()
		{
			// SetCount(0); // 이렇게 하면 안됨! SetCount->GetRecommendedCapacity->invalid enum value참조->kaboom!
			if(!RAWTYPE)
			{
				if(m_Length > 0)
					CallDestructors(m_Data, m_Length);
			}

			if (m_Data != NULL)
			{
				DataBlock_Free(m_Data);
			}
		}

		/**
		\~korean
		이 객체의 배열 크기가 증가할 때 가중치 타입을 설정. 자세한 내용은 GrowPolicy을 참조
		\param val 배열의 증가 가중치에 대한 type

		\~english 
		Set the weighted value type when the array size of this object increases. For details, refer to GrowPolicy.
		\param val The type to the weighted value of the array


		\~chinese
		当此对象的数组大小增加的时候设置加重值类型。详细内容请参考GrowPolicy。
		\param val 对数组增加加重值的type。

		\~japanese
		このオブジェクトの配列サイズが増加する時、加重値タイプを設定。詳しくは、GrowPolicyを参照
		\param val 配列の増加加重値に対するtype

		\~
		 */
		inline void SetGrowPolicy(GrowPolicy val)
		{
			m_growPolicy = val;
		}

		/**
		\~korean
		기존 grow policy(이 객체의 배열 크기가 증가할 때 가중치 타입) 얻기

		\~english
		Getting existing grow policy

		\~chinese
		获取之前grow policy（此对象的数组大小增加时的加重值类型）。

		\~japanese
		既存のgrow policy(このオブジェクトの配列サイズが増加する時の加重値タイプ)を得ます。
		\~
		 */
		inline GrowPolicy GetGrowPolicy()
		{
			return m_growPolicy;
		}

		/**
		\~korean
		최소 버퍼 크기를 설정한다. 버퍼(capacity)크기가 증가할 때 최소한 이 사이즈보다 크게 설정한다.
		\param newCapacity 최소 Capacity size

		\~english 
		Set the minimum buffer size. When the buffer (capacity) size increases, set it at least bigger than this size.
		\param newCapacity Minimum capacity size



		\~chinese
		设置最小buffer大小。Buffer（capacity）大小增加时，尽量设置成比它大的数值。
		\param newCapacity 最小Capacity size。

		\~japanese
		最小バッファーサイズを設定します。バッファー(capacity)サイズが増加する時、最小限このサイズより大きく設定します。
		\param newCapacity 最小 Capacity size 

		\~
		*/
		inline void SetMinCapacity(INDEXTYPE newCapacity)
		{
			m_minCapacity = PNMAX(newCapacity, m_minCapacity);
			//SetCapacity(m_minCapacity); 이러면 세팅만 했는데도 엄한 할당을! 즐!
		}

		/**
		\~korean
		배열이 갖고 있는 버퍼(캐퍼서티)를 조절한다.
		- 캐퍼서티는 증가만 할 뿐, 줄어들지는 않는다.

		\~english
		Balances the buffer(capacity) possessed by array
		- Capacity is only increased, not decreased.

		\~chinese
		调整数组拥有的buffer（capacity）。
		- capacity只会增加，不会减少。

		\~japanese
		配列が持っているバッファー(キャパシティー)を調節します。
		- キャパシティーは増加するだけで減ることはありません。

		\~
		 */
		inline void SetCapacity(INDEXTYPE newCapacity)
		{
			assert(m_Capacity >= m_Length);

			newCapacity = PNMAX(m_minCapacity, PNMAX(newCapacity, m_Capacity) );

			// 늘릴 필요가 있을때만 늘린다.
			if(newCapacity > m_Capacity)
			{
				if (m_Capacity == 0) // 캐퍼가 애당초 없었다면
				{
					assert(!m_Data);

					// 0->N
					m_Data = (T*)DataBlock_Alloc(newCapacity * sizeof(T));

					// capacity만 세팅한 것이므로 ctor,dtor 호출 NO!
				}
				else
				{
					// 있던 캐퍼의 증가
					if(RAWTYPE)
					{
						T* oldData = m_Data;
						T* newData;

						newData = (T*)DataBlock_Realloc(oldData, newCapacity * sizeof(T));

						m_Data = newData;

						// realloc은 구 블럭으로의 복사도 겸하니까 여기서 또 복사 불필요
					}
					else	// raw type이 아닌 경우
					{
						T* oldData = m_Data;
						T* newData;

						// copy operator가 있는 객체 타입인 경우 realloc을 하면 구 데이터 블럭을 소실하게 된다.
						// 따라서 realloc이 아닌 alloc new => do assign oper => delete old를 해야 한다.
						newData = (T*)DataBlock_Alloc(newCapacity * sizeof(T));

						// 새로 할당된 버퍼에 구 버퍼에 있는 객체들의 복사 생성자를 수행한다.
						CallCopyConstructors<T>(newData, oldData, m_Length);

						// 구 데이터 블럭에 있든 객체들은 제거해야. 위에서 복사가 되었으므로. 이게 빠지면 누수로 이어짐!
						CallDestructors<T>(oldData,m_Length);
						DataBlock_Free(oldData);

						m_Data = newData;
					}
				}
				// 새로운 용량으로 셋팅한다.
				m_Capacity = newCapacity;
			}
		}

		// Capacity never shrinks after this is called.
		// called by object pool classes.
		inline void SuspendShrink()
		{
			m_suspendShrink = true;
		}

		// called by object pool classes.
		inline void OnRecycle()
		{
		}

		// called by object pool classes.
		inline void OnDrop()
		{
			// 배열 객체가 재사용 해제되는 경우, 갖고 있는 멤버들을 모두 청소하는 것이 좋다.
			// 행여나 shared_ptr을 가지고 있는 item type이면, 이것 때문에 누수로 이어질 수 있으니까.
			ClearAndKeepCapacity();
		}

		/**
		\~korean
		배열 크기를 조절한다
		- 배열 크기 조절시 capacity가 충분히 증가한다.
		\param newVal 새로운 배열의 크기

		\~english 
		Adjust the array size
		- When adjusting the array size, the capacity increases sufficiently. 
		\param newVal Size of the new array


		\~chinese
		调整数组大小。
		- 调整数组大小时capacity会充分增加。
		\param newVal 新的数组大小。

		\~japanese
		配列サイズを調節します。
		- 配列サイズを調節時にcapacityが十分増加します。
		\param newVal 新しい配列のサイズ

		\~
		 */
		inline void SetCount(INDEXTYPE newVal) // 성능이 중요하므로 루틴이 커도 inline
		{
			/*
			\~korean
			크기 조절시 발생하는 사항
			- 0->N: 버퍼 생성
			- N->0: capacity는 그대로 보유
			- M->N (M>N): capacity는 그대로 보유
			- M->N(M<N): granularity 단위로 크기 증가. 크기 증가시 realloc됨.

			\~english
			Details occur when modifying size
			- 0->N: creates buffer
			- N->0: capacity is still kept
			- M->N (M>N): capacity is still kept
			- M->N(M<N): size increases in granularity unit. to be 'relloc'ed when size increased.

			\~chinese
			调整大小时发生的情况。
			- 0->N: 生成buffer
			- N->0: capacity保留原样
			- M->N (M>N): capacity保留原样
			- M->N(M<N): 以granularity单位增加大小。大小增加时会realloc。

			\~japanese
			サイズ調節時に発生する事項
			- 0->N: バファー生成
			- N->0: capacityはそのまま保有
			- M->N (M>N): capacityはそのまま保有
			- M->N(M<N): granularity 単位でサイズ増加。サイズ増加時にreallocされます。

			\~
			 */

			if (newVal < 0)
			{
				ThrowInvalidArgumentException();
			}

			if (newVal > m_Length)
			{
				// 늘리기
				AddCount(newVal - m_Length);
			}
			else if (newVal < m_Length) // 줄이기
			{
				// 필요시 캐퍼를 줄이자. 메모리를 아낄건 아껴야지.
				// 사용자는, 성능이 필요하면 SuspendShrink를 써도 되고.
				INDEXTYPE newCapacity = GetRecommendedCapacity(newVal);
				SetCapacity(newCapacity);

				if (!RAWTYPE)
				{
					// 크기가 줄고 있으니 파괴자 호출
					CallDestructors<T>(m_Data + newVal, m_Length - newVal);
				}
				else
				{
					// 아무것도 안한다.
					// 비어지게 되는 공간을 zero memory하는 것도 성능에 악영향. 따라서 즐.
				}

				// 실 사이즈 조절
				m_Length = newVal;
			}
		}

		// SetCount보다 처리 속도가 더 빠르다.
		// CMessage.Write에서 자주 호출하므로 성능 최적화를 위함.
		inline void AddCount(INDEXTYPE addLength)
		{
			if (addLength < 0)
			{
				ThrowInvalidArgumentException(); // 크기를 줄이려면 이 함수 말고 다른 함수를 쓸 것.
			}
			else if (addLength > 0)
			{
				// 아래 두 구문은 제법 시간을 먹는다. 따라서 조건문으로 미리 건너뛴다.
				if (m_Length + addLength > m_Capacity) // capacity를 더 늘려야 한다면
				{
					// 버퍼 크기 재할당, 필요시 객체 복사 처리
					INDEXTYPE newCapacity = GetRecommendedCapacity(m_Length + addLength);
					SetCapacity(newCapacity);
				}

				if(!RAWTYPE)
				{
					// 늘어난 크기만큼 생성자 호출
					CallConstructors<T>(m_Data + m_Length, addLength);
				}

				// 실 크기 조절
				m_Length += addLength;
			}
		}

		/// for STL compatibility
		inline void resize(INDEXTYPE sz)
		{
			SetCount(sz);
		}

		/**
		\~korean
		capacity의 크기를 얻는다

		\~english 
		Get the size of the capacity

		\~chinese
		获取capacity的大小。

		\~japanese
		capacityのサイズを得ます。
		\~
		*/
		inline INDEXTYPE GetCapacity() const { return m_Capacity; }

		/**
		\~korean
		배열 크기

		\~english
		Array size

		\~chinese
		数组大小

		\~japanese
		配列サイズ
		\~
		*/
		inline INDEXTYPE GetCount() const
		{
			return m_Length;
		}

		/// for STL compatibility
		inline INDEXTYPE size() const
		{
			return GetCount();
		}

		inline bool IsEmpty() const
		{
			return m_Length == 0;
		}

		/**
		\~korean
		배열 청소. ClearAndKeepCapacity와 같음

		\~english
		Clean up array. Same as ClearAndKeepCapacity

		\~chinese
		清扫数组。与ClearAndKeepCapacity一样。

		\~japanese
		配列掃除。. ClearAndKeepCapacityと同じです。
		\~
		*/
		inline void Clear()
		{
			SetCount(0);    // 메모리 블럭 파괴는 절대 안함
		}

		/**
		\~korean
		배열 청소. Clear와 같음

		\~english
		Clean up array. Same as Clear

		\~chinese
		清扫数组。与clear一样。

		\~japanese
		配列掃除。Clearと同じです。
		\~
		*/
		inline void ClearAndKeepCapacity()
		{
#ifdef _DEBUG
			INDEXTYPE oldCount = m_Length;
#endif
			Clear();
#ifdef _DEBUG
			// 있던 배열을 비운거라면 버퍼가 남아있어야 한다.
			if (oldCount > 0)
			{
				assert(m_Data != NULL);
			}
#endif
		}

#if defined(_MSC_VER)
		/**
		\~korean
		배열 크기

		\~english
		Array size

		\~chinese
		数组大小。

		\~japanese
		配列サイズ
		\~
		*/
		__declspec(property(get = GetCount)) INDEXTYPE Count;
#endif

		/**
		\~korean
		index가 가리키는 배열 항목

		\~english
		Array item that pointed by index

		\~chinese
		Index所指的数组项目。

		\~japanese
		indexが指す配列項目
		\~
		*/
		inline T_IN ElementAt(INDEXTYPE index) const
		{
			BoundCheck(index);
			return m_Data[index];
		}

		/**
		\~korean
		index가 가리키는 배열 항목

		\~english
		Array item that pointed by index

		\~chinese
		Index所指的数组项目。

		\~japanese
		indexが指す配列項目
		\~
		*/
		inline T& ElementAt(INDEXTYPE index)
		{
			BoundCheck(index);
			return m_Data[index];
		}

		/**
		\~korean
		index가 가리키는 배열 항목

		\~english
		Array item that pointed by index

		\~chinese
		Index所指的数组项目。

		\~japanese
		indexが指す配列項目
		\~
		*/
		inline T_IN operator[] (INDEXTYPE index) const
		{
			return ElementAt(index);
		}

		/**
		\~korean
		index가 가리키는 배열 항목

		\~english
		Array item that pointed by index

		\~chinese
		Index所指的数组项目。

		\~japanese
		indexが指す配列項目
		\~
		*/
		inline T& operator[](INDEXTYPE index)
		{
			return ElementAt(index);
		}

		/**
		\~korean
		배열 버퍼 포인터를 얻는다.

		\~english
		Gets array buffer pointer

		\~chinese
		获得数组buffer指针。

		\~japanese
		配列バッファーポインターを得ます。
		\~
		*/
		inline T* GetData()
		{
			if (m_Length == 0)
				return NULL;
			return m_Data;
		}

		/**
		\~korean
		배열 버퍼 포인터를 얻는다.

		\~english
		Gets array buffer pointer

		\~chinese
		获得数组buffer指针。

		\~japanese
		配列バッファーポインターを得ます。
		\~
		*/
		inline const T* GetData() const
		{
			if (m_Length == 0)
				return NULL;
			return m_Data;
		}

		/**
		\~korean
		맨뒤에 추가

		\~english
		Add at the end

		\~chinese
		添加到最后。

		\~japanese
		最後に追加
		\~
		*/
		inline void Add(T_IN value)
		{
			INDEXTYPE length = m_Length;

			// code profile 결과 매우 자주 호출되는 함수이다. 이렇게 숏컷 로직으로 처리하면 대부분의 경우 빠르다.
			if (length < m_Capacity)
			{
				if (!RAWTYPE)
				{
					CallConstructor(&m_Data[length], value);
				}
				else
				{
					m_Data[length] = value;
				}
				length++;
				m_Length = length;
			}
			else
			{
				// 맨 끝에 추가한다.
				Insert(m_Length, value);
			}
		}

		inline void push_back(T_IN value)
		{
			Add(value);
		}

		/**
		\~korean
		indexAt이 가리키는 항목을 모두 한칸씩 뒤로 밀고 value 를 indexAt 이 가리키는 곳에 추가한다.

		\~english
		Move back item that pointed by indexAt then add value to place that pointed by indexAt

		\~chinese
		把indexAt所指的项目往后推一格，再把value添加到indexAt所指的位置。

		\~japanese
		indexAtが指す項目を全て1桁ずつ後ろにずらして、valueをindexAtが指すところに追加します。
		\~
		*/
		inline void Insert(INDEXTYPE indexAt, T_IN value)
		{
			InsertRange(indexAt, &value, 1);
		}

		/**
		\~korean
		배열 뒤에 새로운 배열 추가
		\param data 새로 추가할 배열의 포인터
		\param count 추가할 배열의 크기

		\~english 
		Add a new array behind an array
		\param data Pointer of the array to be newly added. 
		\param count Size of the array to be added


		\~chinese
		数组后添加新的数组。
		\param data 要新添加的指针。
		\param count 要添加的数组大小。

		\~japanese
		配列の後ろに新しい配列を追加
		\param data 新たに追加する配列のポインター 
		\param count 追加する配列のサイズ

		\~
		*/
		inline void AddRange(const T* data, INDEXTYPE count)
		{
			//InsertRange(m_Length, data, count);이게 더 느림

			if (count < 0)
				ThrowInvalidArgumentException();

			if (count == 0)
				return;

			INDEXTYPE oldCount = m_Length;

			// 공간 확보
			AddCount(count);
			INDEXTYPE indexAt = oldCount;

			T* src = GetData();
			if (!RAWTYPE)
			{
				// 값 채우기

				for (INDEXTYPE i = 0; i < count; i++)
				{
					// 가속을 위해 GetData() 사용
					src[i + indexAt] = data[i];
				}
			}
			else
			{
				// ikpil.choi 2016-11-07 : memcpy_s 로 변경, destSize(2번째 인자) 값이 항상 올바른 값이여야 합니다.
				//UnsafeFastMemcpy(src + indexAt, data, sizeof(T)*count);
				memcpy_s(src + indexAt, sizeof(T) * (m_Length - indexAt), data, sizeof(T) * count);
			}
		}

		/**
		\~korean
		배열 중간에 배열 추가. indexAt이 가리키는 부분을 뒤로 밀어놓고 틈새에 추가한다.
		\param indexAt 이 index 번호의 다음부터 추가 됩니다.
		\param data data 삽입할 배열의 포인터 입니다.
		\param count 삽입할 배열의 크기

		\~english 
		Add an array in the middle of an array. Push the area pointed at by indexAt behind and add it in the gap. 
		\param indexAt It is added after this index number. 
		\param data data It is a pointer of the array to be inserted. 
		\param count Size of the array to be added. 




		\~chinese
		在数组中间添加数组。将indexAt所指的部分后推，之后添加到缝隙里。
		\param indexAt 从index号码之后开始添加。
		\param data data要插入的数组指针。
		\param count 插入的数组大小。

		\~japanese
		配列中間に配列追加：indexAtが指す部分を後ろへずらして置いて隙間に追加します。
		\param indexAt このindex番号の次から追加されます。
		\param data data 挿入する配列のポインターです。
		\param count 挿入する配列のサイズ

		\~
		*/
		inline void InsertRange(INDEXTYPE indexAt, const T* data, INDEXTYPE count)
		{
			if (count < 0 || indexAt < 0 || indexAt > m_Length)
				ThrowInvalidArgumentException();

			INDEXTYPE oldCount =  m_Length;

			// 공간 확보
			AddCount(count);

			// 미리 밀어넣고
			INDEXTYPE moveAmount = oldCount - indexAt;
			T* src = GetData(); // getdata안에 if문있음.

			if (moveAmount > 0)
			{
				if(!RAWTYPE)
				{
					for (INDEXTYPE i = moveAmount - 1;i >= 0;i--)
					{
						// 가속을 위해 GetData() 사용
						src[i+indexAt+count] = src[i+indexAt];
					}
				}
				else
				{
					UnsafeFastMemmove(src+indexAt+count,src+indexAt,sizeof(T)*moveAmount);
					assert(indexAt+count+moveAmount<=m_Length);
				}
			}

			if(!RAWTYPE)
			{
				// 값 채우기
				for (INDEXTYPE i = 0;i < count;i++)
				{
					// 가속을 위해 GetData() 사용
					src[i+indexAt] = data[i];
				}
			}
			else
			{
				// ikpil.choi 2016-11-07 : memcpy_s 로 변경, destSize(2번째 인자) 값이 항상 올바른 값이여야 합니다.
				//UnsafeFastMemcpy(src + indexAt, data, sizeof(T) * count);
				// m_Length = 7, indexAt = 0, count = 1 이면, true
				// m_Length = 7, indexAt = 7, count = 1 이면, false

				// m_Length = 7, indexAt = 0, count = 7 이면, true
				// m_Length = 7, indexAt = 6, count = 1 이면, true

				// m_Length = 7, indexAt = 7, count = 1 이면, false
				// m_Length = 0, indexAt = 0, count = 1 이면, false
				memcpy_s(src + indexAt, sizeof(T) * (m_Length - indexAt), data, sizeof(T) * count);
			}
		}

		/**
		\~korean
		dest의 크기를 count로 조정 후 src의 일부나 전체를 dest로 복사한다.
		\param dest 배열을 dest로 복사합니다.
		\param srcOffset 배열 내 srcOffset 이후 Data부터 dest로 복사 한다.
		\param count count갯수 만큼의 배열을 복사 한다.

		\~english 
		After adjusting the size of dest with count, part of all of src is copied to dest. 
		\param dest Array is copied to dest. 
		\param srcOffset Copy the data after srcOffset within the array to dest.  
		\param count count Copy arrays as the same number of count. 




		\~chinese
		把dest的大小调整为count后，将src的部分或全部复制到dest。
		\param dest 把数组复制为dest。
		\param srcOffset 从数组内srcOffset以后Data开始复制到dest。
		\param count  复制出相当于count个数的数组。

		\~japanese
		Destのサイズをcountで調整後、srcの一部または全体をdestへコピーします。 
		\param dest 配列をdestへコピーします。
		\param srcOffset 配列内のsrcOffset以降のデータからdestへコピーします。
		\param count count数だけの配列をコピーします。

		\~
		*/
		inline void CopyRangeTo(CFastArray &dest, INDEXTYPE srcOffset, INDEXTYPE count) const
		{
			if (srcOffset + count > m_Length || srcOffset < 0 || count < 0)
				ThrowInvalidArgumentException();

			dest.SetCount(count);

			if(!RAWTYPE)
			{
				T* pDest = dest.GetData();
				const T* pSrc = GetData();
				for (INDEXTYPE i = 0;i < count;i++)
				{
					// 가속을 위해 GetData() 사용
					pDest[i] = pSrc[i+srcOffset];
				}
			}
			else
			{
				// ikpil.choi 2016-11-07 : memcpy_s 로 변경, destSize(2번째 인자) 값이 항상 올바른 값이여야 합니다.
				//UnsafeFastMemcpy(dest.GetData(),GetData()+srcOffset,sizeof(T)*count);
				
				if (count != 0) // PS4 플랫폼에서 STDERR 출력 방지를 위해
					memcpy_s(dest.GetData(), sizeof(T) * dest.m_Length, GetData() + srcOffset, sizeof(T) * count);
			}
		}

		/**
		\~korean
		\param dest 배열을 dest로 복사한다.

		\~english
		\param dest copy array to dest.

		\~chinese
		\param dest 把数组复制成dest。

		\~japanese
		\param dest 配列をdestへコピーします。 
		\~
		*/
		inline void CopyTo(CFastArray &dest) const
		{
			CopyRangeTo(dest, 0, m_Length);
		}

		inline void CopyFrom(const CFastArray &src)
		{
			src.CopyTo(*this);
		}

		inline void CopyFrom(const T* from, INDEXTYPE fromCount)
		{
			if (fromCount < 0)
				ThrowInvalidArgumentException();
			SetCount(fromCount);
			for (INDEXTYPE i = 0;i < fromCount;i++)
			{
				(*this)[i] = from[i];
			}
		}

		/**
		\~korean
		index번째 항목부터 count만큼 제거한다.
		\param index 제거할 배열의 index
		\param count index로부터 count수 만큼 제거한다.

		\~english 
		Remove as many as the count from the (index)th item. 
		\param index Index of the array to be removed. 
		\param count Remove as many as the number of count from the index.


		\~chinese
		从第index个项目删除相当于count的量。
		\param index 要删除的数组index。
		\param count 从index开始删除相当于count的量。

		\~japanese
		index番目の項目からcountだけ除去します。 
		\param index 除去する配列の index
		\param count indexからcount数だけ除去します。

		\~
		*/
		inline void RemoveRange(INDEXTYPE index, INDEXTYPE count)
		{
			if (index < 0 || count < 0)
				ThrowInvalidArgumentException();
			count = PNMIN(count, m_Length - index);

			//앞으로 땡긴다
			INDEXTYPE amount = m_Length - (index + count);

			T* src = GetData();
			if (amount > 0)
			{
				if(!RAWTYPE)
				{
					for (INDEXTYPE i = 0;i < amount;i++)
					{
						// 가속을 위해 GetData() 사용
						src[i+index] = src[i+index+count];
					}
				}
				else
				{
					UnsafeFastMemmove(src+index,src+index+count,amount*sizeof(T));
					assert(index+count+amount<=m_Length);
				}
			}

			SetCount(m_Length - count);
		}

		/**
		\~korean
		\param index index에 해당하는 data를 제거한다.

		\~english
		\param index Remove index th item

		\~chinese
		\param index 删除等于index的data。

		\~japanese
		\param index indexに該当するデータを除去します。 
		\~
		*/
		inline void RemoveAt(INDEXTYPE index)
		{
			RemoveRange(index, 1);
		}

		/**
		\~korean
		\param value value와 같은 값을 가지는 배열 앞에서부터 찾아서 찾으면 제거한다.

		\~english
		\param value Seek same value as value from the first then remove it

		\~chinese
		\param value 从前开始寻找与value相同值的数组，找到便删除。

		\~japanese
		\param value valueのような値を持つ配列の前から探して、見つかったら除去します。
		\~
		*/
		inline bool RemoveOneByValue(T_IN value)
		{
			T* src = GetData();
			for (INDEXTYPE i = 0;i < m_Length;i++)
			{
				// 가속을 위해 GetData() 사용
				if (src[i] == value)
				{
					RemoveAt(i);
					return true;
				}
			}

			return false;
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
		inline CFastArray& operator=(const CFastArray &src)
		{
			src.CopyTo(*this);
			return *this;
		}

		/**
		\~korean
		\param value value와 같은 값을 가지는 배열 항목이 있는지 확인한다.
		\return value와 같은 값을 가지는 배열 항목이 있으면 그 인덱스를 리턴한다. 찾지 못하면 -1을 리턴한다.

		\~english
		\param value check if there is an array that has the same value as “value”.
		\return If there is the array that has the same value as “value”, the index is returned. If not found, -1 will be returned.

		\~chinese
		\param value 确认是否有与value一样值的数组项目。
		\return 如果有与value一样值的数组项目的话返回那个index。没有找到返回-1。

		\~japanese
		\param value valueのような値を持つ配列項目があるかを確認します。
		\return valueのような値を持つ配列項目があれば、そのインデックスをリターンします。見つからなかったら⁻1をリターンします。

		\~
		 */
		inline INDEXTYPE FindByValue(T_IN value)
		{
			T* src = GetData();

			for (INDEXTYPE i = 0;i <  m_Length;i++)
			{
				if (value == src[i])
					return i;
			}
			return -1;
		}

		/** Returns true if value exists in the array. */
		inline bool Contains(T_IN value)
		{
			return FindByValue(value) >= 0;
		}

		/**
		\~korean
		- 주의: 단순 메모리 비교다. 이 점을 주의할 것.
		\param rhs rhs와 내용이 동일한지 체크한다.

		\~english
		- Note: This is a simple memory comparison. This is crucial.
		\param rhs Checks if this contains same as rhs

		\~chinese
		- 注意：是单纯的内存比较。注意这一点。
		\param rhs 检查是否与rhs内容相同。

		\~japanese
		- ご注意：単純メモリー比較です。これにご注意すること。
		\param rhs rhsと内容が同じかどうかをチェックします。

		\~
		 */
		inline bool Equals(const CFastArray &rhs) const
		{
			if(rhs.m_Length!=m_Length)
				return false;

			// 단순 메모리 비교다.
			const T* p_rhs = rhs.GetData();
			const T* p_lhs = GetData();

			if(RAWTYPE)
			{
				return memcmp(p_rhs,p_lhs,rhs.m_Length * sizeof(T)) == 0;
			}
			else
			{

				for (INDEXTYPE i=0;i<m_Length;i++)
				{
					if(p_rhs[i] != p_lhs[i])
						return false;
				}
				return true;
			}

		}

		/// for STL compatibility
		inline void clear()
		{
			Clear();
		}

		class iterator;

		friend class iterator;

#ifndef PROUDNET_NO_CONST_ITERATOR
		class const_iterator;
		friend class const_iterator;

		/**
		\~korean
		STL의 const_iterator와 같은 역할을 한다.

		\~english
		Acts like const_iterator of STL

		\~chinese
		起着与STL的const_iterator一样的作用。

		\~japanese
		STLのconst_iteratorのような役割をします。
		\~
		 */
		class const_iterator
		{
			friend class CFastArray;
		public:
			const CFastArray* m_owner;
			INDEXTYPE m_pos;

			inline const_iterator() {}
			inline const_iterator(const iterator& src) { m_owner=src.m_owner; m_pos=src.m_pos; }

			inline const_iterator& operator++()
			{
				// preincrement
				if(m_pos<m_owner->m_Length)
					m_pos++;
				else
					ThrowArrayOutOfBoundException();

				return (*this);
			}

			inline const_iterator operator++(int)
			{
				// postincrement
				const_iterator _Tmp = *this;
				++*this;
				return (_Tmp);
			}

			inline bool operator==(const const_iterator& a) const
			{
				return m_pos==a.m_pos && m_owner==a.m_owner;
			}

			inline bool operator!=(const const_iterator& a) const
			{
				return !(m_pos==a.m_pos && m_owner==a.m_owner);
			}

			inline T_IN operator*() const
			{
				return m_owner->ElementAt(m_pos);
			}

		};
#endif
		/**
		\~korean
		STL의 iterator와 같은 역할을 한다.

		\~english
		Acts similar as iterator of STL

		\~chinese
		起着与STL的iterator一样的作用。

		\~japanese
		STLのiteratorのような役割をします。
		\~
		 */
		class iterator
		{
			friend class CFastArray;
		public:
			CFastArray* m_owner;
			INDEXTYPE m_pos;

			inline iterator() {}
			inline iterator(const const_iterator& src) { m_owner=src.m_owner; m_pos=src.m_pos; }

			inline iterator& operator++()
			{
				// preincrement
				if(m_pos<m_owner->m_Length)
					m_pos++;
				else
					ThrowArrayOutOfBoundException();

				return (*this);
			}

			inline iterator operator++(int)
			{
				// postincrement
				iterator _Tmp = *this;
				++*this;
				return (_Tmp);
			}

			inline T_IN operator*() const
			{
				return m_owner->ElementAt(m_pos);
			}

			inline T& operator*()  // 이 메서드가 여지껏 왜 없었지?
			{
				return m_owner->ElementAt(m_pos);
			}

			inline bool operator==(const iterator& a) const
			{
				return m_pos==a.m_pos && m_owner==a.m_owner;
			}

			inline bool operator!=(const iterator& a) const
			{
				return !(m_pos==a.m_pos && m_owner==a.m_owner);
			}
		};

		/**
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Acts similar as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		 */
		inline iterator begin()
		{
			iterator ret;
			ret.m_pos = 0;
			ret.m_owner=this;

			return ret;
		}

		/**
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Acts similar as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		 */
		inline iterator end()
		{
			iterator ret;
			ret.m_pos=m_Length;
			ret.m_owner=this;

			return ret;
		}

#ifndef PROUDNET_NO_CONST_ITERATOR
			/**
			\~korean
			STL의 동명 메서드와 같은 역할을 한다.

			\~english
			Acts similar as same name method of STL

			\~chinese
			起着与STL的同名方法一样的作用。

			\~japanese
			STLの同名メソッドのような役割をします。
			\~
			 */
		inline const_iterator begin() const
		{
			const_iterator ret;
			ret.m_pos = 0;
			ret.m_owner = this;

			return ret;
		}

		/**
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.

		\~english
		Acts similar as same name method of STL

		\~chinese
		起着与STL的同名方法一样的作用。

		\~japanese
		STLの同名メソッドのような役割をします。
		\~
		 */
		inline const_iterator end() const
		{
			const_iterator ret;
			ret.m_pos=m_Length;
			ret.m_owner=this;

			return ret;
		}
#endif
		/**
		\~korean
		STL의 동명 메서드와 같은 역할을 한다.
		\param iter iter가 가르키는 데이터를 제거한다.

		\~english 
		Acts similar as same name method of STL


		\~chinese
		起着与STL的同名方法一样的作用。
		\param iter 删除iter所指的数据。

		\~japanese
		STLの同名メソッドのような役割をします。
		\param iter iterが指すデータを除去します。

		\~
		 */
		iterator erase(iterator iter)
		{
			if(iter.m_owner!=this)
			{
				ThrowInvalidArgumentException();
			}
			iterator ret = iter;
			//ret++; 이건 없어야 한다. 배열 항목을 제거하면서 뒤에있는게 앞으로 주루룩 밀려오고 그러면서 iter는 자동으로 다음걸 가리키니까.
			RemoveAt(iter.m_pos);

			return ret;
		}

		/**
		\~korean
		배열의 마지막 정보를 배열내에서 제거하고 리턴해준다.

		\~english 
		Remove the last information of the array within the array and return it. 

		\~chinese
		从数组内删除数组的最后信息并返回。

		\~japanese
		配列の最後の情報を配列内から除去してリターンします。
		\~
		*/
		inline T PopBack()
		{
			INDEXTYPE cnt = m_Length;

			if(cnt == 0)
				ThrowInvalidArgumentException();

			T ret = GetData()[cnt - 1];
			SetCount(cnt - 1);

			return ret;
		}

		/**
		\~korean
		맨 마지막 항목을 index가 가리키는 곳으로 옮긴 후 맨 마지막 항목을 제거한다.
		순서가 상관없는 콜렉션에서 허리 항목을 제거할 때 Remove 대신 쓰면 효과적이다.
		\param index 제거할 index

		\~english 
		After moving the last item to the place where index is pointing at and remove the last item. 
		It is effective to use instead of Remove when you move the waist item from a collection where sequence is irrelevant. 
		\param index Index to be removed. 

		\~chinese
		把最后项目挪到index指向的地方以后删除最后项。
		在与顺序无关的collection中删除mid-part项目的时候代替Remove的话会有效果。
		\param index 要删除的index。

		\~japanese
		最後の項目をindexが指す所に移した後、最後の項目を除去します。
		手順に関係ないコレクションで腰項目を除去する時、Removeの替わりに使えば効果的です。
		\param index 除去する index

		\~
		 */
		inline void RemoveAndPullLast(INDEXTYPE index)
		{
			INDEXTYPE cnt = m_Length;
			if (index < 0 || index >= cnt)
				ThrowInvalidArgumentException();

			// 맨 끝 항목을 index가 가리키는 곳으로 이동
			if (index < cnt - 1)
			{
				GetData()[index] = GetData()[cnt - 1];
			}

			// 사이즈 1 감소
			SetCount(cnt - 1);
		}


		virtual void DataBlock_Free(void* data)
		{
			CProcHeap::Free(data);
		}

		virtual void* DataBlock_Alloc(size_t length)
		{
			void* ret = CProcHeap::Alloc(length);

			if (ret == NULL)
				ThrowBadAllocException();

			return ret;
		}

		virtual void* DataBlock_Realloc(void* oldPtr, size_t newLength)
		{
			void* newData = (T*)CProcHeap::Realloc(oldPtr, newLength);

			if (newData == NULL)
				ThrowBadAllocException();

			return newData;
		}
	};



	/**  @} */


}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
