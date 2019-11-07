//
// Created by hubin on 2019/11/7.
//
#include <stdio.h>
#include <stdint.h>

// ***** functions in util.c *****
void genRGB24Data(uint8_t *rgbData, int width, int height);
void rgb24ToYuv420p(uint8_t *destination, uint8_t *rgb, int width, int height);

int main() {
    printf("Ready!\n");
    int width = 700, height = 700;
    uint8_t rgb24Data[width*height*3];
    uint8_t yuv420pData[width*height*3/2];

    genRGB24Data(rgb24Data, width, height);
    rgb24ToYuv420p(yuv420pData, rgb24Data, width, height);
    FILE *yuvFile = fopen("/Users/hubin/Desktop/rainbow-700x700-rgb24-toyuv420p.yuv", "wb");
    fwrite(yuv420pData, sizeof(yuv420pData), 1, yuvFile);
    fclose(yuvFile);

    return 0;
}