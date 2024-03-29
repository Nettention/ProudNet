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

namespace Proud
{
	template<typename XCHAR>
	uint32_t HashString(const XCHAR* str)
	{
		// #StringHash
		assert(str != NULL);
		uint32_t nHash = 0;
		const XCHAR* pch = str;
		while (true)
		{
			XCHAR ch;
#if defined(__ANDROID__) || defined(__arm__) // RVCT를 쓰는 경우 __packed keyword를 쓰면 되지만 우리는 __packed 를 못 쓰는 컴파일러(gcc or clang)을 쓰고 있다.
			memcpy(&ch, pch, sizeof(XCHAR));		// size값에 상수를 넣는다. 그리고 clang이 여기를 코드최적화를 제대로 해줘야 한다. 그래서 memcpy_s를 안쓴다.
#else
			ch = *pch;
#endif
			if (ch == 0)
				break;
			nHash = (nHash << 5) + nHash + ch;
			pch++;
		}

		return nHash;
	}
}