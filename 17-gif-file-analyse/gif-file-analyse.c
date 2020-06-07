//
// Created by hubin on 2020/6/5.
//
#include "stdio.h"
#include "stdlib.h"
#include "lzw/src/lzw.h"

// 颜色表
uint32_t rainbowColors[] = {
        0XFF0000, // 赤
        0X00FF00, // 绿
        0XFFA500, // 橙
        0XFFFF00, // 黄
        0X0000FF, // 蓝
        0X007FFF, // 青
        0X8B00FF, // 紫
        0X000000  // 黑

//        0XFF0000, // 赤
//        0XFFA500, // 橙
//        0XFFFF00, // 黄
//        0X00FF00, // 绿
//        0X007FFF, // 青
//        0X0000FF, // 蓝
//        0X8B00FF  // 紫
};

int main () {
    // LZW 编码初始表大小的位数：3
    unsigned char code_size = 3;
    //  GIF 一帧图像的数据压缩文件（rainbow-compressed.gif.frame）大小
    long total_bytes;
    // GIF 一帧图像的数据压缩数据
    unsigned char *img_compressed;
    // GIF 一帧图像的数据解压后的数据
    unsigned char *img;
    //  GIF 一帧图像的数据解压后大小
    unsigned long decompressed_size;

    FILE *gif_compressed_frame = fopen("/Users/hubin/Desktop/rainbow-compressed.gif.frame", "rb+");
    fseek(gif_compressed_frame, 0L, SEEK_END);
    total_bytes = ftell(gif_compressed_frame);
    fseek(gif_compressed_frame, 0L, SEEK_SET);
    printf("Gif 一帧压缩文件大小：%li\n", total_bytes);

    img_compressed = malloc((unsigned long) total_bytes);
    fread(img_compressed, total_bytes, 1, gif_compressed_frame);

    // 进行 LZW 解压
    lzw_decompress(
        code_size,
        total_bytes,
        img_compressed,
        &decompressed_size,
        &img
    );

    printf("Gif 一帧解压文件大小：%li\n", decompressed_size);

    FILE *gif_decompressed_frame = fopen("/Users/hubin/Desktop/rainbow-decompressed.gif.frame", "wb+");
    fwrite(img, decompressed_size, 1, gif_decompressed_frame);
    fflush(gif_decompressed_frame);

    FILE *gif_frame_rgb = fopen("/Users/hubin/Desktop/rainbow-decompressed.gif.frame.rgb", "wb+");
    for(int i = 0; i < decompressed_size; i++) {
        // 颜色索引值
        unsigned char color_index = img[i];
        // 根据颜色索引取出颜色表中的颜色
        uint32_t color_rgb = rainbowColors[color_index];
        // 当前颜色 R 分量
        uint8_t R = (color_rgb & 0xFF0000) >> 16;
        // 当前颜色 G 分量
        uint8_t G = (color_rgb & 0x00FF00) >> 8;
        // 当前颜色 B 分量
        uint8_t B = color_rgb & 0x0000FF;
        fputc(R, gif_frame_rgb);
        fputc(G, gif_frame_rgb);
        fputc(B, gif_frame_rgb);
    }
    fflush(gif_frame_rgb);



    free(img_compressed);
    free(img);
    fclose(gif_compressed_frame);
    fclose(gif_decompressed_frame);
    fclose(gif_frame_rgb);

    return 0;
}