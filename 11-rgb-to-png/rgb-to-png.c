//
// Created by hubin on 2019/10/14.
//

#include <stdio.h>
#include <stdint.h>

void genRGB24Data(uint8_t *rgbData, int width, int height);
uint32_t adler32(unsigned char *data, uint32_t len);

void make_crc32_table();
uint32_t make_crc(uint32_t crc, int8_t *data, uint32_t size);

int main() {
    int width = 700, height = 700;
    uint8_t rgb24Data[width*height*3];
    uint32_t calcuCrc = 0xffffffff;

    // 填充 RGB 数据
    genRGB24Data(rgb24Data, width, height);

    // 计算 adler32
    uint32_t adlerVal = adler32(rgb24Data, width*height*3);

    // 计算 crc32
    make_crc32_table();
    calcuCrc = make_crc(calcuCrc, rgb24Data, sizeof(rgb24Data));


    return 0;
}
