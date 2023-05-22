#pragma once

#include <string> 

extern void NativeToGuid_Delete(void* guid);
extern void NativeToByteArray_Delete(void* byteArray);

extern void* NativeToIntArray_New();
extern void NativeToIntArray_Delete(void* intArray);

extern void* GuidToNative(void* guidData, int dataLength);
extern void CopyGuidToNative(void* guidData, int dataLength, void* nativeGuid);

extern void* ByteArrayToNative(void* data, int dataLength);
extern void CopyByteArrayToNative(void* data, int dataLength, void* nativeData);

extern void IntArray_Add(void* obj, int data);

extern int ByteArray_GetCount(void* obj);
extern void ByteArray_Copy(void* dst, void* obj);
extern void ByteArray_CopyFromData(void* dst, void* src, int length);

extern std::string ByteToString(void* obj);