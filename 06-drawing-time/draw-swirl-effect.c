//
// Created by hubin on 2019/9/16.
//

#include <math.h>
#include "common.h"
#include "bmp/bmp.h"
#include <string.h>
#include <stdio.h>

void writeRGBToBmp(char *baseFilename, char *filename, int width, int height) {

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

    char swirl_image[BMP_SIZE(width, height)];
    bmp_init(swirl_image, width, height);

    const double swirl_center_x = width / 2.0;
    const double swirl_center_y = height / 2.0;
    const double swirl_radius   = fmin(width, height) / 3.0;

    const double pi_ = 3.1415926535897932384626433832795028841971;
    const double swirl_angle = pi_ / 3.0;

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            const double dx = x - swirl_center_x;
            const double dy = y - swirl_center_y;

            const double distance = sqrt((dx * dx) + (dy * dy));

            const double angle = swirl_angle * (distance / swirl_radius);

            const double cosa = cos(angle);
            const double sina = sin(angle);

            int sx = (dx * cosa - dy * sina + swirl_center_x);
            int sy = (dx * sina + dy * cosa + swirl_center_y);

            if (
                    (sx >= 0)                 &&
                    (sy >= 0)                 &&
                    (sx < (int)width) &&
                    (sy < (int)height)
                    ) {
                bmp_set(swirl_image, x, y, bmp_get(base, sx, sy));
            }
        }
    }

    FILE *baseFile = fopen(baseFilename, "wb");
    FILE *file = fopen(filename, "wb");
    fwrite(base, sizeof(base), 1, baseFile);
    fwrite(swirl_image, sizeof(swirl_image), 1, file);
    fclose(baseFile);
    fclose(file);

}

int main() {

    writeRGBToBmp("/Users/hubin/Desktop/draw-swirl-effect-base.bmp", "/Users/hubin/Desktop/draw-swirl-effect.bmp", 600, 600);

    return 0;
}