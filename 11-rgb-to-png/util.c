//
// Created by hubin on 2019/10/14.
//

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "zlib.h"

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

/**
 * 生成索引 PNG 图片的调色板 PLTE
 * @param rgbPLTEData
 */
void genRGBPLTE(uint8_t *rgbPLTEData) {
    for (int i = 0; i < 7; ++i) {
        uint32_t currentColor = rainbowColors[i];
        // 当前颜色 R 分量
        uint8_t R = (currentColor & 0xFF0000) >> 16;
        // 当前颜色 G 分量
        uint8_t G = (currentColor & 0x00FF00) >> 8;
        // 当前颜色 B 分量
        uint8_t B = currentColor & 0x0000FF;

        int currentIndex = 3*i;
        rgbPLTEData[currentIndex] = R;
        rgbPLTEData[currentIndex+1] = G;
        rgbPLTEData[currentIndex+2] = B;
    }
}

/**
 * 生成索引 PNG 图片的图像数据块 IDAT
 * @param rgbIndexData
 * @param width
 * @param height
 */
void genRGBIndexData(uint8_t *rgbIndexData, int width, int height) {
    for (int i = 0; i < height; ++i) {
        uint8_t currentColorIndex = 0;
        if(i < 100) {
            currentColorIndex = 0;
        } else if(i < 200) {
            currentColorIndex = 1;
        } else if(i < 300) {
            currentColorIndex = 2;
        } else if(i < 400) {
            currentColorIndex = 3;
        } else if(i < 500) {
            currentColorIndex = 4;
        } else if(i < 600) {
            currentColorIndex = 5;
        } else if(i < 700) {
            currentColorIndex = 6;
        }
        // 每个扫描行前第一个字节是过滤器类型
        rgbIndexData[(i*width)/2+i] = 0x00;
        for (int j = 0; j < width; ++j) {
            int currentIndex = (i*width+j)/2+(i+1);
            int positionInByte = j%2;
            if(positionInByte == 0) {
                rgbIndexData[currentIndex] = currentColorIndex << 4;
            } else {
                rgbIndexData[currentIndex] += currentColorIndex;
            }
        }
    }
}

void genRGB24Data(uint8_t *rgbData, int width, int height) {

    for (int i = 0; i < height; ++i) {
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

        // 每个扫描行前第一个字节是过滤器类型
        rgbData[3*(i*width)+i] = 0x00;

        for (int j = 0; j < width; ++j) {
            int currentIndex = 3*(i*width+j)+(i+1);
            rgbData[currentIndex] = R;
            rgbData[currentIndex+1] = G;
            rgbData[currentIndex+2] = B;
        }
    }
}

/**
 * 判断是 大端字节序 OR 小端字节序
 * @return
 */
bool isBigEndianOrder() {
    int iVal = 1;
    char *pChar = (char*)(&iVal);
    if(*pChar==1) return false; //(0x01000000) Windows 采用的是小端法
    else return true; //(0x00000001)  Aix采用的是大端法
}

/**
 * 16 位字节序转换
 * @param s
 * @return
 */
uint16_t switchUint16(uint16_t s) {
    return ((s & 0x00FF) << 8) | ((s & 0xFF00) >> 8);
}

/**
 * 32 位字节序转换
 * @param i
 * @return
 */
uint32_t switchUint32(uint32_t i) {
    return ((i & 0x000000FF) << 24) | ((i & 0x0000FF00) << 8) | ((i & 0x00FF0000) >> 8) | ((i & 0xFF000000) >> 24);
}

/**
 * 计算数据块 CRC32
 * @param dataASCII
 * @param data
 * @param dataLength
 * @return
 */
uint32_t calcCrc32(uint32_t dataASCII, uint8_t *data, uint32_t dataLength) {
    uint8_t dataBuffer[4+dataLength];
    memcpy(dataBuffer, &dataASCII, 4);
    memcpy(dataBuffer + 4, data, dataLength);
    uint32_t crc32Value = 0;
    crc32Value = crc32(crc32Value, dataBuffer, 4+dataLength);
    return switchUint32(crc32Value);
}