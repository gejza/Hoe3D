
cmake_minimum_required (VERSION 2.8)
project (Hoe)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH} )
message(${CMAKE_MODULE_PATH})
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

if(WIN32)
	add_definitions(-D_WIN32)
endif()

add_definitions(-DHOE_STATIC_ENGINE)
option (USE_OPENGL "Use OpenGL and OpenAL" OFF)
option (USE_DIRECTX "Use DirectX" OFF)

find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

find_package(FLEX)

add_subdirectory (HoeCore)
add_subdirectory (Hoe3D)
add_subdirectory (HoeGame)
add_subdirectory (Tutorials)

