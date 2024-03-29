/*
ProudNet 1.8.58849-master


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

#include <string>
#include "FakeClr.h"
#include "Message.h" // 이게 어쨌거나 필요. AppendTextOut 때문
#include "AddrPort.h"
#include "Enums.h"
#include "ErrorInfo.h"
#include "FastList.h"
#include "pnguid.h"
#include "PnTime.h"

// NOTE: 여기서 stl관련 marshaler나 stl을 include하면 안된다. ProudNet은 어떤 버전의 stl을 사용자가 쓰는지에 의존하면 안되니까.
// 이것은 모든 C++ library의 특징이기도 하다. (이거 무시하다가 컴파일 에러 지옥을 경험하게 됨.)

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	/** \addtogroup net_group
	*  @{
	*/

	 void ThrowExceptionOnReadString(int length);
	 PROUD_API void ThrowExceptionOnReadArray(int64_t length);
	inline CMessage& operator<<(CMessage &a, const StringA &str)
	{
		a.WriteString(str);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, StringA &str)
	{
		a.ReadString(str);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, const StringW &str)
	{
		a.WriteString(str);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, StringW &str)
	{
		a.ReadString(str);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, const std::string &str)
	{
		a.WriteString(str);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, std::string &str)
	{
		a.ReadString(str);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, const std::wstring &str)
	{
		a.WriteString(str);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, std::wstring &str)
	{
		a.ReadString(str);
		return a;
	}

	// 쌩 ptr을 넣는 경우 엄한 bool <<가 실행되어서 이 함수를 추가했다.
	inline CMessage& operator<<(CMessage& a, const char* str)
	{
		a.WriteStringA(str);
		return a; 
	}
	inline CMessage& operator<<(CMessage& a, const wchar_t* str)
	{
		a.WriteStringW(str);
		return a;
	}

	inline CMessage& operator<<(CMessage &a, bool b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, int8_t b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, uint8_t b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, int16_t b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, uint16_t b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, int32_t b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, uint32_t b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, uint64_t b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, int64_t b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, float b) {
		a.Write(b);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, double b) {
		a.Write(b);
		return a;
	}

	
	//void Write(CMessage &a,Size^ b) { a.Write(b); return a; }
	inline CMessage& operator>>(CMessage &a, bool &b) {
		a.Read(b);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, int8_t &b) {
		a.Read(b);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, uint8_t &b) {
		a.Read(b);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, int16_t &b) {
		a.Read(b);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, uint16_t &b) {
		a.Read(b);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, int32_t &b) {
		a.Read(b);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, uint32_t &b) {
		a.Read(b);
		return a;
	}
    inline CMessage& operator>>(CMessage &a, uint64_t  &b) {
		a.Read(b);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, int64_t  &b) {
		a.Read(b);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, float &b) {
		a.Read(b);
		return a;
	}
	inline CMessage& operator>>(CMessage &a, double &b) {
		a.Read(b);
		return a;
	}
	
	

	//inline CMessage& operator>>(CMessage &a,DateTime b)
	//{
	//    a.Read(b);
	//}

	//inline CMessage& operator<<(CMessage &a,DateTime b)
	//{
	//    a.Write(b);
	//}

	inline CMessage& operator>>(CMessage &a, AddrPort &b)
	{
		a.Read(b.m_addr.v6Byte, sizeof(b.m_addr));
		a.Read(b.m_port);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, AddrPort b)
	{
		a.Write(b.m_addr.v6Byte, sizeof(b.m_addr));
		a.Write(b.m_port);
		return a;
	}


	inline CMessage& operator>>(CMessage &a, ByteArray	&b)
	{
		a.Read(b);
		return a;
	}

	inline CMessage& operator<<(CMessage &a, const ByteArray &b)
	{
		a.Write(b);
		return a;
	}

	inline CMessage& operator>>(CMessage &a, ::timespec &tc)
	{
		int64_t tv_sec = 0;
		int32_t tv_nsec = 0;
		a.Read(tv_sec);
		a.Read(tv_nsec);

		// ikpil.choi 2017-02-24 : time_t 가 32bit, 64bit 환경에서 사이즈가 다르므로, 무조건 64bit 기준으로 하고, time_t 로 변환시켜 사용함
		tc.tv_sec = static_cast<time_t>(tv_sec);
		tc.tv_nsec = tv_nsec;

		return a;
	}

	inline CMessage& operator<<(CMessage &a, const ::timespec &tc)
	{
		// ikpil.choi 2017-02-24 : time_t 가 32bit, 64bit 환경에서 사이즈가 다르므로, 무조건 64bit 기준으로 하고, time_t 로 변환시켜 사용함
		a.Write(static_cast<int64_t>(tc.tv_sec));
		a.Write(static_cast<int32_t>(tc.tv_nsec));
		return a;
	}



	// vector에서 사용할 수 있는 serialization functions
	// for output to stream
	template<typename elem, bool elem_in_ref, bool RAWTYPE, typename INDEXTYPE>
	inline CMessage& operator>>(CMessage &a, CFastArray<elem, elem_in_ref, RAWTYPE, INDEXTYPE> &b)
	{
		// 배열 크기를 얻는다.
		int64_t length;
		a.ReadScalar(length);

		// length가 해킹되어 말도 안되는 값이면 실패 처리하기
		// 물론 모든 경우를 잡지는 못하지만 (sizeof elem 무용지물) 그래도 최소 1바이트는 쓸테니.
		// 원소의 개수가 바이트 개수보다 많은 경우는 무시.
		if (length< 0 || length > a.GetLength() - a.GetReadOffset())
			ThrowExceptionOnReadArray(length);

		// 배열 크기 리셋 및 capacity set
		b.SetCount((INDEXTYPE)length);

		// 각 배열 항목을 읽는다.
		for ( INDEXTYPE i = 0; i < (INDEXTYPE)length; i++ )
		{
			a >> b[i];
		}
		return a;
	}

	// vector, list등 unary item elem 등에서 사용할 수 있는 serialization functions
	// for input from stream
	template<typename elem, bool elem_in_ref, bool RAWTYPE, typename INDEXTYPE>
	inline CMessage& operator<<(CMessage &a, const CFastArray<elem, elem_in_ref, RAWTYPE, INDEXTYPE> &b)
	{
		// 배열 크기를 기록한다.
		int64_t size = (int64_t)b.GetCount();
		a.WriteScalar(size);

		// 각 배열 인자를 기록한다.
		for ( INDEXTYPE i = 0; i < (INDEXTYPE)size; i++ )
		{
			a << b[i];
		}
		return a;
	}

	inline CMessage& operator<<(CMessage &a, const ByteArray &b);
	inline CMessage& operator>>(CMessage &a, ByteArray &b);
	 PROUD_API CMessage& operator<<(CMessage& a, const NamedAddrPort &b);
	 PROUD_API CMessage& operator>>(CMessage& a, NamedAddrPort &b);

	 PROUD_API void AppendTextOut(String &a, const bool &b);
	 PROUD_API void AppendTextOut(String &a, const int8_t &b);
	 PROUD_API void AppendTextOut(String &a, const uint8_t &b);
	 PROUD_API void AppendTextOut(String &a, const int16_t &b);
	 PROUD_API void AppendTextOut(String &a, const uint16_t& b);
	 PROUD_API void AppendTextOut(String &a, const int32_t &b);
	 PROUD_API void AppendTextOut(String &a, const uint32_t& b);
	 PROUD_API void AppendTextOut(String &a, const int64_t &b);
	 PROUD_API void AppendTextOut(String &a, const uint64_t &b);

	 PROUD_API void AppendTextOut(String &a, const float  &b);
	 PROUD_API void AppendTextOut(String &a, const double &b);

	 PROUD_API void AppendTextOut(String &a, const char *&b);
	 PROUD_API void AppendTextOut(String &a, const wchar_t *&b);

	 PROUD_API void AppendTextOut(String &a, const StringA &b);
	 PROUD_API void AppendTextOut(String &a, const StringW &b);

	// NOTE: std.string 을 인자로 다루는 함수는 inline이다. 이렇게 안하면, 서비스팩이나 STL 라이브러리 벤더가 달라지는 것마다에 대한 커스텀 빌드를 다 만들어주어야 하니까.

	inline void AppendTextOut(String &a, const std::string & b)
	{
		a += _PNT("'");
		a += StringA2T(b.c_str());
		a += _PNT("'");
	}

	inline void AppendTextOut(String &a, const std::wstring & b)
	{
		a += _PNT("'");
		a += StringW2T(b.c_str());
		a += _PNT("'");
	}

