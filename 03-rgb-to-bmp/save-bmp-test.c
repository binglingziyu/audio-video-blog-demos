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

uint8_t* getRainbowRGB24Data(int width, int height) {
    // 打开文件
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
            // 按 BGR 顺序写入一个像素 RGB24 到文件中
            rgb24Data[currentPixelIndex] = B;
            rgb24Data[currentPixelIndex+1] = G;
            rgb24Data[currentPixelIndex+2] = R;
        }
    }
    return rgb24Data;
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

/* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
    unsigned short bfType=0x4d42;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+width*height*3;
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

    FILE *file = fopen("/Users/hubin/Desktop/a.bmp", "wb");
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
//            rgb24Data[currentPixelIndex] = B;
//            rgb24Data[currentPixelIndex+1] = G;
//            rgb24Data[currentPixelIndex+2] = R;

            fwrite(&B, 1, 1, file);
            fwrite(&G, 1, 1, file);
            fwrite(&R, 1, 1, file);
        }
    }
//    u_int8_t *rainbowRGBData = getRainbowRGB24Data(700, 700);
//    fwrite(rainbowRGBData, width*height*3, 1, file);

/*Write bitmap*/
//    for (int y = bih.biHeight-1; y>=0; y--) /*Scanline loop backwards*/
//    {
//        for (int x = 0; x < bih.biWidth; x++) /*Column loop forwards*/
//        {
//            /*compute some pixel values*/
//            unsigned char r = 255*((float)x/bih.biWidth);
//            unsigned char g = 255*((float)y/bih.biHeight);
//            unsigned char b = 0;
//            fwrite(&b, 1, 1, file);
//            fwrite(&g, 1, 1, file);
//            fwrite(&r, 1, 1, file);
//        }
//    }
    fclose(file);
}

int main() {
    bitmap(700, 700);
    return 0;
}