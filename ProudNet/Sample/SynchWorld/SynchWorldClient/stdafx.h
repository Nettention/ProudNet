// stdafx.h : It includes a file that contain standard include file and project realated file which does not change often.

#pragma once
#include <SDKDDKVer.h>

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// It excludes information that do not often use at Windows header.
#endif

//// Please modify following definition, if it targets platform that has high priorty than specific platform.
//// Please refer MSDN for new information of specific value that use at other platform.
//#ifndef WINVER				// This function can only use Windows 95 and Windows NT 4 or higher version.
//#define WINVER 0x0400		// Please change reasonable value to suite with Windows 98 and Windows 2000 or later version.
//#endif
//
//#ifndef _WIN32_WINNT		// This function can only use Windows NT 4 or higher version.
//#define _WIN32_WINNT 0x0400		// Please change reasonable value to suite with Windows 98 and Windows 2000 or later version.
//#endif
//
//#ifndef _WIN32_WINDOWS		// This function can only use Windows 98 or later version.
//#define _WIN32_WINDOWS 0x0410 // Please change reasonable value to suite with Windows Me or later version.
//#endif
//
//#ifndef _WIN32_IE			// This function can only use IE 4.0 or later version.
//#define _WIN32_IE 0x0400	// Please change reasonable value to suite with IE 5.0 or later version.
//#endif

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

#ifndef V
#define V(x)           { hr = x; }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#include "../../../../DirectX/include/d3dx9.h"
// 이걸 나중에 포함시켜야 한다.
#include "../../../include/ProudNetClient.h"
#include "../../Common/PositionFollower.h"
#include "../../Common/AngleFollower.h"
#include "../../Common/Vector3DMarshaler.h"
using namespace Proud;


