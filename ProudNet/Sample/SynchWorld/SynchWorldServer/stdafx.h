// stdafx.h : It includes file that contain standard include file and project realated file which does not change often.

#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN		// It excludes information that do not often use at Windows header.
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// Some CString creator delare with state clearly.

#include <SDKDDKVer.h>

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// It excludes information that do not often use at Windows header.
#endif

#include <atlbase.h>
#endif

#include <iostream>

// It has to include later.
#include "../../../include/ProudNetServer.h"
#include "../../Common/PositionFollower.h"
#include "../../Common/AngleFollower.h"
#include "../../Common/Vector3DMarshaler.h"
using namespace Proud;
