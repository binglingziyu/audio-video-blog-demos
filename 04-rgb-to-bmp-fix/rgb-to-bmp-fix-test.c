//
// Created by hubin on 2019/9/13.
//

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

// 计算每一行像素 4 字节对齐后的字节数
int caculateLineBytes(int width) {
    //******* 四字节对齐 *******
    return (24 * width + 31)/32 *4;
    //******* 四字节对齐 *******
}

void bitmap(int width, int height)
{
    typedef struct                       /**** BMP file header structure ****/
    {
        unsigned int   bfSize;           /* Size of file */
        unsigned short bfReserved1;      /* Reserved */
        unsigned short bfReserved2;      /* ... */
        unsigned int   bfOffBits;        /* Offset to bitmap data */
    } BITMAPFILEHEADER;

    typedef struct                       /**** BMP file info structure ****/
    {
        unsigned int   biSize;           /* Size of info header */
        int            biWidth;          /* Width of image */
        int            biHeight;         /* Height of image */
        unsigned short biPlanes;         /* Number of color planes */
        unsigned short biBitCount;       /* Number of bits per pixel */
        unsigned int   biCompression;    /* Type of compression to use */
        unsigned int   biSizeImage;      /* Size of image data */
        int            biXPelsPerMeter;  /* X pixels per meter */
        int            biYPelsPerMeter;  /* Y pixels per meter */
        unsigned int   biClrUsed;        /* Number of colors used */
        unsigned int   biClrImportant;   /* Number of important colors */
    } BITMAPINFOHEADER;

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    int lineBytes = caculateLineBytes(width);

/* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
    unsigned short bfType=0x4d42;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + lineBytes*height;
    bfh.bfOffBits = 0x36;

    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = -height;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = 0;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 5000;
    bih.biYPelsPerMeter = 5000;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    FILE *file = fopen("/Users/hubin/Desktop/7x7.bmp", "wb");
    if (!file)
    {
        printf("Could not write file\n");
        return;
    }

/*Write headers*/
    fwrite(&bfType,1,sizeof(bfType),file);
    fwrite(&bfh, 1, sizeof(bfh), file);
    fwrite(&bih, 1, sizeof(bih), file);

    for (int i = 0; i < width; ++i) {

        // 当前颜色
        uint32_t currentColor = rainbowColors[i];

        // 当前颜色 R 分量
        uint8_t R = (currentColor & 0xFF0000) >> 16;
        // 当前颜色 G 分量
        uint8_t G = (currentColor & 0x00FF00) >> 8;
        // 当前颜色 B 分量
        uint8_t B = currentColor & 0x0000FF;

        uint8_t lineBytesArray[lineBytes];

        for (int j = 0; j < height; ++j) {
            int currentIndex = 3*j;
            lineBytesArray[currentIndex] = B;
            lineBytesArray[currentIndex+1] = G;
            lineBytesArray[currentIndex+2] = R;
        }

        fwrite(lineBytesArray, sizeof(lineBytesArray), 1, file);
    }

    fclose(file);
}

int main() {
    bitmap(7, 7);
    return 0;
}