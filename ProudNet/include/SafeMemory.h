#pragma once

#include <iostream>
#include <errno.h> // for clang

typedef int errno_t;

namespace Proud
{
	template <typename T>
	inline void SafeDelete(T *&p)
	{
		delete p;
		p = static_cast<T *>(0); // for c++03
	}

	template <typename T>
	inline void SafeDeleteArray(T *&p)
	{
		delete[] p;
		p = static_cast<T *>(0); // for c++03
	}

	template <typename T, size_t N>
	inline size_t ArraySize(T (&)[N])
	{
		return N;
	}

	template <typename T, size_t N>
	inline size_t ArrayByteSize(T (&)[N])
	{
		return sizeof(T) * N;
	}
}

// ikpil.choi 2016-12-28, memset_s 
inline errno_t memset_s(void *dest, size_t dest_size, int c, size_t n)
{
	errno_t err = 0;

	if (dest == 0)
		return EINVAL;

	// 복사 대상의 최대값은 size_t / 2 사이즈
	const size_t max_value = (SIZE_MAX >> 1);

	// 대상의 사이즈가 최대값을 넘었을 경우, 문제가 있으므로, 리턴
	if (dest_size > max_value)
		return E2BIG;

	// 채워야 할 사이즈가 최대값을 초과 했을 경우, 대상 사이즈로 조정, 에러 셋팅
	if (n > max_value) 
	{
		err = E2BIG;
		n = dest_size;
	}

	// 채워야 할 바이트가 대상 사이즈보다 클 경우, 대상 사이즈로 조정, 에러 셋팅
	if (n > dest_size) 
	{
		err = EOVERFLOW;
		n = dest_size;
	}

	memset(dest, c, n);

	return err;
}

#if !defined(WIN32)
#include "SafeMemoryCRT.h"
#endif

