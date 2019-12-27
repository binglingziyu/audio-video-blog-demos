cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

aux_source_directory(tool TOOL_SRC)
include_directories(tool)

add_executable(15-rgb-to-jpeg rgb-to-jpeg.c ${TOOL_SRC})