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

#include "Ptr.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud 
{
	// 다양한 함수 콜백에서 사용되는, 함수 인자 클래스.
	// 보통 함수 콜백으로 던져지는 "기타 등등 사용자 정의"값들의 객체가 된다.
	class CallbackContext
	{
	public:
		// NOTE: 이 클래스에는 함수 포인터가 들어가지 않는다. 
		// 이 클래스는 사용자가 어떤 인자를 가진 함수 포인터를 넣을지 모르기 때문에 여기에서 다루지 않는다.

		// 상속받아 쓰이는 경우를 위해 이것이 있어야 한다. 혹시 모르므로.
		virtual ~CallbackContext() {}
	};

	// 함수 호출 당시 인자로 들어간 콜백은 함수 리턴 후 콜백이 뒤늦게 호출되는 경우도 있을 수 있다.
	// 예: 비동기 실행 루틴.
	// 따라서 함수 호출 당시 들어간 콜백과 그것이 쓸 유저 데이터 객체는 보관되어야 한다.
	// (람다로 구현할 경우 이 안에 람다 인자와 함수로 구성된 람다 객체 1개가 들어갈 것이다.)
	// 스마트 포인터인 이유다.
	typedef RefCount<CallbackContext> CallbackContextPtr;
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
