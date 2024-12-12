//////////////////////////////////////////////////////////////////////////
/* Win32에서는 char = MBS, wchar_t = UTF16입니다. [1]
하지만 iOS, Android,Linux 등에서는 char = UTF8, wchar_t = UTF32입니다. [2]
이러한 점 때문에 [1]에서는 wchar_t를, [2]에서는 char을 선호합니다.
ProudNet은 [1] 플랫폼에서는 wchar_t를, [2] 플랫폼에서는 char를 사용하는 API를 제공합니다.
만약 당신의 프로그램이 [1]과 [2]에서 모두 정상적인 빌드가 되어야 한다면, 아래 정의된 것들을 사용하셔야 할 것입니다. */

#pragma once 

#ifdef _MSC_VER
#pragma pack(push,8)
#endif


#if defined(_WIN32)
#define _PNUNICODE
#endif //defined(_WIN32)

#define __PNSTR2WSTR(str)	L## str
#define _PNSTR2WSTR(str)	__PNSTR2WSTR(str)

// PNTCHAR=char로 빌드되더라도 이 define은 존재해야 한다.
// 주의: 가급적 이것을 쓰지 말 것. String 변환 때문에 느리다. 
#ifndef __FUNCTIONW__
#define __FUNCTIONW__		(StringA2W(__FUNCTION__).GetString())

//#define __FUNCTIONW__		_PNSTR2WSTR(__FUNCTION__) 
//  안타깝게도 PS4에서 윗줄이 L__FUNCTION__ 이 없다는 에러를 낸다. 따라서 이 줄을 쓴다. 성능 떨어지지만 어차피 디버그 용도인데.

#endif

#ifndef __FILEW__
//#define __FILEW__		_PNSTR2WSTR(__FILE__)
#define __FILEW__		(StringA2W(__FILE__).GetString())
#endif

// NOTE: 만약 위 define이 문제를 일으키면 최악의 경우 성능 포기하고 다음을 쓰자.
// iOS, Android, Linux에서 빌드 시 컴파일러에서 __FUNCTION__는 char 문자열로 치환 됩니다. 유니코드 기반으로 라이브러리를 빌드하게 되면 char -> wchar_t로 명시적으로 변환을 해주어야 합니다.

#if defined(_PNUNICODE) // NOTE: UE4의 경우 모바일에서도 PNTCHAR = wchar_t이다. 따라서 이것이 있어야 한다.

#define _PNT(x) L##x
#define Tprintf wprintf
#define Tsscanf swscanf
#define Tsprintf swprintf
#define Tstrlen wcslen
#define Tstrcmp wcscmp
#define Tstrncmp wcsncmp
#define Tstrcat wcscat
#define Tstricmp wcsicmp
#define Tstrcpy wcscpy
#define Tofstream ::std::wofstream 
#define Tstringstream ::std::wstringstream 
typedef wchar_t PNTCHAR;

// 주의: 가급적 이것을 쓰지 말 것. String 변환 때문에 느리다. 
#define __FUNCTIONT__ __FUNCTIONW__

// filesys.h 따라함
#ifndef TSTRING
#define TSTRING std::wstring
#endif // TSTRING

#else // linux, mac, ...

#define _PNT(x) x
#define Tprintf printf
#define Tsprintf sprintf
#define Tsscanf sscanf
#define Tstrlen strlen
#define Tstrcmp strcmp
#define Tstrncmp strncmp
#define Tstrcat strcat
#define Tstricmp stricmp
#define Tstrcpy strcpy
#define Tofstream std::ofstream
#define Tstringstream std::stringstream 
typedef char PNTCHAR;

#define __FUNCTIONT__ __FUNCTION__

// filesys.h 따라함
#ifndef TSTRING
#define TSTRING std::string
#endif

#endif
#ifdef _MSC_VER
#pragma pack(pop)
#endif