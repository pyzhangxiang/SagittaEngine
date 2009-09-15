#include_directories(${SAGITTA_ROOT}/src/math)

set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})
source_group_by_dir(Source_File)

if(Static)
	add_library(Sagitta STATIC ${Source_File})
else(Static)
	add_library(Sagitta SHARED ${Source_File})
endif(Static)

if(WIN32)
	target_link_libraries(Sagitta ${OPENGL_glew_LIBRARY} SagiCommon SagiMath)
else(WIN32)
	target_link_libraries(Sagitta ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} SagiCommon SagiMath)
endif(WIN32)