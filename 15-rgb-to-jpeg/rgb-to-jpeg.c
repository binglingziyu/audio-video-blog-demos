//
// Created by hubin on 2019/11/7.
//
#include <stdio.h>
#include <stdint.h>
#include "util.h"


int main() {

    // JPEG 指定压缩质量 1~99
    int quality_scale = 1;

    int width = 80, height = 80;
    uint8_t rgb24Data[width*height*3];
    genRGB24Data(rgb24Data, width, height);

    // 1. 颜色模式转换 RGB -> YUV
    int8_t yuv_y[width*height];
    int8_t yuv_u[width*height];
    int8_t yuv_v[width*height];
    rgb24ToYuv(rgb24Data, yuv_y, yuv_u, yuv_v, width, height);

    // 2. 采样

    // 3. 分块

    // 4. 离散余弦变换（DCT）


    return 0;
}