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
#include "HostIDArray.h"
#include "Ptr.h"

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
	/** \addtogroup net_group
	*  @{
	*/

	class CP2PGroup;
	typedef RefCount<CP2PGroup> CP2PGroupPtr;

	/** 
	\~korean
	P2P 그룹 정보 구조체 

	\~english
	Information construct of P2P group

	\~chinese
	P2P组信息构造体

	\~japanese
	P2Pグループ情報の構造体
	\~
	*/
	class CP2PGroup
	{
	public:
		/** 
		\~korean
		그룹 ID 

		\~english
		Group ID 

		\~chinese
		组ID

		\~japanese
		グループID
		\~
		*/
		HostID m_groupHostID;
		/** 
		\~korean
		그룹에 소속된 client peer들의 HostID 

		\~english
		HostID of client peer that is possessed by group

		\~chinese
		所属于组的client peer的Host ID。

		\~japanese
		グループに所属されたclient peerなどのHostID
		\~
		*/
		HostIDArray m_members;

		 PROUD_API CP2PGroup();

#ifdef _WIN32
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif
	};
	/** 
	\~korean
	P2P 그룹 리스트
	- 보다 자세한 사용법은 베이스 클래스를 참고할 것. 

	\~english
	 P2P group list
	- Please refer base class for more details.

	\~chinese
	P2P组list。
	- 更详细的使用方法请参考base类。

	\~japanese
	P2Pグループリスト
	- より詳しい使用方法はベースクラスを参照すること。
	\~
	*/
	class CP2PGroups : public CFastMap<HostID, CP2PGroupPtr>
	{
	public:
#ifndef SWIG
#pragma push_macro("new")
#undef new
		// 이 클래스는 ProudNet DLL 경우를 위해 커스텀 할당자를 쓰되 fast heap을 쓰지 않는다.
		DECLARE_NEW_AND_DELETE
#pragma pop_macro("new")
#endif //SWIG 
	};

	/**
	\~korean
	\brief <a target="_blank" href="http://guide.nettention.com/cpp_ko#p2p_group" >P2P 그룹</a> 한 개의 사용자 지정 설정입니다. 
	필요한 경우가 아닌 이상 이 객체를 사용하실 필요는 없습니다.

	\~english 
	Additional settings for a P2P group. 
	Unless necessary, you don’t have to use this object. 

	\~chinese
	\brief <a target="_blank" href="http://guide.nettention.com/cpp_zh#p2p_group" >P2P 组</a>%一个用户指定设置。
        如果不是必要的情况，不需要使用此对象。

	\~japanese
	\brief \ref p2p_group  1個のユーザー指定設定です。 
	必要な場合ではない以上、このオブジェクトを使う必要はありません。
	\~
	*/
	class CP2PGroupOption
	{
	public:
		/**
		\~korean
		true이면 그룹 멤버간 Direct P2P 통신을 제공합니다. 기본값은 true입니다.
		그룹을 맺되 멤버간 P2P 통신을 차단하고자 할 때 이 값을 false로 설정하면 됩니다. 
		
		그룹 내 멤버간 이미 타 그룹 종속에 의해 서로 Direct P2P를 이미 하고 있는 상태인 경우, 이 값을 false로 설정하는 것이 그들의 Direct P2P를 차단하지는 않습니다.

		\~english TODO:translate needed.
		If true, this provides direct P2P communication among group members. The default value is true. 
		You can set this value as false when you form a group but want to block P2P communication among members. 

		When direct P2P is already being done due to being subordinated by another group among the members in the group, selecting this value as false does not block the direct P2P among them. 

		\~chinese
		True的话提供组成员之间Direct P2P通信。默认值是true。
		形成组，但是想切断成员之间P2P通信的时候把此值设置成false。

		组内成员之间已经被其他附属组相互进行Direct P2P时候，把此值设置成false不会切断他们的Direct P2P。

		\~japanese
		Trueであればグループメンバー間のDirect P2P通信を提供します。基本値はtrueです。
		グループを結ぶがメンバー間でのP2P通信を遮断したい時はこの値をfalseに設定してください。

		グループ内でのメンバー間において、既に他のグループの従属によってお互いDirect P2Pを既にやっている状態の場合、この値をfalseに設定することがそれらのDirect P2Pを遮断することはありません。
		\~
	*/
		bool m_enableDirectP2P;

		PROUD_API CP2PGroupOption();		

		PROUD_API static  CP2PGroupOption Default;
	};

	/**  @} */
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
