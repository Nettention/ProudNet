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

#include "BasicTypes.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

namespace Proud
{	
	/** \addtogroup util_group
	*  @{
	*/

	class CPnIconv;
	class CStringEncoderImpl;

	/** 
	\~korean
	iconv 래퍼 클래스입니다.
	- Create로 생성 후 필요할 때마다 로컬 변수로서 GetIconv-ReleaseIconv를 쓰시면 됩니다.
	- 여러 스레드에서의 동시 접근을 위한 오브젝트 풀링의 형태로 iconv 객체를 제공합니다. 
	따라서 속도가 빠릅니다. 

	ProudNet은 문자열 처리를 위해 윈도에서도 이것을 사용해서 작동합니다. 
	과거에는 win32 기능을 사용했지만 멀티플랫폼 단일화를 위해서입니다.

	\~english
	It is a wrapper class. 
	- After creating by “create”, you can use GetIconv-ReleaseIconv as the local variable whenever needed. 
	- iconv objective is provided as the form of object pooling for concurrent access to many threads. 
	As a result of this, it is fast. 

	ProudNet used win32 function in the past but for unifying multi-platform, it uses this in Windows for string processing.

	\~chinese
	iconv 是包装类。
	- 用Create生成后，在必要时以本地变量使用GetIconv-ReleaseIconv即可。
	- 是在多个线程中为进行同时接近的对象池形式，提供iconv对象。因此速度较快。 

	为处理字符串，ProudNet在Windows中也使用此iconv运行。
	过去使用了win32功能，但现在使用iconv是为了多平台的单一化。

	\~japanese
	iconv　wrapperクラスです。.
	- Createで生成してから必要な時にローカル変数として GetIconv-ReleaseIconvを使ってください。
	- 色々なスレッドでの同時接近のためのオブジェクプーリングの形態で iconv オブジェクトを提供します。 
	　したがって速度が速いです。

	 ProudNetは文字列処理のためにWindowsでもこれを使って作動します。 
	 過去には win32機能を使用しましたが、現在はマルチプラットフォーム単一化のために次のように変更されました。

	\~

	Example code:
	\code
	StringA2W(a, b); // use platform-specific default encoder

	// use the encoder you select
	shared_ptr<CStringEncoder> e(CStringEncoder::Create("UTF-8", "UTF-16LE"));
	StringA2W(a, b, e);
	\endcode

	*/
	class CStringEncoder
	{
	private:
		CStringEncoder(const char* srcCodepage, const char* destCodepage);
	public:
		PROUD_API static CStringEncoder* Create(const char* srcCodepage, const char* destCodepage);
		virtual ~CStringEncoder();
		CPnIconv* GetIconv();
		void ReleaseIconv(CPnIconv *obj);
	private:
		CStringEncoderImpl* m_pimpl;
	};

	 void iconv_string_convert(CStringEncoder* encoder, const char* input, size_t* inbytesleft, char* out, size_t* outbytesleft);

/**  @} */
}
#ifdef _MSC_VER
#pragma pack(pop)
#endif
