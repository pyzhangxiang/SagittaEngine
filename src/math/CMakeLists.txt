set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})

if(Static)
	add_library(SagiMath STATIC ${Source_File})
else(Static)
	add_library(SagiMath SHARED ${Source_File})
endif(Static)