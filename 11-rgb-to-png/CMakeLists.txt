cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

aux_source_directory(zlib ZLIB_SRC)
include_directories(zlib)

add_executable(11-rgb-to-png rgb-to-png.c util.c ${ZLIB_SRC})
