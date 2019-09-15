cmake_minimum_required(VERSION 3.14)
project(rgb-to-bmp C)

set(CMAKE_C_STANDARD 99)

add_executable(05-rgb-to-bmp-with-libbmp rgb-to-bmp-with-libbmp.c libbmp/libbmp.c)
add_executable(05-rgb-to-bmp-with-bmp rgb-to-bmp-with-bmp.c)
add_executable(05-rgb-to-bmp-with-google-libbmp rgb-to-bmp-with-google-libbmp.c google/libbmp/src/bmpfile.c)
add_executable(05-rgb-to-bmp-with-qdbmp rgb-to-bmp-with-qdbmp.c qdbmp/qdbmp.c)