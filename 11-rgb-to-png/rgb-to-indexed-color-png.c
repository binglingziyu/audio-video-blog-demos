//
// Created by hubin on 2019/10/14.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "zlib.h"

bool IsBigEndianOrder();
void genRGBPLTE(uint8_t *rgbData);
void genRGBIndexData(uint8_t *rgbIndexData, int width, int height);
uint32_t switchUint32(uint32_t i);

uint8_t pngFileType[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
uint8_t IEND[] = {0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
} PNGFILEHEADER;

int main() {
    printf("HERE");
    int width = 700, height = 700;

    //uint8_t *rgbData = (uint8_t *)malloc(7*3);
    //uint8_t *rgbIndexData = (uint8_t *)malloc(width*height/2+height);
    printf("HERE");
    uint32_t IHDR_ASCII = switchUint32(0x49484452);
    uint32_t PLTE_ASCII = switchUint32(0x504C5445);
    uint32_t IDAT_ASCII = switchUint32(0x49444154);

     FILE *file = fopen("/Users/hubin/Desktop/0-indexed-color.png", "wb");
//     FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\0-indexed-color.png", "wb+");

    if (!file) {
        printf("Could not write file\n");
        return -1;
    }
    // 红橙黄绿青蓝紫-七种颜色的调色板
    uint8_t rgbData[7*3] = {};
    uint8_t rgbIndexData[245700] = {}; //(uint8_t *)malloc(width*height/2+height);
    printf("HERE");
    // 填充 红橙黄绿青蓝紫-七种颜色的调色板
    genRGBPLTE(rgbData);
    printf("HERE");
    // 填充 RGB 数据
    genRGBIndexData(rgbIndexData, width, height);
    printf("HERE");
    // 写文件标识
    fwrite(pngFileType, 1, sizeof(pngFileType), file);

    PNGFILEHEADER pngfileheader;
    pngfileheader.width = switchUint32(width);
    pngfileheader.height = switchUint32(height);
    pngfileheader.bitDepth = 4;
    pngfileheader.colorType = 3;// 3：索引彩色图像，1，2，4或8
    pngfileheader.compressionMethod = 0;
    pngfileheader.filterMethod = 0;
    pngfileheader.interlaceMethod = 0;

    uint32_t pngfileheaderSize = switchUint32(0x0000000D);

    uint32_t pngfileheaderCrc32 = 0;//switchUint32(0x0DBA6C43);

    printf("\n文件头：%ld\n", sizeof(PNGFILEHEADER));
    printf("uint32_t: %ld\n", sizeof(uint32_t));

    fwrite(&pngfileheaderSize, 1, sizeof(pngfileheaderSize), file);
    fwrite(&IHDR_ASCII, 1, sizeof(IHDR_ASCII), file);
    // 写入文件头
    fwrite(&pngfileheader, 1, 13/*sizeof(pngfileheader)*/, file);

    uint8_t headerBuffer[13+4];
    memcpy(headerBuffer, &IHDR_ASCII, 4);
    memcpy(headerBuffer + 4, &pngfileheader, 13);
    pngfileheaderCrc32 = crc32(pngfileheaderCrc32, headerBuffer, 13+4);
    pngfileheaderCrc32 = switchUint32(pngfileheaderCrc32);
    fwrite(&pngfileheaderCrc32, 1, sizeof(pngfileheaderCrc32), file);


    // 开始写调色板 PLTE
    uint32_t PLTE_DATA_SIZE = switchUint32(21);

    fwrite(&PLTE_DATA_SIZE, 1, sizeof(PLTE_DATA_SIZE), file);
    fwrite(&PLTE_ASCII, 1, sizeof(PLTE_ASCII), file);
    fwrite(rgbData, 1, sizeof(rgbData), file);
    uint8_t PLtEBuffer[PLTE_DATA_SIZE+4];
    memcpy(PLtEBuffer, &PLTE_ASCII, 4);
    memcpy(PLtEBuffer, rgbData, PLTE_DATA_SIZE);

    uint32_t PLTECrc32 = 0;
    PLTECrc32 = crc32(PLTECrc32, PLtEBuffer, PLTE_DATA_SIZE+4);
    PLTECrc32 = switchUint32(PLTECrc32);
    fwrite(&PLTECrc32, 1, sizeof(PLTECrc32), file);

    // zlib 压缩数据
    // compress()
    //压缩
    /*原始数据*/
    // unsigned char strsrc[]="这些是测试数据。123456789 abcdefghigklmnopqrstuvwxyz\n\tabcdefghijklmnopqrstuvwxyz\n"; //包含\0字符
    unsigned char buf[1470000]={0};
    //unsigned char strdst[1024]={0};
    uint32_t srclen=width*height/2+height;
//    uint32_t srclen=width*height*3;
    uint32_t buflen=sizeof(buf);
    //unsigned long dstlen=sizeof(strdst);

    compress(buf, &buflen, rgbIndexData, srclen);
    printf("压缩后实际长度为:%d\n", buflen);


    uint32_t tmpBuflen = switchUint32(buflen);

    fwrite(&tmpBuflen, 1, sizeof(tmpBuflen), file);
    fwrite(&IDAT_ASCII, 1, sizeof(IDAT_ASCII), file);

     fwrite(buf, 1, buflen, file);
    //解压缩
//    uncompress(strdst, &dstlen, buf, buflen);
//    printf("目的串:");
//    for(int i=0;i<dstlen;++i) {
//        printf("%c",strdst[i]);
//    }

    uint32_t idatCrc32 = 0;
    uint8_t unionBuffer[buflen+4];
    // 这个函数在 socket 中多用于清空数组
    // memset(void *buffer, int c, int count) buffer：为指针或是数组, c：是赋给buffer的值, count：是buffer的长度.
    // memcpy(void *str1, const void *str2, size_t n) 从存储区 str2 复制 n 个字符到存储区 str1
    memcpy(unionBuffer, &IDAT_ASCII, sizeof(IDAT_ASCII));
    memcpy(unionBuffer + sizeof(IDAT_ASCII), buf, buflen);

    idatCrc32 = crc32(idatCrc32, unionBuffer, buflen+4);
    printf("crc32=%x\n", idatCrc32);
    idatCrc32 = switchUint32(idatCrc32);
    printf("crc32=%x\n", idatCrc32);
    // TODO CRC32
    fwrite(&idatCrc32, 1, sizeof(idatCrc32), file);


    // 写入文件结尾
    fwrite(&IEND, 1, sizeof(IEND), file);

    bool big = IsBigEndianOrder();
    if(big) {
        printf("大端字节序");
    } else {
        printf("小端字节序");
    }

    free(rgbData);
    free(rgbIndexData);
    fclose(file);

    return 0;
}
