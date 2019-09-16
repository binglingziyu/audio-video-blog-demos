cmake_minimum_required(VERSION 3.14)
project(rgb-to-bmp C)

set(CMAKE_C_STANDARD 99)

add_executable(06-drawing-time-rainbow draw-rainbow.c)
add_executable(06-drawing-time-checkerboard draw-checkerboard.c)
add_executable(06-drawing-time-gradient draw-gradient.c)
add_executable(06-drawing-time-mandelbrot-set draw-mandelbrot-set.c)
add_executable(06-drawing-time-julia-set draw-julia-set.c)
add_executable(06-drawing-time-magnifying-glass draw-magnifying-glass.c)
add_executable(06-drawing-time-swirl-effect draw-swirl-effect.c)
add_executable(06-drawing-time-frosted-glass-effect draw-frosted-glass-effect.c)