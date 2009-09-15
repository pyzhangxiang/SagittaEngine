set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})

if(WIN32)
	add_executable(Demo_AllInOne WIN32 ${Source_File})
elseif(APPLE)
	add_executable(Demo_AllInOne MACOSX_BUNDLE ${Source_File})
else(WIN32)
	add_library(Demo_AllInOne ${Source_File})
endif(WIN32)

target_link_libraries(Demo_AllInOne Sagitta SagiGui)