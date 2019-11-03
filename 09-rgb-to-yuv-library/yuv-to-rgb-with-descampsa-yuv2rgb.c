//
// Created by hubin on 2019/9/19.
//
#include <stdio.h>
#include "yuv_rgb.h"

void yuv420pToRgb24(uint8_t *YUV, uint8_t *RGB, uint32_t width, uint32_t height) {
    const YCbCrType yuv_format = YCBCR_601;
    // const YCbCrType yuv_format = YCBCR_709;
    // const YCbCrType yuv_format = YCBCR_JPEG;

    yuv420_rgb24_std(
        width,
        height,
        YUV,
        YUV+width*height,
        YUV+width*height+((width+1)/2)*((height+1)/2),
        width,
        (width+1)/2,
        RGB,
        width*3,
        yuv_format);

}

int main() {
    uint32_t width = 700, height = 700;
    uint8_t RGB[width*height*3];
    uint8_t YUV[width*height*3/2];

    FILE *yuv420pFile = fopen("/Users/hubin/Desktop/rainbow-yuv420p.yuv", "rb");
    fread(YUV, sizeof(YUV), 1, yuv420pFile);

    yuv420pToRgb24(YUV, RGB, width, height);

    FILE *rgb24File = fopen("/Users/hubin/Desktop/rainbow-yuv420p-to-rgb24-descampsa.rgb", "wb");
    fwrite(RGB, sizeof(RGB), 1, rgb24File);

    fclose(rgb24File);
    fclose(yuv420pFile);
    return 0;
}