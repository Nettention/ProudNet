cmake_minimum_required (VERSION 3.8.0)
if(NOT WIN32)
	message(FATAL_ERROR "[This sample supports only for windows.]")
endif(NOT WIN32)

#include path
include_directories("../../../include")

#Setting the name of groups of files
file(GLOB SRC_COMMON
		stdafx.cpp
		stdafx.h
		Vars.cpp
		Vars.h)
file(GLOB PIDL_COMMON
		*.PIDL)
source_group("PIDL File" FILES ${PIDL_COMMON})

#Setting the target
add_library(SimpleDB2Common STATIC
			${SRC_COMMON}
			${PIDL_COMMON})
set_property(TARGET SimpleDB2Common PROPERTY LIBRARY_OUTPUT_DIRECTORY ../../bin)
set_property(TARGET SimpleDB2Common PROPERTY LIBRARY_OUTPUT_NAME SimpleDB2Common_${CMAKE_CFG_INTDIR})

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
	target_compile_definitions(SimpleDB2Common PRIVATE _AFXDLL UNICODE _UNICODE)
endif(MSVC)

