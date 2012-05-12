add_definitions(-DSAGITTA_KERNEL)

set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

# find opengl and directx
if(WIN32)
	# use the opengl lib in the misc/deps
	include_directories(${SAGITTA_ROOT}/misc/deps/opengl/include)
	if(Static)
		set(OPENGL_glew_LIBRARY ${SAGITTA_ROOT}/misc/deps/opengl/lib/glew32s.lib)
	else(Static)
		set(OPENGL_glew_LIBRARY ${SAGITTA_ROOT}/misc/deps/opengl/lib/glew32.lib)
	endif(Static)
	
	set(DX_DIR $ENV{DXSDK_DIR})
    if(DX_DIR)
    	message(${DX_DIR})
        include_directories(${DX_DIR}/Include)
        link_directories(${DX_DIR}/Lib/x86)
    else(DX_DIR)
        message("DirectX SDK not found.")
    endif(DX_DIR)
	
else(WIN32)
	# For unix or unix-like system. This should always work if everything is installed and working fine.
	find_package(OpenGL)
	if(OPENGL_FOUND)
		message("OPENGL FOUND")
		message(${OPENGL_LIBRARIES})
	else(OPENGL_FOUND)
		# create project failed
		message(FATAL_ERROR "OPENGL NOT FOUND")
	endif(OPENGL_FOUND)

	find_package(GLU)
endif(WIN32)

if(Static)
	add_library(Sagitta STATIC ${Source_File})
else(Static)
	add_library(Sagitta SHARED ${Source_File})
endif(Static)

if(WIN32)
	if(DX_DIR)
		target_link_libraries(Sagitta ${OPENGL_glew_LIBRARY} d3dxof dxguid d3dx9d d3d9 SagiCommon SagiMath)
	else(DX_DIR)
		target_link_libraries(Sagitta ${OPENGL_glew_LIBRARY} SagiCommon SagiMath)
	endif(DX_DIR)
else(WIN32)
	target_link_libraries(Sagitta ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} SagiCommon SagiMath)
endif(WIN32)