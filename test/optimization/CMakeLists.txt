set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

add_executable(test_optimization ${Source_File})

target_link_libraries(test_optimization SagiMath Sagitta alglib matclpro)
sg_create_vcproj_userfile(test_optimization)