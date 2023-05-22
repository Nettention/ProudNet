#include "StdAfx.h"
#include <atlpath.h>
#include ".\dumpreporterimpl.h"
#include ".\dumpreporterdlg.h"
#include "../../include/DumpCommon.h"

using namespace Proud;

#define _COUNTOF(array) (sizeof(array)/sizeof(array[0]))

CDumpReporter::CDumpReporter(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

}

CDumpReporter::~CDumpReporter(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
}

/** When user program has started then call this function immediatley.
This program is running with special parameter when user program occured minu dump and closed.
If it runs with that special parameter, this function catch that then send dump file to server.
\param cmdLine When this program run, put command argument manually to here.
\param host Internet address of web server which collect dump file. For example, dump.mygame.net
\param page Web page address which collect dump file. For example, /ASP/MyDump.asp */

/** 유저 프로그램이 시작하면 즉시 이 함수를 호출하도록 하라.
만약 유저 프로그램이 미니 덤프를 발생하고 종료할 때 특수 파라메터가 포함된 채로 이 프로그램이 실행된다.
그 특수 파라메터가 들어간 채로 실행되는 경우 이 함수가 캐취해서 서버에 덤프 파일을 보내는 역할을 한다.
\param cmdLine 이 프로그램이 실행될 때 커맨드 아규먼트를 직접 여기에 넣어주도록 하라.
\param host 덤프 파일을 수집하는 웹 서버의 인터넷 주소. 예컨대 dump.mygame.net

\param page 덤프 파일을 수집하는 웹 페이지의 주소. 예컨대 /ASP/MyDump.asp */
bool CDumpReporter::DoReportByExecParam( LPCWSTR cmdLine, LPCWSTR host, WORD port )
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_serverAddr = host;
	m_serverPort = port;
	//AfxMessageBox(cmdLine);

	/** In case of mini dump file creation due to error occurrence.
	In this case, MiniDumpAction_AlarmCrash will return from Startup function of newly created process.
	Please deal with created dump file according to user request.
	DumpReporter dialog is executed here. */

	/** 오류가 발생하여 미니 덤프 파일을 생성하는 경우
	이 경우에는 새롭게 생성된 프로세스에서의 Startup 함수에서는 MiniDumpAction_AlarmCrash의 값이 리턴된다.
	생성된 덤프 파일을 사용자의 요구사항에 따라서 처리하면 된다.
	여기서는 DumpReporter 다이얼로그를 실행하도록 하였다. */

	TCHAR dumpFileName[LongMaxPath] = { 0, };
	GetDumpFilePath(dumpFileName);
	m_dumpFileName = dumpFileName;

	CMiniDumpParameter parameter;
	parameter.m_dumpFileName = dumpFileName;
	parameter.m_miniDumpType = SmallMiniDumpType;

	// 프로세스의 cmdLine에 따라서 다르게 처리한다.
	switch (CMiniDumper::GetUnsafeRef().Startup(parameter))
	{
		// Exceptiong 발생
	case MiniDumpAction::MiniDumpAction_AlarmCrash:
	{

		CDumpReporterDlg dlg(this);
		dlg.DoModal();
	}
		break;
	case MiniDumpAction::MiniDumpAction_DoNothing:

		/** In case of mini dump file creation by user calling. 
		In this case, MiniDumpAction_DoNothing will return from Startup function of newly created process.
		Please do not do anything about this value. */

		/** 유저 호출에 의해 미니 덤프 파일을 생성할 경우
		이 경우에는 새롭게 생성된 프로세스에서의 Startup 함수에서는 MiniDumpAction_DoNothing의 값이 리턴된다.
		이 값에서는 아무것도 하지 말아야 한다. */
		break;
	case MiniDumpAction::MiniDumpAction_None:
		/** In case of executing the program without mini dump file creation.
		MiniDumpAction_None will return from Startup function. */

		/** 미니 덤프 생성이 아닌 일반적으로 프로그램을 실행할 경우,
		Startup 함수에서는 MiniDumpAction_None 값이 리턴된다. */
		return false;
	}


	return true;
}

void CDumpReporter::GetDumpFilePath(LPWSTR output)
{
	WCHAR module_file_name[LongMaxPath];
	GetModuleFileNameW( NULL, module_file_name, _COUNTOF( module_file_name ) );
	CPathT<CStringW> fn(module_file_name);
	fn.StripPath();

	WCHAR dir[LongMaxPath];
	if (!GetTempPathW(LongMaxPath, dir))
	{
#if (_MSC_VER>=1400)
		wcscpy_s(dir, LongMaxPath - 1, L"c:\\temp\\");
#else
		wcscpy( dir, L"c:\\temp\\");
#endif
	}

	CPathT<CStringW> dmpPath;
	dmpPath.Combine(dir, fn);
	dmpPath.RenameExtension(L".DMP");
#if (_MSC_VER>=1400)
	wcscpy_s(output, LongMaxPath - 1, dmpPath);
#else
	wcscpy(output,dmpPath);
#endif
}
