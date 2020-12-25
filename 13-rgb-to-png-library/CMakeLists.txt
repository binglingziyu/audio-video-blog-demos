cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

include_directories(../3rd/svpng)
add_executable(13-rgb-to-png-with-svpng rgb-to-png-with-svpng.c)

set(LIBATTOPNG_SRC ../3rd/libattopng/libattopng.c ../00-examples/array-example.c)
include_directories(../3rd/libattopng)
add_executable(13-rgb-to-png-with-libattopng rgb-to-png-with-libattopng.c ${LIBATTOPNG_SRC})

#aux_source_directory(../3rd/lodepng LODEPNG_SRC)
#list(REMOVE_ITEM LODEPNG_SRC "../3rd/lodepng/lodepng_benchmark.cpp")
# lodepng Demo (lodepng.cpp rename to lodepng.c)
#file(COPY ${CMAKE_SOURCE_DIR}/3rd/lodepng/lodepng.cpp
#        DESTINATION ${CMAKE_SOURCE_DIR}/3rd/lodepng/)
configure_file(${CMAKE_SOURCE_DIR}/3rd/lodepng/lodepng.cpp ${CMAKE_SOURCE_DIR}/3rd/lodepng/lodepng.c COPYONLY)
set(LODEPNG_SRC ../3rd/lodepng/lodepng.c)
include_directories(../3rd/lodepng)
add_executable(13-rgb-to-png-with-lodepng rgb-to-png-with-lodepng.c ${LODEPNG_SRC})

aux_source_directory(../3rd/zlib ZLIB_SRC)
configure_file(
        ${CMAKE_SOURCE_DIR}/3rd/libpng/scripts/pnglibconf.h.prebuilt
        ${CMAKE_SOURCE_DIR}/3rd/libpng/pnglibconf.h
        COPYONLY)
aux_source_directory(../3rd/libpng LIBPNG_SRC)
list(REMOVE_ITEM LIBPNG_SRC "../3rd/libpng/pngtest.c")
include_directories(../3rd/libpng)
include_directories(../3rd/zlib)
add_executable(13-rgb-to-png-with-libpng rgb-to-png-with-libpng.c ${ZLIB_SRC} ${LIBPNG_SRC})