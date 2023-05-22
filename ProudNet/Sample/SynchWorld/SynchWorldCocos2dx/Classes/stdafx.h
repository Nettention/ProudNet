#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN		// It excludes information that do not often use at Windows header.
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// Some CString creator delare with state clearly.

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// It excludes information that do not often use at Windows header.
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard compose factor.
#endif

#include <iostream>

// It has to include later.
#include "../../../include/ProudNetClient.h"
using namespace Proud;