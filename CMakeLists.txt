cmake_minimum_required(VERSION 3.10)
project(muduo_chat)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(MUDUO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/muduo-master)
set(MUDUO_INCLUDE_DIR ${MUDUO_ROOT})

find_package(Threads REQUIRED)

add_subdirectory(src)
