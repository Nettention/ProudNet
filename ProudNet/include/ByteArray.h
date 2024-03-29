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
#include "FastArray.h"
#include "PNString.h"

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
	바이트 배열. ProudNet의 용도에 맞게 최적화되어 있다.
	- 네트워크 패킷을 주고 받는 목적이므로 int32로만 사용을 강제한다.
	- ByteArray는 operator new,delete도 오버라이드되어서 fast heap을 접근하고 있다. 따라서	사용시 Proud.CFastHeap의 주의사항 규약을 준수해야 한다.

	\~english TODO:translate needed.
	Byte array. It is optimized for the use of ProudNet
	- Since the purpose is to give and receive network packets, use is mandatory only in int32
	- The ByteArray is overridden in the operator new and delete, so it approaches fast heap. When using, the caution rules of Proud.CFastHeap must be complied with


	\~chinese
	Byte 排列。已优化成适用ProudNet用途。
	- 因其目的是为网络数据包之间的交换，因此强制使用int32。
	- BytesArray超过了operator new,delete,因此接近fast heap。所以在使用时要遵守 Proud.CFastHeap%的注意事项规章。

	\japanese
	バイト配列。ProudNetの用途に合わせて最適化されています。
	- ネットワークパケットをやり取りする目的ですので、int32でのみ使用を強制します。
	- ByteArrayはoperator new、deleteもオーバーライドされて、fast heapにアプローチしています。よって、使用時にProud.CFastHeapのご注意事項の規約を順守しなければなりません。
	\~
	\todo translate it.
	 */
	class ByteArray :public CFastArray < uint8_t, false, true, int >
	{
	public:
		typedef CFastArray<uint8_t, false, true, int> Base;
// 		 static void* LookasideAllocator_Alloc(size_t length);
// 		 static void LookasideAllocator_Free(void* ptr);
        
	public:
//#pragma push_macro("new")
//#undef new
		//DECLARE_NEW_AND_DELETE
//		void* operator new(size_t size)
//		{
//			return LookasideAllocator_Alloc(size);
//		}
//		void operator delete(void* ptr, size_t /*size */)
//		{
//			LookasideAllocator_Free(ptr);
//		}
//#pragma pop_macro("new")

		/** 
		\~korean
		기본 생성자

		\~english
		Defalut constructor

		\~chinese
		基本生成者。

		\~japanese
		基本生成子
		\~
		 */
		inline ByteArray()
		{
		}

		/** 
		\~korean
		외부 데이터를 복사해오는 생성자
		\param data 복사해올 BYTE배열의 포인터
		\param count count 만큼을 복사한다.

		\~english TODO:translate needed.
		Constructor copies external data 
		\param data
		\param count

		\~chinese
		复制外部数据的生成者。
		\param date 要复制的BYTE排列的指针。
		\param count 复制相当于count的量。 

		\~japanese
		外部データをコピーしてくる生成子
		\param data コピーしてくるBYTE配列のポインター
		\param count count だけをコピーします。
		\~
		 */
		inline ByteArray(const uint8_t* data, int count):Base(data,count)
		{
		}

		/** 
		\~korean
		외부 데이터를 복사해오는 생성자
		\param src 복사할 ByteArray 원본

		\~english TODO:translate needed.
		Constructor copies external data
		\param src

		\~chinese
		复制外部数据的生成者。
		\param src 要复制的BytesArray原件。

		\~japanese
		外部データをコピーしてくる生成子
		\param src コピーするByteArray原本
		\~
		 */
		inline ByteArray(const ByteArray &src):Base(src)
		{
		}

		 PROUD_API virtual ~ByteArray();

// 		BUG PRONE! 따라서 막아버렸다.
		/*
						\~korean
						생성자. 단, 배열 크기를 미리 설정한다.
						\~english
						Constructor. But it must have previously set array size.
						\~
						 */
// 		inline ByteArray(int count):CFastArray<BYTE,true>(count)
// 		{
// 		}

		/** 
		\~korean
		배열을 16진수의 String으로 변환하여 return해준다.
		\return String으로 변환된 16진수 배열 (예 L"FFAB123")

		\~english TODO:translate needed.
		The array is returned by being transformed in a hexadecimal string
		\return Hexadecimal array converted with String (ex: L”FFAB123”)

		\~chinese
		将排列转换成16进数。
		转换成
		\return String的16进数排列。（例：L”FFAB123”）

		\~japanese
		配列を16進数のStringに変換してreturnしてくれます。
		\return Stringに変換された16進数の配列（例：L"FFAB123")
		\~
		*/
		 PROUD_API String ToHexString();

		/**
		\~korean
		16진수 배열 String을 16진수로 바꾸어준다.
		\param 16진수 배열 String (예 text = L"FFFFEAB12")
		\return 변환이 성공하면 true, 실패하면 false

		\~english TODO:translate needed.
		A hexadecimal array string is converted into a hexadecimal number
		\param hexadecimal array String (ex. Text = L"FFFFEAB12")
		\return conversion is successful, it is true. If failed, it is false. 	

		\~chinese
		将16进数排列String换成16进数。
		\param 16进数排列String （例：text=L”FFFFEAB12”）
		\return 转换成功为true，否则为false。

		\~japanese
		16進数の配列Stringを16進数に変えてくれます。
		\param 16進数の配列String (例：text = L"FFFFEAB12")
		\return 変換が成功するとtrue、失敗するとfalse
		\~
		*/
		 PROUD_API  bool FromHexString(String text);


		 inline uint32_t Hash() const
		 {
			 // 4바이트씩 해시를 하고 맨 마지막 잔여 바이트를 해시한다.
			 int bigLen = GetCount() >> 2; // DIV 4
			 int smallLen = GetCount() & 3;	// MOD 4

			 uint32_t ret = 0;
			 uint32_t* bigPtr = (uint32_t*)GetData();
			 for (int i = 0; i < bigLen; i++)
			 {
				 ret ^= *bigPtr;
				 bigPtr++;
			 }
			 uint8_t* smallPtr = (uint8_t*)bigPtr;
			 uint32_t remainderValue = 0;
			 uint8_t* smallPtrDest = (uint8_t*)&remainderValue;
			 for (int i = 0; i < smallLen; i++)
			 {
#ifdef __ANDROID__
				 memcpy(smallPtrDest, smallPtr, 1);		// 혹은 안드로이드 한정으로 __packed keyword를 쓰던지...
#else
				 *smallPtrDest = *smallPtr;
#endif
				 smallPtr++;
				 smallPtrDest++;
			 }
			 ret ^= remainderValue;

			 return ret;
		 }


	};

