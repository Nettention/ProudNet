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
#include "Random.h"
#include "FakeClr.h"
#include "pnguid.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/
	class Random
	{
		CRandom m_rand;
	public:
		Random()
		{
			m_rand.InitializeSeed();
		}

		/** 
		\~korean
		0~1 사이 난수 부동소수점 값을 생성합니다.

		\~english
		Create random floating point value between 0 to 1

		\~chinese
		生成0~1之间随机浮点（floating point）值。

		\~japanese
		0~1の間の乱数不動小数点値を生成します。
		\~
		*/
		 PROUD_API double NextDouble();

		/** 
		\~korean
		난수값을 생성합니다.
		생성된 난수값은 0부터 maxVal까지 중 하나입니다.

		\~english
		Create random number.
		Created random number will be one of between 0 to maxVal.

		\~chinese
		生成随机值。
		生成的随机值是0到maxVal之间的一个。

		\~japanese
		乱数値を生成します。
		生成された乱数値は0からmaxValまでの中の一つです。
		\~
		*/
		 PROUD_API int Next(int maxVal);

		/** 
		\~korean
		GUID 난수값을 생성합니다.

		\~english
		Create GUID random number.

		\~chinese
		生成GUID随机值。

		\~japanese
		GUID乱数値を生成します。
		\~
		*/
		 PROUD_API Guid NextGuid();
	};
	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
