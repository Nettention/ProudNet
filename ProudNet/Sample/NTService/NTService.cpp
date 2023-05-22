#include "stdafx.h"
#include "NTService.h"
#include "../../include/NTService.h"
#include "conio.h"

using namespace Proud;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 유일한 응용 프로그램 개체입니다.
// Unique application program object.

CWinApp theApp;

using namespace std;

class CMySvrEvent: public INTServiceEvent
{
public:
	virtual void Log(int type, LPCWSTR text)
	{
		_tprintf(L"%s\n", text);
	}
	virtual void Run()
	{
		while (1)
		{
			Sleep(100);
			if (_kbhit())
			{
				int ch = _getch();
				switch (ch)
				{
				case 27:
					return;
				}
			}

			MSG msg;
			// 최대 일정 짧은 시간동안, 콘솔 입력, 윈도 메시지 수신, 메인 스레드 종료 중 하나를 기다린다.
			// Waiting one of console input, receiving windows message, close main thread during short period of time.
			MsgWaitForMultipleObjects(0, 0, TRUE, 100, QS_ALLEVENTS);
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (!GetMessage(&msg, NULL, NULL, NULL))
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	virtual void Pause()
	{
		printf("pause");
	}

	virtual void Stop()
	{
		printf("stop");
	}

	virtual void Continue()
	{
		printf("continue");
	}

} g_svrEvent;

int _tmain(int argc, WCHAR* argv[], WCHAR* envp[])
{
	int nRetCode = 0;

	// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
	// Initialize MFC. If it could not then print error.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		// TODO: Modify error code if you need.
		_tprintf(L"Critical Error: Failed to reset MFC.\n");
		nRetCode = 1;
	}
	else
	{
		CNTServiceStartParameter param;
		param.m_serviceName = L"ProudNet NT Service Sample";
		param.m_serviceEvent = &g_svrEvent;

		CNTService::WinMain(argc, argv, envp, param);
	}

	return nRetCode;
}
