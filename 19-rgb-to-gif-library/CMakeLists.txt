cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

aux_source_directory(giflib GIFLIB_SRC)
include_directories(giflib)

add_executable(19-rgb-to-gif-library rgb-to-gif-library.c ${GIFLIB_SRC})