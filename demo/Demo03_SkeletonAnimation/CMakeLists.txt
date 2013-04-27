set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

if(WIN32)
	add_executable(Demo03_SkeletonAnimation WIN32 ${Source_File})
elseif(APPLE)
	add_executable(Demo03_SkeletonAnimation MACOSX_BUNDLE ${Source_File})
else(WIN32)
	add_executable(Demo03_SkeletonAnimation ${Source_File})
endif(WIN32)

target_link_libraries(Demo03_SkeletonAnimation Sagitta DemoCore)
sg_create_vcproj_userfile(Demo03_SkeletonAnimation)

#set_target_properties(Demo03_SkeletonAnimation PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)