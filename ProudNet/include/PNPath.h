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

#include "PNString.h"

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

#ifdef _WIN32
#define PN_PATH_SEPARATOR_TCHAR     _PNT('\\')
#define PN_PATH_SEPARATOR_TSTRING   _PNT("\\")
#else
#define PN_PATH_SEPARATOR_TCHAR     _PNT('/')
#define PN_PATH_SEPARATOR_TSTRING   _PNT("/")
#endif

namespace Proud
{
	class Path
	{
	public:
		/**
		파일명의 확장자를 출력합니다. 예를 들어 c:\aaa\bbb\ccc.txt의 출력은 .txt입니다.
		*/
		PROUD_API static Proud::String GetExtension(const Proud::String& fileName);

		/**
		확장자를 없앤 문자열을 출력합니다. 예를 들어 c:\xxx\aaa.bat는 c:\xxx\aaa가 됩니다.
		*/
		PROUD_API static String RemoveExtension(const Proud::String& fileName);

		/** directory와 fileName을 이어붙인 문자열을 만듭니다.
		directory 뒤에 역슬래시나 슬래시가 있는 것을 고려합니다. */
		PROUD_API static String Combine(const String& directory, const String& fileName); 

		/** 디렉터리 부분을 제외한 순수한 파일 이름만을 얻습니다.
		예를 들어 c:\xxx\yyy.txt이면 yyy.txt만을 리턴합니다. */
		PROUD_API static String GetFileName(const PNTCHAR* fileName);

		/** 파일 이름의 확장자를 변경합니다. 
		가령 c:\xxx\yyy.txt와 .md를 넣으면 c:\xxx\yyy.md가 됩니다. */
		PROUD_API static String ChangeExtension(const String& fileName, const PNTCHAR* fileExtensionWithDot);

		/** 파일이름의 뒤에 뭔가를 붙입니다.
		가령 c:\xxx\yyy.txt와 zzz를 넣으면 c:\xxx\yyyzzz.txt가 됩니다. */
		PROUD_API static String AppendFileNameWithoutExt(const String& fileName, const String& postfix);

		/** 파일 이름의 부모 디렉토리 이름을 출력합니다.
		예를 들어 c:\aaa\bbb\ccc.txt의 출력은 c:\aaa\bbb 입니다. C#의 Path.GetDirectoryName()처럼요.
		*/
		PROUD_API static String GetDirectoryName(const PNTCHAR* fileName);
	};
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif