//
// Created by hubin on 2019/12/27.
//

#ifndef IMAGE_DEMO_RGB_PIXEL_LOG_H
#define IMAGE_DEMO_RGB_PIXEL_LOG_H

#include <stdint.h>
#include <stdio.h>

void print_block_u(uint8_t *data);
void print_block_d(int8_t *data);
void print_block_i(int *data);
void print_block_f(float *data);

#endif //IMAGE_DEMO_RGB_PIXEL_LOG_H
