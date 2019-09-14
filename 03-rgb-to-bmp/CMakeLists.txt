cmake_minimum_required(VERSION 3.14)
project(rgb-to-bmp C)

set(CMAKE_C_STANDARD 99)

add_executable(03-rgb-to-bmp main.c)
add_executable(03-rgb-to-bmp-test save-bmp-test.c)