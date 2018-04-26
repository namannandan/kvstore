#specify minimum version required
cmake_minimum_required(VERSION 2.8.12.2)

#project name
project(kvstore)

#set where the executables need to be put
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#add subdirectories
add_subdirectory(lib/transaction)
add_subdirectory(lib/storage)
add_subdirectory(lib/network)
add_subdirectory(src)

#include directories
include_directories(lib/transaction src)

#executable to be created
add_executable(server_client_test test/server_client_test.c)
add_executable(transaction_test test/transaction_test.c)

#linking
target_link_libraries(server_client_test kvstore)
target_link_libraries(transaction_test transaction)

#add linker flags
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_CXX_FLAGS} -pthread")
