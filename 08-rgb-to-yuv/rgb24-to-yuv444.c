//
// Created by hubin on 2019/9/17.
//

#include <stdio.h>

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

void rgb24ToYuv444(char *yuvFileName, int width, int height) {

    int8_t yuv_y[width*height];
    int8_t yuv_u[width*height];
    int8_t yuv_v[width*height];

    for (int i = 0; i < width; ++i) {
        // 当前颜色
        u_int32_t currentColor = rainbowColors[0];
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
        u_int8_t R = (currentColor & 0xFF0000) >> 16;
        // 当前颜色 G 分量
        u_int8_t G = (currentColor & 0x00FF00) >> 8;
        // 当前颜色 B 分量
        u_int8_t B = currentColor & 0x0000FF;

        for (int j = 0; j < height; ++j) {
            int8_t Y, U, V;

            rgbToYuv(R, G, B, &Y, &U, &V);

            int currentIndex = i*height+j;
            yuv_y[currentIndex] = Y;
            yuv_u[currentIndex] = U;
            yuv_v[currentIndex] = V;

        }
    }

    FILE *yuvFile = fopen(yuvFileName, "wb");

    fwrite(yuv_y, sizeof(yuv_y), 1, yuvFile);
    fwrite(yuv_u, sizeof(yuv_u), 1, yuvFile);
    fwrite(yuv_v, sizeof(yuv_v), 1, yuvFile);

    fclose(yuvFile);
}

int main() {
    rgb24ToYuv444("/Users/hubin/Desktop/rainbow-700x700-yuv444p.yuv", 700, 700);

    return 0;
}