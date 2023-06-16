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
#include "quicksort.h"
#include "pnstdint.h" 

#if defined(_WIN32)
    #include <algorithm>
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
	두 값 v1,v2의 v1->v2로의 내분값 ratio을 구한다.
	ratio가 0에 가까울수록 v1에 가깝다. 

	\~english
	Obtains a interior division ratio of two values of v1,v2, v1->v2.
	As ratio gets closer to 0, it gets closer to v1.

	\~chinese
	求往两个值v1、v2的v1->v2的内值ratio。
	Ration 越接近0就会越接近v1.

	\~japanese
	両値v1、v2のv1->v2への内分値ratioを求めます。
	ratioが0に近づくほどv1に近いです。
	\~
	*/
	template<typename T>
	inline T Lerp(T v1, T v2, T ratio)
	{
		return v1 + (v2 - v1)*ratio;
	}


	/**
	\~korean
 	Lerp의 정수형 버전. ratio의 분자는 ratio1,분모는 ratio2

	\~english TODO:translate needed.
	Integer version of Lerp. Numerator of the ratio is ratio 1. Denominator is ratio 2. 

	\~chinese
	Lerp 的整数型版本。Ratio 的分子是ratio1，分母是ratio2.

	\~japanese
	Lerpの整数型バージョン。Ratioの分子はratio1、分母はratio2
	\~
	*/
	template<typename T>
	inline T LerpInt(T v1, T v2,T ratio1, T ratio2)
	{
		return (v1 + ((v2 - v1) * ratio1) / ratio2);
	}

	/** 
	\~korean
	Round robin 슬롯 숫자 생성자.

	\~english
	Round robin slot number generator 

	\~chinese
	Round robin 插槽（slot）数字生成者。

	\~japanese
	Round robin スロット数字生成子
	\~
	*/
	class CRoundRobinNumberGenerator
	{
		int m_number;
	public:
		 PROUD_API CRoundRobinNumberGenerator();

		 PROUD_API int Next(int maxNumber);
	};
	/**  @} */

	// 출처: http://www.java2s.com/Tutorial/Cpp/0260__template/Genericquicksort.htm

// 	template<typename T>
// 	inline void swap(T& v1,T& v2)
// 	{
// 		T temp=v2;
// 		v2=v1;
// 		v1=temp;
// 	}
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
