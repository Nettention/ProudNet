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

#include <atlcomtime.h>
#include <atlstr.h>

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

/** 이 모듈은 MFC나 D3DX에 의존적인 심볼들을 포함한다.
- __forceinline으로 선언된 이유는 compile time에서 코드를 생성하되 MFC 사용 여부에 따라 실 인스턴스 타입이
달라지는 클래스(CString 등)이 ProudNet과 충돌하지 않게 하기 위함이다.
물론, ProudNet에는 아래 심볼들이 컴파일되어 들어가있지 않다. */
namespace Proud
{
	__forceinline CMessage& operator >> (CMessage &a, COleDateTime &b)
	{
		DATE t;
		a >> t;
		b = t;
		return a;
	}

	__forceinline CMessage& operator<<(CMessage &a, const COleDateTime &b)
	{
		DATE t = b;
		a << t;
		return a;
	}

	__forceinline CMessage& operator >> (CMessage &a, CTime &b)
	{
		__time64_t t;
		a >> t;
		b = t;
		return a;
	}

	__forceinline CMessage& operator<<(CMessage &a, const CTime &b)
	{
		__time64_t t = b.GetTime();
		a << t;
		return a;
	}

	__forceinline CMessage& operator >> (CMessage &a, COleDateTimeSpan &b)
	{
		a.Read((uint8_t*)&b, sizeof(b));
		return a;
	}


	__forceinline CMessage& operator<<(CMessage &a, const COleDateTimeSpan &b)
	{
		a.Write((const uint8_t*)&b, sizeof(b));
		return a;
	}

	__forceinline void AppendTextOut(String &a, COleDateTime &b)
	{
#if defined(_UNICODE)
		a += b.Format();
#else
		a += StringA2T(b.Format());
#endif
	}
	__forceinline void AppendTextOut(String &a, COleDateTimeSpan &b)
	{
#if defined(_UNICODE)
		a += b.Format((uint32_t)0);
#else
		a += StringA2T(b.Format((uint32_t)0));
#endif
	}

	__forceinline void AppendTextOut(String &a, CTime &b)
	{
#if defined(_UNICODE)
		a += b.Format(_PNT("%#c"));
#else
		a += StringA2T(b.Format("%#c"));
#endif
	}
	__forceinline void AppendTextOut(String &a, CTimeSpan &b)
	{
#if defined(_UNICODE)
		a += b.Format(_PNT("%#c"));
#else
		a += StringA2T(b.Format("%#c"));
#endif
	}

	// for MFC CString class
	// note: 이미 non-MFC의 CString class(CAtlString)은 마샬링 함수가 따로 있으므로 _AFX 여부를 구별한다.
	__forceinline CMessage& operator >> (CMessage &a, CStringA &b)
	{
		// 읽어서 CStringA로 변환
		StringA x;
		a >> x;
		b = x.GetString();
		return a;
	}

	// for MFC CString class
	__forceinline CMessage& operator<<(CMessage &a, const CStringA &b)
	{
		// StringA로 변환 후 출력
		StringA x;
		x = b.GetString();
		a << x;
		return a;
	}

	// for MFC CString class
	__forceinline void AppendTextOut(String &a, CStringA &b)
	{
		a += StringA2T(b);
	}

	// for MFC CString class
	__forceinline CMessage& operator >> (CMessage &a, CStringW &b)
	{
		// 읽어서 CStringW로 변환
		String x;
		a >> x;
		b = StringT2W(x).GetString();
		return a;
	}

	// for MFC CString class
	__forceinline CMessage& operator<<(CMessage &a, const CStringW &b)
	{
		// String로 변환 후 출력
		String x;
		x = StringW2T(b);
		a << x;
		return a;
	}

	// for MFC CString class
	__forceinline void AppendTextOut(String &a, CStringW &b)
	{
		a += StringW2T(b);
	}

	//#TODO-MFC 여기다 CAtlArray, CAtlMap에 대한 operator<<,>>,AppendTextOut을 구현하세요.
	// stlmarshal.inl에 있는 std.vector, std.map을 복붙해서 고쳐만드세요.
	// size()대신 GetCount가 쓰이고 size_t대신 intptr_t 혹은 int를 쓰세요. 어떤걸 써야 하는지는 CAtlArray등의 구현부를 참고해서 하시고요.


#ifdef _AFX
	// 	__forceinline CMessage& operator<<(CMessage& a,const CPoint &b)
	// 	{
	// 		a<<b.x<<b.y;
	// 		return a;
	// 	}
	//
	//
	// 	__forceinline CMessage& operator>>(CMessage& a,CPoint &b)
	// 	{
	// 		a>>b.x>>b.y;
	// 		return a;
	// 	}
	//
	// 	__forceinline void AppendTextOut(String &a,CPoint &b)
	// 	{
	// 		String txt;
	// 		txt.Format(_PNT("(%d,%d)"),b.x,b.y);
	// 		a+=txt;
	// 	}
#endif

#ifdef PREPARED_D3DX_MARSHALER

