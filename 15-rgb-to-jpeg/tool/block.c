//
// Created by hubin on 2019/11/8.
//

#include "block.h"

void get_8x8_block(const uint8_t *data, uint8_t *block_data, int h_block_pos, int v_block_pos, int width, int height) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            int h_pos = h_block_pos*8 + i;
            int v_pos = v_block_pos*8 + j;
            if(h_pos >= width || v_pos >= height) {
                block_data[8*i+j] = 0;
            } else {
                block_data[8*i+j] = data[h_pos*width + v_pos];
            }
        }
    }
}

int calc_block_size(int width, int height) {
    int h_block_size = width/8 + (width%8==0?0:1);
    int v_block_size = height/8 + (height%8==0?0:1);
    return h_block_size*v_block_size;
}

void block_data_8x8(uint8_t *data, uint8_t blocks[][64], int width, int height) {
    int h_block_size = width/8 + (width%8==0?0:1);
    int v_block_size = height/8 + (height%8==0?0:1);
    for(int i = 0; i < h_block_size; i++) {
        for(int j = 0; j < v_block_size; j++) {
            uint8_t *tmp = blocks[h_block_size*i+j];
            get_8x8_block(data, tmp, i, j, width, height);

            // print_block(tmp);
        }
    }
}
