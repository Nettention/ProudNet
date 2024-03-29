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

#include "Marshaler.h"

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

	/** 
	\~korean
	DB cache server에서 유저가 정의한 Table 에 실제로 기록할 사건의 종류

	\~english
	Sort of event that actually writting to user defined table in DB cache server

	\~chinese
	在DB cache server上用户定义的Table实际要记录的事件的种类。

	\~japanese
	DB cache serverでユーザーが定義したTableに実際に記録する事件の種類
	\~
	*/
	enum DbmsWritePropNodePendType
	{
		/**
		\~korean
		없음 
		
		\~english
		None 
		
		\~chinese
		无

		\~japanese
		なし
		\~
		*/
		DWPNPT_None,

		/**
		\~korean
		\ref PropNode  추가 

		\~english
		Add \ref PropNode 

		\~chinese
		添加\ref PropNode%。

		\~japanese
		\ref PropNode  追加
		\~
		*/
		DWPNPT_AddPropNode,

		/** 
		\~korean
		\ref PropNode  삭제 

		\~english
		Delete \ref PropNode 
		
		\~chinese
		删除\ref PropNode%。

		\~japanese
		\ref PropNode  削除
		\~
		*/
		DWPNPT_RemovePropNode,

		/**
		\~korean
		Request 에 대한 \ref PropNode 업데이트 
		
		\~english TODO:Translate needed.
		Update \ref PropNode.

		\~chinese TODO:Translate needed.
		升级\ref PropNode%。

		\~japanese
		Request に対する \ref PropNode アップデート
		\~
		*/
		DWPNPT_RequestUpdatePropNode,

		/**
		\~
		\ref PropNode  SetValueIf
		*/
		DWPNPT_SetValueIf,

		/**
		\~
		\ref PropNode  ModifyValueIf
		*/
		DWPNPT_ModifyValue,

		/** 
		\~korean 
		Request 에 대한 \ref PropNode 업데이트 
		
		\~english TODO:Translate needed.
		Update \ref PropNod.
		
		\~chinese TODO:Translate needed.
		升级\ref PropNode%。

		\~japanese
		Request 에 대한 \ref PropNode 업데이트 
		\~
		*/
		DWPNPT_RequestUpdatePropNodeList,

		/** 
		\~korean
		\ref PropNode   이동 

		\~english 
		Update \ref PropNod 
		
		\~chinese
		\ref PropNode%移动。
		\~japanese
		\ref PropNode   移動
		\~
		*/
		DWPNPT_MovePropNode,

		/**
		\~korean
		\~english
		\~chinese
		\~japanese
		\~
		Unilateral Update \ref PropNod.
		*/
		DWPNPT_UnilateralUpdatePropNode,

		/**
		\~korean
		\~english
		\~chinese
		\~japanese
		\~
		Unilateral Update \ref PropNod.
		*/
		DWPNPT_UnilateralUpdatePropNodeList,

		/**
		\~korean
		\ref PropNode 업데이트

		\~english
		Update \ref PropNode

		\~chinese
		升级\ref PropNode%。

		\~japanese
		\ref PropNode アップデート
		\~
		*/
		DWPNPT_UpdatePropNode,
	};

	PROUDSRV_API const PNTCHAR* TypeToString(DbmsWritePropNodePendType t);

	/**  @}  */

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}

PROUDNET_SERIALIZE_ENUM(DbmsWritePropNodePendType);

#ifdef _MSC_VER
#pragma pack(pop)
#endif
