cmake_minimum_required (VERSION 3.8.0)
if(NOT WIN32)
	message(FATAL_ERROR "[This sample supports only for windows.]")
endif(NOT WIN32)

#include path
include_directories("../../../include")

#Setting the name of groups of files
file(GLOB SRC_SERVER
		*.cpp
		*.h
		*.rc)
		
SET(CMAKE_MFC_FLAG 2)

#Setting the target
add_executable(SimpleDB2Server WIN32 ${SRC_SERVER})
add_dependencies(SimpleDB2Server SimpleDB2Common)
set_property(TARGET SimpleDB2Server PROPERTY RUNTIME_OUTPUT_DIRECTORY ../../bin)
set_property(TARGET SimpleDB2Server PROPERTY RUNTIME_OUTPUT_NAME SimpleDB2Server_${CMAKE_CFG_INTDIR})

#Setting about MSVC
if(MSVC)
	if(${CMAKE_VS_PLATFORM_TOOLSET} STRGREATER_EQUAL "v140")
		set(VS_VERSION_FOLDER_NAME "v140")
	else()
		set(VS_VERSION_FOLDER_NAME ${CMAKE_VS_PLATFORM_TOOLSET})
	endif()
	
	target_link_libraries(SimpleDB2Server ../../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetClient)
	target_link_libraries(SimpleDB2Server ../../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetServer)
	target_link_libraries(SimpleDB2Server ../SimpleDB2Common/${CMAKE_CFG_INTDIR}/SimpleDB2Common)
	#For using MFC
	target_compile_definitions(SimpleDB2Server PRIVATE _AFXDLL UNICODE _UNICODE)
endif(MSVC)