#include_directories(common)

if(Static)
message("Static True")
	add_definitions(-DSAGITTA_STATIC_LIB)
endif(Static)

add_subdirectory(common)
add_subdirectory(math)
add_subdirectory(engine)
add_subdirectory(gui)