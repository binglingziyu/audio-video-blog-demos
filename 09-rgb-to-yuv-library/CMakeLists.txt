cmake_minimum_required(VERSION 3.14)
project(rgb-to-bmp C)

set(CMAKE_C_STANDARD 99)

aux_source_directory(libyuv/source LIBYUV_SRC)
include_directories(libyuv/include)
add_executable(09-rgb-to-yuv-with-libyuv rgb-to-yuv-with-libyuv.c ${LIBYUV_SRC})
add_executable(09-yuv-to-rgb-with-libyuv yuv-to-rgb-with-libyuv.c ${LIBYUV_SRC})

add_executable(09-rgb-to-yuv-with-descampsa rgb-to-yuv-with-descampsa-yuv2rgb.c descampsa/yuv2rgb/yuv_rgb.c)
add_executable(09-yuv-to-rgb-with-descampsa yuv-to-rgb-with-descampsa-yuv2rgb.c descampsa/yuv2rgb/yuv_rgb.c)