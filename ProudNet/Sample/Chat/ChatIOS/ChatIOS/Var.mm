//
//  Var.mm
//  ChatForIOS
//
//  Created by nettention1 on 12. 5. 16..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

// ProudNet 을 쓰는 mm혹은 cpp 파일은 꼭 <new>를 포함해야합니다.
#include <new>
#include "Var.h"

int g_ServerPort = 17788;

Proud::PNGUID g_ProtocolVersion0 = {0xafa3c0c, 0x77d7, 0x4b74, { 0x9d, 0xdb, 0x1c, 0xb3, 0xd2, 0x5e, 0x1e, 0x64 } };

Proud::Guid g_ProtocolVersion = Proud::Guid(g_ProtocolVersion0);
