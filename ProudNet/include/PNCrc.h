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

#define UPDC16(cp,crc)      (Proud::CCrc::Crc16Table[((crc^cp) & 0xff)] ^ (crc >> 8))
#define UPDCCCITT(cp,crc)   (Proud::CCrc::CrcCcittTable[((crc >> 8) & 0xff)] ^ (crc << 8) ^ cp)
#define UPDC32(octet,crc)   (Proud::CCrc::Crc32Table[((crc) ^ (octet)) & 0xff] ^ ((crc) >> 8))

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
	CRC 체크섬 클래스입니다.
	
	\~english
	CRC checksum class
	
	\~chinese
	CRC校验总合类。
	
	\~japanese
	CRCチェックサムクラスです。
	\~
	*/
	class CCrc
	{
	public:
		/** 
		\~korean
		Crc-16 
		\param data 데이터의 포인터
		\param length 데이터의 길이
		\return 체크섬 결과값

		\~english TODO:translate needed.
		Crc-16 
		\param data Data pointer
		\param length Data length
		\return Check sum result value

		\~chinese
		Crc-16
		\param data 数据的指针
		\param length 数据的长度
		\return 校验综合结果值

		\~japanese
		Crc-16 
		\param data データのポインター
		\param length データの長さ
		\return チェックサム結果値
		\~
		*/
		 static unsigned short Crc16(const uint8_t* data, unsigned int length);

		/** 
		\~korean
		Crc-ccitt 
		\param data 데이터의 포인터
		\param length 데이터의 길이
		\return 체크섬 결과값
		
		\~english TODO:translate needed.
		Crc-ccitt 
		\param data Data pointer
		\param length Data length
		\return Check sum result value

		
		\~chinese
		Crc-ccitt
		\param data 数据的指针
		\param length 数据的长度
		\return 校验综合结果值
		
		\~japanese
		Crc-ccitt 
		\param data データのポインター
		\param length データの長さ
		\return チェックサム結果値
		\~
		*/
		 static unsigned short CrcCcitt(const uint8_t* data, unsigned int length);

		/**  
		\~korean
		Crc-32
		\param data 데이터의 포인터
		\param length 데이터의 길이
		\return 체크섬 결과값
		
		\~english TODO:translate needed.
		Crc-32
		\param data Data pointer
		\param length Data length
		\return Check sum result value

		
		\~chinese
		Crc-32
		\param data 数据的指针
		\param length 数据的长度
		\return 校验综合结果值
		
		\~japanese
		Crc-32
		\param data データのポインター
		\param length データの長さ
		\return チェックサム結果値
		\~
		*/
		 static unsigned int  Crc32(const uint8_t* data, unsigned int length);

	private:
		static const unsigned short Crc16Table[256];
		static const unsigned short CrcCcittTable[256];
		static const unsigned int  Crc32Table[256];
	};

	typedef unsigned int Crc32;

	/**  @} */

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
