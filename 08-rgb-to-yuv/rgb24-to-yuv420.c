//
// Created by hubin on 2019/9/17.
//

#include <stdio.h>
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

void rgb24ToYuv420p(uint8_t *destination, uint8_t *rgb, int width, int height) {
    size_t image_size = width * height;
    size_t upos = image_size;
    size_t vpos = upos + upos / 4;
    size_t i = 0;

    for( size_t line = 0; line < height; ++line ) {
        if( !(line % 2) ) {
            for( size_t x = 0; x < width; x += 2 ) {
                uint8_t r = rgb[3 * i];
                uint8_t g = rgb[3 * i + 1];
                uint8_t b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;

                destination[upos++] = ((-38*r + -74*g + 112*b) >> 8) + 128;
                destination[vpos++] = ((112*r + -94*g + -18*b) >> 8) + 128;

                r = rgb[3 * i];
                g = rgb[3 * i + 1];
                b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        } else {
            for( size_t x = 0; x < width; x += 1 ) {
                uint8_t r = rgb[3 * i];
                uint8_t g = rgb[3 * i + 1];
                uint8_t b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
    }
}

int main() {
    int width = 700, height = 700;
    uint8_t rgb24Data[width*height*3];
    uint8_t yuv420pData[width*height*3/2];
    
    genRGB24Data(rgb24Data, width, height);
    rgb24ToYuv420p(yuv420pData, rgb24Data, width, height);

    FILE *yuvFile = fopen("/Users/hubin/Desktop/rainbow-700x700-rgb24-toyuv420p.yuv", "wb");
    fwrite(yuv420pData, sizeof(yuv420pData), 1, yuvFile);
    fclose(yuvFile);
    
    return 0;
}