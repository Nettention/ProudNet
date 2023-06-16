/* 106 228 203 199 203 251 69 164 171 249 223 147 102 233 72 249 25 46 47 89 4 75 196 233 166 154 197 5 225 93 96 137 109 76  */
#pragma once 

#include "Variant.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	class CMessage;
	class CVariant;

#if defined(_WIN32)    
	PROUDSRV_API CMessage& operator<<(CMessage &a, const CVariant &b);
	PROUDSRV_API CMessage& operator>>(CMessage &a, CVariant &b);

	PROUDSRV_API void AppendTextOut(String &a, const CVariant &b);
#endif
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
