cmake_minimum_required(VERSION 3.14)
project(rgb-to-bmp C)

set(CMAKE_C_STANDARD 99)

add_executable(04-rgb-to-bmp-fix main.c)
add_executable(04-rgb-to-bmp-fix-test rgb-to-bmp-fix-test.c)