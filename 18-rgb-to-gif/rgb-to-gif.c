//
// Created by hubin on 2020/6/11.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <src/lzw.h>

// 颜色表
uint32_t rainbowColors[] = {
        0XFF0000, // 赤
        0XFFA500, // 橙
        0XFFFF00, // 黄
        0X00FF00, // 绿
        0X007FFF, // 青
        0X0000FF, // 蓝
        0X8B00FF, // 紫
        0X000000  // 黑
};

int main() {

    FILE *gif_file = fopen("/Users/hubin/Desktop/rainbow.gif", "wb+");

    // GIF 文件头，6 个字节内容是 GIF 的署名和版本号
    uint8_t gif_header[] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};
    fwrite(gif_header, 6, 1, gif_file);

    // 逻辑屏幕标识符
    uint16_t gif_width = 700;
    uint16_t gif_height = 700;
    uint8_t gif_logical_screen_pack_byte = 0xF2;
    uint8_t gif_bg_color_index = 0;
    uint8_t gif_pixel_aspect = 0;

    fputc(gif_width >> 0, gif_file); // width low 8
    fputc(gif_width >> 8, gif_file); // width high 8
    fputc(gif_height  >> 0, gif_file); // height low 8
    fputc(gif_height  >> 8, gif_file); // height high 8
    fputc(gif_logical_screen_pack_byte, gif_file);
    fputc(gif_bg_color_index, gif_file);
    fputc(gif_pixel_aspect, gif_file);

    // 全局颜色表
    printf("全局颜色表颜色数：%lu\n", sizeof(rainbowColors)/sizeof(uint32_t));
    for(int i = 0; i < 8; i++) {
        // 根据颜色索引取出颜色表中的颜色
        uint32_t color_rgb = rainbowColors[i];
        // 当前颜色 R 分量
        uint8_t R = (color_rgb & 0xFF0000) >> 16;
        // 当前颜色 G 分量
        uint8_t G = (color_rgb & 0x00FF00) >> 8;
        // 当前颜色 B 分量
        uint8_t B = color_rgb & 0x0000FF;
        fputc(R, gif_file);
        fputc(G, gif_file);
        fputc(B, gif_file);
    }

    // Application Extension
    // NETSCAPE2.0
    uint8_t gif_application_extension[] = {0x21, 0xFF, 0x0B, 0x4E, 0x45, 0x54, 0x53, 0x43, 0x41, 0x50, 0x45, 0x32, 0x2E, 0x30, 0x03, 0x01, 0x00, 0x00, 0x00};
    fwrite(gif_application_extension, 19, 1, gif_file);

    // Comment Extension
    // Created with ezgif.com GIF maker
    uint8_t gif_comment_extension[] = {0x21, 0xFE, 0x20, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x65, 0x7A, 0x67, 0x69, 0x66, 0x2E, 0x63, 0x6F, 0x6D, 0x20, 0x47, 0x49, 0x46, 0x20, 0x6D, 0x61, 0x6B, 0x65, 0x72, 0x00};
    fwrite(gif_comment_extension, 36, 1, gif_file);

    for(int i = 0; i < 7; i++) {
        // 图形控制扩展
        // 21 F9 04 00 32 00 FF 00
        uint8_t gif_graphic_control_extension[] = {0x21, 0xF9, 0x04, 0x00, 0x32, 0x00, 0xFF, 0x00};
        fwrite(gif_graphic_control_extension, 8, 1, gif_file);

        // 图像标识符
        // 2C 00 00 00 00 BC 02 BC 02 00
        uint8_t gif_image_descriptor[] = {0x2C, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x02, 0xBC, 0x02, 0x00};
        fwrite(gif_image_descriptor, 10, 1, gif_file);

        // 基于颜色表的图像数据
        uint8_t *gif_one_frame_raw = malloc(700 * 700);
        memset(gif_one_frame_raw, i, 700*700);
        printf("当前帧对应的颜色索引：%d\n", gif_one_frame_raw[0]);

        //  GIF 一帧图像的数据压缩后大小
        unsigned long compressed_size;
        // GIF 一帧图像的数据解压后的数据
        unsigned char *img;
        lzw_compress_gif(
                3,
                700*700,
                gif_one_frame_raw,
                &compressed_size,
                &img
        );
        printf("GIF 一帧图像压缩后大小：%ld\n", compressed_size);
        fputc(0x03, gif_file);
        unsigned long current_index = 0;
        while (current_index < compressed_size) {
            if((current_index + 0xFF) >= compressed_size) {
                unsigned long diff = compressed_size - current_index;
                fputc(diff, gif_file);
                fwrite(img+current_index, diff, 1, gif_file);
                fputc(0x00, gif_file);
                current_index += diff;
            } else {
                fputc(0xFF, gif_file);
                fwrite(img+current_index, 0xFF, 1, gif_file);
                current_index += 0xFF;
            }
        }
        free(gif_one_frame_raw);
        free(img);
    }

    // GIF 文件结束: 0x3B
    fputc(0x3B, gif_file);

    fflush(gif_file);
    fclose(gif_file);
    return 0;
}
