cmake_minimum_required(VERSION 3.14)
project(rgb-to-bmp)

set(CMAKE_CXX_STANDARD 11)

#add_executable(05-rgb-to-bmp-with-easybmp-test EasyBMPsample.cpp EasyBMP/EasyBMP.cpp)
add_executable(05-rgb-to-bmp-with-easybmp 05-rgb-to-bmp-with-easybmp.cpp EasyBMP/EasyBMP.cpp)
add_executable(05-rgb-to-bmp-with-bitmap 05-rgb-to-bmp-with-bitmap.cpp)