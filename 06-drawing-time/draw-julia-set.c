//
// Created by hubin on 2019/9/16.
//

#include <stdio.h>
#include "bmp/bmp.h"
#include "common.h"

void writeRGBToBmp(char *filename, int width, int height) {
    char bmp[BMP_SIZE(width, height)];
    bmp_init(bmp, width, height);

    const unsigned int max_iterations = 300;

    const double cr = -0.70000;
    const double ci =  0.27015;

    double prevr, previ;

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            double nextr = 1.5 * (2.0 * x / width - 1.0);
            double nexti =       (2.0 * y / height - 1.0);

            for (unsigned int i = 0; i < max_iterations; ++i) {
                prevr = nextr;
                previ = nexti;

                nextr =     prevr * prevr - previ * previ + cr;
                nexti = 2 * prevr * previ + ci;

                if (((nextr * nextr) + (nexti * nexti)) > 4) {
                    const unsigned int index = ((1000.0 * i) / max_iterations);
                    rgb_t color = hsv_colormap[index];
                    bmp_set(bmp, x, y, bmp_encode(color.red, color.green, color.blue));
                    break;
                }
            }
        }
    }

    FILE *f = fopen(filename, "wb");
    fwrite(bmp, sizeof(bmp), 1, f);
    fclose(f);
}

int main() {

    writeRGBToBmp("/Users/hubin/Desktop/draw-julia-set.bmp", 600, 400);

    return 0;
}