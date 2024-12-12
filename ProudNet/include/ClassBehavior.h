#pragma once 

#ifdef _MSC_VER
#pragma pack(push,8)
#endif

#define NOTHROW_FUNCTION_CATCH_ALL \
	catch (std::bad_alloc&) \
		{ \
		outError = ErrorInfo::From(ErrorType_OutOfMemory, \
			HostID_None, Proud::String(ErrorInfo::TypeToString(ErrorType_OutOfMemory))); \
		} \
	catch (Exception& exception) \
		{ \
		outError = ErrorInfo::From(ErrorType_Unexpected, \
			exception.m_remote, StringA2T(exception.chMsg)); \
		}

// 클래스에 이걸 선언하면 복사 불가능한 상태가 된다.
// 실수로 assignment 구문을 넣는 것을 막게 하려면 유용하다. 
#define NO_COPYABLE(typeName) \
	private: \
		inline typeName& operator=(const typeName&) { return *this; } \
		inline typeName(const typeName&) {}

#ifdef _MSC_VER
#pragma pack(pop)
#endif