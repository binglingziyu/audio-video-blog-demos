//
// Created by hubin on 2019/9/17.
//

#include <stdio.h>
#include <string.h>

// 彩虹的七种颜色
u_int32_t rainbowColors[] = {
        0XFF0000, // 红
        0XFFA500, // 橙
        0XFFFF00, // 黄
        0X00FF00, // 绿
        0X007FFF, // 青
        0X0000FF, // 蓝
        0X8B00FF  // 紫
};

void rgbToYuv(u_int8_t R, u_int8_t G, u_int8_t B, int8_t *Y, int8_t *U, int8_t *V) {
    *Y = 0.257*R + 0.504*G + 0.098*B + 16;
    *U = -0.148*R - 0.291*G + 0.439*B + 128;
    *V = 0.439*R - 0.368*G - 0.071*B + 128;
}

void rgb24ToYuv444p(u_int8_t *rgb24Data, int8_t *yuv444pData, int width, int height) {

    int8_t yuv_y[width*height];
    int8_t yuv_u[width*height];
    int8_t yuv_v[width*height];

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int8_t Y, U, V;
            u_int8_t R, G, B;

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
    
    memcpy(yuv444pData, yuv_y, sizeof(yuv_y));
    memcpy(yuv444pData + sizeof(yuv_y), yuv_u, sizeof(yuv_u));
    memcpy(yuv444pData + sizeof(yuv_y) + sizeof(yuv_u), yuv_v, sizeof(yuv_v));
    
}

int main() {
    int width = 700, height = 700;
    int8_t yuv444pData[width*height*3];
    u_int8_t rgb24Data[width*height*3];
    
    FILE *rgb24File = fopen("/Users/hubin/Desktop/rainbow-rgb24.rgb", "rb");
    fread(rgb24Data, sizeof(rgb24Data), 1, rgb24File);
    
    rgb24ToYuv444p(rgb24Data, yuv444pData, width, height);

    FILE *yuv444pFile = fopen("/Users/hubin/Desktop/rainbow-rgb24-to-yuv444p.yuv", "wb");
    fwrite(yuv444pData, sizeof(yuv444pData), 1, yuv444pFile);
    
    fclose(rgb24File);
    fclose(yuv444pFile);
    return 0;
}