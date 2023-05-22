// stdafx.h : It includes a file that contain standard include file and project realated file which does not change that often.

#pragma once

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// Some CString creator delare with state clearly.

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// It excludes information that do not often use at Windows header.
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard compose factor.
#include <afxext.h>         // MFC extention.
#include <afxdtctl.h>		// MFC support about common control of Internet Explorer 4.
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support about windows common control.
#endif // _AFX_NO_AFXCMN_SUPPORT


// TODO: Additional require header refer at here.

#include <iostream>
#include <tchar.h>
#include <atlbase.h>