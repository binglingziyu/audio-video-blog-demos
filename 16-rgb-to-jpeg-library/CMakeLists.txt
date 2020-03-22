cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

#aux_source_directory(../3rd/libjpeg-turbo LIBJPEG_TURBO_SRC)

link_directories(../3rd/libjpeg-turbo/build)
include_directories(../3rd/libjpeg-turbo/build)

add_executable(16-rgb-to-jpeg-library rgb-to-jpeg-with-libjpeg-turbo.c util.c)

# 添加链接库
target_link_libraries(16-rgb-to-jpeg-library libturbojpeg.dylib)