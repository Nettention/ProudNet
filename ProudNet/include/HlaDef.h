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

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** 
	\~korean

	\~english
	HLA message identifier

	\~chinese

	\~japanese
	\~
	HLA message identifier
	*/
	enum HlaMessageType
	{
		HlaMessageType_Appear = 10,		// S2C
		HlaMessageType_Disappear,		// S2C
		HlaMessageType_RequestChange,	// C2S or C2C
		HlaMessageType_NotifyChange,		// S2C
	};

	typedef uint16_t HlaClassID;
	typedef uint8_t HlaFieldGroupID;
	typedef uint64_t HlaEntityID;

	const HlaFieldGroupID HlaFieldGroupID_None = 0;

	const HlaEntityID HlaEntityID_None = 0;

	enum Protocol
	{
		Protocol_Undefined,
		Protocol_UnreliableSend,
		Protocol_ReliableSend,
	};

	/** 
	\~korean

	\~english
	return values from synch class invocation.
	\todo you may change this type to what you want
	
	\~chinese

	\~japanese
	\~
	return values from synch class invocation.
	\todo you may change this type to what you want
	*/
	typedef unsigned int HlaResult;

	/** 
	\~korean

	\~english
	means, "synch class call successful" 
	
	\~chinese

	\~japanese
	\~
	means, "synch class call successful" 
	*/
	static const HlaResult HlaResult_Ok = 0;

}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
