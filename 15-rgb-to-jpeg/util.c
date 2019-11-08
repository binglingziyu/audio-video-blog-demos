//
// Created by hubin on 2019/11/7.
//
#include "util.h"

// Y = 0.299*R + 0.587*G + 0.114*B
// U = Cb = -0.169*R - 0.331*G + 0.500*B + 128
// V = Cr = 0.500*R - 0.419*G - 0.081*B + 128
// R/G/B  [0 ~ 255]
// Y/Cb/Cr[0 ~ 255]
void rgbToYuv(uint8_t R, uint8_t G, uint8_t B, uint8_t *Y, uint8_t *U, uint8_t *V) {
    int y_val = (int)round(0.299*R + 0.587*G + 0.114*B);
    int u_val = (int)round(-0.169*R - 0.331*G + 0.500*B + 128);
    int v_val = (int)round(0.500*R - 0.419*G - 0.081*B + 128);
    *Y = bound(0, y_val, 255);
    *U = bound(0, u_val, 255);
    *V = bound(0, v_val, 255);
}

void genRGB24Data(uint8_t *rgbData, int width, int height) {
    for (int i = 0; i < width; ++i) {
        // 当前颜色
        uint32_t currentColor = rainbowColors[0];
        if(i < 100) {
            currentColor = rainbowColors[0];
        } else if(i < 200) {
            currentColor = rainbowColors[1];
        } else if(i < 300) {
            currentColor = rainbowColors[2];
        } else if(i < 400) {
            currentColor = rainbowColors[3];
        } else if(i < 500) {
            currentColor = rainbowColors[4];
        } else if(i < 600) {
            currentColor = rainbowColors[5];
        } else if(i < 700) {
            currentColor = rainbowColors[6];
        }
        // 当前颜色 R 分量
        uint8_t R = (currentColor & 0xFF0000) >> 16;
        // 当前颜色 G 分量
        uint8_t G = (currentColor & 0x00FF00) >> 8;
        // 当前颜色 B 分量
        uint8_t B = currentColor & 0x0000FF;

        for (int j = 0; j < height; ++j) {
            int currentIndex = 3*(i*height+j);
            rgbData[currentIndex] = R;
            rgbData[currentIndex+1] = G;
            rgbData[currentIndex+2] = B;
        }
    }
}

void rgb24ToYuv(const uint8_t *rgb24Data, int width, int height) {
    int8_t yuv_y[width*height];
    int8_t yuv_u[width*height];
    int8_t yuv_v[width*height];
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            uint8_t Y, U, V;
            uint8_t R, G, B;

            int currentRGBIndex = 3*(i*height+j);
            R = rgb24Data[currentRGBIndex];
            G = rgb24Data[currentRGBIndex+1];
            B = rgb24Data[currentRGBIndex+2];

            rgbToYuv(R, G, B, &Y, &U, &V);

            int currentYUVIndex = i*height+j;
            yuv_y[currentYUVIndex] = Y;
            yuv_u[currentYUVIndex] = U;
            yuv_v[currentYUVIndex] = V;
        }
    }
}