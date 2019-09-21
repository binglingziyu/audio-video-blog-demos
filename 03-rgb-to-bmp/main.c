//
// Created by hubin on 2019/9/12.
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

/*bmp file header*/
typedef struct {
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} BitmapFileHeader;

/*bmp info header*/
typedef struct {
    unsigned int   biSize; /* Size of info header */
    int            biWidth; /* Width of image */
    int            biHeight; /* Height of image */
    unsigned short biPlanes; /* Number of color planes */
    unsigned short biBitCount; /* Number of bits per pixel */
    unsigned int   biCompression; /* Type of compression to use */
    unsigned int   biSizeImage; /* Size of image data */
    int            biXPelsPerMeter; /* X pixels per meter */
    int            biYPelsPerMeter; /* Y pixels per meter */
    unsigned int   biClrUsed; /* Number of colors used */
    unsigned int   biClrImportant; /* Number of important colors */
} BitmapInfoHeader;

void writeRGBToBmp(char *filename, int width, int height) {
    FILE *bitmapFile = fopen(filename, "wb");
    if(!bitmapFile) {
        printf("Could not write file \n");
        return;
    }

    uint16_t bfType = 0x4d42;

    BitmapFileHeader fileHeader;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfSize = 2 + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + width*height*3;
    fileHeader.bfOffBits = 0x36;

    BitmapInfoHeader infoHeader;
    infoHeader.biSize = sizeof(BitmapInfoHeader);
    infoHeader.biWidth = width;
//    infoHeader.biHeight = height;
    infoHeader.biHeight = -height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biSizeImage = 0;
    infoHeader.biCompression = 0;
    infoHeader.biXPelsPerMeter = 5000;
    infoHeader.biYPelsPerMeter = 5000;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    fwrite(&bfType, sizeof(bfType), 1, bitmapFile);
    fwrite(&fileHeader, sizeof(fileHeader), 1, bitmapFile);
    fwrite(&infoHeader, sizeof(infoHeader), 1, bitmapFile);

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
            // 按 BGR 顺序写入一个像素 RGB24 到文件中
            fwrite(&B, 1, 1, bitmapFile);
            fwrite(&G, 1, 1, bitmapFile);
            fwrite(&R, 1, 1, bitmapFile);
        }
    }

    fclose(bitmapFile);
}

int main() {

    writeRGBToBmp("/Users/hubin/Desktop/rainbow-700x700.bmp", 700, 700);

    return 0;
}