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

/**
\~korean
즐겨 쓰는 헤더 파일들을 모은 파일이다.
ProudNet을 쓰기 위해서 반드시 include해야 하는 파일은 아니다.

\~english
A collection of favorite header files.
There is no must need to #include this file when using ProudNet.

\~chinese
是常用头文件的收藏文件。
并不是使用ProudNet所必须要#include 的文件。

\~japanese
お気に入りのヘッダーファイルを集めたファイルです。
ProudNetを使うためには必ず#includeすべきファイルではありません。
\~
*/

#pragma once


#if defined(_WIN32)
#include <assert.h>
#include <algorithm>
//#include <comdef.h> 서버에서만 쓰자. UE4 때문에 클라는 쓰면 안됨.
#include <conio.h>
#include <conio.h>
#include <crtdbg.h>
#include <ctype.h>
#include <deque>
#include <dimm.h>
#include <direct.h>
#include <exception>
#include <float.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <mbstring.h>
#include <memory.h>
#include <mmreg.h>
#include <WinSock2.h>
#include <mmsystem.h>
//#include <new>
#include <oleauto.h>
#include <process.h>
#include <queue>
//#include <rpcdce.h> UE4 비호환되므로 삭제
#include <set>
#include <shellapi.h>
#include <sstream>
#include <stack>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string>
#include <tchar.h>
#include <time.h>
#include <unknwn.h>
#include <usp10.h>
#include <utility>
#include <vector>
#include <wincrypt.h>
#include <windows.h>
#include <windowsx.h>
#include <wininet.h>
#include <winsock2.h>
#include <xmemory>
#endif
