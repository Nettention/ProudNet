#pragma once 

#include <string>

#include "HashString.h"
#include "pnstdint.h"

inline  uint32_t PNHash(const bool& element) { return (uint32_t)element; }
inline  uint32_t PNHash(const int8_t& element) { return (uint32_t)element; }
inline  uint32_t PNHash(const uint8_t& element) { return (uint32_t)element; }
inline  uint32_t PNHash(const int16_t& element) { return (uint32_t)element; }
inline  uint32_t PNHash(const uint16_t& element) { return (uint32_t)element; }
inline  uint32_t PNHash(const int32_t& element) { return (uint32_t)element; }
inline  uint32_t PNHash(const uint32_t& element) { return (uint32_t)element; }
inline  uint32_t PNHash(const int64_t& element)
{
	// 64bit 크기 타입(double등등)을 xor로 해싱하기 위해 앞뒤를 섞는다.
	// cast to smaller type을 하려고 하니 clang에서 에러를 내기에, 이렇게 해야.
	uint32_t* piece = (uint32_t*)&element;
	return piece[0] ^ piece[1];
}
inline  uint32_t PNHash(const uint64_t& element) { return ::PNHash((int64_t)element); }

#if defined(__MACH__)
inline  uint32_t PNHash(const long& element) { return ::PNHash((int64_t)element); }   // Xcode에서 이게 없으니 빌드 에러가 난다.
inline  uint32_t PNHash(const unsigned long& element) { return ::PNHash((int64_t)element); }  // Xcode에서 이게 없으니 빌드 에러가 난다.
#endif //defined(__MACH__)

inline  uint32_t PNHash(const float& element) { return ::PNHash(*(int32_t*)&element); }	// 의도적으로 캐스팅을 이모양으로 해야 한다. 안그러면 소수점을 버리는 오류가 나버린다.
inline  uint32_t PNHash(const double& element) { return ::PNHash(*(int64_t*)&element); }	// 의도적으로 캐스팅을 이모양으로 해야 한다. 안그러면 소수점을 버리는 오류가 나버린다.

inline  uint32_t PNHash(const std::string& element)
{
	return Proud::HashString<char>(element.c_str());
}

inline  uint32_t PNHash(const std::wstring& element)
{
	return Proud::HashString<wchar_t>(element.c_str());
}


template <typename T>
class CPNElementTraits
{
public:
	typedef const T& INARGTYPE;
	typedef T& OUTARGTYPE;

	// 	static void CopyElements( T* pDest, const T* pSrc, size_t nElements )
	// 	{
	// 		for( size_t iElement = 0; iElement < nElements; iElement++ )
	// 		{
	// 			pDest[iElement] = pSrc[iElement];
	// 		}
	// 	}
	//
	// 	static void RelocateElements( T* pDest, T* pSrc, size_t nElements )
	// 	{
	// 		// A simple memmove works for nearly all types.
	// 		// You'll have to override this for types that have pointers to their
	// 		// own members.
	// 		Checked::memmove_s( pDest, nElements*sizeof( T ), pSrc, nElements*sizeof( T ));
	// 	}


	// 예전에는 sizeof(element) 크기가 1,2,4,8에 따라 어떻게 해라...이래 짜졌다.
	// 그러나 그건 위험. element가 plain type이 아닌 경우 1,2,4,8 크기면 어쩌자고.
	inline static uint32_t Hash(const T& element) { return ::PNHash(element); }

	inline static bool CompareElements(const T& element1, const T& element2)
	{
		return (element1 == element2);
	}

	inline static int CompareElementsOrdered(const T& element1, const T& element2)
	{
		if (element1 < element2)
		{
			return(-1);
		}
		else if (element1 == element2)
		{
			return(0);
		}
		else
		{
			assert(element1 > element2);
			return(1);
		}
	}
};


template<typename TYPE>
class CPNElementTraits_Int32PlainType
{
public:
	typedef TYPE INARGTYPE;
	typedef TYPE& OUTARGTYPE;

	inline static uint32_t Hash(TYPE a)
	{
		return (uint32_t)a;
	}

	inline static bool CompareElements(INARGTYPE element1, INARGTYPE element2)
	{
		return element1 == element2;
	}

	inline static int CompareElementsOrdered(INARGTYPE element1, INARGTYPE element2)
	{
		return (int32_t)element1 - (int32_t)element2;
	}
};

/// enum 타입에 대해서 Hash 함수를 만드는 것이 귀찮으면 이 macro를 사용하세요.
#define CPNElementTraits_FOR_ENUM(type) template<> class CPNElementTraits<type>: public CPNElementTraits_Int32PlainType<type> {};
