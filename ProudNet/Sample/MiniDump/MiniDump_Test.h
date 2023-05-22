// MiniDump_Test.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
// MiniDump_Test.h : Main header file about PROJECT_NAME application program.
//

#pragma once

#ifndef __AFXWIN_H__
#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// Main symbol


// CMiniDump_TestApp:
// �� Ŭ������ ������ ���ؼ��� MiniDump_Test.cpp�� �����Ͻʽÿ�.
// Please refer MiniDump_Test.cpp about realizing this class.
//

class CMiniDump_TestApp : public CWinApp
{
public:
	CMiniDump_TestApp();

// ������
// Re-definition
public:
	virtual BOOL InitInstance();

// ����
// Realization

	DECLARE_MESSAGE_MAP()
};

extern CMiniDump_TestApp theApp;
