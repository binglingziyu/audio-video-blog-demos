//
// Created by hubin on 2019/9/16.
//

#include <stdio.h>
#include <math.h>
#include "common.h"
#include "bmp/bmp.h"

void writeRGBToBmp(char *filename, int width, int height) {
    char bmp[BMP_SIZE(width, height)];
    bmp_init(bmp, width, height);

    double    cr,    ci;
    double nextr, nexti;
    double prevr, previ;

    const unsigned int max_iterations = 1000;

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            cr = 1.5 * (2.0 * x / width - 1.0) - 0.5;
            ci =       (2.0 * y / height - 1.0);

            nextr = nexti = 0;
            prevr = previ = 0;

            for (unsigned int i = 0; i < max_iterations; ++i)
            {
                prevr = nextr;
                previ = nexti;

                nextr =     prevr * prevr - previ * previ + cr;
                nexti = 2 * prevr * previ + ci;

                if (((nextr * nextr) + (nexti * nexti)) > 4) {

                    const double z = sqrt(nextr * nextr + nexti * nexti);

                    //https://en.wikipedia.org/wiki/Mandelbrot_set#Continuous_.28smooth.29_coloring
                    const unsigned int index = (1000.0 * log2(1.75 + i - log2(log2(z))) / log2(max_iterations));
                    
                    rgb_t color = jet_colormap[index];
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

    writeRGBToBmp("/Users/hubin/Desktop/draw-mandelbrot-set.bmp", 600, 400);

    return 0;
}