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

// NOTE: 본 모듈은 서버 전용이다. UE4 등에서는 OLE-COM API를 사용 불가하므로.

#if defined(_WIN32)
    #include <comutil.h>
	#include <ATLComTime.h>
#endif

#include "ByteArrayPtr.h"
#include "pnguid-win32.h"
#include "./FakeClrBase.h"
#include "./PnTime.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup db_group
	*  @{
	*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4244)
#endif

#if defined(_WIN32)
	/** 
	\~korean
	variant.
	_variant_t와 비슷한 역할을 하지만, UUID, int64 등의 처리 기능이 부가적으로 들어가 있다.
	- 이 객체는 UUID를 bracket string으로 변환해서 가진다.
	- 이 객체는 int64를 VariantChangeType()대신 자체 구현된 변환기를 가진다. 
	(VariantChangeType에서 64bit integer 지원은 Windows XP에서부터 지원하기 때문이다.) 

	\~english
	variant.
	This module works similar to _variant_t, but additionally handles UUID, int64 and etc.
	- This object has an UUID after converting it to braket string.
	- This object has a custom converter of int64 instead of VariantChangeType(). 
	(Because VariantChangeType에서 64bit integer is already supported in Windows XP.)

	\~chinese
	variant.
	起着与_variant_相似的作用，但另外还添加了UUID, int64等处理功能。
	- 此对象把UUID用bracket string转换而获取。
	- 此对象拥有代替VariantChangeType()将int64进行自身体现的转换机。
	（在VariantChangeType里支持64bit integer是因为从Windows XP开始支持。）

	\~japanese
	variant.
	_variant_tと類似した役割をしますが、UUID、int64などの処理機能が付加的に入っています。
	- このオブジェクトはUUIDをbracket stringに変換して持ちます。
	- このオブジェクトはint64をVariantChangeType()の代わりに自ら実現した変換器を持ちます。 
	(VariantChangeTypeでの64bit integerのサポートは、Windows XPからサポートするからです。) 
	\~
	*/
	class CVariant
	{
	public:
		_variant_t m_val;

		// 아래 많은 constructor들은 _variant_t에서 따온 것이다. 필요한 경우 
		// 이러한 것들(constructor,operator=,extractor operator)를 만들도록 하자. 
		inline CVariant() {}
		inline CVariant(const _variant_t &src) { m_val=src; }
		inline CVariant(VARENUM vartype){V_VT(&m_val) = vartype;}

		inline CVariant(char a) { m_val=(long)a; }
		inline CVariant(short a) { m_val=(long)a; }
		inline CVariant(int a) { m_val=(long)a; }
		inline CVariant(long a) { m_val=a; }
		inline CVariant(int64_t a) { m_val=a; }
		inline CVariant(unsigned char a) { m_val=a; }
		inline CVariant(unsigned short a) { m_val=a; }
		inline CVariant(unsigned int a) { m_val=a; }
		inline CVariant(unsigned long a) { m_val=a; }
		inline CVariant(uint64_t a) { m_val=a; }
		inline CVariant(float a) { m_val=a; }
		inline CVariant(double a) { m_val=a; }
		inline CVariant(const wchar_t* a) { m_val = _bstr_t(a); }
		inline CVariant(const char* a) { m_val=_bstr_t(a); }
		inline explicit CVariant(const String& a) { m_val=_bstr_t(a.GetString()); }
		inline CVariant(UUID a) { m_val = Win32Guid::From(a).ToBracketString().GetString(); }
		inline CVariant(Guid a) { m_val = a.ToBracketString().GetString(); }
		inline CVariant(bool a) { m_val=(long)a; }
		inline CVariant(const COleDateTime& a) { m_val = a; }
		inline CVariant(const ByteArray& a){FromByteArray(a);}
		inline CVariant(const ByteArrayPtr a){FromByteArray(a);}

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator char() const { ThrowIfNull(); return (long)m_val; }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator short() const { ThrowIfNull(); return (long)m_val; }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator int() const { ThrowIfNull(); return (long)m_val; }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator long() const { ThrowIfNull(); return m_val; }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator unsigned char() const { ThrowIfNull(); return m_val; }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오.

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator unsigned short() const { ThrowIfNull(); return m_val; }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator unsigned int() const { ThrowIfNull(); return m_val; }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator unsigned long() const { ThrowIfNull(); return m_val; }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator float() const { ThrowIfNull(); return m_val; }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator double() const { ThrowIfNull(); return m_val; }

		/**
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator String() const { ThrowIfNull(); return String((const PNTCHAR*)(_bstr_t)m_val); }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator UUID() const { ThrowIfNull(); return Win32Guid::ToNative(Guid::GetFromString((_bstr_t)m_val)); }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator Guid() const { ThrowIfNull(); return Guid::GetFromString((_bstr_t)m_val); }

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator bool() const { ThrowIfNull(); return m_val; }	

		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator COleDateTime() const { ThrowIfNull(); return m_val; }

		/**
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오.

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。
		\~
		*/
		inline operator int64_t() const { ThrowIfNull(); return m_val; }
		
		/** 
		\~korean
		리턴값 연산자입니다. 만약 이 객체가 null 값을 갖고 있으면 예외가 throw됩니다. 따라서 필요한 경우 IsNull()로 먼저 null인지 여부를 확인하십시오. 

		\~english
		This is a returned value operator, which throws an exception if it contains null values. So please confirm the value is null or not by IsNull() first.

		\~chinese
		返回值运算符。如果此对象拥有null值的话，例外会被throw。因此需要的话先用IsNull()确认null的与否。

		\~japanese
		リターン値演算子です。もし、このオブジェクトがnull値を持っていれば例外がthrowになります。よって、必要な場合、IsNull()でまずnullなのかどうかを確認してください。 
		\~
		*/
		inline operator uint64_t() const { ThrowIfNull(); return m_val; }
		//inline operator ByteArray() const {return ToByteArray();}
		inline operator ByteArrayPtr() const {return ToByteArrayPtr();}

		/** 
		\~korean
		값이 들어있는지 체크한다. 

		\~english
		This method checks whether or not a value is included.

		\~chinese
		检查是否有值。

		\~japanese
		値が入っているかをチェックします。
		\~
		*/
		inline bool IsNull() const { return m_val.vt == VT_NULL; } 

		/** 
		\~koeran
		이진 데이터 블럭 형태로 데이터를 리턴한다. 

		\~english
		This method returns data in byte data block.

		\~chinese
		以byte数据block形式返回数据。

		\~japanese
		二進データブロック形態でデータをリターンします。
		\~
		*/
		PROUDSRV_API 		 void ToByteArray(ByteArray &output) const;

		/**
		\~korean
		이진 데이터 블럭 형태로 데이터를 리턴한다. 

		\~english
		This method returns data in byte data block.

		\~chinese
		以byte数据block形式返回数据。

		\~japanese
		二進データブロック形態でデータをリターンします。
		\~
		*/
		PROUDSRV_API 		 ByteArrayPtr ToByteArrayPtr() const;

		/** 
		\~korean
		이진 데이터 블럭을 입력받는다. 

		\~english
		This method inputs data in byte data block.

		\~chinese
		输入byte数据block。

		\~japanese
		二進データブロックを入力してもらいます。
		\~
		*/
		 PROUDSRV_API void  FromByteArray(const ByteArray& input);

		/** 
		\~korean
		이진 데이터 블럭을 입력받는다. 

		\~english
		This method inputs data in byte data block.

		\~chinese
		输入byte数据block。

		\~japanese
		二進データブロックを入力してもらいます。
		\~
		*/
		 PROUDSRV_API void  FromByteArray(const ByteArrayPtr input);
	//private: VariantToByteArray 때문에 public
		PROUDSRV_API void ThrowIfNull() const;
	};
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
