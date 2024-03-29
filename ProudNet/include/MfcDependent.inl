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

#include <afx.h>

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

/** 이 모듈은 MFC에 의존적인 심볼들을 포함한다.
- __forceinline으로 선언된 이유는 compile time에서 코드를 생성하되 MFC 사용 여부에 따라 실 인스턴스 타입이
달라지는 클래스(CString 등)이 ProudNet과 충돌하지 않게 하기 위함이다.
물론, ProudNet에는 아래 심볼들이 컴파일되어 들어가있지 않다. */
namespace Proud
{
	// #TODO-MFC 여기다 CArray, CMap의 operator<<,>>,AppendTextOut을 구현하세요.
	// stlmarshal.inl에 있는 std.vector, std.map을 복붙해서 고쳐만드세요.
	// size()대신 GetCount가 쓰이고 size_t대신 intptr_t 혹은 int를 쓰세요. 어떤걸 써야 하는지는 CAtlArray등의 구현부를 참고해서 하시고요.
	// 고객사 이슈가 발생하면 다룹시다. 

}

#ifdef _MSC_VER
#pragma pack(pop)
#endif