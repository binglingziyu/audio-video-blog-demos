cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

aux_source_directory(../17-gif-file-analyse/lzw/src LZW_SRC)
include_directories(../17-gif-file-analyse/lzw)

add_executable(18-rgb-to-gif rgb-to-gif.c ${LZW_SRC})