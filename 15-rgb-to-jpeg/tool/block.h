//
// Created by hubin on 2019/11/8.
//

#ifndef IMAGE_DEMO_RGB_PIXEL_BLOCK_H
#define IMAGE_DEMO_RGB_PIXEL_BLOCK_H

#include <stdint.h>

int calc_block_size(int width, int height);
void block_data_8x8(uint8_t *data, uint8_t blocks[][64], int width, int height);

#endif //IMAGE_DEMO_RGB_PIXEL_BLOCK_H
