
// SimpleDB2Server.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
// SimpleDB2Server.h : Main header file about PROJECT_NAME application program.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "Include 'stdafx.h' before include this file about PCH."
#endif

#include "resource.h"		// Main symbol


// CSimpleDB2ServerApp:
// 이 클래스의 구현에 대해서는 SimpleDB2Server.cpp을 참조하십시오.
// Please refer SimpleDB2Server.cpp to realize this class.
//

class CSimpleDB2ServerApp : public CWinApp
{
public:
	CSimpleDB2ServerApp();

// 재정의입니다.
// Re-definition
	public:
	virtual BOOL InitInstance();

// 구현입니다.
// Realization

	DECLARE_MESSAGE_MAP()
};

extern CSimpleDB2ServerApp theApp;