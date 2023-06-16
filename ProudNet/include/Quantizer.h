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

#include "ProcHeap.h"
#include "Exception.h"

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
	<a target="_blank" href="http://guide.nettention.com/cpp_ko#quantizer" >데이터 양자화</a> 기능을 담당하는 클래스입니다.

	\~english
	Class manages <a target="_blank" href="http://guide.nettention.com/cpp_en#quantizer" >Data Quantization</a> function

	\~chinese
	担任<a target="_blank" href="http://guide.nettention.com/cpp_zh#quantizer" >数据量子化</a>%功能的类 。

	\~japanese
	\ref quantizer  機能を担当するクラスです。
	\~
	*/
	class CQuantizer
	{
		double m_min,m_max;
		uint32_t m_granulation;
	public:
		/** 
		\~korean
		생성자
		\param min 양자화되는 값은 이 값 이하로는 들어가지 않습니다.
		\param max 양자화되는 값은 이 값 이상으로는 들어가지 않습니다.
		\param granulation 양자화되는 값은 정수 타입입니다. 본 값은 양자화되는 값이 0부터 얼마까지의 정수형으로 변환되느냐를 지칭합니다.
		값이 클수록 양자화된 값의 정밀도가 높지만 양자화된 데이터가 필요로 하는 비트수가 증가하게 됩니다. 

		\~english
		Constructor
		\param min the quantized value will not be entered and no less than this value.
		\param max the quantized value will not be entered and no more than this value.
		\param granulation the quantized value has integer type. This value points that the quantized value is to be converted from 0 to which integer.
		Larger value provides more pricise quantized value but the number of bits required by quantized data.

		\~chinese
		生成子
		\param min 输入量子化的值不能低于此值。
		\param ma 输入量子化的值不能高于此值。
		\param granulation 量子化的值是正数类。此值是指称量子化的值转换为从0到多少的正数类。
		值越大，量子化的值精度越大，但是量子化的数据需要的bit值会增加。

		\~japanese
		生成子
		\param min 量子化される値はこの値以下には入りません。
		\param min 量子化される値はこの値以下には入りません。
		\param max 양자화되는 값은 이 값 이하로는 들어가지 않습니다.
		\param granulation 量子化される値は整数タイプです。この値は量子化される値が0からいくらまでの整数型に変換されるかを指します。
		値が大きいほど量子化された値の精密度が高いですが、量子化されたデータが必要とするビッド数が増加します。
		\~
		*/
		CQuantizer( double min, double max, uint32_t granulation )
		{
			m_min = min;
			m_max = max;
			m_granulation = granulation;
		}

		/** 
		\~korean
		양자화를 합니다. 

		\~english
		Quantizing

		\~chinese
		进行量子化。

		\~japanese
		量子化をします。
		\~
		*/
		uint32_t Quantize(double value)
		{
			value = PNMIN(m_max, value);
			value = PNMAX(m_min, value);
			double range = m_max - m_min;
			double offset = value - m_min;
			double offset2 = offset / range * (double)m_granulation;
			return (uint32_t)offset2;
		}

		/** 
		\~korean
		양자화된 값을 복원합니다. 

		\~english
		Recover quantized value

		\~chinese
		恢复量子化的值。

		\~japanese
		量子化された値を復元します。
		\~
		*/
		double Dequantize(uint32_t value)
		{
			double offset = (double)value;
			double range = m_max - m_min;
			double offset2 = offset / (double)m_granulation * range;
			return offset2 + m_min;
		}

#ifdef _WIN32
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif
	};


	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
