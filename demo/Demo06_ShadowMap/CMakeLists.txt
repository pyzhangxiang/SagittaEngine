set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

if(WIN32)
	add_executable(Demo06_ShadowMap WIN32 ${Source_File})
elseif(APPLE)
	add_executable(Demo06_ShadowMap MACOSX_BUNDLE ${Source_File})
else(WIN32)
	add_executable(Demo06_ShadowMap ${Source_File})
endif(WIN32)

target_link_libraries(Demo06_ShadowMap Sagitta DemoCore)
sg_create_vcproj_userfile(Demo06_ShadowMap)