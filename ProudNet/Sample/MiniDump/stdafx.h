// stdafx.h : It includes a file that contain standard include file and project realated file which does not change that often.

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

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// Some CString creator delare with state clearly.

// Cancel hiding about MFC common part and ignorable warning message.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard compose factor
#include <afxext.h>         // MFC extention
#include <afxdisp.h>        // MFC automate class

#include <afxdtctl.h>		// MFC support about Internet Explorer 4 public control
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support about Windows public control
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>

#include "../../include/MiniDumper.h"

using namespace Proud;