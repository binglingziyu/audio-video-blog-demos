//
// Created by hubin on 2019/11/3.
//

#include "svpng.inc"
#include <stdint.h>

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

uint8_t* getRainbowRGB24Data(uint8_t *rgb24Data, int width, int height) {
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
            int currentPixelIndex = 3*(i*height + j);
            // 按 BGR 顺序写入一个像素 RGB24 到文件中
            rgb24Data[currentPixelIndex] = R;
            rgb24Data[currentPixelIndex+1] = G;
            rgb24Data[currentPixelIndex+2] = B;
        }
    }
    return rgb24Data;
}

void rainbow_rgb() {
    int width = 700, height = 700;
    uint8_t rgb24Data[width*height*3];
    FILE *file = fopen("/Users/hubin/Desktop/svpng-rgb-rainbow.png", "wb");
    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\svpng-rgb-rainbow.png", "wb+");

    getRainbowRGB24Data(rgb24Data, width, height);
    svpng(file, width, height, rgb24Data, 0);
    fclose(file);

}

void test_rgb(void) {
    unsigned char rgb[256 * 256 * 3], *p = rgb;
    unsigned x, y;
    FILE *file = fopen("/Users/hubin/Desktop/svpng-rgb.png", "wb");
    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\svpng-rgb.png", "wb+");
    for (y = 0; y < 256; y++)
        for (x = 0; x < 256; x++) {
            *p++ = (unsigned char)x;    /* R */
            *p++ = (unsigned char)y;    /* G */
            *p++ = 128;                 /* B */
        }
    svpng(file, 256, 256, rgb, 0);
    fclose(file);
}

void test_rgba(void) {
    unsigned char rgba[256 * 256 * 4], *p = rgba;
    unsigned x, y;
    FILE *file = fopen("/Users/hubin/Desktop/svpng-rgba.png", "wb");
    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\svpng-rgba.png", "wb+");
    for (y = 0; y < 256; y++)
        for (x = 0; x < 256; x++) {
            *p++ = (unsigned char)x;                /* R */
            *p++ = (unsigned char)y;                /* G */
            *p++ = 128;                             /* B */
            *p++ = (unsigned char)((x + y) / 2);    /* A */
        }
    svpng(file, 256, 256, rgba, 1);
    fclose(file);
}

int main(void) {
    test_rgb();
    test_rgba();
    rainbow_rgb();
    return 0;
}
