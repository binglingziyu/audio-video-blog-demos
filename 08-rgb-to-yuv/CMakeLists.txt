cmake_minimum_required(VERSION 3.14)
project(rgb-to-bmp C)

set(CMAKE_C_STANDARD 99)

add_executable(08-rgb24-to-yuv444 rgb24-to-yuv444.c)
add_executable(08-rgb24-to-yuv420 rgb24-to-yuv420.c)
add_executable(08-yuv444-to-rgb24 yuv444-to-rgb24.c)
add_executable(08-yuv444-to-yuv420 yuv444-to-yuv420.c)
add_executable(08-yuv420-to-yuv444 yuv420-to-yuv444.c)