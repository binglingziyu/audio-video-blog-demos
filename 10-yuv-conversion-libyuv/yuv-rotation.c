//
// Created by hubin on 2019/9/20.
//
#include <stdio.h>
#include <stdint.h>
#include "libyuv.h"

void rotation(uint8_t *srcYuvData, uint8_t *dstYuvData, int width, int height) {
    I420Rotate(
            srcYuvData,
            width,
            srcYuvData+width*height,
            (width+1)/2,
            srcYuvData+width*height+((width+1)/2)*((height+1)/2),
            (width+1)/2,
            dstYuvData,
            width,
            dstYuvData+width*height,
            (width+1)/2,
            dstYuvData+width*height+((width+1)/2)*((height+1)/2),
            (width+1)/2,
            width,
            height,
            kRotate90
            );
}

int main() {

    uint32_t width = 700, height = 700;
    uint8_t YUV[width*height*3/2];
    uint8_t YUV_ROTATION[width*height*3/2];

    FILE *yuv420pFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p.yuv", "rb");
    fread(YUV, sizeof(YUV), 1, yuv420pFile);

    rotation(YUV, YUV_ROTATION, width, height);

    FILE *yuvRotationFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p-rotation-90.yuv", "wb");
    fwrite(YUV_ROTATION, sizeof(YUV_ROTATION), 1, yuvRotationFile);

    fclose(yuvRotationFile);
    fclose(yuv420pFile);

    return 0;
}