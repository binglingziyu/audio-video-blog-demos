//
// Created by hubin on 2021/1/21.
//
#include "rgb2yuv.h"

uint8_t bound(uint8_t start, int value, uint8_t end) {
    if(value <= start) {
        return start;
    }
    if(value >= end) {
        return end;
    }
    return value;
}

// Y = 0.257*R + 0.504*G + 0.098*B + 16;
// U = -0.148*R - 0.291*G + 0.439*B + 128;
// V = 0.439*R - 0.368*G - 0.071*B + 128;
void rgbToYuv(uint8_t R, uint8_t G, uint8_t B, uint8_t *Y, uint8_t *U, uint8_t *V) {
    int y_val = (int)round(0.257*R + 0.504*G + 0.098*B + 16);
    int u_val = (int)round(-0.148*R - 0.291*G + 0.439*B + 128);
    int v_val = (int)round(0.439*R - 0.368*G - 0.071*B + 128);
    *Y = bound(16, y_val, 235);
    *U = bound(16, u_val, 239);
    *V = bound(16, v_val, 239);
}


void rgb24ToYuv444p(const uint8_t *rgb24Data, uint8_t **yuv444pData, int width, int height) {
    *yuv444pData = malloc(width * height * 3);
    uint8_t *pYUV = *yuv444pData;

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
    memcpy(pYUV, yuv_y, sizeof(yuv_y));
    memcpy(pYUV + sizeof(yuv_y), yuv_u, sizeof(yuv_u));
    memcpy(pYUV + sizeof(yuv_y) + sizeof(yuv_u), yuv_v, sizeof(yuv_v));
}


void rgb24ToYuv420p(const uint8_t *rgb24Data, uint8_t **yuv420pData, int width, int height) {
    *yuv420pData = malloc(width * height * 3 / 2);
    uint8_t *pYUV = *yuv420pData;
    size_t image_size = width * height;
    size_t upos = image_size;
    size_t vpos = upos + upos / 4;
    size_t i = 0;

    for( size_t line = 0; line < height; ++line ) {
        if( !(line % 2) ) {
            for( size_t x = 0; x < width; x += 2 ) {
                uint8_t r = rgb24Data[3 * i];
                uint8_t g = rgb24Data[3 * i + 1];
                uint8_t b = rgb24Data[3 * i + 2];

                pYUV[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;

                pYUV[upos++] = ((-38*r + -74*g + 112*b) >> 8) + 128;
                pYUV[vpos++] = ((112*r + -94*g + -18*b) >> 8) + 128;

                r = rgb24Data[3 * i];
                g = rgb24Data[3 * i + 1];
                b = rgb24Data[3 * i + 2];

                pYUV[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        } else {
            for( size_t x = 0; x < width; x += 1 ) {
                uint8_t r = rgb24Data[3 * i];
                uint8_t g = rgb24Data[3 * i + 1];
                uint8_t b = rgb24Data[3 * i + 2];

                pYUV[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
    }
}