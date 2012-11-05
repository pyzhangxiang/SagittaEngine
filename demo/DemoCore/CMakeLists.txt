add_definitions(-DSAGITTA_DEMO)

set(Header)
set(Source)
file(GLOB Header *.h)
file(GLOB Source *.cpp)

set(SubHeader)
set(SubSource)
if(APPLE)
	file(GLOB_RECURSE SubHeader ${CMAKE_CURRENT_SOURCE_DIR}/mac/*.h)
#	file(GLOB_RECURSE SubSource ${CMAKE_CURRENT_SOURCE_DIR}/mac/*.cpp)
	file(GLOB_RECURSE SubSource ${CMAKE_CURRENT_SOURCE_DIR}/mac/*.mm)
elseif(WIN32)
	file(GLOB_RECURSE SubHeader ${CMAKE_CURRENT_SOURCE_DIR}/win32/*.h)
	file(GLOB_RECURSE SubSource ${CMAKE_CURRENT_SOURCE_DIR}/win32/*.cpp)
else(APPLE)
	message("No implement in this platform. Mac OS X and Win32 are supported.")
	return()
endif(APPLE)

set(Source_File ${Header} ${Source} ${SubHeader} ${SubSource})
source_group_by_dir(Source_File)


if(Static)
	add_library(DemoCore STATIC ${Source_File})
else(Static)
    if(WIN32)
        add_library(DemoCore STATIC ${Source_File})
    else(WIN32)
	    add_library(DemoCore SHARED ${Source_File})
    endif(WIN32)
endif(Static)

# for win32, this lib contains the entry function WinMain
# so it has to be static lib
if(WIN32)
	target_link_libraries(DemoCore ${OPENGL_gl_LIBRARY} Sagitta)
    #add_library(DemoCore STATIC ${Source_File})
else(WIN32)
	find_library(COCOA_LIBRARY Cocoa)
	target_link_libraries(DemoCore ${OPENGL_gl_LIBRARY} ${COCOA_LIBRARY} Sagitta)
endif(WIN32)

