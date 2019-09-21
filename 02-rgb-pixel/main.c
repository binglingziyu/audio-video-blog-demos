#include <stdio.h>
#include <stdint.h>

// 彩虹的七种颜色
uint32_t rainbowColors[] = {
        0XFF0000, // 赤
        0XFFA500, // 橙
        0XFFFF00, // 黄
        0X00FF00, // 绿
        0X007FFF, // 青
        0X0000FF, // 蓝
        0X8B00FF  // 紫
};

void writeRainbow(char *outputFile, int width, int height) {

    // 打开文件
    FILE *rgbFile = fopen(outputFile, "wb+");
    for (int i = 0; i < width; ++i) {
        // 当前颜色
        uint32_t currentColor = rainbowColors[i];
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
            // 按 BGR 顺序写入一个像素 RGB24 到文件中
            fputc(R, rgbFile);
            fputc(G, rgbFile);
            fputc(B, rgbFile);
        }
    }

    // 关闭文件
    fclose(rgbFile);
}

int main() {
    writeRainbow("/Users/hubin/Desktop/rainbow-700x700.rgb24", 700, 700);
    return 0;
}