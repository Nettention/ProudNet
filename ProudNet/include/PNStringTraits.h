#pragma once

namespace Proud
{
	/** \addtogroup util_group
	*  @{
	*/

	class AnsiStrTraits
	{
	public:
		typedef char CharType;

		PROUD_API static int SafeStringLen(const CharType* str);

		// ikpil.choi 2016-11-10 : memcpy_s 로 변경, destSize(2번째 인자) 값이 항상 올바른 값이여야 합니다.
		//static void CopyString(CharType* dest, const CharType* src,int length);
		// dest : 복사 받을 대상, destNumOfElements : 복사 받을 대상의 요소 갯수('\0' 자리 포함)
		PROUD_API static void CopyString(CharType* dest, int destNumOfElements, const CharType* src, int length);

		PROUD_API static int StringCompare(const CharType* a, const CharType*b);
        PROUD_API static int StringCompareNoCase(const CharType* a, const CharType*b);
		PROUD_API static int GetFormattedLength(const char* pszFormat, va_list args);
		PROUD_API static int __cdecl Format(char* pszBuffer, size_t nlength, const char* pszFormat, va_list args) throw();

		PROUD_API static char* StringUppercase(char* psz, size_t size) throw();
		PROUD_API static char* StringLowercase(char* psz, size_t size) throw();

		PROUD_API static const char* __cdecl StringFindString(const char* pszBlock, const char* pszMatch) throw();
		PROUD_API static char* __cdecl StringFindString(char* pszBlock, const char* pszMatch) throw();
		PROUD_API static const char* __cdecl StringFindChar(const char* pszBlock, char chMatch) throw();
		PROUD_API static int __cdecl StringSpanIncluding(const char* pszBlock, const char* pszSet) throw();
		PROUD_API static int __cdecl StringSpanExcluding(const char* pszBlock, const char* pszSet) throw();
		PROUD_API static char* __cdecl CharNext(const char* p) throw();
		PROUD_API static int __cdecl IsDigit(char ch) throw();
		PROUD_API static int __cdecl IsSpace(char ch) throw();

		template<typename PNSTRING>
		inline static void ToStdString(const PNSTRING& src, std::string& dest);
		template<typename PNSTRING>
		inline static void ToStdWString(const PNSTRING& src, std::wstring& dest);
		template<typename PNSTRING>
		inline static void FromStdString(const std::string& src, PNSTRING& dest);
		template<typename PNSTRING>
		inline static void FromStdWString(const std::wstring& src, PNSTRING& dest);
	};

	class UnicodeStrTraits
	{
	public:
		typedef wchar_t CharType;

		PROUD_API static int SafeStringLen(const CharType* str);

		// ikpil.choi 2016-11-10 : memcpy_s 로 변경, destSize(2번째 인자) 값이 항상 올바른 값이여야 합니다.
		//static void CopyString(CharType* dest, const CharType* src,int length);
		// dest : 복사 받을 대상, destNumOfElements : 복사 받을 대상의 요소 갯수('\0' 자리 포함)
		PROUD_API static void CopyString(CharType* dest, int destNumOfElements, const CharType* src, int length);

		PROUD_API static int StringCompare(const CharType* a, const CharType*b);
		PROUD_API static int StringCompareNoCase(const CharType* a, const CharType*b);
		PROUD_API static int GetFormattedLength(const wchar_t* pszFormat, va_list args);
		PROUD_API static int __cdecl Format(wchar_t* pszBuffer, size_t nLength, const wchar_t* pszFormat, va_list args) throw();

		// 해결 방법을 찾을때 까지 막아놓음.
#if defined(_WIN32) 
		PROUD_API static wchar_t* StringUppercase(wchar_t* psz, size_t size) throw();
		PROUD_API static wchar_t* StringLowercase(wchar_t* psz, size_t size) throw();
#endif
		PROUD_API static const wchar_t* __cdecl StringFindString(const wchar_t* pszBlock, const wchar_t* pszMatch) throw();
		PROUD_API static wchar_t* __cdecl StringFindString(wchar_t* pszBlock, const wchar_t* pszMatch) throw();
		PROUD_API static const wchar_t* __cdecl StringFindChar(const wchar_t* pszBlock, wchar_t chMatch) throw();
		PROUD_API static int __cdecl StringSpanIncluding(const wchar_t* pszBlock, const wchar_t* pszSet) throw();
		PROUD_API static int __cdecl StringSpanExcluding(const wchar_t* pszBlock, const wchar_t* pszSet) throw();
		PROUD_API static wchar_t* __cdecl CharNext(const wchar_t* psz) throw();
		PROUD_API static int __cdecl IsDigit(wchar_t ch) throw();
		PROUD_API static int __cdecl IsSpace(wchar_t ch) throw();

		template<typename PNSTRING>
		inline static void ToStdString(const PNSTRING& src, std::string& dest);
		template<typename PNSTRING>
		inline static void ToStdWString(const PNSTRING& src, std::wstring& dest);

		template<typename PNSTRING>
		inline static void FromStdString(const std::string& src, PNSTRING& dest);
		template<typename PNSTRING>
		inline static void FromStdWString(const std::wstring& src, PNSTRING& dest);
	};

}
