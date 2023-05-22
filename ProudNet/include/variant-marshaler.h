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
