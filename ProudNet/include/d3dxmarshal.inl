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

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{

	inline CMessage& operator>>(CMessage &a, D3DXVECTOR3 &b)
	{
		a.Read(b.x);
		a.Read(b.y);
		a.Read(b.z);
		return a;
	}
	inline CMessage& operator<<(CMessage &a, const D3DXVECTOR3 &b)
	{
		a.Write(b.x);
		a.Write(b.y);
		a.Write(b.z);
		return a;
	}

	inline CMessage& operator>>(CMessage &a, D3DXMATRIX &b)
	{
		a.Read((uint8_t*)&b,sizeof(b));
		return a;
	}
	inline CMessage& operator<<(CMessage &a, const D3DXMATRIX &b)
	{
		a.Write((const uint8_t*)&b,sizeof(b));
		return a;
	}

	inline CMessage& operator>>(CMessage &a, D3DXQUATERNION &b)
	{
		a.Read((uint8_t*)&b,sizeof(b));
		return a;
	}
	inline CMessage& operator<<(CMessage &a, const D3DXQUATERNION &b)
	{
		a.Write((const uint8_t*)&b,sizeof(b));
		return a;
	}

	inline CMessage& operator>>(CMessage &a,D3DXVECTOR2 &b)
	{
		a>>b.x>>b.y;
		return a;
	}

	inline CMessage& operator<<(CMessage &a,const D3DXVECTOR2 &b)
	{
		a<<b.x<<b.y;
		return a;
	}

	inline void AppendTextOut(String &a,const D3DXMATRIX &)
	{
		a+=_PNT("<D3DXMATRIX>");
	}
	inline void AppendTextOut(String &a,const D3DXQUATERNION &b)
	{
		String f;
		f.Format(_PNT("{x=%f,y=%f,z=%f,w=%f}"),b.x,b.y,b.z,b.w);
		a+=f;
	}
	inline void AppendTextOut(String &a,const D3DXVECTOR3 &b)
	{
		String f;
		f.Format(_PNT("{x=%f,y=%f,z=%f}"),b.x,b.y,b.z);
		a+=f;
	}

	inline void AppendTextOut(String &a,const D3DXVECTOR2 &b)
	{
		String f;
		f.Format(_PNT("{x=%f,y=%f}"),b.x,b.y);
		a+=f;
	}

}
#ifdef _MSC_VER
#pragma pack(pop)
#endif