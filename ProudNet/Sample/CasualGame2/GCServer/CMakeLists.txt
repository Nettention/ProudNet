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
		*.rc
		res/*.rc2
		ReadMe.txt
		res/*.ico
	)
		
SET(CMAKE_MFC_FLAG 2)

#Setting the target
add_executable(CasualGame2LobbyServer WIN32 
			${SRC_SERVER})
add_dependencies(CasualGame2LobbyServer CasualGame2LobbyCommon)
set_property(TARGET CasualGame2LobbyServer PROPERTY RUNTIME_OUTPUT_DIRECTORY ../../bin)
set_property(TARGET CasualGame2LobbyServer PROPERTY RUNTIME_OUTPUT_NAME CasualGame2LobbyServer_${CMAKE_CFG_INTDIR})

#Setting about MSVC
if(MSVC)
	if(${CMAKE_VS_PLATFORM_TOOLSET} STRGREATER_EQUAL "v140")
		set(VS_VERSION_FOLDER_NAME "v140")
	else()
		set(VS_VERSION_FOLDER_NAME ${CMAKE_VS_PLATFORM_TOOLSET})
	endif()
	
	target_link_libraries(CasualGame2LobbyServer ../../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetServer)
	target_link_libraries(CasualGame2LobbyServer ../../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetClient)
	target_link_libraries(CasualGame2LobbyServer ../GCCommon/${CMAKE_CFG_INTDIR}/CasualGame2LobbyCommon)
	target_compile_definitions(CasualGame2LobbyServer PRIVATE _AFXDLL UNICODE _UNICODE)
endif(MSVC)