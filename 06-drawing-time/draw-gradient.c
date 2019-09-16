//
// Created by hubin on 2019/9/16.
//

#include <stdio.h>
#include <stdlib.h>
#include "bmp/bmp.h"

void writeRGBToBmp(char *filename, int width, int height) {
    char bmp[BMP_SIZE(width, height)];
    bmp_init(bmp, width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float r = y / (float)height;
            float g = x / (float)width;
            float b = 1.0f;
            bmp_set(bmp, x, y, bmp_encode(r, g, b));
        }
    }

    FILE *f = fopen(filename, "wb");
    fwrite(bmp, sizeof(bmp), 1, f);
    fclose(f);
}

int main() {

    writeRGBToBmp("/Users/hubin/Desktop/draw-gradient.bmp", 512, 512);

    return 0;
}