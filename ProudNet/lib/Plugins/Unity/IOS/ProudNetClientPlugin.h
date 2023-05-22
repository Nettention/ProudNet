#pragma once

//////////////////////////////////////////////////////////////////////////
// 이 파일은 SWIG에 의해 분석되어 .cs 등의 래퍼 모듈로 변환되는 데 사용된다.

#include "NativeType.h"

//////////////////////////////////////////////////////////////////////////

extern void* NativeToNetClientEventWrap_New();
extern void NativeToNetClientEventWrap_Delete(void* eventWrap);

extern void* NativeToRmiStubWrap_New();
extern void NativeToRmiStubWrap_Delete(void* stubWrap);

extern void* NativeToRmiProxyWrap_New();
extern void NativeToRmiProxyWrap_Delete(void* proxyWrap);

extern void* NativeToRmiContext_New();
extern void NativeToRmiContext_Delete(void* args);

extern void ChangeErrorReactionTypeToDebugOutputTypeWhenUnityEngine();

//////////////////////////////////////////////////////////////////////////

extern void NetClientEvent_SetCSharpHandle(void* obj1, void* obj2);
extern void NetClientEvent_SetCallbackJoinServerComplete(void* obj, CallbackJoinServerComplete callback);
extern void NetClientEvent_SetCallbackLeaveServer(void* obj, CallbackLeaveServer callback);
extern void NetClientEvent_SetCallbackP2PMemberJoin(void* obj, CallbackP2PMemberJoin callback);
extern void NetClientEvent_SetCallbackP2PMemberLeave(void* obj, CallbackP2PMemberLeave callback);
extern void NetClientEvent_SetCallbackChangeP2PRelayState(void* obj, CallbackChangeP2PRelayState callback);
extern void NetClientEvent_SetCallbackChangeServerUdpState(void* obj, CallbackChangeServerUdpState callback);
extern void NetClientEvent_SetCallbackSynchronizeServerTime(void* obj, CallbackSynchronizeServerTime callback);
extern void NetClientEvent_SetCallbackError(void* obj, CallbackError callback);
extern void NetClientEvent_SetCallbackWarning(void* obj, CallbackWarning callback);
extern void NetClientEvent_SetCallbackInformation(void* obj, CallbackInformation callback);
extern void NetClientEvent_SetCallbackException(void* obj, CallbackException callback);

extern void NetClientEvent_SetCallbackServerOffline(void* obj, CallbackServerOffline callback);
extern void NetClientEvent_SetCallbackServerOnline(void* obj, CallbackServerOnline callback);
extern void NetClientEvent_SetCallbackP2PMemberOffline(void* obj, CallbackP2PMemberOffline callback);
extern void NetClientEvent_SetCallbackP2PMemberOnline(void* obj, CallbackP2PMemberOnline callback);

extern void NetClientEvent_SetCallbackNoRmiProcessed(void* obj, CallbackNoRmiProcessed callback);
extern void NetClientEvent_SetCallbackReceiveUserMessage(void* obj, CallbackReceiveUserMessage callback);

//////////////////////////////////////////////////////////////////////////

extern void RmiContext_SetHostTag(Proud::RmiContext* context, int64_t value);
extern void* ReceivedMessage_GetRemoteAddress(void* obj);
extern int64_t RmiContext_GetHostTag(void* obj);

//////////////////////////////////////////////////////////////////////////
extern int AddrPort_GetAddrSize();

//////////////////////////////////////////////////////////////////////////

extern void RmiStub_SetCSharpHandle(void* obj1, void* obj2);
extern void RmiStub_SetCallbackGetRmiIDList(void* obj, CallbackGetRmiIDList callback);
extern void RmiStub_SetCallbackGetRmiIDListCount(void* obj, CallbackGetRmiIDListCount callback);
extern void RmiStub_SetCallbackProcessReceivedMessage(void* obj, CallbackProcessReceivedMessage callback);

//////////////////////////////////////////////////////////////////////////
extern void RmiProxy_SetCSharpHandle(void* obj1, void* obj2);
extern void RmiProxy_SetCallbackGetRmiIDList(void* obj, CallbackGetRmiIDList callback);
extern void RmiProxy_SetCallbackGetRmiIDListCount(void* obj, CallbackGetRmiIDListCount callback);
extern bool RmiProxy_RmiSend(void* obj1, void* remotes, int remoteCount, Proud::RmiContext* context, void* data, int dataLength, std::string RMIName, int RMIId);

//////////////////////////////////////////////////////////////////////////

extern void* ByteArrayToNative(void* data, int dataLength);
extern void CopyManagedByteArrayToNativeByteArray(void* data, int dataLength, void* nativeData);

extern int ByteArray_GetCount(void* obj);

extern void CopyNativeByteArrayToManageByteArray(void* dst, void* obj);
extern void CopyNativeByteArrayToManageByteArray(void* dst, void* src, int length);

extern Proud::String ConvertNatvieStringToManagedString(void* obj);
extern void CopyNativeAddrToManagedAddr(void* dst, void* src, int length);