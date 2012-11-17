cmake_minimum_required(VERSION 2.6)

project(Sagitta)

set(SAGITTA_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SAGITTA_ROOT}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${SAGITTA_ROOT}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${SAGITTA_ROOT}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${SAGITTA_ROOT}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${SAGITTA_ROOT}/bin)
message("\nOutput directory:\n${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\n")

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

# set preprocessor definitions for different configurations
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_RELEASE")

# set usage of C++0x, and signal all the warnings under UNIX enviroment
#if(UNIX)
#	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall _std=c++0x")
#endif(UNIX)

# unit test control
#set(SAGITTA_DO_UNIT_TEST True)

# find opengl and directx
find_package(OpenGL)
if(OPENGL_FOUND)
	message("OPENGL FOUND\n${OPENGL_INCLUDE_DIR}\n" )
else(OPENGL_FOUND)
	# create project failed
	message(FATAL_ERROR "OPENGL NOT FOUND")
endif(OPENGL_FOUND)

# create vcproj.user file for Visual Studio to set debug working directory
function(sg_create_vcproj_userfile TARGETNAME)
    if(MSVC)
        # add a user file to auto config the working directory for debugging
        configure_file(
            ${SAGITTA_ROOT}/misc/cmake/workingdir.vcxproj.user.in 
            ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcxproj.user 
            @ONLY
        )
    endif()
endfunction(sg_create_vcproj_userfile)


#
# Organize projects into folders
#
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)
add_subdirectory(demo)
add_subdirectory(deps)
