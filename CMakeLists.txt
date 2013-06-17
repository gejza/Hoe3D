
cmake_minimum_required (VERSION 2.8)
project (Hoe)

add_definitions(-D_WIN32)

find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

find_package(FLEX)

add_subdirectory (HoeCore)
add_subdirectory (Hoe3D)
add_subdirectory (HoeGame)
add_subdirectory (Tutorials)

