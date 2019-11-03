//
// Created by hubin on 2019/11/3.
//
#include "libattopng.h"

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


void testRainbow() {
    #define RGB(r, g, b) ((r) | ((g) << 8) | ((b) << 16))
    int width = 700, height = 700;
    // uint8_t rgb24Data[width*height*3];
    libattopng_t* png = libattopng_new(width, height, PNG_RGB);

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
            // rgb24Data[currentPixelIndex] = R;
            // rgb24Data[currentPixelIndex+1] = G;
            // rgb24Data[currentPixelIndex+2] = B;
            libattopng_set_pixel(png, j, i, RGB(R, G, B));
        }
    }

    // FILE *file = fopen("/Users/hubin/Desktop/libattopng-rgb-rainbow.png", "wb");
    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\libattopng-rgb-rainbow.png", "wb+");
    libattopng_save(png, "/Users/hubin/Desktop/libattopng-rgb-rainbow.png");
    libattopng_destroy(png);

}

void test01() {
    #define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))
    libattopng_t* png = libattopng_new(250, 200, PNG_RGBA);
    int x, y;
    for (y = 0; y < 200; y++) {
        for (x = 0; x < 250; x++) {
            libattopng_set_pixel(png, x, y, RGBA(x & 255, y & 255, 128, (255 - ((x / 2) & 255))));
        }
    }

    // FILE *file = fopen("/Users/hubin/Desktop/libattopng-test-rgba.png", "wb");
    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\libattopng-test-rgba", "wb+");
    libattopng_save(png, "/Users/hubin/Desktop/libattopng-test-rgba.png");
    libattopng_destroy(png);
}

void test02() {
    #define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))
    // ceate palette image
    libattopng_t *png = libattopng_new(256, 256, PNG_PALETTE);
    uint32_t palette[] = {RGBA(0, 0, 0xff, 0xff), RGBA(0, 0xff, 0, 0x80), RGBA(0xff, 0, 0, 0xff), RGBA(0xff, 0, 0xff, 0x80)};
    // 4 colors: blue, green (50% alpha), red, cyan (50% alpha)
    libattopng_set_palette(png, palette, 4);

    int x, y;
    for (y = 0; y < 256; y++) {
        for (x = 0; x < 256; x++) {
            libattopng_set_pixel(png, x, y, (x % 16) / 4);
        }
    }
    // FILE *file = fopen("/Users/hubin/Desktop/libattopng-test-rgba.png", "wb");
    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\libattopng-test-rgba", "wb+");
    libattopng_save(png, "/Users/hubin/Desktop/libattopng-test-palette.png");
    libattopng_destroy(png);
}

int main() {
    testRainbow();
    test01();
    test02();
    return 0;
}
