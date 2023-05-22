#pragma once

#include "../../../include/ProudNetCommon.h"


/** Synchworld support Action Script Version link mode and pure C++ mode.
- Action Scrip synchronization mode is using Ortho View and C++ mode use projection view.
- This define has to do at Synchworld Server and Client, Common.

/** Synchworld는 Action Script Version 연동 모드와 순수 c++ 모드를 지원합니다.
- Action Scrip 동기화 모드는 오소뷰를 사용하며 c++ 모드는 프로젝션 뷰를 사용합니다.
- Synchworld Server와 Client, Common에 이 define이 되어 있어야 합니다.
*/

using namespace Proud;

extern Guid g_Version;
extern int g_ServerPort;

extern const float ViewportLength;