#if defined(_WIN32) 
	PROUD_API void AppendTextOut(String& a, const POINT& b);
#endif

	

	template<typename elem>
	inline void AppendTextOut(String &a, const CFastArray<elem> &b)
	{
		Proud::String x;
		x.Format(_PNT("<Array %d>"), b.GetCount());
		a += x;
	}

	PROUD_API void AppendTextOut(String &a, const ByteArray &b);
	PROUD_API void AppendTextOut(String &a, const ::timespec &b);

	template<typename K, typename V>
	__forceinline CMessage& operator>>(CMessage &a, CFastMap<K,V> &b)
	{
		int64_t length;
		a.ReadScalar(length);

		// length가 해킹되어 말도 안되는 값이면 실패 처리하기
		// 물론 모든 경우를 잡지는 못하지만 (sizeof elem 무용지물) 그래도 최소 1바이트는 쓸테니.
		// 원소의 개수가 바이트 개수보다 많은 경우는 무시.
		if (length<0 || length > a.GetLength() - a.GetReadOffset())
             ThrowExceptionOnReadArray(length);

		b.Clear();

		for(int64_t i=0;i<length;i++)
		{
			K key; V val;
			a>>key;
			a>>val;
			b.Add(key,val);
		}
		return a;
	}

	template<typename K, typename V>
	__forceinline CMessage& operator<<(CMessage &a, const CFastMap<K,V> &b)
	{
		int64_t length = (int64_t)b.GetCount();
		a.WriteScalar(length);

        typedef typename CFastMap<K,V>::const_iterator ConstIter;

		for(ConstIter i=b.begin();i!=b.end();i++)
		{
			a<<i.GetFirst();
			a<<i.GetSecond();
		}
		return a;
	}

	template<typename K, typename V>
	__forceinline void AppendTextOut(String &a,const CFastMap<K,V> &b)
	{
		String temp;
		temp.Format(_PNT("FastMap(Size=%d){"), b.GetCount());
		a += temp;

         typedef typename CFastMap<K,V>::const_iterator ConstIter;

		for(ConstIter i=b.begin();i!=b.end();i++)
		{
			a+=_PNT("(");
			AppendTextOut(a, i->GetFirst());
			a+=_PNT(",");
			AppendTextOut(a, i->GetSecond());
			a+=_PNT(")");
		}
		a+=_PNT("}");
	}

    template<typename elem>
    __forceinline CMessage& operator>>(CMessage &a, CFastList<elem> &b)
    {
		int64_t length;
		a.ReadScalar(length);

		// length가 해킹되어 말도 안되는 값이면 실패 처리하기
		// 물론 모든 경우를 잡지는 못하지만 (sizeof elem 무용지물) 그래도 최소 1바이트는 쓸테니.
		// 원소의 개수가 바이트 개수보다 많은 경우는 무시.
		if (length<0 || length > a.GetLength() - a.GetReadOffset())
             ThrowExceptionOnReadArray(length);

		b.Clear();

        for(int64_t i=0;i<length;i++)
        {
            elem e;
            a>>e;
            b.AddTail(e);
        }
        return a;
    }

    template<typename elem>
    __forceinline CMessage& operator<<(CMessage &a, const CFastList<elem> &b)
    {
		int64_t length = (int64_t)b.GetCount();
		a.WriteScalar(length);

        typedef typename CFastList<elem>::const_iterator ConstIter;

        for(ConstIter i=b.begin();i!=b.end();i++)
        {
            a<< (*i);
        }
        return a;
    }

    template<typename elem>
    __forceinline void AppendTextOut(String &a,const CFastList<elem> &b)
    {
        Proud::String x;
		x.Format(_PNT("<FastList %d>"), b.GetCount());
        a += x;
    }

	// 가령 Entity Replication의 serialized data를 주고받을 때 쓴다.
	__forceinline CMessage& operator<<(CMessage& a, const ByteArrayPtr& b)
	{
		a << b.GetCount();
		a.Write(b.GetData(), b.GetCount());
		return a;
	}

	__forceinline CMessage& operator>>(CMessage& a, ByteArrayPtr& b)
	{
		if (b.IsNull())
		{
			b.UseInternalBuffer();
		}

		int length;
		a >> length;
		b.SetCount(length);
		a.Read(b.GetData(), length);

		return a;
	}

	__forceinline void AppendTextOut(String& a, const ByteArrayPtr& b)
	{
		Proud::String x;
		x.Format(_PNT("<ByteArrayPtr (length=%d)>"), b.GetCount());
		a += x;
	}

	/**
	\~korean
	사용자가 만든 marshaler 메서드가 정상적으로 작동하는지 테스트합니다.
	디버깅 용도로 사용하세요.

	\param testee 사용자가 만든 변수 타입의 객체입니다. 최대한 복잡하게 값을 넣어서 테스트하는 것을 권합니다.
	이 객체 타입은 Equals() 메서드를 멤버로 갖고 있어야 합니다. Equals()메서드는 두 값을 비교해서 같으면 true를 리턴해야
	합니다.
	\return 오류가 없으면 빈 문자열을, 있는 경우 오류 내용을 담아서 리턴합니다.

	사용 예
	\code
		MyType a;
		a = ...; // 뭔가를 담는다.
		String err = TestMarshaler(a);
		if(!err.IsEmpty())
		show_error(err);
	\endcode

	\~english
	It tests marshaler method that created by user.
	Please use it for debugging.

	\param testee Variable type object that created by user. We recommend to put complex value for testing.
	This object time has to have Equals() method as member. It has return ture when Equals() method compare two value and if it is the same.
	\return If there in no error, it will send an empty string. If there is error then return information of error.
	Example
	\code
		MyType a;
		a = ...; // Put something in
		String err = TestMarshaler(a);
		if(!err.IsEmpty())
		show_error(err);
	\endcode

	\~chinese
	测试用户创建的marshaler方法是否正常运行。
	请使用于调试用途。

	\param testee 用户制造的变数类型对象。建议输入最大程度复杂的值进行测试。
	此对象类型要把Equals()方法拥有成成员。Equals()方法比较两个值，一样的话要返回true。
	\return 没有错误的话返回空字符串，有的话记载错误内容并返回。

	使用例
	\code
		MyType a;
		a = ...; // 记任何东西
		String err = TestMarshaler(a);
		if(!err.IsEmpty())
		show_error(err);

	\~japanese
	ユーザーが作ったmarshalerメソッドが正常に作動するかをテストします。
	デバッギング用途で使ってください。
	\param testee ユーザーが作った変数タイプのオブジェクトです。最大限に複雑に値を入れてテストすることをお勧めします。
	このオブジェクトタイプは、Equals()メソッドをメンバーとして持っている必要があります。Equals()メソッドは両値を比較して同じであればtrueをリターンする必要があります。
	\return エラーがなければ空の文字列を、ある場合はエラー内容を入れてリターンします。

	使用例
	\code
	MyType a;
	a = ...; // 뭔가를 담는다.　何かを入れる。
	String err = TestMarshaler(a);
	if(!err.IsEmpty())
	show_error(err);
	\endcode

	\~
	*/
	template<typename elem>
	String TestMarshaler(const elem& testee)
	{
		CMessage msg;
		msg.UseInternalBuffer();
		msg << testee;

		int msgLength = msg.GetLength();
		msg.SetReadOffset(0);

		elem readee;
		msg >> readee;

		String errText;

		if(msg.GetLength() != msg.GetReadOffset())
		{
			errText.Format(_PNT("An object is serialized into %d bytes of data, but %d bytes are used for deserialization. Its marshaler methods may run incorrectly."), msgLength, msg.GetReadOffset());
			return errText;
		}

		if(!testee.Equals(readee))
		{
			errText = _PNT("The serialzed and deserialized object is not the same to the original object! Its marshaler methods may run incorrectly.");
		}

		return errText;
	}

	/**  @} */
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif

/// enum 타입에 대해서 <<,>>연산자 함수 등을 만드는 것이 귀찮으면 이 macro를 사용하세요.
#define PROUDNET_SERIALIZE_ENUM(type) \
namespace Proud \
{ \
	inline CMessage& operator<<(CMessage& a,type b) \
	{ \
		a<<(int32_t)b; \
		return a; \
	} \
	inline CMessage& operator>>(CMessage& a,type& b) \
	{ \
		int32_t x; \
		a>>x; \
		b=(type)x; \
		return a; \
	} \
	inline void AppendTextOut(String &a,type b) \
	{ \
		String txt; \
		txt.Format(_PNT("%d"),(int32_t)b); \
		a+=txt; \
	} \
}

PROUDNET_SERIALIZE_ENUM(Proud::ErrorType);
PROUDNET_SERIALIZE_ENUM(Proud::Retained::EntityID);
PROUDNET_SERIALIZE_ENUM(Proud::HostID);

#if defined(_WIN32) && defined(ATLASSERT)
    #include "AtlDependent.inl"
#endif

#if defined(_WIN32) && defined(_AFX)
#include "MfcDependent.inl"
#endif


