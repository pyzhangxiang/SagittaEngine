add_definitions(-DSAGITTA_KERNEL)

include_directories(${SAGITTA_ROOT}/src)
include_directories(${SAGITTA_ROOT}/deps)

set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})

set(to_remove_h)
set(to_remove_cpp)
if(MSVC)
    file(GLOB_RECURSE to_remove_h "*gcc*.h")
    file(GLOB_RECURSE to_remove_cpp "*gcc*.cpp")

#    set(CPACK_SOURCE_IGNORE_FILES ".*msvc.*.cpp$;${CPACK_SOURCE_IGNORE_FILES}")
#    file(GLOB_RECURSE Source "*.cpp")
else(MSVC)
    file(GLOB_RECURSE to_remove_h "*msvc*.h")
    file(GLOB_RECURSE to_remove_cpp "*msvc*.cpp")

#    file(GLOB_RECURSE Source "^(?!msvc).*.cpp")
endif(MSVC)

set(to_remove ${to_remove_h} ${to_remove_cpp})
message("To remove files:\n${to_remove}\n")

list(REMOVE_ITEM Source_File ${to_remove})

source_group_by_dir(Source_File)

# find opengl, glew and directx
if(WIN32)
	# use the opengl lib in the misc/deps
    set(GLEW_INCLUDE_DIRS ${SAGITTA_ROOT}/deps/opengl/include)
	if(Static)
		set(GLEW_LIBRARIES ${SAGITTA_ROOT}/deps/opengl/lib/glew32s.lib)
	else(Static)
		set(GLEW_LIBRARIES ${SAGITTA_ROOT}/deps/opengl/lib/glew32.lib)
	endif(Static)
	
	set(DX_DIR $ENV{DXSDK_DIR})
    if(DX_DIR)
    	message(${DX_DIR})
        include_directories(${DX_DIR}Include)
        link_directories(${DX_DIR}Lib/x86)
    else(DX_DIR)
        message("DirectX SDK not found.")
    endif(DX_DIR)
	
else(WIN32)
	# For unix or unix-like system. This should always work if everything is installed and working fine.
    message("Finding GLEW ...")
	find_package(GLEW)
    if(GLEW_FOUND)
        message("GLEW found\n${GLEW_INCLUDE_DIRS}\n")
    else(GLEW_FOUND)
        message(FATAL_ERROR "GLEW not found")
    endif(GLEW_FOUND)

	#find_package(GLU)
endif(WIN32)

include_directories(${GLEW_INCLUDE_DIRS})

if(Static)
	add_library(Sagitta STATIC ${Source_File})
else(Static)
	add_library(Sagitta SHARED ${Source_File})
endif(Static)

if(WIN32)
	if(DX_DIR)
		target_link_libraries(Sagitta ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} d3dxof dxguid d3dx9d d3d9 SagiMath stb_image)
	else(DX_DIR)
		target_link_libraries(Sagitta ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} SagiMath stb_image)
	endif(DX_DIR)
else(WIN32)
	target_link_libraries(Sagitta ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} SagiMath stb_image)
endif(WIN32)