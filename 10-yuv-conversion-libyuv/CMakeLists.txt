cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

aux_source_directory(../3rd/libyuv/source LIBYUV_SRC)
include_directories(../3rd/libyuv/include)

add_executable(10-yuv-rotation yuv-rotation.c ${LIBYUV_SRC})
add_executable(10-yuv-mirror yuv-mirror.c ${LIBYUV_SRC})
add_executable(10-yuv-scale yuv-scale.c ${LIBYUV_SRC})
add_executable(10-yuv-clip yuv-clip.c ${LIBYUV_SRC})
add_executable(10-yuv-blend yuv-blend.c ${LIBYUV_SRC})