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
#include "FastArrayPtr.h"
#include "CriticalSect.h"

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

	/** 
	\~korean
	랜덤값 생성기

	특징
	- seed가 같으면 랜덤값들은 동일하다.

	일반적 용도
	- CRandom 객체를 생성한 후 InitializeSeed()나 SetSeed() 최초 1회 후 GetInt(), GetInt64, GetFloat 등을 쓰면 된다.
	- 명시적 랜덤 분포를 요구하지 않으면 그냥 StaticGet*()를 사용하라.
	- 명시적 랜덤 분포를 필요로 하면 이 객체를 만들어서 InitializeSeed()를 최초 한번만 호출한 후
	Get*()를 사용하라. 

	\~english
	Random value generator

	Characteristics
	- If seeds are same then the randome values are same.

	General usage
	- After creating CRandom object,use GetInt(), GetInt64, GetFloat and so on after calling InitializeSeed() or SetSeed() once for the first time.
	- If explicit random distribution is not required then just use StaticGet*().
	- If explicit random distribution is required then create this object, call InitializeSeed() once for the first time then use Get*().

	\~chinese
	随机值生成器

	特征
	- seed相同的话随机值相同。

	一般的用途
	- 生成 CRandom%对象以后InitializeSeed()或者SetSeed()最初1回以后使用GetInt(), GetInt64, GetFloat等即可。
	- 不要求清晰的随机分布的话就使用StaticGet*()吧。
	- 需要清晰的随机分布的话创建此对象以后，至少呼叫一次InitializeSeed()再使用Get*()吧。

	\~japanese
	ランダム値ジェレレーター

	特徴
	- seedが同じであればランダム値は同じです。

	一般的な用途
	- CRandom オブジェクトを生成した後、InitializeSeed()とかSetSeed()を最初1回後、GetInt(), GetInt64, GetFloat などを使ってください。
	- 明示的ランダム分布を要求しなければただ StaticGet*()を使ってください。
	- 明示的ランダム分布を必要とすればこのオブジェクトを作ってInitializeSeed()を最初1回のみ呼び出した後、Get*()を使ってください。 
	\~
	*/
	class CRandom
	{
	private:
		uint32_t m_dwState[625];
		uint32_t *m_pdwNext;
		int m_lLeft;
	
		uint32_t LoadMersenneTwister();
		double GetFloat_INTERNAL();
		static double StaticGetFloat_INTERNAL();

		CriticalSection m_cs;
	public:
		 PROUD_API CRandom();
		 PROUD_API virtual ~CRandom();

		/** 
		\~korean
		난수생성 seed 값을 설정합니다. 

		\~english
		Set random number creating seed value

		\~chinese
		设置乱数生成seed值。

		\~japanese
		乱数生成seed値を設定します。
		\~
		*/
		 PROUD_API void InitializeSeed();

		/** 
		\~korean
		난수생성 seed 값을 특정 값으로 설정합니다. 
		\param dwSeed 시드값

		\~english TODO:translate needed.
		Set random number creating seed value to specific value.

		\~chinese
		设置乱数生成seed值为特定值。
		\param dwSeed seed值

		\~japanese
		乱数生成seed値を特定値に設定します。
		\param dwSeed シード値
		\~
		*/
		 void SetSeed(uint32_t dwSeed);

		/** 
		\~korean
		랜덤값을 만든다. INT_MIN~INT_MAX 중 하나이다. 

		\~english
		Creates random value. One value between INT_MIN~INT_MAX

		\~chinese
		生成随机值。INT_MIN~INT_MAX 中之一。

		\~japanese
		ランダム値を作ります。INT_MIN~INT_MAXの一つです。 
		\~
		*/
		 PROUD_API int GetInt();

		/** 
		\~korean
		랜덤값을 만든다. INT64_MIN~INT64_MAX 중 하나이다.

		\~english
		Creates random value. One value between INT64_MIN~INT64_MAX

		\~chinese
		生成随机值。INT64_MIN~INT64_MAX 中之一。

		\~japanese
		ランダム値を作ります。INT64_MIN~INT64_MAXの一つです。
		\~
		*/
		 int64_t GetInt64();

		/** 
		\~korean
		전역 객체로 만들어진 CRandom 으로부터 값을 구한다. thread safe하다.

		\~english
		Gets value from CRandom that created with global object. It does thread safe.

		\~chinese
		在全域对象生成的 CRandom%中求值。是thread safe的。

		\~japanese
		全域オブジェクトで作られたCRandomより値を求めます。thread safeです。
		\~
		*/
		 PROUD_API static int StaticGetInt();

		/** 
		\~korean
		전역 객체로 만들어진 CRandom 으로부터 값을 구한다. thread safe하다. 

		\~english
		Gets value from CRandom that created with global object. It does thread safe.

		\~chinese
		在全域对象生成的 CRandom%中求值。是thread safe的。

		\~japanese
		全域オブジェクトで作られたCRandomより値を求めます。thread safeです。
		\~
		*/
		 static int64_t StaticGetInt64();

		/** 
		\~korean
		전역 객체로 만들어진 CRandom 으로부터 값을 구한다. 0~1 사이의 부동소수점 값이다. thread safe하다. 

		\~english
		Gets value from CRandom that created with global object. It is floating point value between 0 to 1. It does thread safe.

		\~chinese
		在全域对象生成的 CRandom%中求值。0~1之间的浮点值。是thread safe的。

		\~japanese
		全域オブジェクトで作られたCRandomより値を求めます。0~1の間の不動小数点値です。thread safeです。
		\~
		*/
		 double GetFloat();
		
		/** 
		\~korean
		전역 객체로 만들어진 CRandom 으로부터 값을 구한다. 0~1 사이의 부동소수점 값이다. thread safe하다. 

		\~english
		Gets value from CRandom that created with global object. It is floating point value between 0 to 1. It does thread safe.

		\~chinese
		在全域对象生成的 CRandom%中求值。0~1之间的浮点值。是thread safe的。

		\~japanese
		全域オブジェクトで作られたCRandomより値を求めます。0~1の間の不動小数点値です。thread safeです。
		\~
		*/
		 PROUD_API static double StaticGetFloat();
	};


	template<typename T>
	T GetRandomElement(const CFastArray<T>& arr)
	{
		if (arr.GetSize() == 0)
			return 0;
		else
			return arr[CRandom::StaticGetInt()%arr.GetSize()];
	}

#if defined(_WIN32)    
	template<typename T>
	T GetRandomElement(const std::vector<T>& arr)
	{
		if (arr.size() == 0)
			return 0;
		else
			return arr[CRandom::StaticGetInt()%arr.size()];
	}
#endif
    
	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
