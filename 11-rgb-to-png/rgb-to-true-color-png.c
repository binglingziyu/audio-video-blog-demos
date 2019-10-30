//
// Created by hubin on 2019/10/14.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zlib.h"

// ***** functions in util.c *****
bool isBigEndianOrder();
void genRGB24Data(uint8_t *rgbData, int width, int height);
uint32_t switchUint32(uint32_t i);
uint32_t calcCrc32(uint32_t dataASCII, uint8_t *data, uint32_t length);

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
} PNG_IHDR_DATA;

int main() {
    // PNG 图片尺寸
    int width = 700, height = 700;
    // IDAT 中数据部分长度
    uint32_t IDAT_RGB_DATA_LENGTH = width*height*3+height;

    // PNG 文件包括 8 字节文件署名（89 50 4E 47 0D 0A 1A 0A，十六进制），用来识别 PNG 格式。
    uint8_t PNG_FILE_SIGNATURE[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    // IHDR 每个字母对应的 ASCII
    uint32_t IHDR_ASCII = switchUint32(0x49484452);
    // IDAT 每个字母对应的ASCII
    uint32_t IDAT_ASCII = switchUint32(0x49444154);
    // PNG 文件的结尾 12 个字节看起来总应该是这样的：（00 00 00 00 49 45 4E 44 AE 42 60 82，十六进制）
    uint8_t PNG_IEND_DATA[] = {0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};

    FILE *file = fopen("/Users/hubin/Desktop/0-true-color.png", "wb");
    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\0-true-color.png", "wb+");
    if (!file) {
        printf("Could not write file\n");
        return -1;
    }

    // 真彩 PNG 图片 存储的是 RGB 数
    uint8_t *rgb24Data = (uint8_t *)malloc(IDAT_RGB_DATA_LENGTH);
    // 填充 IDAT 的 RGB 数据
    genRGB24Data(rgb24Data, width, height);

    // 写 PNG 文件署名
    fwrite(PNG_FILE_SIGNATURE, 1, sizeof(PNG_FILE_SIGNATURE), file);

    // 准备 IHDR 数据
    PNG_IHDR_DATA pngIhdrData;
    pngIhdrData.width = switchUint32(width);
    pngIhdrData.height = switchUint32(height);
    pngIhdrData.bitDepth = 8;
    pngIhdrData.colorType = 2;// 2：真彩色图像，8或16    6：带α通道数据的真彩色图像，8或16
    pngIhdrData.compressionMethod = 0;
    pngIhdrData.filterMethod = 0;
    pngIhdrData.interlaceMethod = 0;

    // IHDR 数据长度
    uint32_t IHDR_DATA_LENGTH = 13;
    // IHDR 数据长度 转换成大端字节序
    uint32_t pngIhdrDataSize = switchUint32(IHDR_DATA_LENGTH);
    // 计算 IHDR CRC32
    uint32_t ihdrDataCrc32 = calcCrc32(IHDR_ASCII, (uint8_t *) &pngIhdrData, IHDR_DATA_LENGTH);

    // 写 IHDR 数据长度
    fwrite(&pngIhdrDataSize, 1, sizeof(pngIhdrDataSize), file);
    // 写 IHDR ASCII
    fwrite(&IHDR_ASCII, 1, sizeof(IHDR_ASCII), file);
    // 写 IHDR 数据
    fwrite(&pngIhdrData, 1, IHDR_DATA_LENGTH, file);
    // 写 IHDR CRC32
    fwrite(&ihdrDataCrc32, 1, sizeof(ihdrDataCrc32), file);

    // zlib 压缩数据
    uint8_t buf[IDAT_RGB_DATA_LENGTH];
    // 压缩后 buf 的数据长度 压缩完成后就是实际大小了
    uint32_t buflen = IDAT_RGB_DATA_LENGTH;

    // 执行 zlib 的压缩方法
    compress(buf, (uLongf *) &buflen, rgb24Data, IDAT_RGB_DATA_LENGTH);
    printf("\n压缩前数据长度：%d \n压缩后数据长度为:%d \n", IDAT_RGB_DATA_LENGTH, buflen);

    // 计算 IDAT CRC32
    uint32_t idatDataCrc32 = calcCrc32(IDAT_ASCII, buf, buflen);
    // IDAT 数据长度 转换成大端字节序
    uint32_t tmpBuflen = switchUint32(buflen);

    // 写 IDAT 数据长度
    fwrite(&tmpBuflen, 1, sizeof(tmpBuflen), file);
    // 写 IDAT ASCII
    fwrite(&IDAT_ASCII, 1, sizeof(IDAT_ASCII), file);
    // 写 IDAT 数据
    fwrite(buf, 1, buflen, file);
    // 写 IDAT CRC32
    fwrite(&idatDataCrc32, 1, sizeof(idatDataCrc32), file);

    // 写 IEND 信息
    fwrite(PNG_IEND_DATA, 1, sizeof(PNG_IEND_DATA), file);

    // 查看字节序
    if(isBigEndianOrder()) {
        printf("大端字节序");
    } else {
        printf("小端字节序");
    }

    // 收尾工作
    fflush(file);
    free(rgb24Data);
    fclose(file);
    return 0;
}
