// GCClient.h : Main header file about PROJECT_NAME application program.
// GCClient.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
#error Please include 'stdafx.h' before include this file at PCH.
#endif

#include "resource.h"		// Main symbol

/**
\~korean
클라이언트 어플리케이션 메인입니다.

\~english
Client application main

\~chinese
客户端应用main

\~japanese
クライアントアプリケーションのメインです。

\~
*/
class CGCClientApp : public CWinApp
{
public:
	CGCClientApp();

	/**
	\~korean
	재정의

	\~english
	Re-definition

	\~chinese
	重新定义

	\~japanese
	再定義

	\~
	*/
public:
	/**
	\~korean
	 CGCClientApp 초기화

	\~english
	Initialize CGCClientApp

	\~chinese
	初始化CGCClientApp

	\~japanese
	CGCClientAppの初期化

	\~
	*/
	virtual BOOL InitInstance();

	/**
	\~korean
	구현

	\~english
	Realization

	\~chinese
	实现

	\~japanese
	実装
	\~
	*/
	DECLARE_MESSAGE_MAP()
};

extern CGCClientApp theApp;
