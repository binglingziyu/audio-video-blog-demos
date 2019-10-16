//
// Created by hubin on 2019/10/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "zlib.h"

int main() {
    printf("HERE\n");
    FILE *inFile = fopen("/Users/hubin/Desktop/0.data", "rb");
    FILE *outFile = fopen("/Users/hubin/Desktop/0-uncompress.data", "wb");
    printf("HERE\n");
    fseek(inFile, 0L, SEEK_END);
    long size = ftell(inFile);
    fseek(inFile, 0L, SEEK_SET);

    uint8_t dataBuf[size];
    fread(dataBuf, size, 1, inFile);
    printf("压缩文件大小：%ld\n", size);

    uint8_t destBuf[1500000]={0};
    uint32_t destLen = 0;

    printf("HERE\n");
    uncompress(destBuf, &destLen, dataBuf, size);
    printf("解压后大小：%d\n", destLen);

    printf("HERE\n");
    fwrite(destBuf, destLen, 1, outFile);

    fflush(outFile);
    fclose(inFile);
    fclose(outFile);

    return 0;
}