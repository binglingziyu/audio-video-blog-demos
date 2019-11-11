//
// Created by Administrator on 2019/11/11.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void wrap_content(int lines, uint8_t container[][64]) {
    for(int i = 0; i < lines; i++) {
        uint8_t *line = container[i];
        for(int j = 0; j < 64; j++) {
            line[j] = i;
        }
    }
}

int main() {

    int lines = 222;

    uint8_t blocks[lines][64];
    wrap_content(lines, blocks);

    printf("Hello");
    return 0;
}