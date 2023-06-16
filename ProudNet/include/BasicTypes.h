/* 106 228 203 199 203 251 69 164 171 249 223 147 102 233 72 249 25 46 47 89 4 75 196 233 166 154 197 5 225 93 96 137 109 76  */
/*
ProudNet 1.8.00002-master


이 프로그램의 저작권은 넷텐션에게 있습니다.
이 프로그램의 수정, 사용, 배포에 관련된 사항은 본 프로그램의 소유권자와의 계약을 따르며,
계약을 준수하지 않는 경우 원칙적으로 무단 사용을 금지합니다.
무단 사용에 의한 책임은 본 프로그램의 소유권자와의 계약서에 명시되어 있습니다.

** 주의: 저작물에 관한 위의 명시를 제거하지 마십시오.


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

// 사용자가 프라우드넷 같은 라이브러리의 헤더파일과 LIB가 서로 버전이 안맞게 사용하면 찾기 힘든 런타임오류가 된다. 
// 이를 링크오류로 끌어올리는 역할을 한다.
#ifdef _DEBUG
#define PROUDNET_H_LIB_SIGNATURE VER_95919510_DEBUG
#else
#define PROUDNET_H_LIB_SIGNATURE VER_95919510_RELEASE 
#endif

#include <assert.h>

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

#if !defined(_WIN32)
#include <sys/types.h>
#include <limits.h>
#include <string.h>
#endif

// fd_set does not name a type 컴파일 에러 해결 -> 헤더 파일 include, __linux__ 버전
#ifdef __linux__
#include <sys/select.h>
#endif

#include <new> // for placement new

#include "pnstdint.h"
#include "HashString.h"
#include "PNElementTraits.h"

#define PNMIN(a,b)            (((a) < (b)) ? (a) : (b))
#define PNMAX(a,b)            (((a) > (b)) ? (a) : (b))

#define PN_INFINITE 0xFFFFFFFF

#if defined(WCHAR_MAX) && (WCHAR_MAX > UINT16_MAX)
#	define WCHAR_LENGTH 4
#else
#	if defined(_WIN32) || defined(__ORBIS__) // NOTE: ORBIS는 타겟 머신이 unix 기반인데도 크기2다.
#		define WCHAR_LENGTH 2
#	else
#		define WCHAR_LENGTH 4
#	endif
#endif

#include "pndefine.h"

#if !defined(_MSC_VER) || (_MSC_VER < 1300)
#define __noop (void)0 		// __noop는 msvc 1300 이상 전용 키워드다.
#endif


// C4100 warning을 막고 싶다면 쓰자.
#define _pn_unused(x) ((void)(x))



#if !defined(_WIN32)

// NOTE: DWORD, UINT_PTR 같은 것들은 uint32_t or uintptr_t 등 stdint에 정의된 타입으로 쓸 것이고, 윈도용으로 따로 재정의하지 말자.
// 찾기 힘든 버그 나와서 개고생했음.

typedef int SOCKET;
typedef fd_set FD_SET;

#define WINAPI __stdcall
#define _HEAPOK (-2)

//#define _Noreturn
#define __cdecl
#define __forceinline inline

#define LOWORD(l) ((uint16_t)(((uint32_t)(l)) & 0xffff))
#define HIWORD(l) ((uint16_t)((((uint32_t)(l)) >> 16) & 0xffff))
#define LOBYTE(w) ((uint8_t)(((uint32_t)(w)) & 0xff))
#define HIBYTE(w) ((uint8_t)((((uint32_t)(w)) >> 8) & 0xff))
#define MAKEWORD(a, b) ((uint16_t)(((uint8_t)(((uint32_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint32_t)(b)) & 0xff))) << 8))
#define MAKELONG(a, b) ((long)(((uint16_t)(((uint32_t)(a)) & 0xffff)) | ((uint32_t)((uint16_t)(((uint32_t)(b)) & 0xffff))) << 16))

#endif // non win32 platform

#if (defined(_MSC_VER) && _MSC_VER>=1600) || (__cplusplus > 199711L)
// NOTE: IoT 툴체인에서는 C++11이 지원안되는 경우가 있다. 따라서 클라 모듈에서는 C++11 기능을 가급적 피해야.
#define SUPPORTS_LAMBDA_EXPRESSION
#endif

#if (defined(_MSC_VER) && _MSC_VER>=1800) || (__cplusplus > 199711L)
//NOTE: VS2013부터 가변템플릿 인자를 지원합니다.
#define SUPPORTS_CPP11
#endif

// WebGL 지원
#if defined(SUPPORTS_CPP11) && defined(USE_OPENSSL) && (defined(_WIN32) || defined(__linux__))
#define SUPPORTS_WEBGL
#endif

#ifdef SUPPORTS_CPP11
#define MOVE_OR_COPY(arg) std::move(arg)
#else
#define MOVE_OR_COPY
#endif// SUPPORTS_CPP11

#if ((defined(_MSC_VER) && _MSC_VER > 1700) || (__cplusplus > 199711L)) // VS2012 이상부터 final keyword를 지원함. C++11을 지원하면 NDK 버전 상관없이 아래 키워드가 사용 가능함.
#define PN_OVERRIDE override
#define PN_FINAL final
#define PN_SEALED final // sealed는 VC++ specific이며, C++11은 final을 채택.
#else // old compilers
#define PN_OVERRIDE
#define PN_FINAL
#define PN_SEALED
#endif

// 여기에 sstream을 포함 안시키면 AtlDependent.inl에서 ::std::wstringstream을 못 찾는 경우가 있다.
#include <sstream>
#include "pntchar.h"

template< typename T >
void CallConstructor(T* pInstance)
{
#pragma push_macro("new")
#undef new

	::new(pInstance)T();

#pragma pop_macro("new")
};

template< typename T, typename Src >
void CallConstructor(T* pInstance, const Src& src)
{
#pragma push_macro("new")
#undef new

	::new(pInstance)T(src);

#pragma pop_macro("new")
};

// 주의: 반드시!! 함수 사용시 <T>를 명료하게 붙일 것. 일부 컴파일러는 엉뚱한 것을 템플릿 인스턴스화한다.
template< typename T, typename Src, typename Src1 >
void CallConstructor(T* pInstance, const Src& src, const Src1 src1)
{
#pragma push_macro("new")
#undef new

	::new(pInstance)T(src, src1);

#pragma pop_macro("new")
};

// 주의: 반드시!! 함수 사용시 <T>를 명료하게 붙일 것. 일부 컴파일러는 엉뚱한 것을 템플릿 인스턴스화한다.
template< typename T >
void CallDestructor(T* pInstance)
{
	pInstance->T::~T();
};

#pragma push_macro("new")
#undef new

// 주의: 반드시!! 함수 사용시 <T>를 명료하게 붙일 것. 일부 컴파일러는 엉뚱한 것을 템플릿 인스턴스화한다.
template< typename T >
static inline void CallConstructors(T* pElements, intptr_t nElements)
{
	int iElement = 0;

	//	try
	//	{
	for (iElement = 0; iElement < nElements; iElement++)
	{
		::new(pElements + iElement) T;

	}
	//	}
	// 	catch(...)
	// 	{
	// 		while( iElement > 0 )
	// 		{
	// 			iElement--;
	// 			pElements[iElement].~T();
	// 		}
	//
	// 		throw;
	// 	}
}

// 주의: 반드시!! 함수 사용시 <T>를 명료하게 붙일 것. 일부 컴파일러는 엉뚱한 것을 템플릿 인스턴스화한다.
template< typename T >
static inline void CallCopyConstructors(T* pElements, const T* pOldElements, intptr_t nElements)
{
	int iElement = 0;

	for (iElement = 0; iElement < nElements; iElement++)
	{
		::new(pElements + iElement) T(pOldElements[iElement]);
	}
}


// 주의: 반드시!! 함수 사용시 <T>를 명료하게 붙일 것. 일부 컴파일러는 엉뚱한 것을 템플릿 인스턴스화한다.
template< typename T >
static inline void CallDestructors(T* pElements, intptr_t nElements) throw()
{
	(void)pElements;

	for (int iElement = 0; iElement < nElements; iElement++)
	{
		pElements[iElement].~T();
	}
}
#pragma pop_macro("new")


// ProudNet DLL 프로젝트 안에서는...
#ifdef PROUDNETCLIENT_EXPORTS
#define PROUD_API __declspec(dllexport)
#endif

#ifdef PROUDNETSERVER_EXPORTS
#define PROUDSRV_API __declspec(dllexport)
#endif


// 사용자가 ProudNet DLL을 쓸 때는 이걸 켜야 한다.
#ifdef USE_PROUDNET_DLL 
#ifndef PROUD_API
#define PROUD_API __declspec(dllimport)
#endif
#ifndef PROUDSRV_API
#define PROUDSRV_API __declspec(dllimport)
#endif
#else
#ifndef PROUD_API
#	define PROUD_API
#endif
#ifndef PROUDSRV_API
#	define PROUDSRV_API
#endif
#endif



//#pragma pack(push,8)

#include "throw.h"

namespace Proud
{
	PROUD_API _Noreturn void ThrowInt32OutOfRangeException(const char* where);
	PROUD_API extern volatile int PROUDNET_H_LIB_SIGNATURE;

#ifdef _W64
	inline void BoundCheckInt32(int64_t v, const char* where)
	{
		if (v < INT64_MIN || v > INT64_MAX)
			ThrowInt32OutOfRangeException(where);
	}

	inline void BoundCheckUInt32(uint64_t v, const char* where)
	{
		if (v > UINT64_MAX)
			ThrowInt32OutOfRangeException(where);
	}
#else
#	define BoundCheckInt32 __noop
#endif


	struct __Position {};

	typedef __Position *Position;
}


#define PN_LODWORD(QWordValue) ((uint32_t)(QWordValue))
#define PN_HIDWORD(QWordValue) ((uint32_t)(((QWordValue) >> 32) & 0xffffffff))

#include "atomic.h"

#include "SafeMemory.h"


#if !defined(_MSC_VER)
// SAL을 지원하지 않는 타 컴파일러를 위해.
// 주의: _WIN32를 검사하지 말 것. msvc spec이지 win32 spec이 아니다.
// 주의: _MSC_VER를 검사할 것. clang 등 비 msvc는 SAL이 없어서다.
// 프넷1은 이미 VS2010 이하는 미지원. SAL은 VS2010부터 지원. 따라서 msvc 버전검사 불필요.
#define _In_
#define _Out_
#define _InOut_
#define _Outptr_
#define _Check_return_
#define _Guarded_by_(x) 
#else
#include <sal.h>
#endif


// 맨위 pack(push,8)의 반대처리
#ifdef _MSC_VER
#pragma pack(pop)
#endif
