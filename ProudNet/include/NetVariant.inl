#include "NetVariant.h"

namespace Proud
{
	void ThrowInvalidTypeException(NetVariantType type1, NetVariantType type2);

	// NetVariant의 buffer 값에 바로 POD 값을 넣고, 변수 타입을 지정한다.
	// 생성자에서만 콜 하자.
	template<typename T>
	inline void NetVariant::Init_Raw(NetVariantType type, T value)
	{
		m_type = type;
		*(T*)(GetBuffer_Internal()) = value;
	}

	// NetVariant가 POD type일 때, buffer로부터 값을 바로 가져온다.
	// 내부 전용.
	// 타입이 안 맞으면 예외가 던져진다.
	template<typename T>
	inline T NetVariant::Get_Raw(NetVariantType type) const
	{
		if (m_type != type)
			ThrowInvalidTypeException(type, m_type);
		T* pv = (T*)GetBuffer_Internal();

		// 여기서 굳이 memcpy를 하는 이유
		// arm 프로세서에서는 memory alignment rule의 배수에 해당하지 않는 메모리에는 접근할 수 없다.
		// 물론 문자열은 읽을 수 있어야 하기 때문에 바이트 단위로 접근하는 것은 허용되지만, 그 이상의 사이즈로 접근하면 Bus Error 발생.
		// 그 문제를 회피하려고 memcpy를 사용.
		T ret;
		memcpy(&ret, pv, sizeof(T));

		return ret;
	}

	inline NetVariant::NetVariant()
	{
		m_type = NetVariantType_None;
		m_indexed = false;
	}

#define DEFINE_NETVARIANT_RAWTYPE(TYPE, TYPENAME) \
	inline NetVariant::NetVariant(TYPE value) \
	{ \
		Init_Raw<TYPE>(NetVariantType_##TYPENAME, value); \
	} \
	inline NetVariant::operator TYPE() const \
	{ \
		return Get_Raw<TYPE>(NetVariantType_##TYPENAME); \
	}

	PROUD_API DEFINE_NETVARIANT_RAWTYPE(int8_t, Int8)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(uint8_t, Uint8)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(int16_t, Int16)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(uint16_t, Uint16)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(int32_t, Int32)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(uint32_t, Uint32)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(int64_t, Int64)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(uint64_t, Uint64)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(bool, Bool)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(float, Float)
	PROUD_API DEFINE_NETVARIANT_RAWTYPE(double, Double)

	inline NetVariant::~NetVariant()
	{
		Reset();
	}

	inline bool NetVariant::operator<(const NetVariant& a) const
	{
		if (GetType() < a.GetType())
			return true;

		if (GetType() < a.GetType())
			return false;

		switch (GetType())
		{
		case NetVariantType_Bool:
			return ((bool)(*this) < (bool)a.GetBool());
		case NetVariantType_Int8:
			return ((int8_t)(*this) < (int8_t)a);
		case NetVariantType_Uint8:
			return ((uint8_t)(*this) < (uint8_t)a);
		case NetVariantType_Int16:
			return ((int16_t)(*this) < (int16_t)a);
		case NetVariantType_Uint16:
			return ((uint16_t)(*this) < (uint16_t)a);
		case NetVariantType_Int32:
			return ((int32_t)(*this) < (int32_t)a);
		case NetVariantType_Uint32:
			return ((uint32_t)(*this) < (uint32_t)a);
		case NetVariantType_Int64:
			return ((int64_t)(*this) < (int64_t)a);
		case NetVariantType_Uint64:
			return ((uint64_t)(*this) < (uint64_t)a);
		case NetVariantType_Float:
			return ((float)(*this) < (float)a);
		case NetVariantType_Double:
			return ((double)(*this) < (double)a);
		case NetVariantType_String:
			return ((String)(*this) < (String)a);
		case NetVariantType_Binary:
			if (CompBinary(a) < 0)
				return true;
			else
				return false;
		case NetVariantType_None:
			// TODO : 현재는 사용 못하는 타입
		case NetVariantType_TimeStamp:
		case NetVariantType_Uuid:
			return false;
		}

		return false;
	}

	inline bool NetVariant::operator==(const NetVariant& a) const
	{
		if (GetType() != a.GetType())
			return false;

		switch (GetType())
		{
		case NetVariantType_Bool:
			return ((bool)(*this) == (bool)a);
		case NetVariantType_Int8:
			return ((int8_t)(*this) == (int8_t)a);
		case NetVariantType_Uint8:
			return ((uint8_t)(*this) == (uint8_t)a);
		case NetVariantType_Int16:
			return ((int16_t)(*this) == (int16_t)a);
		case NetVariantType_Uint16:
			return ((uint16_t)(*this) == (uint16_t)a);
		case NetVariantType_Int32:
			return ((int32_t)(*this) == (int32_t)a);
		case NetVariantType_Uint32:
			return ((uint32_t)(*this) == (uint32_t)a);
		case NetVariantType_Int64:
			return ((int64_t)(*this) == (int64_t)a);
		case NetVariantType_Uint64:
			return ((uint64_t)(*this) == (uint64_t)a);
		case NetVariantType_Float:
			return ((float)(*this) == (float)a);
		case NetVariantType_Double:
			return ((double)(*this) == (double)a);
		case NetVariantType_String:
			return ((String)(*this) == (String)a);
		case NetVariantType_Binary:
			return !CompBinary(a);
		case NetVariantType_None:
			// TODO : 현재는 사용 못하는 타입
		case NetVariantType_TimeStamp:
		case NetVariantType_Uuid:
			return false;
		}

		return false;
	}

	inline bool NetVariant::operator!=(const NetVariant& a) const
	{
		return !(*this == a);
	}
}
