set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

if(WIN32)
	add_executable(Demo04_GLSL WIN32 ${Source_File})
elseif(APPLE)
	add_executable(Demo04_GLSL MACOSX_BUNDLE ${Source_File})
else(WIN32)
	add_executable(Demo04_GLSL ${Source_File})
endif(WIN32)

target_link_libraries(Demo04_GLSL Sagitta DemoCore)
sg_create_vcproj_userfile(Demo04_GLSL)