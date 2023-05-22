// MiniDumpServer.cpp : Define entance point about console application program.
// MiniDumpServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "MiniDumpServer.h"
#include "MyDumpServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Unique application program object.
// ������ ���� ���α׷� ��ü�Դϴ�.

CWinApp theApp;

using namespace std;

int _tmain(int argc, WCHAR* argv[], WCHAR* envp[])
{
	int nRetCode = 0;

	// Initialize MFC. If it could not initializing then print error.
	// MFC�� �ʱ�ȭ�մϴ�. �ʱ�ȭ���� ���� ��� ������ �μ��մϴ�.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: Modify error code if it need.
		// TODO: ���� �ڵ带 �ʿ信 ���� �����մϴ�.
		_tprintf(L"Critical Error: Failed to reset MFC.\n");
		nRetCode = 1;
	}
	else
	{
		// TODO: Coding motion of application program at here.3.
		// TODO: ���� ���α׷��� ������ ���⿡�� �ڵ��մϴ�.
		CMyDumpServer srv;
		srv.Run();
	}

	return nRetCode;
}
