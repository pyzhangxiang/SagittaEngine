#add_definitions(-DSAGITTA_COMMON)

set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)


add_library(SagiCommon STATIC ${Source_File})