//
// Created by hubin on 2019/11/8.
//

#include "color.h"

uint8_t bound(uint8_t start, int value, uint8_t end) {
    if(value <= start) {
        return start;
    }
    if(value >= end) {
        return end;
    }
    return value;
}

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

void rgb24ToYuv(const uint8_t *rgb24Data, uint8_t *yuv_y, uint8_t *yuv_u, uint8_t *yuv_v, int width, int height) {
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




