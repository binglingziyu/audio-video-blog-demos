cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

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

add_executable(19-gif-to-rgb-library gif-to-rgb-library.c ${GIFLIB_SRC})
add_executable(19-rgb-to-gif-library rgb-to-gif-library.c ${GIFLIB_SRC})