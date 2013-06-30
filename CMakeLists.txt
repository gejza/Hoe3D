
cmake_minimum_required (VERSION 2.8)
project (Hoe)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH} )
message(${CMAKE_MODULE_PATH})
# The version number.
set (HOE_VERSION_MAJOR 1)
set (HOE_VERSION_MINOR 0)

if(WIN32)
	add_definitions(-D_WIN32)
endif()

#
# Organize projects into folders
#
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

add_definitions(-DHOE_STATIC_ENGINE)
option (USE_OPENGL "Use OpenGL and OpenAL" OFF)
option (USE_DIRECTX "Use DirectX" OFF)

option (BUILD_EDITOR "Build editor support" OFF)
option (BUILD_TUTORIALS "Build editor support" OFF)

find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

find_package(FLEX)

#-- Add an Option to toggle the generation of the API documentation
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR
      "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  #-- Configure the Template Doxyfile for our specific project
  configure_file(Doxyfile.in
                 ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
  #-- Add a custom target to run Doxygen when ever the project is built
  add_custom_target (Docs ALL
  										COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
  										SOURCES ${PROJECT_BINARY_DIR}/Doxyfile Doxyfile.in)
  # IF you do NOT want the documentation to be generated EVERY time you build the project
  # then leave out the 'ALL' keyword from the above command.
endif()

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
#set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${HOE_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${HOE_VERSION_MINOR}")
include (CPack)


enable_testing()

include_directories(include)

add_subdirectory (HoeCore)
add_subdirectory (HoeResource)
add_subdirectory (Hoe3D)
add_subdirectory (HoeGame)
IF(BUILD_EDITOR)
	add_subdirectory (HoeEditor)
ENDIF()
add_subdirectory (HoeUtils)
add_subdirectory (Tools)
IF(BUILD_TUTORIALS)
	add_subdirectory (Tutorials)
ENDIF()



