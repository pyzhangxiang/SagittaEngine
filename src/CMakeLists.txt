
if(Static)
    message("Static True")
	add_definitions(-DSAGITTA_STATIC_LIB)
endif(Static)

add_subdirectory(math)
add_subdirectory(engine)