set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

if(WIN32)
	add_executable(Demo01_Lighting WIN32 ${Source_File})
elseif(APPLE)
	add_executable(Demo01_Lighting MACOSX_BUNDLE ${Source_File})
else(WIN32)
	add_executable(Demo01_Lighting ${Source_File})
endif(WIN32)

target_link_libraries(Demo01_Lighting Sagitta DemoCore)
sg_create_vcproj_userfile(Demo01_Lighting)