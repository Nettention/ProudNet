/*
ProudNet

이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의: 저작물에 관한 위의 명시를 제거하지 마십시오.

ProudNet

This program is soley copyrighted by Nettention.
Any use, correction, and distribution of this program are subject to the terms and conditions of the License Agreement.
Any violated use of this program is prohibited and will be cause of immediate termination according to the License Agreement.

** WARNING : PLEASE DO NOT REMOVE THE LEGAL NOTICE ABOVE.

ProudNet

此程序的版权归Nettention公司所有。
与此程序的修改、使用、发布相关的事项要遵守此程序的所有权者的协议。
不遵守协议时要原则性的禁止擅自使用。
擅自使用的责任明示在与此程序所有权者的合同书里。

** 注意：不要移除关于制作物的上述明示。

ProudNet
このプログラムの著作権はNettentionにあります。
このプログラムの修正、使用、配布に関する事項は本プログラムの所有権者との契約に従い、
契約を遵守しない場合、原則的に無断使用を禁じます。
無断使用による責任は本プログラムの所有権者との契約書に明示されています。

**注意　：著作物に関する上記の明示を除去しないでください。

*/

#pragma once

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

#include <cstdint>

namespace Proud
{
#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif

	/*
	\~korean
	StartRoundTripLatencyTest의 인풋 파라미터로 쓰이는 구조체이다.

	\~english
	ToDo

	\~chinese
	ToDo

	\~japanese
	ToDo

	\~
	*/
	struct StartRoundTripLatencyTestParameter
	{
		/*
		\~korean
		RoundTripLatencyTest 도중에 언제까지 StopRoundTripLatencyTest가 호출되기를 기다릴 것인지 지정하는 변수이다.
		단위는 밀리초입니다.
		디폴트 값으로 5000 ms를 가진다.

		\~english
		ToDo

		\~chinese
		ToDo

		\~japanese
		ToDo

		\~
		*/
		int32_t testDurationMs;
		/*
		\~korean
		RoundTripLatencyTest 도중에 핑을 보내는 주기를 지정하는 변수이다.
		단위는 밀리초입니다.
		디폴트 값으로 300 ms를 가진다.

		\~english
		ToDo

		\~chinese
		ToDo

		\~japanese
		ToDo

		\~
		*/
		int32_t pingIntervalMs;

		inline StartRoundTripLatencyTestParameter()
		{
			testDurationMs = 5000;
			pingIntervalMs = 300;
		}
	};

	/*
	\~korean
	GetRoundTripLatency의 아웃풋 파라미터로 쓰이는 구조체이다.
	
	\~english
	ToDo

	\~chinese
	ToDo
	
	\~japanese
	ToDo
	
	\~
	*/
	struct RoundTripLatencyTestResult
	{
		/*
		\~korean
		RoundTripLatencyTest로 측정된 라운드트립 레이턴시 평균입니다.
		단위는 밀리초입니다.

		\~english
		ToDo

		\~chinese
		ToDo

		\~japanese
		ToDo

		\~
		*/
		int32_t latencyMs;

		/*
		\~korean
		RoundTripLatencyTest로 측정된 라운드트립 레이턴시 표준편차입니다.
		단위는 밀리초입니다.
		\~english
		ToDo

		\~chinese
		ToDo

		\~japanese
		ToDo

		\~
		*/
		int64_t standardDeviationMs;

		/*
		\~korean
		라운드트립 레이턴시 측정을 위해 실행된 핑퐁 횟수입니다.

		\~english
		ToDo

		\~chinese
		ToDo

		\~japanese
		ToDo

		\~
		*/
		int64_t totalTestCount;
	};

#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif
}


#ifdef _MSC_VER
#pragma pack(pop)
#endif