	__forceinline CMessage& operator >> (CMessage &a, D3DXVECTOR3 &b)
	{
		a.Read(b.x);
		a.Read(b.y);
		a.Read(b.z);
		return a;
	}
	__forceinline CMessage& operator<<(CMessage &a, const D3DXVECTOR3 &b)
	{
		a.Write(b.x);
		a.Write(b.y);
		a.Write(b.z);
		return a;
	}

	__forceinline CMessage& operator >> (CMessage &a, D3DXMATRIX &b)
	{
		a.Read((uint8_t*)&b, sizeof(b));
		return a;
	}
	__forceinline CMessage& operator<<(CMessage &a, const D3DXMATRIX &b)
	{
		a.Write((const uint8_t*)&b, sizeof(b));
		return a;
	}

	__forceinline CMessage& operator >> (CMessage &a, D3DXQUATERNION &b)
	{
		a.Read((uint8_t*)&b, sizeof(b));
		return a;
	}
	__forceinline CMessage& operator<<(CMessage &a, const D3DXQUATERNION &b)
	{
		a.Write((const uint8_t*)&b, sizeof(b));
		return a;
	}

	__forceinline CMessage& operator >> (CMessage &a, D3DXVECTOR2 &b)
	{
		a >> b.x >> b.y;
		return a;
	}

	__forceinline CMessage& operator<<(CMessage &a, const D3DXVECTOR2 &b)
	{
		a << b.x << b.y;
		return a;
	}

	__forceinline void AppendTextOut(String &a, const D3DXMATRIX &b)
	{
		a += _PNT("<D3DXMATRIX>");
	}
	__forceinline void AppendTextOut(String &a, const D3DXQUATERNION &b)
	{
		String f;
		f.Format(_PNT("{x=%f,y=%f,z=%f,w=%f}"), b.x, b.y, b.z, b.w);
		a += f;
	}
	__forceinline void AppendTextOut(String &a, const D3DXVECTOR3 &b)
	{
		String f;
		f.Format(_PNT("{x=%f,y=%f,z=%f}"), b.x, b.y, b.z);
		a += f;
	}

	__forceinline void AppendTextOut(String &a, const D3DXVECTOR2 &b)
	{
		String f;
		f.Format(_PNT("{x=%f,y=%f}"), b.x, b.y);
		a += f;
	}

#endif // PREPARED_D3DX_MARSHALER

#ifdef _INC_COMDEF

#include <string>

	__forceinline CMessage& operator<<(CMessage& packet, const _variant_t &rhs)
	{
		// 모든 VARTYPE을 지원하지는 않는다. 따라서 필요에 따라 추가 기능을 구현하자.
		packet.Write((uint16_t)rhs.vt);

		switch (rhs.vt)
		{
		case VT_I1:
		case VT_I2:
		case VT_I4:
		case VT_I8:
		case VT_INT:
			// C++/CLI에서 int64_t으로 형변환하면 빌드 실패하여 아래의 방법으로 변경.
			//packet.WriteScalar((int64_t)rhs);
			packet.WriteScalar(rhs.llVal);
			break;
		case VT_R4:
			packet.Write((float)rhs);
			break;
		case VT_R8:
		case VT_DATE:
			packet.Write((double)rhs);
			break;
		case VT_CY:
		{
			CY val = (CY)rhs;
			packet.Write(val.int64);
		}
		break;
		case VT_UI1:
		case VT_UI2:
		case VT_UI4:
		case VT_UI8:
		case VT_UINT:
			// C++/CLI에서 int64_t으로 형변환하면 빌드 실패하여 아래의 방법으로 변경.
			//packet.WriteScalar((uint64_t)rhs);
			packet.WriteScalar(rhs.ullVal);
			break;
		case VT_BOOL:
			packet.Write((bool)rhs);
			break;
		case VT_BSTR:
		case VT_LPWSTR:
		case VT_LPSTR:
		{
			String val = StringW2T(rhs.bstrVal);
			packet.WriteString(val);
		}
		break;
		case VT_HRESULT:
			packet.Write((int32_t)rhs);
			break;
		case VT_SAFEARRAY:
		case VT_UI1 | VT_ARRAY:
			//case VT_ARRAY:
		{
			uint8_t* pBytes = NULL;

			uint32_t lSize = rhs.parray->rgsabound->cElements;
			HRESULT hr = SafeArrayAccessData(rhs.parray, (void**)&pBytes);

			if (FAILED(hr))
				throw Exception("operator << Failed - SafeArrayAccessData Failed!!");

			packet.Write((const uint8_t*)&lSize, sizeof(lSize));
			packet.Write(pBytes, lSize);

			SafeArrayUnaccessData(rhs.parray);
		}
		break;
		case VT_DECIMAL:
			//나중에 구현...
		{
			packet.Write(rhs.decVal.signscale);
			packet.Write((uint32_t)rhs.decVal.Hi32);
			packet.Write((uint64_t)rhs.decVal.Lo64);
		}
		break;
		case VT_CLSID:
		{
			String val = StringW2T(rhs.bstrVal);
			packet.WriteString(val);
		}
		break;
		}

		return packet;
	}


