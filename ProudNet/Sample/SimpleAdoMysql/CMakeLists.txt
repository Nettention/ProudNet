cmake_minimum_required (VERSION 3.8.0)
if(NOT WIN32)
	message(FATAL_ERROR "[This sample supports only for windows.]")
endif(NOT WIN32)
set(CMAKE_SUPPRESS_REGENERATION true)

project (SimpleAdoMysql)

#include path
include_directories("../../include")

#Setting the name of groups of files
file(GLOB SRC_CLIENT
		*.cpp
		*.h
		*.rc)

#Setting the target
add_executable(SimpleAdoMysql ${SRC_CLIENT})
set_property(TARGET SimpleAdoMysql PROPERTY RUNTIME_OUTPUT_DIRECTORY ../bin)
set_property(TARGET SimpleAdoMysql PROPERTY RUNTIME_OUTPUT_NAME SimpleAdoMysql_${CMAKE_CFG_INTDIR})

#Setting about MSVC
if(MSVC)
	if(${CMAKE_VS_PLATFORM_TOOLSET} STRGREATER_EQUAL "v140")
		set(VS_VERSION_FOLDER_NAME "v140")
	else()
		set(VS_VERSION_FOLDER_NAME ${CMAKE_VS_PLATFORM_TOOLSET})
	endif()
	
	target_link_libraries(SimpleAdoMysql ../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetServer)
	target_link_libraries(SimpleAdoMysql ../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetClient)
	target_compile_definitions(SimpleAdoMysql PRIVATE UNICODE _UNICODE)
endif(MSVC)