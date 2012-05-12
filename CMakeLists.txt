cmake_minimum_required(VERSION 2.6)

project(Sagitta)

set(SAGITTA_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# set build type
#if(NOT CMAKE_BUILD_TYPE)
#	SET(CMAKE_BUILD_TYPE "Debug")
#	SET(CMAKE_BUILD_TYPE "Release")
#endif(NOT CMAKE_BUILD_TYPE)

# include Sagitta CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SAGITTA_ROOT}/misc/cmake)
include(SagittaCMakeConfig)

# set output place (default output place)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# set usage of C++0x, and signal all the warnings under UNIX enviroment
#if(UNIX)
#	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall _std=c++0x")
#endif(UNIX)

# unit test control
set(SAGITTA_DO_UNIT_TEST True)

add_subdirectory(src)
add_subdirectory(demo)
add_subdirectory(test)
