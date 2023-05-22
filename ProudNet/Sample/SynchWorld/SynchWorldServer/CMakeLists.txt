cmake_minimum_required (VERSION 3.8.0)

#include path
include_directories("../../../include")

#Setting the name of groups of files
file(GLOB SRC_SERVER
		*.cpp
		*.h
		*.rc
	)

#Setting the target
add_executable(SynchWorldServer ${SRC_SERVER})
add_dependencies(SynchWorldServer SynchWorldCommon)
set_property(TARGET SynchWorldServer PROPERTY RUNTIME_OUTPUT_DIRECTORY ../../bin)
set_property(TARGET SynchWorldServer PROPERTY RUNTIME_OUTPUT_NAME SynchWorldServer)

#Setting about MSVC
if(MSVC)
	if(${CMAKE_VS_PLATFORM_TOOLSET} STRGREATER_EQUAL "v140")
		set(VS_VERSION_FOLDER_NAME "v140")
	else()
		set(VS_VERSION_FOLDER_NAME ${CMAKE_VS_PLATFORM_TOOLSET})
	endif()
	
	target_link_libraries(SynchWorldServer ../../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetServer)
	target_link_libraries(SynchWorldServer ../../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetClient)
	target_link_libraries(SynchWorldServer ../Common/${CMAKE_CFG_INTDIR}/SynchWorldCommon)
	#For using MFC
	target_compile_definitions(SynchWorldServer PRIVATE _AFXDLL UNICODE _UNICODE)
else(MSVC)#Setting about None-Winodws 
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lodbc -lssl -lcrypto")
	target_link_libraries(SynchWorldServer
		${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/x86_x64-linux/Release/libProudNetServer.a
		${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/x86_x64-linux/Release/libProudNetClient.a
		${CMAKE_CURRENT_SOURCE_DIR}/../../bin/libSynchWorldCommon.a
		ssl
		crypto
		odbc
	)
endif(MSVC)