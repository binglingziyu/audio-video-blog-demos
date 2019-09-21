//
// Created by hubin on 2019/9/17.
//
#include <stdio.h>
#include <stdint.h>
#include <math.h>


uint8_t bound(uint8_t start, int value, uint8_t end) {
    if(value <= start) {
        return start;
    }
    if(value >= end) {
        return end;
    }
    return value;
}


void yuv444pToRGB(uint8_t *yuv444pData,uint8_t *rgb24Data, int width, int height) {
    uint8_t *srcY = yuv444pData, *srcU = srcY + width * height, *srcV = srcU + width * height;

    for(int i = 0 ; i < height ; i ++) {
        for (int j = 0; j < width; j++) {
            int currentYUVIndex = i * height + j;
            uint8_t Y = srcY[currentYUVIndex], U = srcU[currentYUVIndex], V = srcV[currentYUVIndex];

            int r_val = (int)round(1.164*(Y-16)+1.596*(V-128));
            int g_val = (int)round(1.164*(Y-16)-0.813*(V-128)-0.391*(U-128));
            int b_val = (int)round(1.164*(Y-16)+2.018*(U-128));

            int currentRGBIndex = 3*(i * width + j);
            rgb24Data[currentRGBIndex] = bound(0, r_val, 255);
            rgb24Data[currentRGBIndex+1] = bound(0, g_val, 255);
            rgb24Data[currentRGBIndex+2] = bound(0, b_val, 255);
        }
    }
}

int main() {
    int width = 700, height = 700;
    uint8_t yuv444pData[width*height*3];
    uint8_t rgb24Data[width*height*3];

    FILE *yuv444pFile = fopen("/Users/hubin/Desktop/rainbow-yuv444p.yuv", "rb");
    fread(yuv444pData, sizeof(yuv444pData), 1, yuv444pFile);

     yuv444pToRGB(yuv444pData, rgb24Data, width, height);

    FILE *rgb24File = fopen("/Users/hubin/Desktop/rainbow-yuv444p-to-rgb24.rgb", "wb");
    fwrite(rgb24Data, sizeof(rgb24Data), 1, rgb24File);

    fclose(yuv444pFile);
    fclose(rgb24File);
    return 0;
}