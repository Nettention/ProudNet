#pragma once 

#include "BasicTypes.h"
#include "PNString.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	class CMessage;

	struct PNGUID
	{
		uint32_t Data1;
		uint16_t Data2;
		uint16_t Data3;
		uint8_t Data4[8];
	};

	/** Platform independent Global Unique ID object. 
	For Win32 features, refer to Proud::Win32Guid class.
	*/
	class  Guid :public PNGUID
	{
	public:
		/**
		\~korean
		GUID를 랜덤으로 생성합니다. NewGuid()와 달리, 이 값은 고유성이 보장되지 않습니다.

		\~english
		Generates a 'random' GUID. Unlike NewGuid(), this does not guarantee uniqueness.

		\~chinese
		随机生成GUIDE。与NewGuid（）不同，此值不保障固有性。

		\~japanese
		GUIDをランダムで生成します。NewGuid()とは違って、この値は固有性が保障されなければなりません。
		\~
		*/
		 PROUD_API static Guid RandomGuid();

		 PROUD_API Guid();
		 PROUD_API Guid(PNGUID src);

		/**
		\~korean
		Guid를 String값으로 변환해 줍니다.

		\~english TODO:translate needed.
		It converts Guid to a string value. 

		\~chinese
		把Guid转换成String值。

		\~japanese
		GuidをString値に変換してくれます。
		\~
		*/
		 PROUD_API String ToString() const;

		/**
		\~korean
		Guid를 Bracket({}) 를 포함한 String으로 변환해 줍니다.

		\~english TODO:translate needed.
		It converts Guid with a string including brakets({}). 

		\~chinese
		把Guid转换成包括Bracket({})的String。

		\~japanese
		GuidをBracket({})を含んだStringに変換してくれます。
		\~
		*/
		 PROUD_API String ToBracketString() const;

		/**
		\~korean
		Guid를 String값으로 변환해 줍니다.
		\param uuid Guid입니다.
		\param uuidStr uuid를 String형태로 변환한 정보를 담을 reference 입니다
		\return 변환에 성공 true, 실패 false

		\~english TODO:translate needed.
		It converts Guid to a string value.
		\param This is uuid Guid. 
		\param This is the reference to hold the information which is uuid converted into a string format. 
		\return True when successful in conversion, otherwise false. 


		\~chinese
		把Guid转换成String值。
		\param uuid 是Guid。
		\param uuidStr 是包含着把uuid转换成String形式信息的reference。
		\return 转换成功的话true，失败的话false。

		\~japanese
		GuidをString値に変換してくれます。
		\param uuid Guidです。
		\param uuidStr uuidをString形態に変換した情報を入れるreferenceです。
		\return 変換に成功するとtrue、失敗するとfalse
		\~
		*/
		 PROUD_API static bool  ConvertUUIDToString(const Guid &uuid, String &uuidStr);

		/**
		\~korean
		Guid를 Bracket({}) 를 포함한 String으로 변환해 줍니다.
		\param uuid Guid입니다.
		\param uuidStr uuid를 String형태로 변환한 정보를 담을 reference 입니다
		\return 변환에 성공 true, 실패 false

		\~english TODO:translate needed.
		It converts Guid to a string value
		\param This is uuid Guid.
		\param This is the reference to hold the information which is uuid converted into a string format. 
		\return True when successful in conversion, otherwise false.

		

		\~chinese
		把Guid转换成包括Bracket({}) 的String。
		\param uuid 是Guid。
		\param uuidStr 是包含着把uuid转换成String形式信息的reference。
		\return 转换成功的话true，失败的话false。

		\~japanese
		GuidをBracket({})を含んだStringに変換してくれます。
		\param uuid Guidです。
		\param uuidStr uuidをString形態に変換した情報を入れるreferenceです。
		\return 変換に成功するとtrue、失敗するとfalse
		\~
		*/
		 PROUD_API static bool  ConvertUUIDToBracketString(const Guid &uuid, String &uuidStr);

		/**
		\~korean
		String을 uuid값으로 변환합니다.
		\param uuidStr uuid의 String형태 입니다.
		\param uuidStr String에서 uuid로 변환한 정보를 담을 reference 입니다
		\return 변환에 성공 true, 실패 false

		\~english TODO:translate needed.
		It converts string to a uuid value.
		\param uuidStr This is the string type of uuid. 
		\param uuidStr This is the reference to hold the information which is string converted into an uuid format.
		\return True when successful in conversion, otherwise false.


		\~chinese
		把String转换成uuid值。
		\param uuidStr 是uuid的String形式。
		\param uuidStr 是包含着从String转换成uuid信息的reference。
		\return 转换成功的话true，失败的话false。

		\~japanese
		Stringをuuid値に変換しまsう。
		\param uuidStr uuidのString形態です。
		\param uuidStr Stringでuuidに変換した情報を入れるreferenceです。
		\return 変換に成功するとtrue、失敗するとfalse
		
		\~
		*/
		 PROUD_API static bool  ConvertStringToUUID(String uuidStr, Guid &uuid);

		/**
		\~korean
		Guid를 String값으로 변환해 줍니다.
		\param uuid Guid입니다.
		\return String

		\~english TODO:translate needed.
		It converts Guid to a string value.
		\param This is Guid. 
		\return String


		\~chinese
		把Guid转换成String值。
		\param uuid 是Guid。
		\return String

		\~japanese
		GuidをString値に変換してくれます。
		\param uuid Guidです。
		\return String

		\~
		*/
		 static inline String GetString(const Guid &uuid)
		{
			String ret;
			if (ConvertUUIDToString(uuid, ret))
				return ret;
			else
				return String();
		}

		inline String GetString() const
		{
			return Guid::GetString(*this);
		}

		/**
		\~korean
		Guid를 Bracket({}) 를 포함한 String으로 변환해 줍니다.
		\param uuid Guid입니다.
		\return String

		\~english TODO:translate needed.
		It converts Guid with a string including brakets({}). 
		\param This is Guid.
		\return String


		\~chinese
		把Guid转换成包括Bracket({})的String。
		\param uuid 是Guid。
		\return String

		\~japanese
		GuidをBracket({})を含んだStringに変換してくれます。
		\param uuid Guidです。
		\return String

		\~
		*/
		 static inline String GetBracketString(const Guid &uuid)
		{
			String ret;
			if (ConvertUUIDToBracketString(uuid, ret))
				return ret;
			else
				return String();
		}

		/**
		\~korean
		String을 uuid값으로 변환합니다.
		\param uuidStr uuid의 String형태 입니다.
		\return Guid 변환된 Guid 입니다.

		\~english TODO:translate needed.
		It converts string to a uuid value.
		\param uuidStr This is the string type of uuid. 
		\return Guid This is the converted Guid. 


		\~chinese
		把String转换成uuid值。
		\param uuidStr 是uuid的String形式。
		\return Guid被转换的Guid。

		\~japanese
		Stringをuuid値に変換します。
		\param uuidStr uuidのString形態です。
		\return Guid 変換されたGuidです。
		\~
		*/
		 static inline Guid GetFromString(const PNTCHAR* uuidStr)
		{
			Guid uuid;
			if (ConvertStringToUUID(uuidStr, uuid))
				return uuid;
			else
				return Guid();
		}

		/**
		\~korean
		PNGUID 객체로부터의 캐스팅
		\param uuid	PNGUID입니다.
		\return Guid객체

		\~english TODO:translate needed.
		Casting from PNGUID object
		\param uuid  This is PNGUID
		\return Guid object. 


		\~chinese TODO:translate needed.
		从PNGUID 对象开始casting
		\paramuuid	PNGUID
		\return Guid对象

		\~japanese TODO:translate needed.
		PNGUID オブジェクトからのキャスティング
		\param uuid	PNGUIDです。
		\return Guidオブジェクト
		\~
		*/
		 static inline Guid From(const PNGUID& uuid)
		{
			return Guid(uuid);
		}

	};

	inline bool operator<(const Guid& d1, const Guid& d2)
	{
		return memcmp(&d1, &d2, sizeof(Guid)) < 0;
	}

	inline bool operator<=(const Guid& d1, const Guid& d2)
	{
		return memcmp(&d1, &d2, sizeof(Guid)) <= 0;
	}

	inline bool operator>(const Guid& d1, const Guid& d2)
	{
		return memcmp(&d1, &d2, sizeof(Guid)) > 0;
	}

	inline bool operator>=(const Guid& d1, const Guid& d2)
	{
		return memcmp(&d1, &d2, sizeof(Guid)) >= 0;
	}

	inline bool operator==(const Guid& d1, const Guid& d2)
	{
		return memcmp(&d1, &d2, sizeof(Guid)) == 0;
	}

	inline bool operator!=(const Guid& d1, const Guid& d2)
	{
		return memcmp(&d1, &d2, sizeof(Guid)) != 0;
	}

	 PROUD_API CMessage& operator>>(CMessage &a, Guid &b);
	 PROUD_API CMessage& operator<<(CMessage &a, const Guid &b);
	 PROUD_API void AppendTextOut(String &a, const Guid &b);
};

/**
\~korean
CFastMap 등에 쓰려면 이것이 필요하다.

\~english
This is needed to use CFastMap or others.

\~chinese
想用于 CFastMap等的话需要这个。

\~japanese
CFastMap などに使うためにはこれが必要です。
\~
*/
template<>
class CPNElementTraits < Proud::Guid >
{
public:
	typedef const Proud::Guid& INARGTYPE;
	typedef Proud::Guid& OUTARGTYPE;

	inline static uint32_t Hash(INARGTYPE element)
	{
		uint32_t ret = 0;
		uint32_t* data = (uint32_t*)&element;
		for (int i = 0; i < 4; i++)
		{
			ret ^= data[i];
		}

		return ret;
	}

	inline static bool CompareElements(INARGTYPE element1, INARGTYPE element2)
	{
		return (element1 == element2) ? true : false;
	}

	inline static int CompareElementsOrdered(INARGTYPE element1, INARGTYPE element2)
	{
		if (element1 < element2)
			return -1;
		else if (element1 > element2)
			return 1;
		else
			return 0;
	}
};

#ifdef _MSC_VER
#pragma pack(pop)
#endif
