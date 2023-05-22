// SynchWorldClient.h : Main header file about PROJECT_NAME application program.
// SynchWorldClient.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
#error Include 'stdafx.h' before include this file from PCH.
#endif

#include "resource.h"		// Main symbol


// CSynchWorldClientApp:
// Please refer to SynchWorldClient.cpp about realize this class.
// 이 클래스의 구현에 대해서는 SynchWorldClient.cpp을 참조하십시오.
//

class CSynchWorldClientApp : public CWinApp
{
public:
	CSynchWorldClientApp();

// Re-definition
// 재정의
public:
	virtual BOOL InitInstance();

// Realization
// 구현

	DECLARE_MESSAGE_MAP()
};

extern CSynchWorldClientApp theApp;
