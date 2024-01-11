#pragma once

#include "pnstdint.h"
#include "PNString.h"
#include "ByteArray.h"

namespace Proud
{
	// variant type 값들.
	enum NetVariantType
	{
		NetVariantType_None,
		NetVariantType_Bool,
		NetVariantType_Int8,
		NetVariantType_Uint8,
		NetVariantType_Int16,
		NetVariantType_Uint16,
		NetVariantType_Int32,
		NetVariantType_Uint32,
		NetVariantType_Int64,
		NetVariantType_Uint64,
		NetVariantType_Float,
		NetVariantType_Double,
		NetVariantType_String,
		NetVariantType_TimeStamp,
		NetVariantType_Uuid,
		NetVariantType_Binary,
	};

	/** 일반적인 variant type 클래스.

	비슷한 종류의 다른 객체와의 차이는 다음과 같습니다.

	- CVariant는 ADO,OLE,COM등을 위해 쓰이는 것인지라, 윈도 전용이다. 하지만 이것은 모든 플랫폼에서 쓸 수 있다.
	- COdbcVariant는 ODBC에서 지원하는 변수 타입을 위한 것인지라, 클라,서버 모두가 쓰기에는 불필요하게 무겁다. 하지만 이것은 외부 코드에 의존하지 않는다.

	일반적인 사용법

	- 변수를 선언한다.
	- 평소처럼 변수에 값을 입력하거나 출력하면 된다.

	\code
	NetVariant a;
	a = (int)1;
	assert(a.GetType()==NetVariantType_Int32);
	int b = a;

	NetVariant b;
	b = _PNT("abc");

	\endcode
	*/
	class NetVariant
	{
		// TODO: Guid보다 더 큰 타입을 담을 일이 있으면 여기를 늘려라!
		static const int BufferLength = 16;
	public:
		PROUD_API NetVariant();
		PROUD_API NetVariant(const NetVariant& rhs);
		~NetVariant();


		/** 내용물을 없애고 초기화합니다. */
		PROUD_API void Reset();

		//////////////////////////////////////////////////////////////////////////
		// 값 입출력용 함수들

#define DECLARE_NETVARIANT_RAWTYPE(TYPE) NetVariant(TYPE value); operator TYPE() const;

		PROUD_API DECLARE_NETVARIANT_RAWTYPE(int8_t)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(uint8_t)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(int16_t)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(uint16_t)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(int32_t)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(uint32_t)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(int64_t)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(uint64_t)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(bool)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(float)
		PROUD_API DECLARE_NETVARIANT_RAWTYPE(double)

		inline bool operator<(const NetVariant& a) const;
		inline bool operator==(const NetVariant& a) const;
		inline bool operator!=(const NetVariant& a) const;

		// 문자열
#if defined(_PNUNICODE) || defined(SWIG)
		// SWIG는 wchar_t를 사용할 수 없지만 SWIG가 생성하는 wrap.cxx에서 PNTCHAR로 변환하려 하기 때문에
		// 강제로 char* 로 인식할 수 있게 하는 코드가 필요하여 추가하였다.
		PROUD_API NetVariant(const char* value);
#endif
		PROUD_API NetVariant(const PNTCHAR* value);

		operator String() const;

		//////////////////////////////////////////////////////////////////////////

		// Binary
		PROUD_API NetVariant(const ByteArray& value);
		operator ByteArray() const;
		//////////////////////////////////////////////////////////////////////////

		NetVariantType GetType() const
		{
			return m_type;
		}

		bool GetIndexed() const
		{
			return m_indexed;
		}

		void SetIndexed(bool indexed)
		{
			m_indexed = indexed;
		}

	private:
		// 변수 타입
		NetVariantType m_type;

		// index 사용여부
		bool m_indexed;

		// 이 안에 모든 종류의 객체가 캐스팅되어 들어간다.
		char m_buffer[BufferLength];

		String m_string;
		ByteArray m_binary;

		template<typename T>
		void Init_Raw(NetVariantType type, T value);

		template<typename T>
		bool TryGet_Raw(NetVariantType type, T&);

	public:
		template<typename T>
		T Get_Raw(NetVariantType type) const;

		// public이지만, 내부 전용 함수들이다.
		inline void* GetBuffer_Internal() { return m_buffer; }
		inline const void* GetBuffer_Internal() const { return m_buffer; }

		int8_t GetInt8() const { return Get_Raw<int8_t>(NetVariantType_Int8); }
		uint8_t GetUint8() const { return Get_Raw<uint8_t>(NetVariantType_Uint8); }
		int16_t GetInt16() const { return Get_Raw<int16_t>(NetVariantType_Int16); }
		uint16_t GetUint16() const { return Get_Raw<uint16_t>(NetVariantType_Uint16); }
		int32_t GetInt32() const { return Get_Raw<int32_t>(NetVariantType_Int32); }
		uint32_t GetUint32() const { return Get_Raw<uint32_t>(NetVariantType_Uint32); }
		int64_t GetInt64() const { return Get_Raw<int64_t>(NetVariantType_Int64); }
		uint64_t GetUint64() const { return Get_Raw<uint64_t>(NetVariantType_Uint64); }
		bool GetBool() const { return Get_Raw<bool>(NetVariantType_Bool); }
		float GetFloat() const { return Get_Raw<float>(NetVariantType_Float); }
		double GetDouble() const { return Get_Raw<double>(NetVariantType_Double); }
		String GetString() const
		{
			return m_string;
		}

		ByteArray GetBinary() const
		{
			return m_binary;
		}

		ByteArray* GetBinaryPtr() const
		{
			return (ByteArray*)&m_binary;
		}

		void WriteBinary(const uint8_t* data, int length);
		int GetBinaryLength();
		int ReadBinary(uint8_t* data, int length);
		int CompBinary(const NetVariant& a) const;
	};

}

// 성능에 민감한 클래스다. 따라서 inline 함수로 구현되어 있다.
#include "NetVariant.inl"
