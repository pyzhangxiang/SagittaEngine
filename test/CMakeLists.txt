include_directories(${SAGITTA_ROOT}/src)
include_directories(${SAGITTA_ROOT}/deps)
include_directories(${SAGITTA_ROOT}/deps/bullet/src)

if(WIN32)
    set(DX_DIR $ENV{DXSDK_DIR})
    if(DX_DIR)
    	message(${DX_DIR})
        include_directories(${DX_DIR}Include)
        link_directories(${DX_DIR}Lib/x86)
    else(DX_DIR)
        message("DirectX SDK not found.")
    endif(DX_DIR)
endif(WIN32)

add_subdirectory(optimization)

SET_PROPERTY(TARGET test_optimization PROPERTY FOLDER "test")
