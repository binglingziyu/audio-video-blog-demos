//
// Created by hubin on 2019/9/16.
//

#include <cmath>
#include <cstdlib>
#include <vector>
#include "bitmap/bitmap_image.hpp"

void shuffle(unsigned int n, std::vector<unsigned int>& v)
{
    ::srand(0x13A1515A);
    for (unsigned int i = 0; i < n; ++i) v.push_back(i);
    for (unsigned int i = v.size() - 1; i > 0; --i)
    { std::swap(v[i], v[rand() % (i + 1)]); }
}

int main()
{
    bitmap_image image("/Users/hubin/Documents/Cpp/image-demo-rgb-pixel/06-drawing-time-cpp/tiger.bmp");

    const unsigned int divisions    = 3;
    const unsigned int block_width  = image.width () / divisions;
    const unsigned int block_height = image.height() / divisions;

    std::vector<unsigned int> cell;

    shuffle(divisions * divisions, cell);

    bitmap_image shuffled(image.width(), image.height());
    bitmap_image region;

    shuffled.clear();

    for (std::size_t i = 0; i < cell.size(); ++i)
    {
        unsigned int x_offset = block_width  * (i % divisions);
        unsigned int y_offset = block_height * (i / divisions);

        // Copy region from original image
        image.region(x_offset, y_offset,
                     block_width, block_height, region);

        x_offset = block_width  * (cell[i] % divisions);
        y_offset = block_height * (cell[i] / divisions);

        // Paste region to new location in shuffled image
        shuffled.copy_from(region, x_offset, y_offset);
    }

    shuffled.save_image("/Users/hubin/Desktop/draw-image-shuffled.bmp");

    return 0;
}