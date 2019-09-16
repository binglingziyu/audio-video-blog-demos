//
// Created by hubin on 2019/9/16.
//

#include <cstdlib>
#include "bitmap/bitmap_image.hpp"

int main()
{
    bitmap_image base("/Users/hubin/Documents/Cpp/image-demo-rgb-pixel/06-drawing-time-cpp/sunflower.bmp");

    cartesian_canvas canvas(base.width(),base.height());
    canvas.image() = base;

    const int pixel_count  = base.width() * base.height();
    const int N            = static_cast<int>(pixel_count * 0.03); // 3% of pixels
    const double rnd_ratio = pixel_count / (1.0 + RAND_MAX);

    ::srand(0xA57A57A5);

    for (int i = 0; i < N; ++i)
    {
        const int    r  = static_cast<int>(rand() * rnd_ratio);
        const int    x  = (r % base.width());
        const int    y  = (r / base.width());
        const double cx = x - (base.width() / 2.0);
        const double cy = (base.height() / 2.0) - y;
        const double radius = 1.0 + (r % 7);

        canvas.pen_color(base.get_pixel(x, y));
        canvas.fill_circle(cx, cy, radius);
    }

    canvas.image().save_image("/Users/hubin/Desktop/draw-pointillist-effect.bmp");

    return 0;
}