//
// Created by hubin on 2019/11/8.
//

#ifndef IMAGE_DEMO_RGB_PIXEL_UTIL_H
#define IMAGE_DEMO_RGB_PIXEL_UTIL_H

#include <stdint.h>
#include <math.h>


// 彩虹的七种颜色
uint32_t rainbowColors[] = {
        0XFF0000, // 红
        0XFFA500, // 橙
        0XFFFF00, // 黄
        0X00FF00, // 绿
        0X007FFF, // 青
        0X0000FF, // 蓝
        0X8B00FF  // 紫
};

uint8_t bound(uint8_t start, int value, uint8_t end) {
    if(value <= start) {
        return start;
    }
    if(value >= end) {
        return end;
    }
    return value;
}

void genRGB24Data(uint8_t *rgbData, int width, int height);
void rgb24ToYuv(const uint8_t *rgb24Data, int8_t *yuv_y, int8_t *yuv_u, int8_t *yuv_v, int width, int height);
#endif //IMAGE_DEMO_RGB_PIXEL_UTIL_H
