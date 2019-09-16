//
// Created by hubin on 2019/9/16.
//

#include <math.h>
#include "common.h"
#include "bmp/bmp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void writeRGBToBmp(char *baseFilename, char *filename, int width, int height) {
    
    const int kernel_size =  10;
    char base[BMP_SIZE(width, height)];
    bmp_init(base, width, height);

    {
        // Draw a checkerboard pattern:
        for (size_t y = 0; y < width; y++){
            for (size_t x = 0; x < height; x++) {
                if ((y % 128 < 64 && x % 128 < 64) ||
                    (y % 128 >= 64 && x % 128 >= 64)) {
                    bmp_set(base, x, y, bmp_encode(255, 255, 255));
                } else {
                    bmp_set(base, x, y, bmp_encode(0, 0, 0));
                }
            }
        }
    }

    char glass_image[BMP_SIZE(width, height)];
    bmp_init(glass_image, width, height);

    memcpy(glass_image, base, sizeof(base));

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const unsigned int min_x = fmax(0, x - kernel_size);
            const unsigned int min_y = fmax(0, y - kernel_size);
            const unsigned int max_x = fmin(x + kernel_size, width  - 1);
            const unsigned int max_y = fmin(y + kernel_size, height - 1);
            const unsigned int dx    = (max_x - min_x);
            const unsigned int dy    = (max_y - min_y);
            const unsigned int N     = rand() % (dx * dy);
            const unsigned int cx    = (N % dx) + min_x;
            const unsigned int cy    = (N / dx) + min_y;

            bmp_set(glass_image, x, y, bmp_get(base, cx, cy));
        }
    }


    FILE *baseFile = fopen(baseFilename, "wb");
    FILE *file = fopen(filename, "wb");
    fwrite(base, sizeof(base), 1, baseFile);
    fwrite(glass_image, sizeof(glass_image), 1, file);
    fclose(baseFile);
    fclose(file);

}

int main() {

    writeRGBToBmp("/Users/hubin/Desktop/draw-frosted-glass-effect-base.bmp", "/Users/hubin/Desktop/draw-frosted-glass-effect.bmp", 600, 600);

    return 0;
}