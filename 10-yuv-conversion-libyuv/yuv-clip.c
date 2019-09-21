//
// Created by hubin on 2019/9/21.
//
#include <stdio.h>
#include <stdint.h>
#include "libyuv.h"

//const uint8_t* sample,
//size_t sample_size,
//uint8_t* dst_y,
//int dst_stride_y,
//uint8_t* dst_u,
//int dst_stride_u,
//uint8_t* dst_v,
//int dst_stride_v,
//int crop_x,
//int crop_y,
//int src_width,
//int src_height,
//int crop_width,
//int crop_height,
//enum RotationMode rotation,
//uint32_t fourcc
void clip(uint8_t *srcYuvData, uint8_t *dstYuvData, int width, int height, int cropX, int cropY, int cropWidth, int cropHeight) {
    ConvertToI420(
            srcYuvData,
            width*height*3/2,
            dstYuvData,
            cropWidth,
            dstYuvData+cropWidth*cropHeight,
            (cropWidth+1)/2,
            dstYuvData+cropWidth*cropHeight+((cropWidth+1)/2)*((cropHeight+1)/2),
            (cropWidth+1)/2,
            cropX,
            cropY,
            width,
            height,
            cropWidth,
            cropHeight,
            kRotate0,
            FOURCC_YU12);
//    I420Copy(
//            srcYuvData,
//            width,
//            srcYuvData+width*height,
//            (width+1)/2,
//            srcYuvData+width*height+((width+1)/2)*((height+1)/2),
//            (width+1)/2,
//            dstYuvData,
//            width,
//            dstYuvData+width*height,
//            (width+1)/2,
//            dstYuvData+width*height+((width+1)/2)*((height+1)/2),
//            (width+1)/2,
//            width,
//            -height
//            );
}

int main() {

    uint32_t width = 700, height = 700;
    uint32_t clipWidth = 200, clipHeight = 200;
    uint8_t YUV[width*height*3/2];
    uint8_t YUV_CLIP[clipWidth*clipHeight*3/2];

    FILE *yuv420pFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p.yuv", "rb");
    fread(YUV, sizeof(YUV), 1, yuv420pFile);

    clip(YUV, YUV_CLIP, width, height, 200, 200, clipWidth, clipHeight);

    FILE *yuvClipFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p-clip-200-200.yuv", "wb");
    fwrite(YUV_CLIP, sizeof(YUV_CLIP), 1, yuvClipFile);

    fclose(yuvClipFile);
    fclose(yuv420pFile);

    return 0;
}