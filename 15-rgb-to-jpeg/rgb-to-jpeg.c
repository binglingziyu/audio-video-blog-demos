//
// Created by hubin on 2019/11/7.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "log.h"
#include "bitstr.h"
#include "color.h"
#include "block.h"
#include "dct.h"
#include "zigzag.h"
#include "quant.h"
#include "huffman.h"

void encode_du(HUFCODEC *hfcac, HUFCODEC *hfcdc, int du[64], int *dc);

int main() {

    // 0. 准备 RGB 数据
    int width = 70, height = 70;
    uint8_t rgb24Data[width*height*3];
    genRGB24Data(rgb24Data, width, height);

    // 1. 颜色模式转换 RGB -> YUV
    uint8_t yuv_y[width*height];
    uint8_t yuv_u[width*height];
    uint8_t yuv_v[width*height];
    rgb24ToYuv(rgb24Data, yuv_y, yuv_u, yuv_v, width, height);

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
    init_dct_module();
    int y_blocks_dct[block_size][64];
    int u_blocks_dct[block_size][64];
    int v_blocks_dct[block_size][64];
    for(int y_index = 0; y_index < block_size; y_index++) {
        uint8_t *y_block = y_blocks[y_index];
        // if(y_index == 0) print_block_u(y_blocks[y_index]);
        // DCT 之前减去 128
        for(int i = 0; i < 64; i++) {y_blocks_dct[y_index][i] = y_block[i]-128;}
        for(int i = 0; i < 64; i++) {
            y_blocks_dct[y_index][i] = y_blocks_dct[y_index][i] << 2;
        }
        // if(y_index == 0) print_block_i(y_blocks_dct[y_index]);
        fdct2d8x8(y_blocks_dct[y_index], NULL);
//        for (int i=0; i<64; i++) data[i] /= dctfactor[i];
//        for (i=0; i<64; i++) data[i] /= 8.0f;
        // fdct(y_blocks_dct[y_index]);
        // if(y_index == 11) print_block_i(y_blocks_dct[y_index]);
    }
    for(int u_index = 0; u_index < block_size; u_index++) {
        uint8_t *u_block = u_blocks[u_index];
        // DCT 之前减去 128
        for(int i = 0; i < 64; i++) {u_blocks_dct[u_index][i] = u_block[i] - 128;}
        for(int i = 0; i < 64; i++) {
            u_blocks_dct[u_index][i] = u_blocks_dct[u_index][i] << 2;
        }
        fdct2d8x8(u_blocks_dct[u_index], NULL);
        //fdct(u_blocks_dct[u_index]);
    }
    for(int v_index = 0; v_index < block_size; v_index++) {
        uint8_t *v_block = v_blocks[v_index];
        // DCT 之前减去 128
        for(int i = 0; i < 64; i++) {v_blocks_dct[v_index][i] = v_block[i] - 128;}
        for(int i = 0; i < 64; i++) {
            v_blocks_dct[v_index][i] = v_blocks_dct[v_index][i] << 2;
        }
        fdct2d8x8(v_blocks_dct[v_index], NULL);
        //fdct(v_blocks_dct[v_index]);
    }

    // 5. 量化
    // JPEG 指定压缩质量 1~99
    // int quality_scale = 1;
    int *pqtab[2];
    pqtab[0] = malloc(64*sizeof(int));
    pqtab[1] = malloc(64*sizeof(int));
    memcpy(pqtab[0], STD_QUANT_TAB_LUMIN, 64*sizeof(int));
    memcpy(pqtab[1], STD_QUANT_TAB_CHROM, 64*sizeof(int));
    for(int y_index = 0; y_index < block_size; y_index++) {
        quant_encode(y_blocks_dct[y_index], pqtab[0]);
    }
    for(int u_index = 0; u_index < block_size; u_index++) {
        quant_encode(u_blocks_dct[u_index], pqtab[1]);
    }
    for(int v_index = 0; v_index < block_size; v_index++) {
        quant_encode(v_blocks_dct[v_index], pqtab[1]);
    }

    // 6. Zigzag 扫描排序
    for(int y_index = 0; y_index < block_size; y_index++) {
        zigzag_encode(y_blocks_dct[y_index]);
    }
    for(int u_index = 0; u_index < block_size; u_index++) {
        zigzag_encode(u_blocks_dct[u_index]);
    }
    for(int v_index = 0; v_index < block_size; v_index++) {
        zigzag_encode(v_blocks_dct[v_index]);
    }

    int   dc[3]= {0};
    char *buffer = malloc(width*height*2);
    void *bs = bitstr_open(BITSTR_MEM, buffer, (char *) (width * height * 2));
    // huffman codec ac
    HUFCODEC *phcac[2];
    phcac[0]= calloc(1, sizeof(HUFCODEC));
    phcac[1]= calloc(1, sizeof(HUFCODEC));
    // huffman codec dc
    HUFCODEC *phcdc[2];
    phcdc[0] = calloc(1, sizeof(HUFCODEC));
    phcdc[1] = calloc(1, sizeof(HUFCODEC));
    // init huffman codec
    memcpy(phcac[0]->huftab, STD_HUFTAB_LUMIN_AC, MAX_HUFFMAN_CODE_LEN + 256);
    memcpy(phcac[1]->huftab, STD_HUFTAB_CHROM_AC, MAX_HUFFMAN_CODE_LEN + 256);
    memcpy(phcdc[0]->huftab, STD_HUFTAB_LUMIN_DC, MAX_HUFFMAN_CODE_LEN + 256);
    memcpy(phcdc[1]->huftab, STD_HUFTAB_CHROM_DC, MAX_HUFFMAN_CODE_LEN + 256);
    phcac[0]->output = bs; huffman_encode_init(phcac[0], 1);
    phcac[1]->output = bs; huffman_encode_init(phcac[1], 1);
    phcdc[0]->output = bs; huffman_encode_init(phcdc[0], 1);
    phcdc[1]->output = bs; huffman_encode_init(phcdc[1], 1);

    // 7.  DC 系数的差分脉冲调制编码
    // 8.  DC 系数的中间格式计算
    // 9.  AC 系数的游程长度编码
    // 10. AC 系数的中间格式计算
    // 11. 熵编码
    // 7、8、9、10、11 在 encode_du 里完成
    for(int index = 0; index < block_size; index++) {
        encode_du(phcac[0], phcdc[0], y_blocks_dct[index], &(dc[0]));
        encode_du(phcac[1], phcdc[1], u_blocks_dct[index], &(dc[1]));
        encode_du(phcac[1], phcdc[1], v_blocks_dct[index], &(dc[2]));
    }

    long dataLength = bitstr_tell(bs);
    printf("result = %ld", dataLength);


    // 下面开始将数据写入文件
    //FILE *fp = fopen("C:\\Users\\Administrator\\Desktop\\rainbow-rgb-to-jpeg.jpg", "wb+");
    FILE *fp = fopen("/Users/hubin/Desktop/rainbow-rgb-to-jpeg.jpg", "wb");

    // output SOI
    fputc(0xff, fp);
    fputc(0xd8, fp);

    // output DQT
    for (int i = 0; i < 2; i++) {
        int len = 2 + 1 + 64;
        fputc(0xff, fp);
        fputc(0xdb, fp);
        fputc(len >> 8, fp);
        fputc(len >> 0, fp);
        fputc(i   , fp);
        for (int j = 0; j < 64; j++) {
            fputc(pqtab[i][ZIGZAG[j]], fp);
        }
    }

    // output SOF0
    int SOF0Len = 2 + 1 + 2 + 2 + 1 + 3 * 3;
    fputc(0xff, fp);
    fputc(0xc0, fp);
    fputc(SOF0Len >> 8, fp);
    fputc(SOF0Len >> 0, fp);
    fputc(8   , fp); // precision 8bit
    fputc(height >> 8, fp); // height
    fputc(height >> 0, fp); // height
    fputc(width  >> 8, fp); // width
    fputc(width  >> 0, fp); // width
    fputc(3, fp);

//    for (int i = 0; i < 3; i++) {
//        fputc(jfif->comp_info[i].id, fp);
//        fputc((jfif->comp_info[i].samp_factor_v << 0)|(jfif->comp_info[i].samp_factor_h << 4), fp);
//        fputc(jfif->comp_info[i].qtab_idx, fp);
//    }
    fputc(0x01, fp); fputc(0x11, fp); fputc(0x00, fp);
    fputc(0x02, fp); fputc(0x11, fp); fputc(0x01, fp);
    fputc(0x03, fp); fputc(0x11, fp); fputc(0x01, fp);

    // output DHT AC
    for (int i = 0; i < 2; i++) {
        fputc(0xff, fp);
        fputc(0xc4, fp);
        int len = 2 + 1 + 16;
        for (int j = 0; j < 16; j++) len += phcac[i]->huftab[j];
        fputc(len >> 8, fp);
        fputc(len >> 0, fp);
        fputc(i + 0x10, fp);
        fwrite(phcac[i]->huftab, len - 3, 1, fp);
    }

    // output DHT DC
    for (int i = 0; i < 2; i++) {
        fputc(0xff, fp);
        fputc(0xc4, fp);
        int len = 2 + 1 + 16;
        for (int j = 0; j < 16; j++) len += phcdc[i]->huftab[j];
        fputc(len >> 8, fp);
        fputc(len >> 0, fp);
        fputc(i + 0x00, fp);
        fwrite(phcdc[i]->huftab, len - 3, 1, fp);
    }

    // output SOS
    int SOSLen = 2 + 1 + 2 * 3 + 3;
    fputc(0xff, fp);
    fputc(0xda, fp);
    fputc(SOSLen >> 8, fp);
    fputc(SOSLen >> 0, fp);
    fputc(3, fp);

    fputc(0x01, fp); fputc(0x00, fp);
    fputc(0x02, fp); fputc(0x11, fp);
    fputc(0x03, fp); fputc(0x11, fp);

    fputc(0x00, fp);
    fputc(0x3F, fp);
    fputc(0x00, fp);

    // output data
    if (buffer) {
        fwrite(buffer, dataLength, 1, fp);
    }

    // output EOI
    fputc(0xff, fp);
    fputc(0xd9, fp);

    fflush(fp);
    fclose(fp);

    // 释放；释放；
    free(pqtab[0]);
    free(pqtab[1]);
    free(buffer);
    // close huffman codec
    huffman_encode_done(phcac[0]);
    huffman_encode_done(phcac[1]);
    huffman_encode_done(phcdc[0]);
    huffman_encode_done(phcdc[1]);
    // close bit stream
    bitstr_close(bs);
    return 0;
}

