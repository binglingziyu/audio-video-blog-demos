//
// Created by hubin on 2019/9/16.
//

#include <cmath>
#include <cstdlib>
#include "bitmap/bitmap_image.hpp"

int main()
{
    const int canvas_width  = 600;
    const int canvas_height = 600;

    const double pi       = 3.1415926535897932384626433832795028841971;
    const double phi      = pi * (3.0 - std::sqrt(5.0));
    const double radius   = (std::min(canvas_width, canvas_height) / 2.0) - 5.0;
    const double N        = 1200.0;
    const double spread   = radius / std::sqrt(N);
    const double p_radius = std::floor(spread / 2.0);

    cartesian_canvas canvas(canvas_width,canvas_height);

    {
        // Render background using Plasma effect
        const double c1 = 0.9;
        const double c2 = 0.5;
        const double c3 = 0.3;
        const double c4 = 0.7;

        bitmap_image& image = canvas.image();

        ::srand(0xA5AA5AA5);

        plasma(image, 0, 0, image.width(), image.height(), c1, c2, c3, c4, 3.0, copper_colormap);
    }

    for (double i = 0.0; i < N; ++i)
    {
        const double theta = phi * i;
        const double d     = spread * std::sqrt(i);
        const double x     = d * std::cos(theta);
        const double y     = d * std::sin(theta);

        canvas.pen_color(hsv_colormap[static_cast<std::size_t>(1000.0 * (i  / N))]);
        canvas.fill_circle(x, y, p_radius);
    }

    canvas.image().save_image("/Users/hubin/Desktop/draw-phyllotaxis-spiral.bmp");

    return 0;
}