//
// Created by hubin on 2019/9/17.
//
#include <stdio.h>

//void rgbToYuv(u_int8_t R, u_int8_t G, u_int8_t B, int8_t *Y, int8_t *U, int8_t *V) {
//    *Y = 0.257*R + 0.504*G + 0.098*B + 16;
//    *U = -0.148*R - 0.291*G + 0.439*B + 128;
//    *V = 0.439*R - 0.368*G - 0.071*B + 128;
//}

void yuvToRgb(int8_t Y, int8_t U, int8_t V, u_int8_t *R, u_int8_t *G, u_int8_t *B) {
    *R = 1.164*(Y - 16) + 1.596*(V - 128);
    *G = 1.164*(Y - 16) - 0.392*(U - 128)-0.813*(V - 128);
    *B = 1.164*(Y - 16) + 2.017*(U - 128);
}

void yuv444ToRGB(u_int8_t *rgb24Data, int8_t *yuv444Data, int width, int height) {
    int8_t *srcY = NULL, *srcU = NULL, *srcV = NULL;
    srcY = yuv444Data;//Y
    srcU = srcY + width * height;//U
    srcV = srcU + width * height;;//V
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int currentIndex = i * height + j;
            int8_t Y = srcY[currentIndex], U = srcU[currentIndex], V = srcV[currentIndex];
            u_int8_t R, G, B;
            yuvToRgb(Y, U, V, &R, &G, &B);

            int currentRGBIndex = 3*(i * height + j);
            rgb24Data[currentRGBIndex] = R;
            rgb24Data[currentRGBIndex+1] = G;
            rgb24Data[currentRGBIndex+2] = B;

        }
    }
}

int main() {
    int width = 700, height = 700;
    int8_t yuv444pData[width*height*3];
    u_int8_t rgb24Data[width*height*3];

    FILE *yuv444File = fopen("/Users/hubin/Desktop/rainbow-yuv444p.yuv", "rb");
    fread(yuv444pData, sizeof(yuv444pData), 1, yuv444File);

    yuv444ToRGB(rgb24Data, yuv444pData, width, height);

    FILE *rgb24File = fopen("/Users/hubin/Desktop/rainbow-700x700-yuv444-to-rgb24.rgb24", "wb");
    fwrite(rgb24Data, sizeof(rgb24Data), 1, rgb24File);

    fclose(yuv444File);
    fclose(rgb24File);
    return 0;
}