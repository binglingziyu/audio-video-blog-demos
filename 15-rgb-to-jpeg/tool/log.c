//
// Created by hubin on 2019/12/27.
//
#include "log.h"

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