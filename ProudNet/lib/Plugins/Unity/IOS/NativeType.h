#pragma once

#if defined(__ORBIS__)
#include <kernel.h>

#include <assert.h>
#define Assert assert

#include "PlayerInterface/UnityPrxPlugin.h"
#include "PlayerInterface/UnityEventQueue.h"
#include <system_service.h>
#endif //defined(__ORBIS__)


#include <string>

#include "ProudNetClient.h"
#include "ThreadPool.h"

#if defined(_WIN32)
#define PN_STDCALL __stdcall				// msvc: stdcall, other: cdecl or something

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Shell32.lib")

#else
#define PN_STDCALL
#endif

typedef void (PN_STDCALL  *CallbackJoinServerComplete)(void* charpHandle, void* info, void* replyFromServer);
typedef void (PN_STDCALL *CallbackLeaveServer)(void* charpHandle, void *errorinfo);
typedef void (PN_STDCALL *CallbackP2PMemberJoin)(void* charpHandle, int memberHostID, int groupHostID, int memberCount, void* message);
typedef void (PN_STDCALL *CallbackP2PMemberLeave)(void* charpHandle, int memberHostID, int groupHostID, int memberCount);
typedef void (PN_STDCALL *CallbackChangeP2PRelayState)(void* charpHandle, int remoteHostID, int reason);
typedef void (PN_STDCALL *CallbackChangeServerUdpState)(void* charpHandle, int reason);
typedef void (PN_STDCALL *CallbackSynchronizeServerTime)(void* charpHandle);

typedef void (PN_STDCALL *CallbackError)(void* charpHandle, void* errorInfo);
typedef void (PN_STDCALL *CallbackWarning)(void* charpHandle, void* errorInfo);
typedef void (PN_STDCALL *CallbackInformation)(void* charpHandle, void* errorInfo);
typedef void (PN_STDCALL *CallbackException)(void* charpHandle, int remote, void* what);

typedef void (PN_STDCALL *CallbackServerOffline)(void* charpHandle, void* args);
typedef void (PN_STDCALL *CallbackServerOnline)(void* charpHandle, void* args);
typedef void (PN_STDCALL *CallbackP2PMemberOffline)(void* charpHandle, void* args);
typedef void (PN_STDCALL *CallbackP2PMemberOnline)(void* charpHandle, void* args);

typedef void (PN_STDCALL *CallbackNoRmiProcessed)(void* charpHandle, int rmiID);
typedef void (PN_STDCALL *CallbackReceiveUserMessage)(void* charpHandle, int sender, void* rmiContext, void* payload, int payloadLength);

//////////////////////////////////////////////////////////////////////////

typedef void* (PN_STDCALL *CallbackGetRmiIDList)(void* charpHandle);
typedef int (PN_STDCALL *CallbackGetRmiIDListCount)(void* charpHandle);
typedef bool (PN_STDCALL *CallbackProcessReceivedMessage)(void* charpHandle, void* pa, int64_t hostTag);