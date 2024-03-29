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
#include "Exception.h"
#include "UnsafeMem.h"
#include "sysutil.h"
#include "FastArray.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/

	 PROUD_API extern const char* ExternalBufferArrayIsUninitializedError;
	 PROUD_API extern const char* ExternalBufferArrayIsInitializedError;

	/** 
	\~korean
	외부 버퍼를 사용하는 배열 클래스입니다.

	주의: 외부 버퍼안에 이미 할당된 객체들이 존재하지 않아야 합니다. 즉 단순히 초기화된 메모리 영역이어야 합니다. 왜냐하면
	CArrayWithExternalBuffer 가 초기화할 것이기 때문입니다. 

	\param TYPE 배열의 원소 타입입니다.
	\param RAWTYPE 배열 원소가 raw memory copy를 해도 안전한 타입인지에 대한 여부입니다. int는 안전하지만 std.string은 안전하지 않습니다.
	배열의 항목 타입이 생성자, 파괴자, 복사 대입 연산자가 사용되지 않아도 되는 타입인 경우 true로 지정할 수 있습니다.
	이를 true로 지정하면 배열에 삽입, 삭제, 크기 변경 등을 할 때 내부적으로 발생하는 배열 항목의 생성, 파괴, 복사 과정을 위해
	생성자, 파괴자, 복사 대입 연산자를 호출하지 않습니다. 따라서 처리 속도가 향상됩니다. 기본값은 false입니다.
	\param INDEXTYPE 배열 최대 크기 및 index의 type. int32,int64,intPtr 중 하나를 쓰는 것을 권장합니다. 32-64bit int간 casting 부하도 무시 못하므로 적절한 것을 쓸 것을 권합니다.
	예를 들어 패킷 크기의 경우 웬만하면 2GB를 넘는 것을 안 다루므로 int32를 권장합니다. 로컬 프로세스에서만 다루는 것이면 intPtr을 권합니다. 네트워크 통계 등 32bit로는 불충분한 값을 다루면
	int64를 권합니다.

	\~english
	Allocation class that uses external buffer

	Note: There must be NO previously allocated object within external buffer. In other words,it must be an initialized memory area since CArrayWithExternalBuffer will be initialized.

	\param TYPE Allocation type
	\param RAWTYPE To check if the type is safe even if array element is processed as “raw memory copy”. int is safe but std.string is not safe.
	In case element type of array is not related to constructor, destructor and copy assignment operator, you can set it as “true”. 
	If setting it as “true”, constructor, destructor and copy assignment will not be called for progression of construction, destruction and copy of array element that internally occurs when inserting & deleting & changing size.
	Therefore, processing speed will be improved. 
	Default is “false”.
	\param INDEXTYP It is strongly recommended to use the maximum size of array and one of index types like int32,int64 and intPtr.
	Casting load between int32 and int64 should be considered, so using the appropriate one is recommended. 
	For example, packet size should be int32 because it does not exceed 2GB and if it is only for local process, intPtr is recommended. 
	int64 is appropriate when int32 cannot deal with accurate value like network statistics.  

	\~chinese
	使用外部缓存区的排列类。
	注意：外部缓存区不应存在已经分配的对象。即应该是单纯初始化的内存领域。因为 CArrayWithExternalBuffer%会进行初始话。

	\param TYPE 排列的元素类型。
	\param RAWTYPE 数组项目进行raw memory copy是否是安全的类型。Int 虽然安全，但 std.string%不安全。
	如果排列的项目类型可以不使用Constructor、Destructor、复制赋值运算符（Copy Assignment Operator）那么可以指定为true。
	如果将其指定为true，在对排列进行插入、删除、变更大小等操作时，为了进行在内部发生的排列项目的生成、破坏、复制过程，不呼叫Constructor、Destructor、复制赋值运算符。
	这会提高处理速度。其初始值为false。
	\param INDEXTYP 建议在数组最大大小及index的type. int32,int64,intPtr 之中使用一个。32-64bit int 之间的casting负荷也不能忽视，所以建议使用恰当的方法。
	例如数据包大小基本都不使用超过2GB的，所以建议使用int32。使用本地程序的话建议intPtr。如互联网统计等即使利用32bit也不充分的话，建议用int64。

	\~japanese
	外部バッファーを使う配列クラスです。

	ご注意：外部バッファー内に既に割り当てられたオブジェクトが存在してはいけません。即ち、単純に初期化されたメモリー領域ではなければなりません。なぜなら、CArrayWithExternalBufferが初期化されるからです。
	\param TYPE 配列の元素タイプです。
	\param RAWTYPE 配列元素がraw memory copyをしても安全なタイプなのかに対する可否です。Intは安全ですが、std.stringは安全ではありません。
	配列の項目タイプが生成子、破壊子、コピー代入演算子が使われなくても良いタイプである場合、trueに指定することができます。
	これをtureに指定すると配列に挿入、削除、サイズ変更などをする時、内部的に発生する配列項目の生成、破壊、コピー過程のために、生成子、破壊子、コピー代入演算子を呼び出しません。よって、処理速度が向上されます。基本値はfalseです。
	\param INDEXTYPE配列最大サイズおよびindexのtype. int32、int64、intPtrのうち一つを使うことをお勧めします。32-64bit int間のcasting負荷も無視できませんので適切なものを使うことをお勧めします。
	例えば、パケットのサイズの場合、できれば2GBを超過しないものは扱わないのでint32をお勧めします。ローカルプロセスでのみ扱いものならintPtrをお勧めします。ネットワーク統計など32bitでは不十分な値を扱えばint64をお勧めします。

	\~
	*/
	template < typename TYPE, bool TYPE_IN_REF = true, bool RAWTYPE = false, typename INDEXTYPE = intptr_t>
	class  CArrayWithExternalBuffer :public CFastArray < TYPE, TYPE_IN_REF, RAWTYPE, INDEXTYPE >
	{
		/****************************
		주의!!! 고정 크기 배열을 멤버로 가지는 로컬 변수가 이것과는 PS4에서 꼬인다!! 
		*/

	public:
		inline CArrayWithExternalBuffer(void *buffer, INDEXTYPE capacity)
		{
			// CFastArray에서 설정되었던 상태를 변조한다.
			this->m_Capacity = capacity;
			this->m_Data = (TYPE*)buffer;
		}

		

		/**
		\~korean
		NULL이면 예외를 낸다

		\~english TODO:translate needed.
		Generate an exception if null. 

		\~chinese
		NULL 的话会例外。

		\~japanese
		NULLであれば例外になります。
		\~
		*/
		inline void MustNotNull() const
		{
			if (this->m_Data == NULL)
				ThrowException(ExternalBufferArrayIsUninitializedError);
		}
		
		/**
		\~korean
		NULL이 아니면 예외를 낸다

		\~english TODO:translate needed.
		Generate an exception if not null.

		\~chinese
		不是NULL的话例外。

		\~japanese
		NULLではなければ例外になります。
		\~
		*/
		inline void MustNull() const
		{
			if (this->m_Data != NULL)
				ThrowException(ExternalBufferArrayIsInitializedError);
		}

		/**
		\~korean
		현재 NULL이다

		\~english TODO:translate needed.
		It’s null now

		\~chinese
		现在是NULL。

		\~japanese
		現在NULLです。
		\~
		*/
		inline bool IsNull() const
		{
			return this->m_Data == NULL;
		}

		/** 
		\~korean 
		세팅한다.
		\param buffer buffer포인터 
		\param capacity 버퍼의 크기 

		\~english TODO:translate needed.
		Set it up. 
		\param buffer buffer pointer
		\param capacity buffer size

		\~chinese
		设置。
		\param buffer buffer指针。
		\param capacity buffer的大小。		

		\~japanese
		設定します。
		\param buffer bufferポインター
		\param capacity バッファーのサイズ
		\~
		*/
		inline void Init(void* buffer, INDEXTYPE capacity)
		{
			MustNull();

			/*
			// TODO:구체적인 사례 기입 필요
			if(buffer == 0 || capacity == 0)
				return;   // 일부러 이렇게 설정하는 경우도 있을 수 있다. 이러한 경우 아무 것도 처리하지 말고 리턴해야 한다. 이것을 원하는 업체가 있었다.

			위 코드는, 패러미터 검증 후 하나라도 null인 경우 무조건 return하여 초기화 작업이 중간에 중지되게 만듭니다.
			그러나 윗줄 MustNull()에 의해 this->m_Data가 null인 상태에서 멈춥니다.
			혹시나 m_Data를 접근 할 시 null pointer exception이 출현합니다.

			m_externalBuffer가 NULL이지만, 이것의 길이와 용량이 0이 아니어도 되는 사례를 찾았다면 위 2줄 코드의 주석을 해제해 주십시오.
			만약 패러미터 하나라도 null이면 초기화를 중지하고자 하는 의도라면 위 2줄의 코드를 MustNull()위에 위치하여 주십시오. 
			*/
						
			// 상태를 변조한다.
			this->m_Capacity = capacity;
			this->m_Length = 0;
			this->m_Data = (TYPE*)buffer;
		}

		/**
		\~korean
		모든 멤버 변수를 초기화 한다.

		\~english TODO:translate needed.
		Initialize all member parameters. 

		\~chinese
		初始化所有成员变数。

		\~japanese
		全てのメンバーの変数を初期化します。
		\~
		*/
		inline void Uninit()
		{
			this->SetCount(0); // 이걸 해주어서 완전 초기화부터 해 버려야!
			this->m_Capacity = 0;
			this->m_Length = 0;
			this->m_Data = NULL;
		}

		inline CArrayWithExternalBuffer()
		{
			this->m_Capacity = 0;
			this->m_Length = 0;
			this->m_Data = NULL;
		}

		inline ~CArrayWithExternalBuffer()
		{
			// 이걸 해줘야 상위 클래스의 파괴자에서 DataBlock_Free를 호출 안한다.
			// 이 함수가 리턴할 때에는 사실상 모든게 청소된 상태이어야 한다.
			if (IsNull() == false)
			{
				this->SetCount(0);
				this->m_Data = NULL; 
			}
		}

		/**
		\~korean 
		dest와 버퍼를 공유한다. 

		\~english Shares data with dest.

		\~chinese
		dest 与dest共享缓存区。

		\~japanese
		Destとバッファーを供給します。
		\~
		*/
		inline void ShareTo(CArrayWithExternalBuffer& dest) const
		{
			dest.m_Capacity = this->m_Capacity;
			dest.m_Data = this->m_Data;
			dest.m_Length = this->m_Length;
		}
	private:



		/* 
		\~korean
		복사 자체는 금지된다. CopyTo 또는 ShareTo를 써야 한다.
		이렇게 되어 있는 이유는, CArrayWithExternalBuffer 는 = 연산자를 쓸 경우 이것이
		포인터 공유인지, 내용물 복사인지가 모호하기 때문이다.

		\~english
		Duplication itself is prohibited. Must use CopyTo or ShareTo.
		The reason why is that it is unclear whether CArrayWithExternalBuffer is a pointer share or copying contents 
		when CArrayWithExternalBuffer uses '=' opeartor.

		\~chinese
		复制本身是被禁止的。要使用CopyTo或者ShareTo。
		因为，使用 CArrayWithExternalBuffer%运算者的话，很难确定其是指针共享还是内容复制。

		\~japanese
		コピー自体は禁止です。CopyToまたはShareToを使ってください。
		このようになる理由は、CArrayWithExternalBufferは=演算子を使う場合、これがポインター共有なのか、内容物コピーなのかが曖昧だからです。
		\~
		*/
		CArrayWithExternalBuffer& operator=(const CArrayWithExternalBuffer&);
		CArrayWithExternalBuffer(const CArrayWithExternalBuffer&);

		virtual INDEXTYPE GetRecommendedCapacity(INDEXTYPE actualCount)
		{
			// 항상 이미 잡혀있는 외장 메모리의 크기를 줘야.
			if (actualCount > this->m_Capacity)
				ThrowArrayOutOfBoundException();
			return this->m_Capacity;
		}

		virtual void DataBlock_Free(void* /*data*/)
		{
			// 하는 일이 없다. 
			// 주의: CFastArray가 m_Data=null 하는 일이 없음을 가정해야 한다!
		}

		virtual void* DataBlock_Alloc(size_t length)
		{
			if (length > 0)
			{
				MustNotNull();
			}
			return this->m_Data;
		}

		virtual void* DataBlock_Realloc(void* /*oldPtr*/, size_t newLength)
		{
			if (newLength > 0)
			{
				MustNotNull();
			}
			return this->m_Data;
		}
	};

	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
