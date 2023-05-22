#pragma once 

/* The _Noreturn keyword of draft C1X.  */
// iconv 소스에서 발췌.
// http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1453.htm
#ifdef _MSC_VER
#  define _Noreturn __declspec (noreturn)
#else
#  define _Noreturn __attribute__ ((__noreturn__))
#endif


// #ifndef _Noreturn
// # if (3 <= __GNUC__ || (__GNUC__ == 2 && 8 <= __GNUC_MINOR__) \
// 	  || 0x5110 <= __SUNPRO_C)
// #  define _Noreturn __attribute__ ((__noreturn__))
// # elif 1200 <= _MSC_VER
// #  define _Noreturn __declspec (noreturn)
// # else
// #  define _Noreturn
// # endif
// #endif

#ifdef _MSC_VER
#define _Noinline __declspec(noinline)
#else
#define _Noinline __attribute__ ((noinline))
#endif

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/* C++ exception을 throw 하는 구문을 래핑한 함수들.
	
	Q: 왜 직접 throw를 안하고 함수로 래핑했나요?
	A: NDK, PS4, UE4 등 일부 상황에서는 throw keyword를 못 씁니다. 즉 exception handling을 금하는 컴파일러 설정을 씁니다.
	이러한 상황에서는 이렇게 함수로 래핑된 것을 쓸 수밖에 없습니다.
	한편, 그러한 곳에서는, 호출되는 함수X가 throw를 하면 안되지만, 호출되는 함수에서 호출하는 내부 함수들Y은 throw를 하더라도,
	X에서 catch를 해준다면, 문제 없습니다. 
	
	Q: exception.h에 있으면 될텐데요?
	A: exception.h는 pnstring.h를 include하는데, pnstring.h는 아래 함수들을 씁니다.
	*/
	 PROUD_API _Noreturn void ThrowInvalidArgumentException();
	 PROUD_API _Noreturn void ThrowArrayOutOfBoundException();
	 PROUD_API _Noreturn void ThrowBadAllocException();
	 PROUD_API _Noreturn void ThrowException(const char* text);
}
#ifdef _MSC_VER
#pragma pack(pop)
#endif
