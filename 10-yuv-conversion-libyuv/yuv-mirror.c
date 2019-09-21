//
// Created by hubin on 2019/9/21.
//
#include <stdio.h>
#include <stdint.h>
#include "libyuv.h"

//const uint8_t* src_y,
//int src_stride_y,
//const uint8_t* src_u,
//int src_stride_u,
//const uint8_t* src_v,
//int src_stride_v,
//uint8_t* dst_y,
//int dst_stride_y,
//uint8_t* dst_u,
//int dst_stride_u,
//uint8_t* dst_v,
//int dst_stride_v,
//int width,
//int height
void mirror(uint8_t *srcYuvData, uint8_t *dstYuvData, int width, int height) {
    I420Mirror(
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
            height
            );
}

int main() {

    uint32_t width = 700, height = 700;
    uint8_t YUV[width*height*3/2];
    uint8_t YUV_MIRROR[width*height*3/2];

    FILE *yuv420pFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p-rotation-90.yuv", "rb");
    fread(YUV, sizeof(YUV), 1, yuv420pFile);

    mirror(YUV, YUV_MIRROR, width, height);

    FILE *yuvRotationFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p-rotation-90-mirror.yuv", "wb");
    fwrite(YUV_MIRROR, sizeof(YUV_MIRROR), 1, yuvRotationFile);

    fclose(yuvRotationFile);
    fclose(yuv420pFile);

    return 0;
}