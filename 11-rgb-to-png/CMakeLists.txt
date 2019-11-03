cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

aux_source_directory(../3rd/zlib ZLIB_SRC)
include_directories(../3rd/zlib)

add_executable(11-rgb-to-indexed-color-png rgb-to-indexed-color-png.c util.c ${ZLIB_SRC})

add_executable(11-rgb-to-true-color-png rgb-to-true-color-png.c util.c ${ZLIB_SRC})
