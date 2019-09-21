//
// Created by hubin on 2019/9/19.
//

#include <stdio.h>
#include <stdint.h>
#include "libyuv.h"

void libyuv_I444_to_Rgb(char *srcFileName, char *dstFileName, int width, int height) {
    FILE *src_file = fopen(srcFileName, "rb");
    FILE *dst_file = fopen(dstFileName, "wb");

    int size_src = width * height * 3;
    int size_dest = width * height * 4;
    char *buffer_src = (char *)malloc(size_src);
    char *buffer_dest = (char *)malloc(size_dest);

    fread(buffer_src, 1, size_src, src_file);

    I444ToARGB((const uint8_t*)buffer_src, width,
                           (const uint8_t*)(buffer_src + width * height), width,
                           (const uint8_t*)(buffer_src + 2 * width * height), width,
                           (uint8_t*)buffer_dest, width * 4,
                           width, height);

    fwrite(buffer_dest, 1, size_dest, dst_file);

    free(buffer_src);
    free(buffer_dest);
    fclose(dst_file);
    fclose(src_file);
}

int main() {
    int width = 700, height = 700;
    libyuv_I444_to_Rgb("/Users/hubin/Desktop/rainbow-yuv444p.yuv", "/Users/hubin/Desktop/rainbow-yuv444p-to-rgb32-libyuv.yuv", width, height);
    return 0;
}