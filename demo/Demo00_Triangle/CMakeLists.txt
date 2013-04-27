set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

if(WIN32)
	add_executable(Demo00_Triangle WIN32 ${Source_File})
elseif(APPLE)
	add_executable(Demo00_Triangle MACOSX_BUNDLE ${Source_File})
else(WIN32)
	add_executable(Demo00_Triangle ${Source_File})
endif(WIN32)

target_link_libraries(Demo00_Triangle Sagitta DemoCore)
sg_create_vcproj_userfile(Demo00_Triangle)