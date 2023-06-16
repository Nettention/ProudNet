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

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud 
{
#if (defined(_MSC_VER) && _MSC_VER>=1400)
// 아래 주석처리된 pragma managed 전처리 구문은 C++/CLI 버전이 있었을 때에나 필요했던 것입니다.
// 현재는 필요없는 구문이고, 일부 환경에서 C3295 "#pragma managed는 전역 또는 네임스페이스 범위에서만 사용할 수 있습니다."라는 빌드에러를 일으킵니다.
//#pragma managed(push,off)
#endif

	/* a,b가 가리키는 두개의 int32 배열의 대소 여부를 판별한다.
	memcmp(a,b,16)과 같은 역할을 한다. 다만 크기가 작은 경우 이것이 더 연산 속도가 빨라서 이것을 쓸 때가 있다.
	배열 크기가 템플릿 인자라서 loop unroll 최적화도 이루어진다.
	#TODO 아닐수도 있다. compiler가 알아서 16바이트 정도 크기면
	intrinsic level로 최적화된 코드가 나올 수 있다.
	일단은 보수적으로 가지만 추후 체크해서 이 함수를 버리던지 유지하던지 하자. */
	template <size_t LENGTH> 
	int CompareInt32Array(const void* a, const void* b)
	{
		const int32_t* a2 = (const int32_t*)a;
		const int32_t* b2 = (const int32_t*)b;
		for (int i = 0; i < LENGTH; i++)
		{
			int c = (*a2) - (*b2);
			if (c != 0)
				return c;
			a2++;
			b2++;
		}
		// 끝까지 다 비교해도 동일하다.
		return 0;
	}

//
//
//	// 작은 크기의 데이터에 대한 memcpy. memcpy 자체는 내부적으로 SIMD 최적화를 하지만 CPU 상황이라던지 여러가지를 알아보는 코스트가 크다.
//	// 오히려 작은 크기는 이게 더 빠름.
//	inline void SmallMemcpy(uint8_t*dst, const uint8_t*src, size_t size)  
//	{
//		static const size_t sizetSize = sizeof(size_t);
//
//		size_t i;
//
//#if defined(_WIN64) || defined(__LP64__)
//		assert(sizetSize == 8);
//		size_t pad = size & 7; // X mod 8과 같은 연산
//		size_t blsize = size >> 3; // X / 8 과 같은 연산
//#else
//		assert(sizetSize == 4);
//		size_t pad = size & 3; // X mod 8과 같은 연산
//		size_t blsize = size >> 2; // X / 8 과 같은 연산
//#endif
//		for (i = 0; i < blsize; i++)
//		{
//			*(size_t*)dst = *(size_t*)src;
//			dst += sizetSize;
//			src += sizetSize;
//		}
//		for (i = 0; i < pad; i++)
//		{
//			*(uint8_t *)dst = *(uint8_t *)src;
//			dst++;
//			src++;
//		}
//	}

	inline void UnsafeFastMemmove(void *dest,const void* src,size_t length)
	{
		// array.add에서 length=0인 경우가 잦다.
		if(length==0)
			return;

		// memmove_s나 memcpy_s는 memmove, memcpy보다 훨씬 느리다. 여러가지 바운드 체크와 내부적인 zeromem 처리가 있기 때문이다.
		// 따라서 이걸 그냥 쓴다.
		// 추후에는 AMD64,IA64,Itanium,Win32를 위한 어셈 루틴을 따로 만드는 것이 더 나을지도.
		memmove(dest,src,length);
	}

//// sysutil.cpp에 이걸 켠 이유가 설명되어 있음
//// Steam OS에서 경고레벨3에서 크래시 이슈로 인해 Unsafememcopy 사용 비활성화 하도록 USE_UnsafeFastMemcpy define 주석 처리
////#define USE_UnsafeFastMemcpy
//
//#ifdef USE_UnsafeFastMemcpy
//
//	inline void UnsafeFastMemcpy(void *dest, const void* src, size_t length)
//	{
//		// array.add에서 length=0인 경우가 잦다.
//		if ( length == 0 )
//			return;
//
//#if defined(__ANDROID__) || defined(__arm__) // RVCT를 쓰는 경우 __packed keyword를 쓰면 되지만 우리는 __packed 를 못 쓰는 컴파일러(gcc or clang)을 쓰고 있다.
//		memcpy(dest,src,length);
//		// memmove_s나 memcpy_s는 memmove, memcpy보다 훨씬 느리다. 여러가지 바운드 체크와 내부적인 zeromem 처리가 있기 때문이다.
//		// 따라서 이걸 그냥 쓴다.
//		// 추후에는 AMD64,IA64,Itanium,Win32를 위한 어셈 루틴을 따로 만드는 것이 더 나을지도.
//#else
//		if ( length <= 256 ) // 32에서 상향. memcpy를 호출하는 조건을 조절해서 성능을 올렸음.
//			SmallMemcpy((uint8_t*)dest, (uint8_t*)src, length);
//		else
//			memcpy(dest, src, length);
//#endif
//	}
//
//#else 
//
//#	define UnsafeFastMemcpy memcpy 
//
//#endif
//
#if (defined(_MSC_VER) && _MSC_VER>=1400)
//#pragma managed(pop)
#endif

}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
