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

#include "pndefine.h"
#if defined(_MSC_VER) && (_MSC_VER < 1600)

#include <yvals.h> // _C2라는 매크로 때문에

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef signed char int_least8_t;
typedef short int_least16_t;
typedef int int_least32_t;

typedef unsigned char uint_least8_t;
typedef unsigned short uint_least16_t;
typedef unsigned int uint_least32_t;

typedef char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;

typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;

#ifndef _INTPTR_T_DEFINED
#define _INTPTR_T_DEFINED
#ifdef _WIN64
typedef __int64 intptr_t;
#else /* _WIN64 */
typedef _W64 int intptr_t;
#endif /* _WIN64 */
#endif /* _INTPTR_T_DEFINED */

#ifndef _UINTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64 uintptr_t;
#else /* _WIN64 */
typedef _W64 unsigned int uintptr_t;
#endif /* _WIN64 */
#endif /* _UINTPTR_T_DEFINED */

typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

typedef __int64 int_least64_t;
typedef unsigned uint_least64_t;

typedef __int64 int_fast64_t;
typedef unsigned uint_fast64_t;

typedef __int64 intmax_t;
typedef unsigned uintmax_t;

/* LIMIT MACROS */
#define INT8_MIN	(-0x7f - _C2)
#define INT16_MIN	(-0x7fff - _C2)
#define INT32_MIN	(-0x7fffffff - _C2)

#define INT8_MAX	0x7f
#define INT16_MAX	0x7fff
#define INT32_MAX	0x7fffffff
#define UINT8_MAX	0xff
#define UINT16_MAX	0xffff
#define UINT32_MAX	0xffffffff

#define INT_LEAST8_MIN		(-0x7f - _C2)
#define INT_LEAST16_MIN		(-0x7fff - _C2)
#define INT_LEAST32_MIN		(-0x7fffffff - _C2)

#define INT_LEAST8_MAX		0x7f
#define INT_LEAST16_MAX		0x7fff
#define INT_LEAST32_MAX		0x7fffffff
#define UINT_LEAST8_MAX		0xff
#define UINT_LEAST16_MAX	0xffff
#define UINT_LEAST32_MAX	0xffffffff

#define INT_FAST8_MIN		(-0x7f - _C2)
#define INT_FAST16_MIN		(-0x7fff - _C2)
#define INT_FAST32_MIN		(-0x7fffffff - _C2)

#define INT_FAST8_MAX		0x7f
#define INT_FAST16_MAX		0x7fff
#define INT_FAST32_MAX		0x7fffffff
#define UINT_FAST8_MAX		0xff
#define UINT_FAST16_MAX		0xffff
#define UINT_FAST32_MAX		0xffffffff

#if _INTPTR == 0 || _INTPTR == 1
#define INTPTR_MAX			0x7fffffff
#define INTPTR_MIN			(-INTPTR_MAX - _C2)
#define UINTPTR_MAX			0xffffffff

#else /* _INTPTR == 2 */
#define INTPTR_MIN			(-_LLONG_MAX - _C2)
#define INTPTR_MAX			_LLONG_MAX
#define UINTPTR_MAX			_ULLONG_MAX
#endif /* _INTPTR */

#define INT8_C(x)	(x)
#define INT16_C(x)	(x)
#define INT32_C(x)	((x) + (INT32_MAX - INT32_MAX))

#define UINT8_C(x)	(x)
#define UINT16_C(x)	(x)
#define UINT32_C(x)	((x) + (UINT32_MAX - UINT32_MAX))

#ifdef _WIN64
#define PTRDIFF_MIN		INT64_MIN
#define PTRDIFF_MAX		INT64_MAX
#else /* _WIN64 */
#define PTRDIFF_MIN		INT32_MIN
#define PTRDIFF_MAX		INT32_MAX
#endif /* _WIN64 */

#define SIG_ATOMIC_MIN	INT32_MIN
#define SIG_ATOMIC_MAX	INT32_MAX

#ifndef SIZE_MAX
#ifdef _WIN64
#define SIZE_MAX		UINT64_MAX
#else /* _WIN64 */
#define SIZE_MAX		UINT32_MAX
#endif /* _WIN64 */
#endif /* SIZE_MAX */

#define WINT_MIN	0x0000
#define WINT_MAX	0xffff

#define INT64_MIN	(-0x7fffffffffffffff - _C2)
#define INT64_MAX	0x7fffffffffffffff
#define UINT64_MAX	0xffffffffffffffffU

#define INT_LEAST64_MIN	(-0x7fffffffffffffff - _C2)
#define INT_LEAST64_MAX	0x7fffffffffffffff
#define UINT_LEAST64_MAX	0xffffffffffffffffU

#define INT_FAST64_MIN		(-0x7fffffffffffffff - _C2)
#define INT_FAST64_MAX		0x7fffffffffffffff
#define UINT_FAST64_MAX	0xffffffffffffffffU

#define INTMAX_MIN		(-0x7fffffffffffffff - _C2)
#define INTMAX_MAX		0x7fffffffffffffff
#define UINTMAX_MAX	0xffffffffffffffffU

#define INT64_C(x)		((x) + (INT64_MAX - INT64_MAX))
#define UINT64_C(x)		((x) + (UINT64_MAX - UINT64_MAX))
#define INTMAX_C(x)		INT64_C(x)
#define UINTMAX_C(x)	UINT64_C(x)

#else
#include <stdint.h>
#endif