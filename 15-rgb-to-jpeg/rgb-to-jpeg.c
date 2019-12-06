//
// Created by hubin on 2019/11/7.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "color.h"
#include "block.h"
#include "dct.h"
#include "zigzag.h"
#include "quant.h"

void print_block_u(uint8_t *data) {
    for(int i = 0; i < 8; i++) {
        int start_pos = i*8;
        printf("%3u  %3u  %3u  %3u  %3u  %3u  %3u  %3u\n",
               data[start_pos],
               data[start_pos+1],
               data[start_pos+2],
               data[start_pos+3],
               data[start_pos+4],
               data[start_pos+5],
               data[start_pos+6],
               data[start_pos+7]);
    }
    printf("\n\n");
}
void print_block_d(int8_t *data) {
    for(int i = 0; i < 8; i++) {
        int start_pos = i*8;
        printf("%3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d\n",
               data[start_pos],
               data[start_pos+1],
               data[start_pos+2],
               data[start_pos+3],
               data[start_pos+4],
               data[start_pos+5],
               data[start_pos+6],
               data[start_pos+7]);
    }
    printf("\n\n");
}

void print_block_i(int *data) {
    for(int i = 0; i < 8; i++) {
        int start_pos = i*8;
        printf("%3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d\n",
               data[start_pos],
               data[start_pos+1],
               data[start_pos+2],
               data[start_pos+3],
               data[start_pos+4],
               data[start_pos+5],
               data[start_pos+6],
               data[start_pos+7]);
    }
    printf("\n\n");
}

void print_block_f(float *data) {
    for(int i = 0; i < 8; i++) {
        int start_pos = i*8;
        printf("%3.2f  %3.2f  %3.2f  %3.2f  %3.2f  %3.2f  %3.2f  %3.2f\n",
               data[start_pos],
               data[start_pos+1],
               data[start_pos+2],
               data[start_pos+3],
               data[start_pos+4],
               data[start_pos+5],
               data[start_pos+6],
               data[start_pos+7]);
    }
    printf("\n\n");
}

int main() {

    // JPEG 指定压缩质量 1~99
//    int quality_scale = 1;

    // 0. 准备 RGB 数据
    int width = 80, height = 80;
    uint8_t rgb24Data[width*height*3];
    genRGB24Data(rgb24Data, width, height);

    // 1. 颜色模式转换 RGB -> YUV
    uint8_t yuv_y[width*height];
    uint8_t yuv_u[width*height];
    uint8_t yuv_v[width*height];
    rgb24ToYuv(rgb24Data, yuv_y, yuv_u, yuv_v, width, height);

//    FILE *y_file = fopen("/Users/hubin/Desktop/yuv_y", "wb");
//    FILE *u_file = fopen("/Users/hubin/Desktop/yuv_u", "wb");
//    FILE *v_file = fopen("/Users/hubin/Desktop/yuv_v", "wb");
//    fwrite(yuv_y, 1, sizeof(yuv_y), y_file);
//    fwrite(yuv_u, 1, sizeof(yuv_u), u_file);
//    fwrite(yuv_v, 1, sizeof(yuv_v), v_file);
//    fclose(y_file);
//    fclose(u_file);
//    fclose(v_file);

    // 2. 采样
    // 在 1. 颜色模式转换 生成了 yuv_y yuv_u yuv_v 三个分量，等于 YUV444 采样

    // 3. 分块
    int block_size = calc_block_size(width, height);
    uint8_t y_blocks[block_size][64];
    uint8_t u_blocks[block_size][64];
    uint8_t v_blocks[block_size][64];
    block_data_8x8(yuv_y, y_blocks, width, height);
    block_data_8x8(yuv_u, u_blocks, width, height);
    block_data_8x8(yuv_v, v_blocks, width, height);

    // 4. 离散余弦变换（DCT）
    transMat();
    int y_blocks_dct[block_size][64];
    int u_blocks_dct[block_size][64];
    int v_blocks_dct[block_size][64];
    for(int y_index = 0; y_index < block_size; y_index++) {
        uint8_t *y_block = y_blocks[y_index];
        // if(y_index == 0) print_block_u(y_blocks[y_index]);
        // DCT 之前减去 128
        for(int i = 0; i < 64; i++) {y_blocks_dct[y_index][i] = y_block[i]-128;}
        // if(y_index == 0) print_block_i(y_blocks_dct[y_index]);
        fdct(y_blocks_dct[y_index]);
        // if(y_index == 11) print_block_i(y_blocks_dct[y_index]);
    }
    for(int u_index = 0; u_index < block_size; u_index++) {
        uint8_t *u_block = u_blocks[u_index];
        // DCT 之前减去 128
        for(int i = 0; i < 64; i++) {u_blocks_dct[u_index][i] = u_block[i] - 128;}
        fdct(u_blocks_dct[u_index]);
    }
    for(int v_index = 0; v_index < block_size; v_index++) {
        uint8_t *v_block = v_blocks[v_index];
        // DCT 之前减去 128
        for(int i = 0; i < 64; i++) {v_blocks_dct[v_index][i] = v_block[i] - 128;}
        fdct(v_blocks_dct[v_index]);
    }

    // 5. Zigzag 扫描排序
    for(int y_index = 0; y_index < block_size; y_index++) {
        // if(y_index == 11) print_block_i(y_blocks_dct[y_index]);
        zigzag_encode(y_blocks_dct[y_index]);
         if(y_index == 11) print_block_i(y_blocks_dct[y_index]);
    }
    for(int u_index = 0; u_index < block_size; u_index++) {
        zigzag_encode(u_blocks_dct[u_index]);
    }
    for(int v_index = 0; v_index < block_size; v_index++) {
        zigzag_encode(v_blocks_dct[v_index]);
    }

    // 6. 量化
    int lumin_quant_tab[64];
    for(int i = 0; i < 64; i++) {lumin_quant_tab[i] = STD_QUANT_TAB_LUMIN[i];}
    zigzag_encode(lumin_quant_tab);
    for(int y_index = 0; y_index < block_size; y_index++) {
        if(y_index == 11) print_block_i(y_blocks_dct[y_index]);
        quant_encode(y_blocks_dct[y_index], lumin_quant_tab);
        if(y_index == 11) print_block_i(y_blocks_dct[y_index]);
    }
    int chrom_quant_tab[64];
    for(int i = 0; i < 64; i++) {lumin_quant_tab[i] = STD_QUANT_TAB_CHROM[i];}
    zigzag_encode(chrom_quant_tab);
    for(int u_index = 0; u_index < block_size; u_index++) {
        quant_encode(u_blocks_dct[u_index], chrom_quant_tab);
    }
    for(int v_index = 0; v_index < block_size; v_index++) {
        quant_encode(v_blocks_dct[v_index], chrom_quant_tab);
    }


    return 0;
}