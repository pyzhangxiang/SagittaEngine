add_definitions(-DSG_DEP_STB_IMAGE)

set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

if(Static)
    add_library(stb_image STATIC ${Source_File})
else(Static)
    add_library(stb_image SHARED ${Source_File})
endif(Static)