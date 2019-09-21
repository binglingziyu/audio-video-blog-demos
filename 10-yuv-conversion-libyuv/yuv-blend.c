//
// Created by hubin on 2019/9/20.
//
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libyuv.h"

//const uint8_t* src_y0,
//int src_stride_y0,
//const uint8_t* src_u0,
//int src_stride_u0,
//const uint8_t* src_v0,
//int src_stride_v0,
//const uint8_t* src_y1,
//int src_stride_y1,
//const uint8_t* src_u1,
//int src_stride_u1,
//const uint8_t* src_v1,
//int src_stride_v1,
//const uint8_t* alpha,
//int alpha_stride,
//uint8_t* dst_y,
//int dst_stride_y,
//uint8_t* dst_u,
//int dst_stride_u,
//uint8_t* dst_v,
//int dst_stride_v,
//int width,
//int height

void blend(uint8_t *srcYuvData, uint8_t *dstYuvData, int width, int height) {
    uint8_t YUV_ROTATION[width*height*3/2];
    I420Rotate(
            srcYuvData,
            width,
            srcYuvData+width*height,
            (width+1)/2,
            srcYuvData+width*height+((width+1)/2)*((height+1)/2),
            (width+1)/2,
            YUV_ROTATION,
            width,
            YUV_ROTATION+width*height,
            (width+1)/2,
            YUV_ROTATION+width*height+((width+1)/2)*((height+1)/2),
            (width+1)/2,
            width,
            height,
            kRotate90
    );

    uint8_t alpha[width*height];
    memset(alpha, 0X88, width*height);

    I420Blend(
            srcYuvData,
            width,
            srcYuvData+width*height,
            (width+1)/2,
            srcYuvData+width*height+((width+1)/2)*((height+1)/2),
            (width+1)/2,
            YUV_ROTATION,
            width,
            YUV_ROTATION+width*height,
            (width+1)/2,
            YUV_ROTATION+width*height+((width+1)/2)*((height+1)/2),
            (width+1)/2,
            alpha,
            width,
            dstYuvData,
            width,
            dstYuvData+width*height,
            (width+1)/2,
            dstYuvData+width*height+((width+1)/2)*((height+1)/2),
            (width+1)/2,
            width,
            height
            );

//    I420Interpolate()
}

int main() {

    uint32_t width = 700, height = 700;
    uint8_t YUV[width*height*3/2];
    uint8_t YUV_BLEND[width*height*3/2];

    FILE *yuv420pFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p.yuv", "rb");
    fread(YUV, sizeof(YUV), 1, yuv420pFile);

    blend(YUV, YUV_BLEND, width, height);

    FILE *yuvBlendFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p-blend.yuv", "wb");
    fwrite(YUV_BLEND, sizeof(YUV_BLEND), 1, yuvBlendFile);

    fclose(yuvBlendFile);
    fclose(yuv420pFile);

    return 0;
}