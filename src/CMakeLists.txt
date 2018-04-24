#add all source files
set (kvstore_source_files
	kvstore.c
	kvstore.h
)

#indicate where the library files need to be put
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/obj)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/obj)

#include directories
include_directories(../lib/transaction ../lib/network)

#add as a library
add_library(kvstore STATIC ${kvstore_source_files})

#add link targets
target_link_libraries(kvstore transaction network)
