// stdafx.h : It is include file that contain standard include file and project realated file which does not change often.

#pragma once
#include <SDKDDKVer.h>


#define WIN32_LEAN_AND_MEAN		// It excludes information that do not often use at Windows header.
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// Some CString creator delare with state clearly.

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// It excludes information that do not often use at Windows header.
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard compose factor.

#include "../../../../DirectX/Include/d3dx9.h"
#include "../../../include/ProudNetCommon.h"


using namespace Proud;