//
// Created by hubin on 2019/9/15.
//

#include "EasyBMP/EasyBMP.h"
using namespace std;

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

int main( int argc, char* argv[]) {

    int width = 711, height = 711;
    int bitDepth = 24;

    BMP bmp;
    bmp.SetSize(width, height);
    bmp.SetBitDepth(bitDepth);

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

        RGBApixel pixel;
        pixel.Red = R;
        pixel.Green = G;
        pixel.Blue = B;
        pixel.Alpha = 0;
        for (int j = 0; j < height; ++j) {
            bmp.SetPixel(j, i, pixel);
        }
    }

    bmp.WriteToFile("/Users/hubin/Desktop/test-easybmp.bmp");

    return 0;
}