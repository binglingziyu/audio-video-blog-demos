cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

aux_source_directory(../3rd/zlib ZLIB_SRC)
include_directories(../3rd/zlib)

add_executable(demo-zlib demo-zlib.c ${ZLIB_SRC})
add_executable(demo-sizeof demo-sizeof.c)
