cmake_minimum_required (VERSION 3.8.0)

#include path
include_directories("../../../include")

#Setting the name of groups of files
file(GLOB SRC_CLIENT
		*.cpp
		*.h)

#Setting the target
add_executable(SimpleClient ${SRC_CLIENT})
add_dependencies(SimpleClient SimpleCommon)
set_property(TARGET SimpleClient PROPERTY OUTPUT_DIRECTORY ../../bin)
set_property(TARGET SimpleClient PROPERTY OUTPUT_NAME SimpleClient)

#Setting about MSVC
if(MSVC)
	if(${CMAKE_VS_PLATFORM_TOOLSET} STRGREATER_EQUAL "v140")
		set(VS_VERSION_FOLDER_NAME "v140")
	else()
		set(VS_VERSION_FOLDER_NAME ${CMAKE_VS_PLATFORM_TOOLSET})
	endif()

	target_link_libraries(SimpleClient ../../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetClient)
	target_link_libraries(SimpleClient ../../bin/${CMAKE_CFG_INTDIR}/SimpleCommon)
else(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
	target_link_libraries(SimpleClient
		${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/x86_x64-linux/Release/libProudNetClient.a
#		${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/x86_x64-linux/Release/libProudNetClient.so
		${CMAKE_CURRENT_SOURCE_DIR}/../../bin/libSimpleCommon.a
	)
endif(MSVC)