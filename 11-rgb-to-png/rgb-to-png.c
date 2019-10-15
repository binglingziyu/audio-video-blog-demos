//
// Created by hubin on 2019/10/14.
//

#include <stdio.h>
#include <stdint.h>
#include <i386/endian.h>
#include <stdbool.h>
#include <string.h>
#include "zlib.h"

bool IsBigEndianOrder();
void genRGB24Data(uint8_t *rgbData, int width, int height);

void make_crc32_table();
uint32_t make_crc(uint32_t crc, int8_t *data, uint32_t size);

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
    int width = 700, height = 700;
    uint8_t rgb24Data[width*height*3];
    uint32_t calcuCrc = 0xffffffff;

    FILE *file = fopen("/Users/hubin/Desktop/0.png", "wb");

    // 填充 RGB 数据
    genRGB24Data(rgb24Data, width, height);

    // 计算 crc32
    // make_crc32_table();
    // calcuCrc = make_crc(calcuCrc, rgb24Data, sizeof(rgb24Data));

    // 写文件标识
    fwrite(pngFileType, 1, sizeof(pngFileType), file);

    PNGFILEHEADER pngfileheader;
    pngfileheader.width = htonl(width);
    pngfileheader.height = htonl(height);
    pngfileheader.bitDepth = 8;
    pngfileheader.colorType = 6;
    pngfileheader.compressionMethod = 0;
    pngfileheader.filterMethod = 0;
    pngfileheader.interlaceMethod = 0;

    uint32_t pngfileheaderSize = htonl(0x0000000D);
    uint32_t pngfileheaderTmp = htonl(0x49484452);
    uint32_t pngfileheaderCrc32 = htonl(0x0DBA6C43);

    printf("\n文件头：%ld\n", sizeof(PNGFILEHEADER));
    printf("uint32_t: %ld\n", sizeof(uint32_t));

    fwrite(&pngfileheaderSize, 1, sizeof(pngfileheaderSize), file);
    fwrite(&pngfileheaderTmp, 1, sizeof(pngfileheaderTmp), file);
    // 写入文件头
    fwrite(&pngfileheader, 1, 13/*sizeof(pngfileheader)*/, file);
    fwrite(&pngfileheaderCrc32, 1, sizeof(pngfileheaderCrc32), file);


    // zlib 压缩数据
    // compress()
    //压缩
    /*原始数据*/
    // unsigned char strsrc[]="这些是测试数据。123456789 abcdefghigklmnopqrstuvwxyz\n\tabcdefghijklmnopqrstuvwxyz\n"; //包含\0字符
    unsigned char buf[1470000]={0};
    //unsigned char strdst[1024]={0};
    uint32_t srclen=sizeof(rgb24Data);
    uint32_t buflen=sizeof(buf);
    //unsigned long dstlen=sizeof(strdst);

    compress(buf, &buflen, rgb24Data, srclen);
    printf("压缩后实际长度为:%ld\n", buflen);


    uint32_t tmpBuflen = htonl(buflen);
    uint8_t IDAT[] = {0x49, 0x44, 0x41, 0x54};
    fwrite(&tmpBuflen, 1, sizeof(tmpBuflen), file);
    fwrite(IDAT, 1, sizeof(IDAT), file);

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
    memcpy(unionBuffer, IDAT, sizeof(IDAT));
    memcpy(unionBuffer + sizeof(IDAT), buf, buflen);

    idatCrc32 = crc32(idatCrc32, unionBuffer, buflen+4);
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

    return 0;
}
