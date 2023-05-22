// stdafx.h :  It includes a file that contain standard include file and project realated file which does not change that often.

#pragma once
#include <SDKDDKVer.h>

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// It excludes information that do not often use at Windows header.
#endif

#if defined(_MSC_VER) && _MSC_VER <1600    
// _MSC_VER is already defined and defined version is lower than version 10.0
#ifndef WINVER				// This function can only use Windows 95 and Windows NT 4 or higher version.
#define WINVER 0x0400		// Please change reasonable value to suite with Windows 98 and Windows 2000 or later version.
#endif

#ifndef _WIN32_WINNT		// This function can only use Windows NT 4 or higher version.
#define _WIN32_WINNT 0x0400		// Please change reasonable value to suite with Windows 98 and Windows 2000 or later version.
#endif

#ifndef _WIN32_WINDOWS		// This function can only use Windows 98 or later version.
#define _WIN32_WINDOWS 0x0410 // Please change reasonable value to suite with Windows Me or later version.
#endif

#ifndef _WIN32_IE			// This function can only use IE 4.0 or later version.
#define _WIN32_IE 0x0400	// Please change reasonable value to suite with IE 5.0 or later version.
#endif

#else
// Otherwise
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif 

#ifndef _WIN32_WINDOWS // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE // Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0500 // Change this to the appropriate value to target IE 5.0 or later.
#endif

#endif


#define _ATL_String_EXPLICIT_CONSTRUCTORS	// Some CString creator delare with state clearly.

// Cancel hiding about MFC common part and ignorable warning message.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard compose factor
#include <afxext.h>         // MFC extention
#include <afxdisp.h>        // MFC automate class

#include <afxdtctl.h>		// MFC support about Internet Explorer 4 public control
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support about Windows public control
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../../../../DirectX/Include/d3dx9.h"
#include "../../../include/ProudNetServer.h"
#include "../../../include/ProudDB.h"
#include "../../Common/PositionFollower.h"
#include "../../Common/AngleFollower.h"

/*
32bit, 64bit 에서 링크해야 하는 lib 가 다르기 때문에.
32bit 는 _WIN32 만 정의.
64bit 에서는 _WIN32 _WIN64 모두 정의 됨.
따라서 _WIN32 가 조건문에 먼저 나올경우 32bit 64bit 에 대한 구분이 되지 않음.
*/
#ifdef _WIN64
#pragma comment(lib,"../../../../directx/lib/x64/d3d9.lib")
#pragma comment(lib,"../../../../directx/lib/x64/d3dx9.lib")
#pragma comment(lib,"../../../../directx/lib/x64/d3dx10.lib")
#pragma comment(lib,"../../../../directx/lib/x64/dxerr.lib")
#else _WIN32
#pragma comment(lib,"../../../../directx/lib/x86/d3d9.lib")
#pragma comment(lib,"../../../../directx/lib/x86/d3dx9.lib")
#pragma comment(lib,"../../../../directx/lib/x86/d3dx10.lib")
#pragma comment(lib,"../../../../directx/lib/x86/dxerr.lib")
#endif

using namespace Proud;


