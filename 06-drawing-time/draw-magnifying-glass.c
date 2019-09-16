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

    char lens_image[BMP_SIZE(width, height)];
    bmp_init(lens_image, width, height);

    memcpy(lens_image, base, sizeof(base));

    const double lens_center_x = width / 2.0;
    const double lens_center_y = height / 2.0;
    const double lens_radius   = fmin(width, height) / 4.0;
    const double lens_factor   = 0.7;

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            const double dx = x - lens_center_x;
            const double dy = y - lens_center_y;

            const double distance = sqrt((dx * dx) + (dy * dy));

            if (distance <= lens_radius) {
                const double radius     = distance / lens_radius;
                const double angle      = atan2(dy, dx);
                const double distortion = pow(radius, lens_factor) * distance;

                int sx = (distortion * cos(angle) + lens_center_x);
                int sy = (distortion * sin(angle) + lens_center_y);

                if (
                        (sx >= 0)                 &&
                        (sy >= 0)                 &&
                        (sx < (int)width) &&
                        (sy < (int)height)
                        ) {

                    bmp_set(lens_image, x, y, bmp_get(base, sx, sy));
                }
            }
        }
    }

    FILE *baseFile = fopen(baseFilename, "wb");
    FILE *file = fopen(filename, "wb");
    fwrite(base, sizeof(base), 1, baseFile);
    fwrite(lens_image, sizeof(lens_image), 1, file);
    fclose(baseFile);
    fclose(file);

}

int main() {

    writeRGBToBmp("/Users/hubin/Desktop/draw-magnifying-glass-base.bmp", "/Users/hubin/Desktop/draw-magnifying-glass.bmp", 600, 600);

    return 0;
}