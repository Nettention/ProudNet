
// SimpleDB2Client_2008.h : Main header file about PROJECT_NAME application program.
// SimpleDB2Client_2008.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "Include 'stdafx.h' before include this file about PCH."
#endif

#include "resource.h"		// Main symbol


// CSimpleDB2ClientApp:
// Please refer SimpleDB2Client_2008.cpp to realize this class.
// 이 클래스의 구현에 대해서는 SimpleDB2Client_2008.cpp을 참조하십시오.
//

class CSimpleDB2ClientApp : public CWinApp
{
public:
	CSimpleDB2ClientApp();

// Re-definition
// 재정의입니다.
	public:
	virtual BOOL InitInstance();

// Realization
// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CSimpleDB2ClientApp theApp;