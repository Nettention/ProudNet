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

#include "FastArrayPtr.h"
#include "ByteArray.h"
#include "BiasManagedPointer.h"

#if defined(_MSC_VER)
#pragma warning(disable:4290) // 이 경고는 VC++에서만 발생하니 막자
#endif

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
	ByteArray 에 smart pointer 기능이 추가된 형태. 네트워크 패킷 형태로 쓰이므로 32-64 변환에도 민감. 따라서 intPtr이 아닌 int32 타입이다.
	ByteArray 가 typedef가 아닌 상속 클래스이기 때문에 필요한 메서드를 이 객체도 오버라이드해야
	빌드 에러를 피한다.

	\~english
	It is the form that smart pointer function has been added to ByteArray. It is very sensitive to conversion from 32 to 64 because of being used as the form of network packet, so its form is int32 (not intPtr).
	To avoid build error, this object has to override require method because ByreArray is inherit class instead of typedef.

	\~chinese
	在 ByteArray%添加了smartpointer技能的形态。因其也用于网络数据包的形态所以对32-64变换较为敏感。因此其不是inPtr而是int32类型。
	因为 ByteArray%不是typedef而是接续类，因此将必要的方法超越此客体才能避开Build错误。

	\~japanese
	ByteArrayにsmart pointer機能が追加された形態。ネットワークパケット形態で使われますので、32-64変換にも敏感です。よって、intPtrではないint32タイプです。
	ByteArrayがtypedefではない相続クラスであるため、必要なメソッドをこのオブジェクトもオーバーライドしてビルドエラーを避けます。
	\~
	 */
	class  ByteArrayPtr :public BiasManagedPointer<ByteArray, true>
	{
	private:
		// base class의 자체적 buffer와, 이 external buffer 중에서는 하나만 유효하고, 나머지는 빈 상태가 된다.
		CArrayWithExternalBuffer<uint8_t, false, true, int> m_externalBuffer;

	public:
		// ikpil.choi 2016-11-29 : T의 타입 사이즈 추가
		static const size_t TYPE_SIZE = sizeof(uint8_t);

	public:

		inline ByteArrayPtr() {}

		inline ByteArrayPtr(const ByteArrayPtr& src)
			: BiasManagedPointer<ByteArray, true>(src)
		{
			src.m_externalBuffer.ShareTo(m_externalBuffer);
		}

#ifdef SUPPORTS_CPP11
		// 이동 생성자
 		inline ByteArrayPtr(ByteArrayPtr&& src)
 			: BiasManagedPointer<ByteArray, true>(src)
 		{
 			src.m_externalBuffer.ShareTo(m_externalBuffer);
 			src.m_externalBuffer.Uninit();
 		}
#endif
		inline ByteArrayPtr& operator=(const ByteArrayPtr& src)
		{
			// base class
			BiasManagedPointer<ByteArray, true>::operator=(src);

			src.m_externalBuffer.ShareTo(m_externalBuffer);

			return *this;
		}

#ifdef SUPPORTS_CPP11
		// 이동 연산자
 		inline ByteArrayPtr& operator=(ByteArrayPtr&& src)
 		{
 			// base class
 			BiasManagedPointer<ByteArray, true>::operator=(src);

 			if (&src != this) // 이동 연산자는 이거 꼭 체크해야.
 			{
 				src.m_externalBuffer.ShareTo(m_externalBuffer);
 				src.m_externalBuffer.Uninit();
 			}

 			return *this;
 		}
#endif

	public:

		inline int GetCapacity() const
		{
			if (GetTombstone())
			{
				return GetTombstone()->m_substance.GetCapacity();
			}
			else if (m_externalBuffer.IsNull())
			{
				ThrowArrayIsNullError();
			}

			return m_externalBuffer.GetCapacity();
		}

		inline int GetCount() const
		{
			if (GetTombstone())
			{
				return GetTombstone()->m_substance.GetCount();
			}
			else if (m_externalBuffer.IsNull())
			{
				ThrowArrayIsNullError();
			}

			return m_externalBuffer.GetCount();
		}

#if defined(_MSC_VER)
		__declspec(property(get = GetCount)) int Count;
#endif

		inline void MustNotNull() const
		{
			if (IsNull())
			{
				ThrowArrayIsNullError();
			}
		}

		inline void MustNull() const /*throw(Exception) Linux C++11 빌드 경고 때문에 막음  */
		{
			if (!IsNull())
				ThrowException(ArrayPtrIsNotNullError);
		}

		/**
		\~korean
		\param length 변경할 Capacity 사이즈

		\~english TODO:translate needed.
		\param length Capacity size to change the length

		\~chinese
		\param length 要变换的Capacity大小。

		\~japanese
		\param length 変更するCapacityサイズ 
		\~
		*/
		inline void SetCapacity(int length)
		{
			if(length < 0)
				ThrowInvalidArgumentException();

			if (!m_externalBuffer.IsNull())
			{
				// 아무것도 안함
			}
			else
			{
				if (GetTombstone() != NULL)
					GetTombstone()->m_substance.SetCapacity(length);
			}
		}

		/**
		\~korean
		배열의 사이즈를 setting 한다.
		\param length 변경할 배열의 사이즈

		\~english TODO:translate needed.
		Set the size of the array
		\param length Size of the array to change the length

		\~chinese
		Setting 排列的大小。
		\param length 要变换的排列之大小。

		\~japanese
		配列のサイズを設定する。
		\param length 変更する配列のサイズ
		\~
		*/
		inline void SetCount(int length)
		{
			if(length < 0)
				ThrowInvalidArgumentException();

			if (!m_externalBuffer.IsNull())
			{
				m_externalBuffer.SetCount(length);
			}
			else
			{
				if(length > 0)
				{
					if(GetTombstone())
						GetTombstone()->m_substance.SetCount(length);
					else
					{
						ThrowArrayIsNullError();
					}
				}
				else
				{
					if(GetTombstone())
						GetTombstone()->m_substance.SetCount(0);
				}
			}
		}


		inline void AddCount(int length)
		{
			// CMessage.Write에 의한 호출빈도가 높아서, 성능 최적화가 되어 있다.
			if (!m_externalBuffer.IsNull())
			{
				m_externalBuffer.AddCount(length);
			}
			else
			{
				if (length > 0) // 크기 늘림이 없으면 그냥 무시하고 넘어간다.
				{
					if (GetTombstone())
						GetTombstone()->m_substance.AddCount(length);
					else
					{
						ThrowArrayIsNullError();
					}
				}
			}

		}

		/**
		\~korean
		1 BYTE 단위의 Data를 추가합니다.
		\param data 추가할 BYTE

		\~english TODO:translate needed.
		Add data in 1 byte unit
		\param data Byte to add data

		\~chinese
		添加1BYTE单位的Data。
		\param data 要添加的BYTE。

		\~japanese
		1バイト単位のデータを追加します。
		\param data 追加するバイト 
		\~
		*/
		inline void Add(uint8_t data)
		{
			AddRange(&data, 1);
		}

		/**
		\~korean
		현재의 배열에 BYTE배열 data를 추가 합니다.
		\param data 추가할 BYTE배열 포인터
		\param count 배열의 크기

		\~english TODO:translate needed.
		Add byte array data to the current array
		\param data Byte array pointer to add data
		\param count Size of the count array

		\~chinese
		在现有的排列上添加BYTE排列data。
		\param data 要添加的BYTE排列指针。
		\param count 排列的大小。

		\~japanese
		現在の配列にバイト配列データを追加します。
		\param data 追加するBYTE配列ポインター
		\param count 配列のサイズ
		\~
		*/
		inline void AddRange(const uint8_t* data, int count)
		{
			// InsertRange보다 AddRange바로 콜이 더 빠름
			if (m_externalBuffer.IsNull() == false)
			{
				m_externalBuffer.AddRange(data, count);
			}
			else
			{
				if(GetTombstone())
				{
					GetTombstone()->m_substance.AddRange(data, count);
				}
				else
				{
					ThrowArrayIsNullError();
				}
			}
		}

		/*
		\~korean
		현재의 배열에 BYTE배열 data를 추가 합니다.
		\param indexAt 배열내에서 indexAt번째뒤에 삽입합니다. 원래의 indexAt뒤의 데이터는 삽입된 데이터 뒤로 밀립니다.
		\param data 추가할 BYTE배열 포인터
		\param count 배열의 크기

		\~english TODO:translate needed.
		Add byte array data to the current array
		\Insert after (indexAt) th within the indexAt array. The data after the original indexAt is moved back to the inserted data.
		\param data Byte array pointer to add data
		\param count Size of the count array

		\~chinese
		在现有的排列上添加BYTE排列data。
		\param indexAt 在排列中的indexAt后插入。原有的indexAt后的数据将会推到插入的数据之后。
		\param data 要添加的BYTE排列指针。
		\param count 排列的大小。

		\~japanese
		現在の配列にバイト配列データを追加します。
		\param indexAt 配列内でindexAt番目の後に挿入します。本来のindexAtの後のデータは挿入されたデータの後にずらされます。
		\param data 追加するバイト配列ポインター 
		\param count 配列のサイズ 
		\~
		*/
		inline void InsertRange(int indexAt, const uint8_t* data, int count)
		{
			if (m_externalBuffer.IsNull() == false)
			{
				m_externalBuffer.InsertRange(indexAt, data, count);
			}
			else
			{
				if(GetTombstone())
					GetTombstone()->m_substance.InsertRange(indexAt, data, count);
				else
				{
					ThrowArrayIsNullError();
				}
			}
		}

		/**
		\~korean
		데이터들을 제거 합니다.
		\param indexAt 제거한 data의 index입니다.
		\param count 제거할 배열의 수 입니다.

		\~english TODO:translate needed.
		Remove data
		\param indexAt Index of the removed data
		\param count Number of the array to be removed


		\~chinese
		删除Data。
		\param indexAt 是删除的Data index。
		\param count  要删除的排列数。

		\~japanese
		データを除去します。
		\param indexAt 除去したデータのindexです。
		\param count 除去する配列の数です。
		\~
		*/
		inline void RemoveRange(int indexAt, int count)
		{
			if (m_externalBuffer.IsNull() == false)
			{
				m_externalBuffer.RemoveRange(indexAt, count);
			}
			else
			{
				if(GetTombstone())
					GetTombstone()->m_substance.RemoveRange(indexAt, count);
				else
				{
					ThrowArrayIsNullError();
				}
			}
		}

		/**
		\~korean
		배열내의 하나의 데이터를 제거합니다.
		\param index 제거할 배열의 index값

		\~english TODO:translate needed.
		Remove one datum within the array
		\param index Index value of the array to be removed


		\~chinese
		删除排列中的一个Data。
		\param index 要删除的排列的index值。

		\~japanese
		配列内の一つのデータを除去します。
		\param index 除去する配列のindex値
		\~
		*/
		inline void RemoveAt(int index)
		{
			RemoveRange(index, 1);
		}

		/**
		\~korean
		배열을 비웁니다. capacity는 변화하지 않습니다.

		\~english 
		Empty the array. The capacity does not change

		\~chinese
		清空排列。Capacity 不会变。

		\~japanese
		配列に空にします。Capacityは変化しません。
		\~
		*/
		inline void Clear()
		{
			SetCount(0);
			// capacity는 그대로 두어야 함. 안그러면 성능이 곳곳에서 크게 저하된다!
		}

		/**
		\~korean
		\return Data배열의 포인터를 리턴합니다.

		\~english 
		\return the pointer of the data array.

		\~chinese
		\return return Data排列的指针。

		\~japanese
		\return Data配列のポインターをリターンします。
		\~
		*/
		inline uint8_t* GetData()
		{
			if (!m_externalBuffer.IsNull())
			{
				return m_externalBuffer.GetData();
			}
			else if (GetTombstone() == NULL)
			{
				ThrowArrayIsNullError();
			}

			return GetTombstone()->m_substance.GetData();
		}

		/**
		\~korean
		\return Data배열의 const 포인터를 리턴합니다.

		\~english TODO:translate needed.
		\return the const pointer of the data array. 

		\~chinese
		\return return Data排列的const指针。

		\~japanese
		\return Data配列のconstポインターをリターンします。
		\~
		*/
		inline const uint8_t* GetData() const
		{
			if (m_externalBuffer.IsNull() == false)
				return m_externalBuffer.GetData();
			else if (GetTombstone() == NULL)
			{
				ThrowArrayIsNullError();
			}

			return GetTombstone()->m_substance.GetData();
		}

		/**
		\~korean
		\return 배열의 복사본을 리턴합니다.

		\~english TODO:translate needed.
		\return the copy of the array

		\~chinese
		\return  return排列的复本。

		\~japanese
		\return 配列のコピーをリターンします。
		\~
		*/
		inline ByteArrayPtr Clone()
		{
			if(!m_externalBuffer.IsNull())
				ThrowException(MustNotExternalBufferError);

			ByteArrayPtr ret;

			if(GetTombstone())
			{
				ret.UseInternalBuffer();
				ret.SetCount(GetCount());
			}
			CopyRangeTo(ret, 0, GetCount());

			return ret;
		}

		/**
		\~korean
		데이터들을 dest로 복사합
		\param dest 복사받을 객체
		\param srcOffset 복사를 시작할 원본 배열의 위치
		\param count 복사 받을 배열의 크기

		\~english TODO:translate needed.
		copy data to dest
		\param dest object to be copied 
		\param srcOffset Location of the original array to start copying
		\param count Size of the array to be copied 

		\~chinese
		将数据复制到dest。
		\param dest  接受复制的客体。
		\param srcOffset  要开始复制的原本的排列位置。
		\param count 接受复制的排列的大小。

		\~japanese
		データをdestへコピーします。
		\param dest コピーしてもらうオブジェクト
		\param srcOffset コピーを開始する原本配列の位置
		\param count コピーしてもらう配列のサイズ
		\~
		*/
		template<typename BYTEARRAYT>
		inline void CopyRangeToT(BYTEARRAYT &dest, int srcOffset, int count) const
		{
			if (count <= 0)
				return;

			if (dest.GetCount() < srcOffset + count)
				ThrowArrayOutOfBoundException();

			if (!GetData() || !dest.GetData())
				ThrowInvalidArgumentException();

			UnsafeFastMemmove(dest.GetData(), GetData() + srcOffset, count * sizeof(uint8_t));
		}

		/**
		\~korean
		데이터들을 dest로 복사합
		\param dest 복사받을 ByteArrayPtr
		\param srcOffset 복사를 시작할 원본 배열의 위치
		\param count 복사 받을 배열의 크기

		\~english TODO:translate needed.
		copy data to dest
		\param dest ByteArrayPtr to be copied
		\param srcOffset Location of the original array to start copying
		\param count Size of the array to be copied

		\~chinese
		将数据复制到dest。
		\param dest 接受复制的 ByteArrayPtr
		\param srcOffset 要开始复制的原本排列的位置。
		\param count 接受复制的排列的大小。

		\~japanese
		データをdestへコピーします。
		\param dest コピーしてもらうByteArrayPtr
		\param srcOffset コピーを開始する原本配列の位置
		\param count コピーしてもらう配列のサイズ
		\~
		*/
		void CopyRangeTo(ByteArrayPtr &dest, int srcOffset, int count) const
		{
			CopyRangeToT<ByteArrayPtr>(dest,srcOffset,count);
		}

		/**
		\~korean
		데이터들을 dest로 복사합
		\param dest 복사받을 ByteArray
		\param srcOffset 복사를 시작할 원본 배열의 위치
		\param count 복사 받을 배열의 크기

		\~english TODO:translate needed.
		copy data to dest
		\param dest ByteArray to be copied
		\param srcOffset Location of the original array to start copying
		\param count Size of the array to be copied


		\~chinese
		将数据复制到dest。
		\param dest 要接受复制的 ByteArray
		\param srcOffset 要开始复制的原本的排列位置。
		\param count 要接受复制的排列的大小。

		\~japanese
		データをdestへコピーします。
		\param dest コピーしてもらうByteArray
		\param srcOffset コピーを開始する原本配列の位置
		\param count コピーしてもらう配列のサイズ
		\~
		*/
		void CopyRangeTo(ByteArray& dest, int srcOffset, int count) const
		{
			CopyRangeToT<ByteArray>(dest,srcOffset,count);
		}

		inline uint8_t& operator[](int index)
		{
			if (m_externalBuffer.IsNull()==false)
				return m_externalBuffer[index];
			else
			{
				if(GetTombstone())
					return GetTombstone()->m_substance.operator[](index);
				else
				{
					// UE4에서는 throw 키워드를 못 쓴다. 따라서 이렇게 함수로 포장하여 throw 키워드를 비노출시킨다.
					ThrowArrayIsNullError();
					return m_externalBuffer[0]; // unreachable code. just preventing 'Control may reach end of non-void function' warning
				}
			}
		}

		inline const uint8_t operator[](int index) const
		{
			if (m_externalBuffer.IsNull() == false)
				return m_externalBuffer[index];
			else
			{
				if (GetTombstone())
					return GetTombstone()->m_substance.operator[](index);
				else
				{
					// UE4에서는 throw 키워드를 못 쓴다. 따라서 이렇게 함수로 포장하여 throw 키워드를 비노출시킨다.
					ThrowArrayIsNullError();
					return m_externalBuffer[0]; // unreachable code. just preventing 'Control may reach end of non-void function' warning
				}
			}
		}


		/**
		\~korean
		\return 이 객체의 배열 크기가 증가할 때 가중치 타입

		\~english 
		\return Weighted value when the array size of this object increases. 

		\~chinese
		\return 当此客体的排列大小增加时的加重值类型。

		\~japanese
		\return このオブジェクトの配列サイズが増加する時の加重値タイプ
		\~
		*/
		inline GrowPolicy GetGrowPolicy() const
		{
			if (GetTombstone() != NULL)
				GetTombstone()->m_substance.GetGrowPolicy();

			return GrowPolicy_Normal;
		}

		/**
		\~korean
		이 객체의 배열 크기가 증가할 때 가중치 타입을 설정한다.
		자세한 내용은 GrowPolicy 을 참조
		\param val 증가할 때의 가중치 타입

		\~english TODO:translate needed.
		The weighted value type is set when the array size of this object increases. 
		For more details, refer to GrowPolicy
		\param val The weighted value type when increasing

		\~chinese
		设置此客体的排列大小增加时的加重值。
		详细内容参考GrowPolicy。
		\param val 增加时的加重值的类型。

		\~japanese
		このオブジェクトの配列サイズが増加する時の加重値タイプを設定します。
		詳しくは、GrowPolicy をご参照ください。
		\param val 増加する時の加重値タイプ 
		\~
		*/
		void SetGrowPolicy(GrowPolicy val)
		{
			if (GetTombstone() != NULL)
				GetTombstone()->m_substance.SetGrowPolicy(val);
			else
			{
				// none
			}
		}

		/**
		\~korean
		최소 버퍼 크기를 설정한다. 버퍼(capacity)크기가 증가할 때 최소한 이 사이즈보다 크게 설정한다.
		\param val 최소 Capacity size

		\~english TODO:translate needed.
		Set the minimum buffer size. When the buffer (capacity) size increases, set it at least bigger than this size. 
		\param val Minimum capacity size

		\~chinese
		设置最小buffer的大小。当Buffer（Capacity）大小增加时至少要设置成比此大小大的值。
		\param val 最小Capacity size

		\~japanese
		最小バッファーサイズを設定します。バッファー(capacity)サイズが増加する時、少なくともこのサイズより大きく設定します。 
		\param val 最小 Capacity size 
		\~
		*/
		void SetMinCapacity(int val)
		{
			if(GetTombstone())
				GetTombstone()->m_substance.SetMinCapacity(val);
			else
			{
				// none
			}
		}

		/**
		\~korean
		이것을 초기에 호출하면 내부 버퍼를 생성하여 사용한다.

		\~english TODO:translate needed.
		If you call this at the initial stage, generate and use an internal buffer

		\~chinese
		在初期呼出这个，会生成并使用内部Buffer。

		\~japanese
		これを初期に呼び出すと内部バッファーを生成して使います。
		\~
		*/
		inline void UseInternalBuffer()
		{
			if(!m_externalBuffer.IsNull())
				ThrowException(AlreadyHasExternalBufferError);

			InitTombstoneIfEmpty();

		}

		/**
		\~korean
		이걸 초기에 호출하면 이 객체는 외부 버퍼를 사용한다.

		\~english
		If you call this at the beginning, this object use external buffer

		\~chinese
		在初期呼出这个，此客体将使用外部Buffer。

		\~japanese
		これを初期に呼び出すと内部バッファーを生成して使います。
		\~
		*/
		inline void UseExternalBuffer(uint8_t* buf, int capacity)
		{
			if (GetTombstone() != NULL)
				ThrowException(AlreadyHasInternalBufferError);
			else if (m_externalBuffer.IsNull() == false)
				ThrowException(AlreadyHasExternalBufferError);

			m_externalBuffer.Init(buf, capacity);
		}

		/**
		\~korean
		UseInternalBuffer, UseExternalBuffer 를 재사용하려면 이 메서드를 호출할 것.

		\~english
		Call this method if you want to reuse UseInternalBuffer,UseExternalBuffer

		\~chinese
		如果想再次使用UseInternalBuffer , UseExternalBuffer ， 呼出此方法。

		\~japanese
		UseInternalBuffer, UseExternalBufferを再使用するためにはこのメソッドを呼び出すこと。
		\~
		*/
		void UninitBuffer()
		{
			UninitTombstone();
			m_externalBuffer.Uninit();
		}

		/**
		\~korean
		\return NULL이면 true, NULL이 아니면 false를 리턴한다.

		\~english TODO:translate needed.
		\return true if null, otherwise return false.

		\~chinese
		\return  NULL的话true,不是NULL的话return false。

		\~japanese
		\return NULLであればtrue、NULLではなければfalseをリターンします。 
		\~
		*/
		inline bool IsNull() const
		{
			return (GetTombstone() == NULL && m_externalBuffer.IsNull());
		}

// 		inline ByteArray& GetInternalBufferRef()
// 		{
// 			MustInternalBuffer();
// 			return *m_tombstone;
// 		}

		/**
		\~korean
		내부 버퍼를 사용하고 있는 것이 아니면 예외를 발생시킨다.

		\~english TODO:translate needed.
		Generate an exception if an internal buffer is not being used.

		\~chinese
		如果不是使用内部Buffer，使之生成例外。

		\~japanese
		内部バッファーを使っているのではなければ例外を発生させます。
		\~
		*/
		inline void MustInternalBuffer()
		{
			if (GetTombstone() == NULL)
			{
				ThrowException(MustInternalBufferError);
			}
		}

		/**
		\~korean
		rhs와 내용이 동일한지 체크한다.
		- 주의: 단순 메모리 비교다. 이 점을 주의할 것.
		\param rhs 비교할 ByteArrayPtr
		\return 같으면 true 다르면 false를 리턴한다.

		\~english TODO:translate needed.
		 Check to see if the content is the same as rhs
		- Caution: this is a simple comparison of memory. Use caution.
		\param rhs ByteArrayPtr to be compared
		\return true if the same, otherwise return false

		\~chinese
		检查是否与rhs的内容一致。
		- 注意：只是简单的内存比较。要注意这一点。
		\param rhs 要比较的 ByteArrayPtr
		\return 如果一样的话true，否则return false。

		\~japanese
		Rhsと内容が同じなのかをチェックします。
		- ご注意：単純メモリー比較です。これにご注意ください。
		\param rhs 比較する ByteArrayPtr
		\return 同じであるとtrue、異なるとfalseをリターンします。
		\~
		*/
		inline bool Equals(const ByteArrayPtr &rhs) const
		{
			if (rhs.GetCount() != GetCount())
				return false;

			// 단순 메모리 비교다.
			const uint8_t* p_rhs = (uint8_t*)rhs.GetData();
			const uint8_t* p_lhs = (uint8_t*)GetData();
			return memcmp(p_rhs, p_lhs, rhs.GetCount() * sizeof(uint8_t)) == 0;
		}


	};

	typedef ByteArrayPtr::Tombstone ByteArrayPtr_Tombstone;

	//////////////////////////////////////////////////////////////////////////
	// UE4 x86 or x64에서 link error가 난다. 따라서 불가피하게 이렇게 비 템플릿 형태의 함수를 호출하도록 구현함.
	// #UE4_PATCH
	// TODO: 업그레이드 개발을 하다가, 언젠가 아래 코드가 반복될 것 같으면 매크로화하여 우회하도록 하자.

	PROUD_API void* ByteArrayPtr_AllocTombstone();
	PROUD_API void ByteArrayPtr_FreeTombstone(void* tombstone);

	template<>
	inline BiasManagedPointer<ByteArray, true>::Tombstone* BiasManagedPointer<ByteArray, true>::AllocTombstone()
	{
		return (BiasManagedPointer<ByteArray, true>::Tombstone*)ByteArrayPtr_AllocTombstone();
	}

	template<>
	inline void BiasManagedPointer<ByteArray, true>::FreeTombstone(BiasManagedPointer<ByteArray, true>::Tombstone* tombstone)
	{
		return ByteArrayPtr_FreeTombstone((void*)tombstone);
	}


	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
