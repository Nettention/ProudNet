cmake_minimum_required (VERSION 3.8.0)
if(NOT WIN32)
	message(FATAL_ERROR "[This sample supports only for windows.]")
endif(NOT WIN32)

#include path
include_directories("../../../include")

#Setting the name of groups of files
file(GLOB SRC_CLIENT
		*.cpp
		*.h
		*.rc
	)
		
SET(CMAKE_MFC_FLAG 2)

#Setting the target
add_executable(SynchWorldClient WIN32 ${SRC_CLIENT})
add_dependencies(SynchWorldClient SynchWorldCommon)
add_dependencies(SynchWorldClient DXUT)
set_property(TARGET SynchWorldClient PROPERTY RUNTIME_OUTPUT_DIRECTORY ../../bin)
set_property(TARGET SynchWorldClient PROPERTY RUNTIME_OUTPUT_NAME SynchWorldClient_${CMAKE_CFG_INTDIR})

#Setting about MSVC
if(MSVC)
	if(${CMAKE_VS_PLATFORM_TOOLSET} STRGREATER_EQUAL "v140")
		set(VS_VERSION_FOLDER_NAME "v140")
	else()
		set(VS_VERSION_FOLDER_NAME ${CMAKE_VS_PLATFORM_TOOLSET})
	endif()

	target_link_libraries(SynchWorldClient ../../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetClient)
	target_link_libraries(SynchWorldClient ../DXUtil/${CMAKE_CFG_INTDIR}/DXUT)
	target_link_libraries(SynchWorldClient ../Common/${CMAKE_CFG_INTDIR}/SynchWorldCommon)
	
	#This should be exists for solving "erorr LNK2019:__vsnwprintf"
	target_link_libraries(SynchWorldClient legacy_stdio_definitions)
	#For using MFC
	target_compile_definitions(SynchWorldClient PRIVATE _AFXDLL UNICODE _UNICODE)
endif(MSVC)