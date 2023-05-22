// ChatServer.h : Main header file of PROJECT_NAME application program.
// ChatServer.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// Main symbol


// CChatServerApp:
// Please refer to ChatServer.cpp to realize this class.
// 이 클래스의 구현에 대해서는 ChatServer.cpp을 참조하십시오.
//

class CChatServerApp : public CWinApp
{
public:
	CChatServerApp();

// Re-definition
// 재정의
public:
	virtual BOOL InitInstance();

// Realization
// 구현

	DECLARE_MESSAGE_MAP()
};

extern CChatServerApp theApp;
