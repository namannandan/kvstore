#add all source files
set (transaction_source_files
	transaction.c
	transaction.h
)

#indicate where the library files need to be put
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/obj)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/obj)

#add as a library
add_library(transaction STATIC ${transaction_source_files})
