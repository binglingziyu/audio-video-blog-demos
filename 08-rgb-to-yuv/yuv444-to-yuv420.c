//
// Created by hubin on 2019/9/17.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>

// 彩虹的七种颜色
uint32_t rainbowColors[] = {
        0XFF0000, // 红
        0XFFA500, // 橙
        0XFFFF00, // 黄
        0X00FF00, // 绿
        0X007FFF, // 青
        0X0000FF, // 蓝
        0X8B00FF  // 紫
};

void rgbToYuv(uint8_t R, uint8_t G, uint8_t B, int8_t *Y, int8_t *U, int8_t *V) {
    *Y = 0.257*R + 0.504*G + 0.098*B + 16;
    *U = -0.148*R - 0.291*G + 0.439*B + 128;
    *V = 0.439*R - 0.368*G - 0.071*B + 128;
}

void rgb24ToYuv444(uint8_t *outbuf, int width, int height) {

    int8_t yuv_y[width*height];
    int8_t yuv_u[width*height];
    int8_t yuv_v[width*height];

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
            int8_t Y, U, V;

            rgbToYuv(R, G, B, &Y, &U, &V);

            int currentIndex = i*height+j;
            yuv_y[currentIndex] = Y;
            yuv_u[currentIndex] = U;
            yuv_v[currentIndex] = V;

        }
    }

    memcpy(outbuf, yuv_y, sizeof(yuv_y));
    memcpy(outbuf+width*height, yuv_u, sizeof(yuv_u));
    memcpy(outbuf+width*height*2, yuv_v, sizeof(yuv_v));

}

void yuv444ToYuv420(uint8_t *inbuf, uint8_t *outbuf, int w, int h) {
    uint8_t *srcY = NULL, *srcU = NULL, *srcV = NULL;
    uint8_t *desY = NULL, *desU = NULL, *desV = NULL;
    srcY = inbuf;//Y
    srcU = srcY + w * h;//U
    srcV = srcU + w * h;//V

    desY = outbuf;
    desU = desY + w * h;
    desV = desU + w * h / 4;

    int half_width = w / 2;
    int half_height = h / 2;
    memcpy(desY, srcY, w * h * sizeof(uint8_t));//Y分量直接拷贝即可
    //UV
    for (int i = 0; i < half_height; i++) {
        for (int j = 0; j < half_width; j++) {
            *desU = *srcU;
            *desV = *srcV;
            desU++;
            desV++;
            srcU += 2;
            srcV += 2;
        }
        srcU = srcU + w;
        srcV = srcV + w;
    }
}

int main() {

    int width = 700, height = 700;
    uint8_t yuv444pData[width*height*3];
    uint8_t yuv420pData[width*height*3/2];
    rgb24ToYuv444(yuv444pData, width, height);
    yuv444ToYuv420(yuv444pData, yuv420pData, width, height);

    FILE *yuvFile = fopen("/Users/hubin/Desktop/rainbow-700x700-yuv420p.yuv", "wb");

    fwrite(yuv420pData, sizeof(yuv420pData), 1, yuvFile);

    fclose(yuvFile);

    return 0;
}
