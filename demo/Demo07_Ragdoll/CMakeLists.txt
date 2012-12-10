set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

if(WIN32)
	add_executable(Demo07_Ragdoll WIN32 ${Source_File})
elseif(APPLE)
	add_executable(Demo07_Ragdoll MACOSX_BUNDLE ${Source_File})
else(WIN32)
	add_executable(Demo07_Ragdoll ${Source_File})
endif(WIN32)

target_link_libraries(Demo07_Ragdoll Sagitta DemoCore)
sg_create_vcproj_userfile(Demo07_Ragdoll)