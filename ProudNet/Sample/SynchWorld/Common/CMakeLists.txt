cmake_minimum_required (VERSION 3.8.0)

#include path
include_directories("../../../include")

#Setting the name of groups of files
file(GLOB SRC_COMMON
		stdafx.h
		Common.cpp
		Common.h
		../../Common/PositionFollower.*
		../../Common/AngleFollower.*
	)
file(GLOB PIDL_COMMON
		*.PIDL)
source_group("PIDL File" FILES ${PIDL_COMMON})

#Setting the target
add_library(SynchWorldCommon STATIC
			${SRC_COMMON}
			${PIDL_COMMON})
set_property(TARGET SynchWorldCommon PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET SynchWorldCommon PROPERTY OUTPUT_NAME SynchWorldCommon)
set_property(TARGET SynchWorldCommon PROPERTY OUTPUT_DIRECTORY ../../bin)

#Setting about MSVC
if(MSVC)
	foreach(PIDL_FILE ${PIDL_COMMON})
		get_filename_component(PFNAME ${PIDL_FILE} NAME_WE)
		add_custom_command(
			OUTPUT ${PFNAME}_common.cpp ${PFNAME}_common.h ${PFNAME}_proxy.cpp ${PFNAME}_proxy.h ${PFNAME}_stub.cpp ${PFNAME}_stub.h
			MAIN_DEPENDENCY ${PIDL_FILE}
			COMMAND ../../../util/PIDL.exe
			ARGS ${PIDL_FILE})
	endforeach(PIDL_FILE)
	#For using MFC
	target_compile_definitions(SynchWorldCommon PRIVATE _AFXDLL UNICODE _UNICODE)
else(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")
endif(MSVC)