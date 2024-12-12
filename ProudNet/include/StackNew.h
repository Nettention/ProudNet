#pragma once 

#ifndef _WIN32
//#include <alloca.h>
#endif

/* 주의: _MALLOCA, _FREEA에 대해
이 함수들은 스택에 충분한 공간이 있으면 스택에 할당을, 그렇지 않으면 heap에 할당을 한다.
즉 매우 작은 크기가 아닌 이상 heap에 할당하는 역할을 한다. 즉 실행 실패보다는 성능이 저하되더라도 heap을 액세스하는 것이다.
만약 당신이 할당하고자 하는 크기가 큰 경우가 흔한 경우 이 함수는 항상 heap을 액세스할 것이다. 즉 성능의 이익을 얻지 못한다.
이러한 경우에는 _MALLOCA 대신 object pooling을 하라. 가령 Proud.CPooledArrayObjectAsLocalVar를 쓰는 것이 훨씬 낫다.

*/

/* _malloca가 작은 크기조차 heap access를 해버린다. 그래서 win32에서 성능을 크게 낭비한다. 
차라리 TLS 기반 obj-pool이 빠름. 아무튼 웬만하면 그냥 _alloca를 쓰지, _malloc는 피하자. 
물론 매우 작은 크기에 한해서 써야 하겠고.
*/
#if 0 

#if !defined(_WIN32)
#define _MALLOCA alloca
#define _FREEA(...) 
#else
#if (defined(_MSC_VER) && _MSC_VER>=1400)
#define _MALLOCA _malloca
#define _FREEA _freea
#else
#define _MALLOCA _alloca
#define _FREEA __noop
#endif
#endif

#else  // 0

#	ifndef _WIN32
#		define _MALLOCA alloca
#		define _FREEA(...) 
#	else 
#		define _MALLOCA _alloca
#		define _FREEA 
#	endif

#endif  // 1

/* 스택에 C++ 객체를 할당/해제하는 매크로.

주의사항:
스택에 너무 큰 크기를 할당하지 말 것. 스택이라는 것 자체가 용량 모자라면 답 없는 환경이기 때문이다.
어차피 win32 _malloca에서도 1KB 이상의 그냥 heap 할당해 버린다. 
1KB 미만의 작은 크기가 아니라면 NEW_ON_STACK이나 _MALLOCA 대신 POOLED_LOCAL_VAR를 쓸 것.

사용예:
int arrayLength;
NEW_ON_STACK(array, MyType, arrayLength);

...

FREE_ON_STACK(array);


*/

#define NEW_ON_STACK(VARNAME, TYPE, LENGTH) \
	TYPE* VARNAME = (TYPE*)_MALLOCA(sizeof(TYPE) * LENGTH); \
	CallConstructors(VARNAME, LENGTH); 

#define FREE_ON_STACK(VARNAME, LENGTH) \
	CallDestructors(VARNAME, LENGTH); \
	_FREEA(VARNAME);

