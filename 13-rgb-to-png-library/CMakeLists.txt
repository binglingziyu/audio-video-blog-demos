cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

include_directories(../3rd/svpng)
add_executable(13-rgb-to-png-with-svpng rgb-to-png-with-svpng.c)

set(LIBATTOPNG_SRC ../3rd/libattopng/libattopng.c)
include_directories(../3rd/libattopng)
add_executable(13-rgb-to-png-with-libattopng rgb-to-png-with-libattopng.c ${LIBATTOPNG_SRC})