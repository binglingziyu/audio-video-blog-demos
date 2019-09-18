//
// Created by hubin on 2019/9/17.
//
#include <stdio.h>

//R = 1.164(Y - 16) + 1.596(V - 128)
//G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
//B = 1.164(Y - 16)                   + 2.018(U - 128)

u_int8_t getR(int8_t y, int8_t Cr){
//    int num = y + 1.779*(u - 128) ;
//    int num = 1.164*y + 1.596*Cr - 222.912;
    int num = 1.164*(y - 16) + 1.596*(Cr - 128);
    if( num < 0 ){
        num = 0;
    }else if( num > 255){
        num = 255 ;
    }
    return num;
}


u_int8_t getG(int8_t y, int8_t Cb, int8_t Cr){
//    int num = y - 0.34413*(v - 128)  - 0.71414*(u - 128) ;
//    int num = 1.164*y - 0.391*Cb - 0.813*Cr + 135.488;
    int num = 1.164*(y - 16) - 0.813*(Cr - 128) - 0.391*(Cb - 128);
    if( num < 0){
        num = 0;
    }else if(num > 255){
        num = 255 ;
    }
    return  num ;
}

u_int8_t getB(int8_t y, int8_t Cb) {
//    int num = y + 1.402*(v - 128);
//    int num = 1.164*y + 2.018*Cb - 276.928;
    int num =  1.164*(y - 16) + 2.018*(Cb - 128);
    if (num < 0) {
        num = 0;
    } else if (num > 255) {
        num = 255;
    }
    return num;
}

void yuv444pToRGB(int8_t *yuv444pData,u_int8_t *rgb24Data, int width, int height) {
    int8_t *srcY = yuv444pData, *srcU = srcY + width * height, *srcV = srcU + width * height;

    for(int i = 0 ; i < height ; i ++) {
        for (int j = 0; j < width; j++) {
            int currentYUVIndex = i * height + j;
            int8_t Y = srcY[currentYUVIndex], U = srcU[currentYUVIndex], V = srcV[currentYUVIndex];

            int currentRGBIndex = 3*(i * width + j);
            rgb24Data[currentRGBIndex] = getR(Y, U);
            rgb24Data[currentRGBIndex+1] = getG(Y, U, V);
            rgb24Data[currentRGBIndex+2] = getB(Y, V);
        }
    }
}

int main() {
    int width = 700, height = 700;
    int8_t yuv444pData[width*height*3];
    u_int8_t rgb24Data[width*height*3];

    FILE *yuv444pFile = fopen("/Users/hubin/Desktop/rainbow-yuv444p.yuv", "rb");
    fread(yuv444pData, sizeof(yuv444pData), 1, yuv444pFile);

     yuv444pToRGB(yuv444pData, rgb24Data, width, height);

    FILE *rgb24File = fopen("/Users/hubin/Desktop/rainbow-yuv444p-to-rgb24.rgb", "wb");
    fwrite(rgb24Data, sizeof(rgb24Data), 1, rgb24File);

    fclose(yuv444pFile);
    fclose(rgb24File);
    return 0;
}