// 	// ByteArray에 한해서는 == 연산자를 허락해주자. map key로 사용되는 경우 때문이다.
	// => 아래 클래스 구현 때문에 허락 할 필요 없다.
// 	inline bool operator==(const ByteArray& lhs, const ByteArray& rhs)
// 	{
// 		return lhs.Equals(rhs);
// 	}
	
	/**  @} */


}

template<>
class CPNElementTraits<Proud::ByteArray>
{
public:
	typedef const Proud::ByteArray& INARGTYPE;
	typedef Proud::ByteArray& OUTARGTYPE;

	inline static uint32_t Hash(INARGTYPE a)
	{
		return a.Hash();
	}

	inline static bool CompareElements(INARGTYPE element1, INARGTYPE element2)
	{
		return element1.Equals(element2);
	}

	inline static int CompareElementsOrdered(INARGTYPE element1, INARGTYPE element2)
	{
		// 사전적 비교.
		// element1,2의 앞단 부분의 내용을 비교한다.
		int minLength = PNMIN(element1.GetCount(), element2.GetCount());
		int ret1 = memcmp(element1.GetData(), element2.GetData(), minLength);

		// element1,2의 앞단 부분에 내용에 차이가 있다. 그것의 대소여부를 판별해서 리턴한다.
		if (ret1 != 0)
			return ret1;

		if (element1.GetCount() < element2.GetCount())
		{
			// elements1와 element2의 앞단은 서로 같고 element2가 더 길이가 길다. 따라서 element1이 더 작다.
			return -1;
		}
		else if (element1.GetCount() == element2.GetCount())
		{
			// elements1와 element2가 서로 완전히 같다.
			return 0;
		}
		else
		{
			return 1;
		}
	}

};



#ifdef _MSC_VER
#pragma pack(pop)
#endif

