//
// Created by hubin on 2019/9/20.
//
#include <stdio.h>
#include <stdint.h>
#include "libyuv.h"

void scale(uint8_t *srcYuvData, uint8_t *dstYuvData, int width, int height, int dstWidth, int dstHeight) {
    I420Scale(
            srcYuvData,
            width,
            srcYuvData+width*height,
            (width+1)/2,
            srcYuvData+width*height+((width+1)/2)*((height+1)/2),
            (width+1)/2,
            width,
            height,
            dstYuvData,
            dstWidth,
            dstYuvData+dstWidth*dstWidth,
            (dstWidth+1)/2,
            dstYuvData+dstWidth*dstHeight+((dstWidth+1)/2)*((dstHeight+1)/2),
            (dstWidth+1)/2,
            dstWidth,
            dstHeight,
            kFilterBox
            );
}

int main() {

    uint32_t width = 700, height = 700;
    uint32_t dstWidth = 100, dstHeight = 100;
    uint8_t YUV[width*height*3/2];
    uint8_t YUV_SCALE[dstWidth*dstHeight*3/2];

    FILE *yuv420pFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p.yuv", "rb");
    fread(YUV, sizeof(YUV), 1, yuv420pFile);

    scale(YUV, YUV_SCALE, width, height, dstWidth, dstHeight);

    FILE *yuvScaleFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p-scale-6.yuv", "wb");
    fwrite(YUV_SCALE, sizeof(YUV_SCALE), 1, yuvScaleFile);

    fclose(yuvScaleFile);
    fclose(yuv420pFile);

    return 0;
}