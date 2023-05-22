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
		res/*.rc2
		ReadMe.txt
		*.fx
		res/*.ico
	)
		
SET(CMAKE_MFC_FLAG 2)

#Setting the target
add_executable(CasualGame2LobbyClient WIN32 ${SRC_CLIENT})
add_dependencies(CasualGame2LobbyClient CasualGame2LobbyCommon)
add_dependencies(CasualGame2LobbyClient DXUT)
set_property(TARGET CasualGame2LobbyClient PROPERTY RUNTIME_OUTPUT_DIRECTORY ../../bin)
set_property(TARGET CasualGame2LobbyClient PROPERTY RUNTIME_OUTPUT_NAME CasualGame2LobbyClient)

#Setting about MSVC
if(MSVC)
	if(${CMAKE_VS_PLATFORM_TOOLSET} STRGREATER_EQUAL "v140")
		set(VS_VERSION_FOLDER_NAME "v140")
	else()
	set(VS_VERSION_FOLDER_NAME ${CMAKE_VS_PLATFORM_TOOLSET})
	endif()
	
	target_link_libraries(CasualGame2LobbyClient ../../../lib/${CMAKE_VS_PLATFORM_NAME}/${VS_VERSION_FOLDER_NAME}/${CMAKE_CFG_INTDIR}/ProudNetClient)
	target_link_libraries(CasualGame2LobbyClient ../DXUtil/${CMAKE_CFG_INTDIR}/DXUT)
	target_link_libraries(CasualGame2LobbyClient ../GCCommon/${CMAKE_CFG_INTDIR}/CasualGame2LobbyCommon)
	
	#This should be exists for solving "erorr LNK2019:__vsnwprintf"
	target_link_libraries(CasualGame2LobbyClient legacy_stdio_definitions)
	target_compile_definitions(CasualGame2LobbyClient PRIVATE _AFXDLL UNICODE _UNICODE)
endif(MSVC)