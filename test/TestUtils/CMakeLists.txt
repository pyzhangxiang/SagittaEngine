set(Header)
set(Source)
file(GLOB_RECURSE Header *.h)
file(GLOB_RECURSE Source *.cpp)

set(Source_File ${Header} ${Source})

#if(WIN32)
#	add_executable(Test_Utils WIN32 ${Source_File})
#else(WIN32)
	add_executable(Test_Utils ${Source_File})
#endif(WIN32)

target_link_libraries(Test_Utils SagiCommon)