#include "stdafx.h"

#include "Common.h"

// {3AE33249-ECC6-4980-BC5D-7B0A999C0739}
PNGUID guid = { 0x3ae33249, 0xecc6, 0x4980, { 0xbc, 0x5d, 0x7b, 0xa, 0x99, 0x9c, 0x7, 0x39 } };

Guid g_Version = Guid(guid);
int g_ServerPort = 33334;

// Limited visible area for user to synchronize character position (radius of circle)
// 캐릭 위치 동기화를 위해, 유저에게 제한된 가시 거리(원의 반경)
const float ViewportLength = 50;