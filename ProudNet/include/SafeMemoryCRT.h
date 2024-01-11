#pragma once

#ifdef _WIN32
#error This header file is NOT for Win32 platform
#endif

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

namespace Proud
{
#ifndef __ORBIS__ // PS4 플랫폼은 해당 플랫폼이 지원하는 memcpy_s를 사용하도록 함.
	// ikpil.choi, 2016-12-28 : memcpy_s 안전성 보강
	inline errno_t memcpy_s(void *dest, size_t dest_size, const void* src, size_t src_size)
	{
		errno_t err = 0;

		// 복사 대상/소스가 널일 경우
		if (0 == dest || 0 == src)
			return EINVAL;

		// 복사 대상의 최대값은 size_t / 2 사이즈
		const size_t max_value = (SIZE_MAX >> 1);

		// 대상의 사이즈가 최대값을 넘었다면, 대상의 사이즈가 잘못 된것으로 판단, 할일이 없음
		if (dest_size > max_value)
			return E2BIG;

		// 소스의 사이즈가 최대값보다 크다면, 너무 큰 값이지만 대상 사이즈로 조정
		if (src_size > max_value)
		{
			err = E2BIG;
			src_size = dest_size;
		}

		// 소스 사이즈가 대상 사이즈보다 크다면, 오버플로우 셋팅 후 대상 사이즈로 조정
		if (src_size > dest_size)
		{
			err = EOVERFLOW;
			src_size = dest_size;
		}

		memcpy(dest, src, src_size);
		return err;
	}
#endif


	template <size_t _Size>
	inline int sprintf_s(char(&_Dst)[_Size], const char *_TArg1, ...)
	{
		va_list _ArgList;
		va_start(_ArgList, _TArg1);

		int ret = vsnprintf(_Dst, _Size, _TArg1, _ArgList);

		va_end(_ArgList);
		return ret;
	}

	inline int sprintf_s(char *_Dst, size_t _Size, const char *_TArg1, ...)
	{
		va_list _ArgList;
		va_start(_ArgList, _TArg1);

		int ret = vsnprintf(_Dst, _Size, _TArg1, _ArgList);

		va_end(_ArgList);
		return ret;
	}

	template <size_t _Size>
	inline int _snscanf_s(char (&_Dst)[_Size], const char *_TArg1, ...)
	{
		va_list _ArgList;
		va_start(_ArgList, _TArg1);
		return vsscanf(_Dst, _TArg1, _ArgList);
	}

	inline int _snscanf_s(char *_Dst, size_t _Size, const char *_TArg1, ...)
	{
		va_list _ArgList;
		va_start(_ArgList, _TArg1);

		int ret = vsscanf(_Dst, _TArg1, _ArgList);

		va_end(_ArgList);
		return ret;
	}

	template <size_t _Size>
	inline errno_t strcpy_s(char(&_Dst)[_Size], const char *_Src)
	{
		strncpy(_Dst, _Src, _Size);
		// 입력받은 버퍼의 맨 끝 데이터에 일부러 표식을 내고, 잠재 에러를 조기 발견한다.
		// 더 나은 것은 sigabort등을 내는 것이지만 일단은 이렇게 하자.
		// 아래 함수들도 마찬가지.
		_Dst[_Size - 1] = '\0';
		return 0;
	}

	inline errno_t strcpy_s(char *_Dst, size_t _Size, const char *_Src)
	{
		strncpy(_Dst, _Src, _Size);
		_Dst[_Size - 1] = '\0';
		return 0;
	}

	template <size_t _Size>
	inline errno_t strncpy_s(char(&_Dst)[_Size], const char *_Src, size_t _MaxCount)
	{
		strncpy(_Dst, _Src, PNMIN(_Size, _MaxCount));
		_Dst[_Size - 1] = '\0';
		return 0;
	}

	inline errno_t strncpy_s(char *_Dst, size_t _Size, const char *_Src, size_t _MaxCount)
	{
		strncpy(_Dst, _Src, PNMIN(_Size, _MaxCount));
		_Dst[_Size - 1] = '\0';
		return 0;
	}


	template <size_t _Size>
	inline errno_t wcscpy_s(wchar_t(&_Dst)[_Size], const wchar_t *_Src)
	{
		wcsncpy(_Dst, _Src, _Size);
		_Dst[_Size - 1] = '\0';
		return 0;
	}

	inline errno_t wcscpy_s(wchar_t *_Dst, size_t _Size, const wchar_t *_Src)
	{
		wcsncpy(_Dst, _Src, _Size);
		_Dst[_Size - 1] = '\0';
		return 0;
	}

	template <size_t _Size>
	inline errno_t wcsncpy_s(wchar_t(&_Dst)[_Size], const wchar_t *_Src, size_t _MaxCount)
	{
		wcsncpy(_Dst, _Src, PNMIN(_Size, _MaxCount));
		_Dst[_Size - 1] = '\0';
		return 0;
	}

	inline errno_t wcsncpy_s(wchar_t *_Dst, size_t _Size, const wchar_t *_Src, size_t _MaxCount)
	{
		wcsncpy(_Dst, _Src, PNMIN(_Size, _MaxCount));
		_Dst[_Size - 1] = '\0';
		return 0;
	}

	inline int vprintf_s(char const* const _Format, va_list _ArgList)
	{
		return vprintf(_Format, _ArgList);
	}

	inline int vwprintf_s(wchar_t const* const _Format, va_list _ArgList)
	{
		return vwprintf(_Format, _ArgList);
	}



	template <size_t _Size>
	inline errno_t _tcscpy_s(wchar_t(&_Dst)[_Size], const wchar_t *_Src)
	{
		return wcscpy_s(_Dst, _Src);
	}

	template <size_t _Size>
	inline errno_t _tcscpy_s(char(&_Dst)[_Size], const char*_Src)
	{
		return strcpy_s(_Dst, _Src);
	}

	// ikpil.choi 2017-02-20 : 크로스 플랫폼을 위한 wcsftime 대응 함수
	inline size_t _tcsftime(wchar_t *_Buf, size_t _SizeInWords, const wchar_t * _Format, const struct tm * _Tm)
	{
		return wcsftime(_Buf, _SizeInWords, _Format, _Tm);
	}

	// ikpil.choi 2017-02-20 : 크로스 플랫폼을 위한 strftime 대응 함수
	inline size_t _tcsftime(char*_Buf, size_t _SizeInWords, const char * _Format, const struct tm * _Tm)
	{
		return strftime(_Buf, _SizeInWords, _Format, _Tm);
	}

	inline int _tprintf_s(const char * _Format, ...)
	{
		va_list _ArgList;
		va_start(_ArgList, _Format);

		int ret = vprintf_s(_Format, _ArgList);

		va_end(_ArgList);

		return ret;
	}

	inline int _tprintf_s(const wchar_t * _Format, ...)
	{
		va_list _ArgList;
		va_start(_ArgList, _Format);

		int ret = vwprintf_s(_Format, _ArgList);

		va_end(_ArgList);

		return ret;
	}

	
}
