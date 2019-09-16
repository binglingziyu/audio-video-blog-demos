//
// Created by hubin on 2019/9/16.
//

#include <cmath>
#include <cstdlib>
#include "bitmap/bitmap_image.hpp"

struct point_t { double x,y; };

int main()
{
    const int canvas_width  = 700;
    const int canvas_height = 460;

    cartesian_canvas canvas(canvas_width,canvas_height);

    canvas.image().clear(255);

    canvas.pen_width(1);

    ::srand(0xA5A5A5A5);

    for (std::size_t i = 0; i < 100; ++i)
    {
        double cx = ((rand() % canvas_width ) - canvas_width  / 2.0);
        double cy = ((rand() % canvas_height) - canvas_height / 2.0);
        double r  = (rand() % 70) + 10;

        const double pi_   = 3.1415926535897932384626433832795028841971;
        const double _120d = (2.0 * pi_/ 3.0);

        double rndang = ((2.0 * pi_) / 360.0) * ((rand() % 360) + 1.0);

        point_t p0, p1, p2;
        point_t p3, p4, p5;

        p0.x = r * std::sin(1.0 * _120d + rndang) + cx;
        p0.y = r * std::cos(1.0 * _120d + rndang) + cy;
        p1.x = r * std::sin(2.0 * _120d + rndang) + cx;
        p1.y = r * std::cos(2.0 * _120d + rndang) + cy;
        p2.x = r * std::sin(3.0 * _120d + rndang) + cx;
        p2.y = r * std::cos(3.0 * _120d + rndang) + cy;

#define rnd_colour (copper_colormap[rand() % 1000])

        // Draw and fill the circle
        canvas.pen_color(rnd_colour);
        canvas.fill_circle(cx, cy, r);
        canvas.pen_color(rnd_colour);
        canvas.circle(cx, cy, r);

        // Draw and fill the main equilateral triangles
        canvas.pen_color(rnd_colour);
        canvas.fill_triangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
        canvas.pen_color(rnd_colour);
        canvas.triangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);

        // Draw the inner-equilateral triangles
        for (unsigned int j = 0; j < 4; ++j)
        {
            p3.x = (p0.x + p1.x) / 2.0;
            p3.y = (p0.y + p1.y) / 2.0;
            p4.x = (p1.x + p2.x) / 2.0;
            p4.y = (p1.y + p2.y) / 2.0;
            p5.x = (p2.x + p0.x) / 2.0;
            p5.y = (p2.y + p0.y) / 2.0;

            p0 = p3; p1 = p4; p2 = p5;

            canvas.pen_color(rnd_colour);
            canvas.fill_triangle(p3.x, p3.y, p4.x, p4.y, p5.x, p5.y);
            canvas.pen_color(rnd_colour);
            canvas.triangle(p3.x, p3.y, p4.x, p4.y, p5.x, p5.y);
        }
    }

    canvas.image().save_image("/Users/hubin/Desktop/draw-circles_and_triangles.bmp");

    return 0;
}