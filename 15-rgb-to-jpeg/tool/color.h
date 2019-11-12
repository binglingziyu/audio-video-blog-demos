//
// Created by hubin on 2019/11/8.
//

#ifndef IMAGE_DEMO_RGB_PIXEL_COLOR_H
#define IMAGE_DEMO_RGB_PIXEL_COLOR_H

// 包含头文件
#include <stdint.h>
#include <math.h>

uint8_t bound(uint8_t start, int value, uint8_t end);

void rgb24ToYuv(const uint8_t *rgb24Data, uint8_t *yuv_y, uint8_t *yuv_u, uint8_t *yuv_v, int width, int height);

#endif //IMAGE_DEMO_RGB_PIXEL_COLOR_H











