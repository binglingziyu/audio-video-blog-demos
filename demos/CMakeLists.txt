cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

aux_source_directory(../11-rgb-to-png/zlib ZLIB_SRC)
include_directories(../11-rgb-to-png/zlib)

add_executable(demos-zlib demos-zlib.c ${ZLIB_SRC})
