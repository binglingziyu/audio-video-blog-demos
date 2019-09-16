//
// Created by hubin on 2019/9/16.
//

#include <cmath>
#include <iterator>
#include <vector>
#include "bitmap/bitmap_image.hpp"

struct lissajous_curve
{
    lissajous_curve(const double xs, const double ys)
            : scale_x(xs),
              scale_y(ys)
    {}

    inline double x(const double t) const { return scale_x * std::sin(4 * t); }
    inline double y(const double t) const { return scale_y * std::cos(3 * t); }

    double scale_x;
    double scale_y;
};

int main()
{
    bitmap_image image(700,500);

    image.clear();

    lissajous_curve curve(image.width () / 2.0 - 25, image.height() / 2.0 - 25);

    const double pi_   = 3.1415926535897932384626433832795028841971;
    const double delta = (2.0 * pi_) / 100000.0;

    const std::size_t max_fire_balls   = 30;
    const std::size_t number_of_frames = 3000;

    const double cooling_factor = 0.940; // [0,1]

    // Arc-length of curve: x(t) = a0 * sin(4t), y(t) = a1 * cos(3t)
    const double curve_length = 6151.0;

    double segment_length  = curve_length / max_fire_balls;
    double curr_seg_length = 0;

    double prev_x = curve.x(0);
    double prev_y = curve.y(0);

    std::vector<double> fire_ball;

    // Set the initial location for each fireball
    for (double t = delta;  fire_ball.size() < max_fire_balls; t += delta)
    {
        const double center_x = curve.x(t);
        const double center_y = curve.y(t);

        const double dx = (prev_x - center_x);
        const double dy = (prev_y - center_y);

        curr_seg_length += std::sqrt((dx * dx) + (dy * dy));

        prev_x = center_x;
        prev_y = center_y;

        if (curr_seg_length >= segment_length)
        {
            curr_seg_length = 0.0;
            fire_ball.push_back(t);
        }
    }

    response_image<double> resp_image(image.width(),image.height(), -1.0);
    response_image<double> fb_misses (image.width(),image.height(), -1.0);

    resp_image.set_all(-999.0);
    fb_misses .set_all(   0.0);

    std::vector<rgb_t> fire_palette;

    // Baseline colours used in fire palette
    rgb_t black  = make_colour(  0,   0,   0);
    rgb_t red    = make_colour(255,   0,   0);
    rgb_t yellow = make_colour(255, 255,   0);
    rgb_t white  = make_colour(255, 255, 255);

    // Setup the fire palette:
    // Black (Coolest - 0) --> Red --> Yellow --> White (Hottest - 999)
    generate_colours(334,  black,    red, std::back_inserter(fire_palette));
    generate_colours(333,    red, yellow, std::back_inserter(fire_palette));
    generate_colours(333, yellow,  white, std::back_inserter(fire_palette));

    for (std::size_t k = 0; k < number_of_frames; ++k)
    {
        fb_misses.inc_all(1);

        // Render fireballs on response image
        for (std::size_t i = 0; i < fire_ball.size(); ++i)
        {
            const double fb_x = curve.x(fire_ball[i]) + image.width () / 2.0;
            const double fb_y = curve.y(fire_ball[i]) + image.height() / 2.0;

            // Draw circles with radii in the range [1,10]
            for (double t = 0;  t < (2 * pi_); t += (((2.0 * pi_) / 360)))
            {
                for (double r = 1; r <= 10; ++r)
                {
                    std::size_t rx = static_cast<std::size_t>(r * std::sin(t) + fb_x);
                    std::size_t ry = static_cast<std::size_t>(r * std::cos(t) + fb_y);

                    // Per-frame in the range [0,100]
                    double heat_distortion = 50.0 * std::cos(delta * i) + 50;

                    resp_image(rx,ry) = fire_palette.size() * 0.8 +
                                        heat_distortion +
                                        (::rand() % 100);

                    fb_misses (rx,ry) = 0;
                }
            }

            // Move fireball to its next location
            fire_ball[i] += delta;
        }

        // Apply cooling process to the entire plane
        for (std::size_t y = 1; y < resp_image.height() - 1; ++y)
        {
            for (std::size_t x = 1; x < resp_image.width() - 1; ++x)
            {
                double avg = (
                                     resp_image(x - 1, y - 1) + resp_image(x    , y - 1) +
                                     resp_image(x + 1, y - 1) + resp_image(x - 1, y    ) +
                                     resp_image(x + 1, y    ) + resp_image(x    , y + 1) +
                                     resp_image(x - 1, y + 1) + resp_image(x + 1, y + 1)
                             ) / (7.0 + cooling_factor);

                // Only allow cooler averages to be applied
                if (avg > resp_image(x, y))
                    continue;

                // More rapidly cool points that haven't seen fireballs in the last N-frames
                if (fb_misses(x,y) > 2000)
                    avg *= 0.90 + ((::rand() % 10) / 100.0);

                // Clamp average in the range [0,999]
                resp_image(x,y) = ((avg < 0.0) ? 0.0 : ((avg > 999.0) ? 999.0 : avg));
            }
        }
    }

    convert_rsp_to_image(resp_image, fire_palette, image);

    image.save_image("/Users/hubin/Desktop/draw-fireballs.bmp");

    return 0;
}