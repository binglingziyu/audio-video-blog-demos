//
// Created by hubin on 2019/9/16.
//

#include <cstdlib>
#include "bitmap/bitmap_image.hpp"

enum compass { N = 1, E = 2, S = 4, W = 8 };

const int untouched = (N | E | S | W);

struct move_t { int x, y, inverse; };

const move_t move[5] =
        {
                { 0, 0, 0 },
                // North        East         South         West
                { 0, -1, S }, { 1, 0, W }, { 0, 1, N },  { -1, 0, E }
        };

const int movemap[] = {0, 1, 2, 0, 3, 0, 0, 0, 4};

const compass permutations[] =
        {
                N,S,E,W, S,N,E,W, E,N,S,W, N,E,S,W, S,E,N,W, E,S,N,W, E,S,W,N, S,E,W,N,
                W,E,S,N, E,W,S,N, S,W,E,N, W,S,E,N, W,N,E,S, N,W,E,S, E,W,N,S, W,E,N,S,
                N,E,W,S, E,N,W,S, S,N,W,E, N,S,W,E, W,S,N,E, S,W,N,E, N,W,S,E, W,N,S,E
        };

void generate_maze(int cx, int cy, response_image<int>& maze)
{
    const unsigned int perm_index = 4 * ((rand() % 24));

    std::vector<compass>
            directions(&permutations[perm_index], &permutations[perm_index + 4]);

    for (std::size_t i = 0; i < directions.size(); ++i)
    {
        const move_t m = move[movemap[directions[i]]];

        const int x = cx + m.x;
        const int y = cy + m.y;

        if (
                (x < 0) || (y < 0)        ||
                (x >= (int)maze.width ()) ||
                (y >= (int)maze.height()) ||
                (untouched != maze(x,y))
                )
            continue;

        // Eliminate the wall corresponding to the selected direction.
        maze(cx,cy) = maze(cx,cy) & ~directions[i];
        maze( x, y) = maze( x, y) & ~m.inverse;

        generate_maze(x,y, maze);
    }
}

int main() {
    const std::size_t maze_width  = 80;
    const std::size_t maze_height = 70;

    response_image<int> maze(maze_width, maze_height);

    maze.set_all(untouched);

    generate_maze(rand() % maze_width, rand() % maze_height, maze);

    const std::size_t wall_size_x = 10; // 10 pixels
    const std::size_t wall_size_y =  8; //  8 pixels
    const std::size_t pen_size    =  2; //  2 pixels

    bitmap_image image(
            (maze_width ) * wall_size_x + (pen_size - 1),
            (maze_height) * wall_size_y + (pen_size - 1)
    );

    image.clear();

    {
        // Render background using Plasma effect
        const double c1 = 0.9;
        const double c2 = 0.5;
        const double c3 = 0.3;
        const double c4 = 0.7;

        ::srand(0xA5AA5AA5);

        plasma(image, 0, 0, image.width(), image.height(), c1, c2, c3, c4, 3.0, jet_colormap);
    }

    image_drawer draw(image);

    draw.pen_width(2);
    draw.pen_color(0, 0, 0);

    draw.rectangle(0, 0, image.width() - pen_size, image.height() - pen_size);

    // Draw the maze
    for (std::size_t y = 0; y < maze.height(); ++y)
    {
        for (std::size_t x = 0; x < maze.width(); ++x)
        {
            // Cell(x,y) coordinates
            const int x0 = x * wall_size_x;
            const int y0 = y * wall_size_y;
            const int x1 = x * wall_size_x + wall_size_x;
            const int y1 = y * wall_size_y + wall_size_y;

            const int cell = maze(x,y);

            // Is north wall present?
            if ((cell & N))
                draw.line_segment(x0, y0, x1, y0);

            // Is south wall present?
            if ((cell & S))
                draw.line_segment(x0, y1, x1, y1);

            // Is east wall present?
            if ((cell & E))
                draw.line_segment(x1, y0, x1, y1);

            // Is west wall present?
            if ((cell & W))
                draw.line_segment(x0, y0, x0, y1);
        }
    }

    image.save_image("/Users/hubin/Desktop/draw-maze.bmp");

    return 0;
}