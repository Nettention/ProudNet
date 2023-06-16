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

#include "PNString.h"
#if !defined(_WIN32)
#include "BasicTypes.h"
#endif
#include <time.h>

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

#ifdef _WIN32
// 윈도 버전이라도 #include <oledb.h>를 include할 수 없는 환경 때문에 전방 선언만 한다.
struct tagDBTIMESTAMP;
typedef tagDBTIMESTAMP DBTIMESTAMP;
#endif // _WIN32



// ikpil.choi 2017-02-17 : Visual Studio 2015 이상부터 timespec 지원, 그 이하는 지원 안하므로, 별도로 정의
#if defined(_WIN32) && !defined(_CRT_NO_TIME_T) && (defined(_MSC_VER) && 1900 > _MSC_VER)
struct timespec
{
	time_t tv_sec;  // Seconds - >= 0
	long   tv_nsec; // Nanoseconds - [0, 999999999]
};
#endif

namespace Proud
{
	// ikpil.choi 2017-02-17 : timespec_get 의 필요 define, 크로스 플랫폼 지원으로 선언되어 있지 않으면, 선언합니다.
#ifndef TIME_UTC
	enum
	{
		// ikpil.choi 2017-03-03 : 다른곳에서 TIME_UTC 를 재정의 할 수 있으므로, enum으로 정의
		TIME_UTC = 1
	};
#endif
	/**
	\~korean
	\brief localtime_s 를 크로스 플랫폼에서 지원하기 위한 함수.
	\param[in]  time tm으로 바꾸기 위한 utc 기준의 time_t 포인터 변수
	\param[out] t    tm 인자로 넣은 time을 tm으로 출력하는 변수

	\~english
	\brief

	\~chinese
	\brief

	\~japanese
	\brief

	\~
	*/
	 PROUD_API errno_t localtime_pn(const ::time_t* time, ::tm* t);

	/**
	\~korean
	\brief gmtime_s 를 크로스 플랫폼에서 지원하기 위한 함수.
	\param[in]  time tm으로 바꾸기 위한 utc 기준의 time_t 포인터 변수
	\param[out] t    tm 인자로 넣은 time을 tm으로 출력하는 변수

	\~english
	\brief

	\~chinese
	\brief

	\~japanese
	\brief

	\~
	*/
	 errno_t gmtime_pn(const ::time_t* time, ::tm* t);

	/**
	\~korean
	\brief timespec_get 을 크로스 플랫폼에서 지원하기 위한 함수.
	\param[out] tp   출력될 timespec 구조체
	\param[in]  base TIME_UTC 또는 타임베이스를 나타내는 다른 0이 아닌 정수 값 (현재는 TIME_UTC만 지원함)

	\~english
	\brief

	\~chinese
	\brief

	\~japanese
	\brief

	\~
	*/
	 PROUD_API int timespec_get_pn(::timespec *tp, int const base);

	/**
	\~korean
	\brief struct timespec을 나노세컨드로 바꾸는 함수
	\param[in] ts 나노세컨트로 변환시킬 struct timespec

	\~english
	\brief

	\~chinese
	\brief

	\~japanese
	\brief

	\~
	*/
	 PROUD_API long long timespec_to_nanosecond(const ::timespec& ts);

	/**
	\~korean
	\brief 나노세컨드를 struct timespec으로 바꾸는 함수
	\param[in]  ns struct timespec 으로 바꿀 나노세컨드
	\param[out] tp 출력될 struct timespec

	\~english
	\brief

	\~chinese
	\brief

	\~japanese
	\brief

	\~
	*/
	 PROUD_API int nanosecond_to_timespec(const long long& ns, timespec *tp);

	/**
	\~korean
	\brief struct timespec을 밀리세컨드로 바꾸는 함스
	\param[in] ts 밀리세컨드로 바꿀 struct timespec

	\~english
	\brief

	\~chinese
	\brief

	\~japanese
	\brief

	\~
	*/
	 PROUD_API long long timespec_to_millisecond(const ::timespec& ts);

	/**
	\~korean
	\brief 밀리세컨드를 struct timespec으로 바꾸는 함수
	\param[in] ts 밀리세컨드로 바꿀 struct timespec

	\~english
	\brief

	\~chinese
	\brief

	\~japanese
	\brief

	\~
	*/

	 PROUD_API int millisecond_to_timespec(const long long& ms, timespec *tp);

	/**
	\~korean
	\brief UTC 기준으로 현재 머신의 나노세컨드를 반환하는 함수

	\~english
	\brief

	\~chinese
	\brief

	\~japanese
	\brief

	\~
	*/
	 long long GetCurrentTimeNanoSecond();


}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