typedef struct {
    unsigned runlen   : 4;
    unsigned codesize : 4;
    unsigned codedata : 16;
} RLEITEM;

void category_encode(int *code, int *size) {
    unsigned absc = abs(*code);
    unsigned mask = (1 << 15);
    int i    = 15;
    if (absc == 0) { *size = 0; return; }
    while (i && !(absc & mask)) { mask >>= 1; i--; }
    *size = i + 1;
    if (*code < 0) *code = (1 << *size) - absc - 1;
}

void encode_du(HUFCODEC *hfcac, HUFCODEC *hfcdc, int du[64], int *dc) {
    void     *bs    = hfcac->output;
    int       diff, code, size;
    RLEITEM   rlelist[63];
    int       i, j, n, eob;

    // 7.  DC 系数的差分脉冲调制编码
    // dc
    diff = du[0] - *dc;
    *dc  = du[0];

    // 8.  DC 系数的中间格式计算
    // category encode for dc
    code = diff;
    category_encode(&code, &size);

    // 11. 熵编码 DC
    // huffman encode for dc
    huffman_encode_step(hfcdc, size);
    bitstr_put_bits(bs, code, size);

    // 9.  AC 系数的游程长度编码
    // 10. AC 系数的中间格式计算
    // rle encode for ac
    for (i=1, j=0, n=0, eob=0; i<64 && j<63; i++) {
        if (du[i] == 0 && n < 15) {
            n++;
        } else {
            code = du[i]; size = 0;
            // 10. AC 系数的中间格式计算
            category_encode(&code, &size);
            rlelist[j].runlen   = n;
            rlelist[j].codesize = size;
            rlelist[j].codedata = code;
            n = 0;
            j++;
            if (size != 0) eob = j;
        }
    }

    // set eob
    if (du[63] == 0) {
        rlelist[eob].runlen   = 0;
        rlelist[eob].codesize = 0;
        rlelist[eob].codedata = 0;
        j = eob + 1;
    }

    // 11. 熵编码 AC
    // huffman encode for ac
    for (i=0; i<j; i++) {
        huffman_encode_step(hfcac, (rlelist[i].runlen << 4) | (rlelist[i].codesize << 0));
        bitstr_put_bits(bs, rlelist[i].codedata, rlelist[i].codesize);
    }
}