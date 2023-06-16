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

#include "Marshaler.h"
#include <vector>
#include <list>
#include <map>

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{
	template<typename K, typename V>
	__forceinline CMessage& operator>>(CMessage &a, std::map<K, V> &b)
	{
		int64_t length;
		a.ReadScalar(length);

		b.clear();

		for (int64_t i = 0; i<length; i++)
		{
			K key; V val;
			a>>key>>val;
			b.insert(typename std::map<K, V>::value_type(key, val));
		}
		return a;
	}

	template<typename K, typename V>
	__forceinline CMessage& operator<<(CMessage &a, const std::map<K, V> &b)
	{
		int64_t length = b.size();
		a.WriteScalar(length);

		for ( typename std::map<K, V>::const_iterator i=b.begin(); i!=b.end(); ++i )
		{
            const K& key = i->first;
            const V& val = i->second;
			a << key << val;
		}
		return a;
	}

	template<typename K, typename V>
	__forceinline void AppendTextOut(String &a, const std::map<K, V> &b)
	{
		String temp;
		temp.Format(_PNT("std::map(Size=%d){"), b.size());
		a += temp;
		for ( typename std::map<K, V>::const_iterator i=b.begin(); i!=b.end(); ++i )
		{
            const K& key = i->first;
            const V& val = i->second;
            
			a+=_PNT("(");
			AppendTextOut(a, key);
			a+=_PNT(",");
			AppendTextOut(a, val);
			a+=_PNT(")");
		}
		a+=_PNT("}");
	}

	// vector에서 사용할 수 있는 serialization functions
	// for output to stream
	template<typename elem>
	__forceinline CMessage& operator>>(CMessage &a, std::vector<elem> &b)
	{
		// 크기를 얻는다.
		int64_t length;
		a.ReadScalar(length);

		// length가 해킹되어 말도 안되는 값이면 실패 처리하기
		// 물론 모든 경우를 잡지는 못하지만 (sizeof elem 무용지물) 그래도 최소 1바이트는 쓸테니.
		// 원소의 개수가 바이트 개수보다 많은 경우는 무시.
		if (length<0 || length > a.GetLength() - a.GetReadOffset())
			ThrowExceptionOnReadArray(length);

		// 메모리 frag를 줄이기 위해
        b.reserve((size_t)length);
		b.resize(0);

		// 배열 항목 하나 하나를 읽는다.
		for (size_t i = 0; i < (size_t)length; i++)
		{
            elem e;
			a >> e;
            b.push_back(e);
		}
		return a;
	}

	template<typename elem>
	__forceinline CMessage& operator<<(CMessage &a, const std::vector<elem> &b)
	{
		// 배열 크기를 기록한다.
		int64_t size = (int64_t)b.size();
		a.WriteScalar(size);

		// 각 배열 인자를 기록한다.
		for ( typename std::vector<elem>::const_iterator i = b.begin(); i != b.end(); ++i )
		{
            const elem& e = *i;
			a << e;
		}
		return a;
	}

	template<typename elem>
	__forceinline void AppendTextOut(String &a, std::vector<elem> &b)
	{
		Proud::String x;
		x.Format(_PNT("<std::vector %d>"), b.size());
		a += x;
	}

	template<typename elem>
	__forceinline CMessage& operator>>(CMessage &a, std::list<elem> &b)
	{
		// 크기를 얻는다.
		int64_t length;
		a.ReadScalar(length);

		// length가 해킹되어 말도 안되는 값이면 실패 처리하기
		// 물론 모든 경우를 잡지는 못하지만 (sizeof elem 무용지물) 그래도 최소 1바이트는 쓸테니.
		// 원소의 개수가 바이트 개수보다 많은 경우는 무시.
		if (length<0 || length >(int64_t)(a.GetLength() - a.GetReadOffset()))
			ThrowExceptionOnReadArray(length);

		b.clear();

		// 배열 항목 하나 하나를 읽는다.
		for ( int64_t i = 0; i < length; i++ )
		{
			elem e;
			a >> e;
			b.push_back(e);
		}
		return a;
	}

	template<typename elem>
	__forceinline CMessage& operator<<(CMessage &a, std::list<elem> &b)
	{
		int64_t size = (int64_t)b.size();
		a.WriteScalar(size);

		for ( typename std::list<elem>::const_iterator i = b.begin(); i != b.end(); ++i )
		{
            const elem& e = *i;
			a << e;
		}

		return a;
	}

	template<typename elem>
	__forceinline void AppendTextOut(String &a, std::list<elem> &b)
	{
		Proud::String x;
		x.Format(_PNT("<std::list %d>"), b.size());
		a += x;
	}
}
#ifdef _MSC_VER
#pragma pack(pop)
#endif