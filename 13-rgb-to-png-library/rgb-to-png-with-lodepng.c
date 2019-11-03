//
// Created by hubin on 2019/11/3.
//
#include "lodepng.h"

#include <stdio.h>
#include <stdlib.h>


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
    int width = 700, height = 700;
    uint8_t rgb24Data[width*height*3];

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
            // 按 RGB 顺序写入一个像素 RGB24 到文件中
             rgb24Data[currentPixelIndex] = R;
             rgb24Data[currentPixelIndex+1] = G;
             rgb24Data[currentPixelIndex+2] = B;
        }
    }

    // FILE *file = fopen("/Users/hubin/Desktop/lodepng-rgb-rainbow.png", "wb");
    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\lodepng-rgb-rainbow.png", "wb+");
    unsigned error = lodepng_encode24_file("/Users/hubin/Desktop/lodepng-rgb-rainbow.png", rgb24Data, width, height);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}

/*
3 ways to encode a PNG from RGBA pixel data to a file (and 2 in-memory ways).
NOTE: this samples overwrite the file or test.png without warning!
*/

/*
Example 1
Encode from raw pixels to disk with a single function call
The image argument has width * height RGBA pixels or width * height * 4 bytes
*/
void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
    /*Encode the image*/
    unsigned error = lodepng_encode32_file(filename, image, width, height);

    /*if there's an error, display it*/
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}

/*
Example 2
Encode from raw pixels to an in-memory PNG file first, then write it to disk
The image argument has width * height RGBA pixels or width * height * 4 bytes
*/
void encodeTwoSteps(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
    unsigned char* png;
    size_t pngsize;

    unsigned error = lodepng_encode32(&png, &pngsize, image, width, height);
    if(!error) lodepng_save_file(png, pngsize, filename);

    /*if there's an error, display it*/
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

    free(png);
}

/*
Example 3
Save a PNG file to disk using a State, normally needed for more advanced usage.
The image argument has width * height RGBA pixels or width * height * 4 bytes
*/
void encodeWithState(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
    unsigned error;
    unsigned char* png;
    size_t pngsize;
    LodePNGState state;

    lodepng_state_init(&state);
    /*optionally customize the state*/

    error = lodepng_encode(&png, &pngsize, image, width, height, &state);
    if(!error) lodepng_save_file(png, pngsize, filename);

    /*if there's an error, display it*/
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

    lodepng_state_cleanup(&state);
    free(png);
}

int main(int argc, char *argv[]) {
    // FILE *file = fopen("/Users/hubin/Desktop/lodepng-test.png", "wb");
    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\lodepng-test.png", "wb+");
    const char* filename = argc > 1 ? argv[1] : "/Users/hubin/Desktop/lodepng-test.png";

    /*generate some image*/
    unsigned width = 512, height = 512;
    unsigned char* image = malloc(width * height * 4);
    unsigned x, y;
    for(y = 0; y < height; y++)
        for(x = 0; x < width; x++) {
            image[4 * width * y + 4 * x + 0] = 255 * !(x & y);
            image[4 * width * y + 4 * x + 1] = x ^ y;
            image[4 * width * y + 4 * x + 2] = x | y;
            image[4 * width * y + 4 * x + 3] = 255;
        }

    /*run an example*/
    encodeOneStep(filename, image, width, height);

    free(image);

    testRainbow();
    return 0;
}
