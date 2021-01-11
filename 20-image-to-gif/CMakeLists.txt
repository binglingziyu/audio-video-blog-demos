cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

#set(QDBMP_SRC
#        ${CMAKE_SOURCE_DIR}/05-rgb-to-bmp-library/qdbmp/qdbmp.c
#)
#include_directories(${CMAKE_SOURCE_DIR}/05-rgb-to-bmp-library/qdbmp)
set(LIBBMP_SRC
        ${CMAKE_SOURCE_DIR}/05-rgb-to-bmp-library/libbmp/libbmp.c
)
include_directories(${CMAKE_SOURCE_DIR}/05-rgb-to-bmp-library/libbmp)

aux_source_directory(${CMAKE_SOURCE_DIR}/3rd/zlib ZLIB_SRC)
include_directories(${CMAKE_SOURCE_DIR}/3rd/zlib)
configure_file(
        ${CMAKE_SOURCE_DIR}/3rd/libpng/scripts/pnglibconf.h.prebuilt
        ${CMAKE_SOURCE_DIR}/3rd/libpng/pnglibconf.h
        COPYONLY)
aux_source_directory(${CMAKE_SOURCE_DIR}/3rd/libpng LIBPNG_SRC)
list(REMOVE_ITEM LIBPNG_SRC "${CMAKE_SOURCE_DIR}/3rd/libpng/pngtest.c")
include_directories(${CMAKE_SOURCE_DIR}/3rd/libpng)

link_directories(${CMAKE_SOURCE_DIR}/3rd/libjpeg-turbo/build)
include_directories(${CMAKE_SOURCE_DIR}/3rd/libjpeg-turbo/build)

set(GIFLIB_SRC
        ${CMAKE_SOURCE_DIR}/3rd/giflib/dgif_lib.c
        ${CMAKE_SOURCE_DIR}/3rd/giflib/egif_lib.c
        ${CMAKE_SOURCE_DIR}/3rd/giflib/getarg.c
        ${CMAKE_SOURCE_DIR}/3rd/giflib/gif_err.c
        ${CMAKE_SOURCE_DIR}/3rd/giflib/gif_font.c
        ${CMAKE_SOURCE_DIR}/3rd/giflib/gif_hash.c
        ${CMAKE_SOURCE_DIR}/3rd/giflib/gif_lib.h
        ${CMAKE_SOURCE_DIR}/3rd/giflib/gifalloc.c
        ${CMAKE_SOURCE_DIR}/3rd/giflib/openbsd-reallocarray.c
        ${CMAKE_SOURCE_DIR}/3rd/giflib/qprintf.c
        ${CMAKE_SOURCE_DIR}/3rd/giflib/quantize.c
        )
include_directories(${CMAKE_SOURCE_DIR}/3rd/giflib)

add_executable(20-image-to-gif image-to-gif.c ${LIBBMP_SRC} ${ZLIB_SRC} ${LIBPNG_SRC} ${GIFLIB_SRC})

# 添加链接库
target_link_libraries(20-image-to-gif libturbojpeg.dylib)