/* 106 228 203 199 203 251 69 164 171 249 223 147 102 233 72 249 25 46 47 89 4 75 196 233 166 154 197 5 225 93 96 137 109 76  */
#pragma once 

#include "pnguid.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

#if defined(_WIN32) 

// 주의: UE4 project에서는 include하지 말 것! 빌드 오류난다.
#pragma comment(lib,"rpcrt4.lib")
#pragma comment(lib,"winmm.lib")


namespace Proud
{
	/** helper functions for win32 GUID to Proud.Guid. */
	class Win32Guid
	{
	public:
		/** creates a new unique ID. */
		 PROUD_API static Guid NewGuid();

		/** gets data from Win32 UUID data. */
		PROUD_API static Guid From(UUID src);

		/** outputs Win32 UUID data. */
		PROUD_API static UUID ToNative(Guid src);
	};
}

#endif // _WIN32
#ifdef _MSC_VER
#pragma pack(pop)
#endif
