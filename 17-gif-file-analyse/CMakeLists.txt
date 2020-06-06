cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

aux_source_directory(lzw/src LZW_SRC)
include_directories(lzw)

add_executable(17-gif-file-analyse gif-file-analyse.c ${LZW_SRC})