	__forceinline CMessage& operator >> (CMessage& packet, _variant_t &rhs)
	{
		// 모든 VARTYPE을 지원하지는 않는다. 따라서 필요에 따라 추가 기능을 구현하자.
		uint16_t vt;
		packet.Read(vt);

		rhs.Clear();

		// VT_UI1|VT_ARRAY의 경우에는 처리하지 않습니다.
		// 대신, 아래 switch-case 구문에서 이러한 케이스를 다룰 때 vt가 세팅되므로 ok.
		if (vt != (VT_UI1 | VT_ARRAY))
		{
			rhs.ChangeType(vt);
		}

		switch (vt)
		{
		case VT_I1:
		case VT_I2:
		case VT_I4:
		case VT_I8:
		case VT_INT:
		{
			int64_t val;
			packet.ReadScalar(val);

			rhs.llVal = val;
		}
		break;
		case VT_R4:
		{
			float val;
			packet.Read(val);

			rhs = val;
		}
		break;
		case VT_R8:
		case VT_DATE:
		{
			double val;
			packet.Read(val);

			rhs = val;
		}
		break;
		case VT_CY:
		{
			CY val;
			packet.Read(val.int64);

			rhs = val;
		}
		break;
		case VT_UI1:
		case VT_UI2:
		case VT_UI4:
		case VT_UI8:
		case VT_UINT:
		{
			uint64_t val;
			packet.ReadScalar(val);

			rhs.ullVal = val;
		}
		break;
		case VT_BOOL:
		{
			bool val;
			packet.Read(val);

			rhs = val;
		}
		break;
		case VT_BSTR:
		case VT_LPWSTR:
		case VT_LPSTR:
		{
			String val;
			packet.ReadString(val);
			rhs = val.GetString();
		}
		break;
		case VT_HRESULT:
		{
			int32_t val;
			packet.Read(val);

			rhs = (HRESULT)val;
		}
		break;
		case VT_SAFEARRAY:
		case VT_UI1 | VT_ARRAY:
			//case VT_ARRAY:
		{
			uint32_t length = 0;
			VARIANT v;
			SAFEARRAY FAR* psa;
			SAFEARRAYBOUND saBound;
			uint8_t byIn = 0;

			packet.Read((uint8_t*)&length, sizeof(length));

			saBound.cElements = length;
			saBound.lLbound = 0;
			psa = SafeArrayCreate(VT_UI1, 1, &saBound);

			VariantInit(&v);
			V_VT(&v) = VT_ARRAY | VT_UI1;
			V_ARRAY(&v) = psa;

			for (long i = 0; i < (long)length; ++i)
			{
				packet >> byIn;

				HRESULT ret = SafeArrayPutElement(psa, &i, (void*)&byIn);
				if (S_OK != ret)
				{
					Tstringstream strStm;
					strStm << _PNT("SafeArrayPutElement in ") << StringA2T(__FUNCTION__).GetString() << " returns error " << ret;
					throw Proud::Exception(strStm.str().c_str());
				}
			}

			VariantClear(&rhs); // 이게 없으면 아래 구문에 의해 누수로 이어질 수 있음. 기존에 있던걸 없애야 하니까.
			rhs = v;
		}
		break;
		case VT_DECIMAL:
		{
			USHORT signscale;
			uint32_t Hi32;
			uint64_t Lo64;
			packet.Read(signscale);
			packet.Read(Hi32);
			packet.Read(Lo64);

			rhs.decVal.signscale = signscale;
			rhs.decVal.Hi32 = Hi32;
			rhs.decVal.Lo64 = Lo64;
		}
		break;
		case VT_CLSID:
		{
			String val;
			packet.ReadString(val);
			rhs.bstrVal = _bstr_t(val.GetString());
		}
		break;
		}

		return packet;
	}

	__forceinline void AppendTextOut(String &a, const _variant_t &b)
	{
		a += _PNT("<_variant_t>");
	}
#endif // _INC_COMDEF
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
