//
// Created by hubin on 2019/9/19.
//

#include <stdio.h>
#include "libyuv.h"

void libyuv_Rgb24_to_Yuv420p(char *rgbFileName, char *yuvFileName, int width, int height) {
    FILE *rgb_file = fopen(rgbFileName, "rb");
    FILE *yuv_file = fopen(yuvFileName, "wb");

    int size_rgb = width * height * 3;
    int size_yuv = width * height * 3 / 2;
    uint8_t *buffer_rgb = (uint8_t *)malloc(size_rgb);
    uint8_t *buffer_yuv = (uint8_t *)malloc(size_yuv);

    fread(buffer_rgb, 1, size_rgb, rgb_file);

    // RGB to BGR
//    uint8_t temp;
//    for(int i = 0; i < size_rgb; i+=3) {
//        temp = buffer_rgb[i + 0];
//        buffer_rgb[i + 0] = buffer_rgb[i + 2];
//        buffer_rgb[i + 2] = temp;
//    }

//    RGB24ToI420(
    RAWToI420(
            buffer_rgb, width*3,
            buffer_yuv, width,
            buffer_yuv + width*height, (width+1)/2,
            buffer_yuv + width*height + ((width+1)/2)*((height+1)/2), (width+1)/2,
            width, height);

    fwrite(buffer_yuv, 1, size_yuv, yuv_file);

    free(buffer_rgb);
    free(buffer_yuv);
    fclose(yuv_file);
    fclose(rgb_file);
}

int main() {
    int width = 700, height = 700;
    libyuv_Rgb24_to_Yuv420p("/Users/hubin/Desktop/rainbow-rgb24.rgb", "/Users/hubin/Desktop/rainbow-rgb24-to-yuv420p-libyuv.yuv", width, height);
    return 0;
}