//
// Created by hubin on 2021/1/21.
//

#ifndef AUDIO_VIDEO_BLOG_DEMOS_RGB2YUV_H
#define AUDIO_VIDEO_BLOG_DEMOS_RGB2YUV_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

uint8_t bound(uint8_t start, int value, uint8_t end);
void rgbToYuv(uint8_t R, uint8_t G, uint8_t B, uint8_t *Y, uint8_t *U, uint8_t *V);
void rgb24ToYuv444p(const uint8_t *rgb24Data, uint8_t **yuv444pData, int width, int height);
void rgb24ToYuv420p(const uint8_t *rgb24Data, uint8_t **yuv420pData, int width, int height);

#endif //AUDIO_VIDEO_BLOG_DEMOS_RGB2YUV_H
