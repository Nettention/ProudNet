cmake_minimum_required (VERSION 3.8.0)

#Setting the name of groups of files
file(GLOB SRC_DXUT
		*.cpp
		*.h
		*.ico)
		
#Setting the target
add_library(DXUT STATIC ${SRC_DXUT})
set_property(TARGET DXUT PROPERTY RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_VS_PLATFORM_NAME}/${CMAKE_VS_PLATFORM_TOOLSET}/${CMAKE_CFG_INTDIR})
set_property(TARGET DXUT PROPERTY RUNTIME_OUTPUT_NAME ${CMAKE_PROJECT_NAME}DXUT_${CMAKE_CFG_INTDIR})

#Setting about MSVC
if(MSVC)
	target_compile_definitions(DXUT PRIVATE UNICODE _UNICODE)
endif(MSVC)