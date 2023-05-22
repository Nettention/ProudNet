// SimpleMiniDump.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlpath.h>
#include "../../include/MiniDumper.h"
#include "../../include/DumpCommon.h"

using namespace Proud;

#define _COUNTOF(array) (sizeof(array)/sizeof(array[0]))

void GetDumpFilePath(LPWSTR output) 
{
	WCHAR module_file_name[LongMaxPath];
	GetModuleFileNameW(NULL, module_file_name, _COUNTOF(module_file_name));
	CPathT<CStringW> fn(module_file_name);
	fn.StripPath();

	WCHAR path[LongMaxPath];
	WCHAR drive[LongMaxPath];
	WCHAR dir[LongMaxPath];
	WCHAR fname[LongMaxPath];
	WCHAR ext[LongMaxPath];
	_tsplitpath_s(module_file_name, drive, LongMaxPath, dir, LongMaxPath, fname, LongMaxPath, ext, LongMaxPath);
	_tmakepath_s(path, LongMaxPath, drive, dir, L"", L"");

	CPathT<CStringW> dmpPath;
	dmpPath.Combine(path, fn);
	dmpPath.RenameExtension(L".DMP");
	wcscpy_s(output, LongMaxPath - 1, dmpPath);
};

void AccessViolation()
{
	int* a = 0;
	*a = 1;
}

// Release모드에서는 Optimize하면서 Debug모드일 때와의 Stack의 크기가 달라진다.
// 이로인해 Release모드에서는 stack overflow가 발생되지 않는다.
// 의도적으로 stack overflow 발생시키기 위해 아래의 구문을 추가하였다.
#ifndef _DEBUG // DEBUG모드가 아니면 optimize를 끈다.
#pragma optimize( "", off)
#endif

// 재귀함수로 stack overflow 발생
void StackOverFlow()
{
	return StackOverFlow();
}

#ifndef _DEBUG
#pragma optimize( "", on)
#endif

void ManualMiniDump()
{
	CMiniDumper::GetSharedPtr()->ManualMiniDump();
}

void main(int argc, char* argv[])
{
	int nRetCode = 0;
	int *data = 0;
	int menu = 0;

	TCHAR filePath[LongMaxPath] = { 0, };
	GetDumpFilePath(filePath);

	CMiniDumpParameter parameter;
	parameter.m_dumpFileName = filePath;
	parameter.m_miniDumpType = SmallMiniDumpType;

	switch (CMiniDumper::GetSharedPtr()->Startup(parameter))
	{
	case MiniDumpAction_AlarmCrash:
		// 오류 발생으로 새로운 프로세스에서 덤프 파일을 생성한 후, 이 값이 return이 됩니다.            
		// 생성된 덤프 파일을 메일로 보내거나 에러 창을 보이는 등 유저가 덤프 파일 생성 후, 처리해야할 작업을 처리해주시면 됩니다. 
		//myDumpClient->Start(L"localhost", 200004, L"./mydump.DMP");
		return;
	case MiniDumpAction_DoNothing:
		// 유저 호출로 새로운 프로세스에서 덤프 파일을 생성한 후, 이 값이 반환됩니다.            
		// 이 경우에는 아무것도 하지 말아야합니다. 
		return;
	default:
		// MiniDumpAction_None            
		// 일반적으로 앱 실행 시, 이 값이 반환됩니다.            
		// 여기서는 일반적으로 처리해야할 일을 처리해주시면 됩니다.            
		break;
	}

	while (1)
	{
		puts("MENU: 1. Access Violation('a')	2. Stack Overflow('s')	3. Manual MiniDump('m')");
		printf("> ");

		menu = getchar();

		switch (menu)
		{
		case 'a':
			AccessViolation();
			break;
		case 's':
			StackOverFlow();
			break;
		case 'm':
			ManualMiniDump();
			break;
		default:
			break;
		}
	}
}

