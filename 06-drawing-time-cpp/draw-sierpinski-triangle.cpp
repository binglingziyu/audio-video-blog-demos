//
// Created by hubin on 2019/9/16.
//

#include <cmath>
#include <cstdlib>
#include "bitmap/bitmap_image.hpp"

struct point_t { double x,y; };

int main()
{
    const int canvas_width  = 600;
    const int canvas_height = 400;

    cartesian_canvas canvas(canvas_width,canvas_height);

    {
        // Render background using Plasma effect
        const double c1 = 0.9;
        const double c2 = 0.3;
        const double c3 = 0.5;
        const double c4 = 0.7;

        ::srand(0xA5AA5AA5);

        plasma(canvas.image(), c1, c2, c3, c4, 3.5, jet_colormap);
    }

    point_t triangle[3];

    triangle[0].x = 0;
    triangle[0].y = +canvas_height / 2.0;
    triangle[1].x = -canvas_width  / 2.0;
    triangle[1].y = -canvas_height / 2.0;
    triangle[2].x = +canvas_width  / 2.0;
    triangle[2].y = -canvas_height / 2.0;

    point_t pnt = triangle[0];

    const std::size_t max_iterations = 1000000;

    for (std::size_t i = 0; i < max_iterations; ++i)
    {
        const point_t target_point = triangle[rand() % 3];

        pnt.x += (target_point.x - pnt.x) / 2.0;
        pnt.y += (target_point.y - pnt.y) / 2.0;

        canvas.plot_pen_pixel(pnt.x,pnt.y);
    }

    canvas.image().save_image("/Users/hubin/Desktop/draw-sierpinski-triangle.bmp");

    return 0;
}