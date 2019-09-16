//
// Created by hubin on 2019/9/16.
//

#include <cmath>
#include <cstdlib>
#include "bitmap/bitmap_image.hpp"

struct point_t
{
    point_t(double _x = 0.0, double _y = 0.0) : x(_x), y(_y) {}
    double x,y;
};

int main()
{
    const double pi_ = 3.1415926535897932384626433832795028841971;
    const double a   = 20.0;
    const double b   = 20.0;
    const double dr  = (2.0 * pi_) / 1000.0;

    const std::size_t N = 5;
    const double delta_angle = (2.0 * pi_) / N;

    std::vector<point_t> spiral;

    for (std::size_t i = 0; i < N; ++i)
    {
        spiral.push_back(
                point_t(a * std::cos((delta_angle * i)),
                        a * std::sin((delta_angle * i))));
    }

    const int canvas_width  = 600;
    const int canvas_height = 600;

    cartesian_canvas canvas(canvas_width,canvas_height);

    canvas.image().clear(0);

    {
        // Render background using Plasma effect
        const double c1 = 0.9;
        const double c2 = 0.5;
        const double c3 = 0.3;
        const double c4 = 0.7;

        bitmap_image& image = canvas.image();

        ::srand(0xA5AA5AA5);

        plasma(image, 0, 0, image.width(), image.height(), c1, c2, c3, c4, 3.0, gray_colormap);
    }

    for (double angle = dr; (a + b * angle) < canvas.image().width() / 2.0; angle += dr)
    {
        for (std::size_t i = 0; i < spiral.size(); ++i)
        {
            const double theta = angle + delta_angle * i;
            const double d     = (a + b * angle);
            const point_t curr(d * std::cos(theta), d * std::sin(theta));

            const double centre_ratio =
                    (sqrt(curr.x * curr.x + curr.y * curr.y) / (canvas.image().width() / 2.0));

            if (centre_ratio <= 0.25) canvas.pen_width(1);
            else if (centre_ratio <= 0.50) canvas.pen_width(2);
            else if (centre_ratio <= 0.75) canvas.pen_width(3);
            else if (centre_ratio <= 1.00) canvas.pen_width(4);

            unsigned int index = (unsigned int)(1000.0 * centre_ratio);

            canvas.pen_color(hsv_colormap[index]);

            canvas.line_segment(spiral[i].x, spiral[i].y, curr.x, curr.y);

            spiral[i] = curr;
        }
    }

    canvas.image().save_image("/Users/hubin/Desktop/draw-archimedean-spirals.bmp");

    return 0;
}