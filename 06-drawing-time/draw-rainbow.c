//
// Created by hubin on 2019/9/16.
//

#include <stdio.h>
#include <stdint.h>
#include "bmp/bmp.h"

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

void writeRGBToBmp(char *filename, int width, int height) {
    char bmp[BMP_SIZE(width, height)];
    bmp_init(bmp, width, height);

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
        for (int j = 0; j < height; ++j) {
            bmp_set(bmp, j, i, currentColor);
        }
    }

    FILE *f = fopen(filename, "wb");
    fwrite(bmp, sizeof(bmp), 1, f);
    fclose(f);
}

int main() {

    writeRGBToBmp("/Users/hubin/Desktop/draw-rainbow.bmp", 700, 700);

    return 0;
}