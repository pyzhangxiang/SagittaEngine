
set(Header)
set(Source)
file(GLOB Header *.h)
file(GLOB Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

add_library(alglib STATIC ${Source_File})
