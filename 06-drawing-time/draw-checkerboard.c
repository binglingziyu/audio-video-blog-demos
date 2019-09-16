//
// Created by hubin on 2019/9/16.
//
#include <stdio.h>
#include "bmp/bmp.h"

void writeRGBToBmp(char *filename, int width, int height) {
    char bmp[BMP_SIZE(width, height)];
    bmp_init(bmp, width, height);

    // Draw a checkerboard pattern:
    for (size_t y = 0; y < width; y++){
        for (size_t x = 0; x < height; x++) {
            if ((y % 128 < 64 && x % 128 < 64) ||
                (y % 128 >= 64 && x % 128 >= 64)) {
                bmp_set(bmp, x, y, bmp_encode(255, 255, 255));
            } else {
                bmp_set(bmp, x, y, bmp_encode(0, 0, 0));
            }
        }
    }

    FILE *f = fopen(filename, "wb");
    fwrite(bmp, sizeof(bmp), 1, f);
    fclose(f);
}

int main() {

    writeRGBToBmp("/Users/hubin/Desktop/draw-checkerboard.bmp", 512, 512);

    return 0;
}
