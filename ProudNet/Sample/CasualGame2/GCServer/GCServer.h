#pragma once

#include "resource.h"		// Main symbol

class CGCServerDlg;


/**
\~korean
어플리케이션 메인입니다.

\~english
Application main

\~chinese
是应用 main。

\~japanese
アプリケーションのメインです。

\~
*/
class CGCServerApp : public CWinApp
{
public:
	CGCServerApp();

// Re-definition
// 재정의
public:
	virtual BOOL InitInstance();

// Realization
// 구현

	DECLARE_MESSAGE_MAP()
};

extern CGCServerApp theApp;
