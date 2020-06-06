//
// Created by hubin on 2020/6/5.
//

#ifndef IMAGE_DEMO_RGB_PIXEL_LZW_COMPRESS_H
#define IMAGE_DEMO_RGB_PIXEL_LZW_COMPRESS_H

#include <stdint.h>
#include <stdio.h>

typedef struct onode* sequence_ptr;
typedef struct onode {
    unsigned char* chars;
    int length;
} sequence;

int addToDic(sequence_ptr new_sequence);
void populateDic(unsigned char minCodeSize);
int lookInDic(sequence_ptr cmp_sequence);
void addToBitStream(uint16_t code);
void addPixel(unsigned char pixel, sequence_ptr sequence);
void LZWCompress(FILE* file, unsigned char minCodeSize, unsigned char pixels[], int n_elem);
sequence_ptr createNewSequence(unsigned char array[], int length);
void resetSequence(sequence_ptr* sequence);
void printSequence(sequence_ptr sequence);
void resetDic();
void freeDic();
uint16_t reverseBin16(uint16_t);
uint8_t reverseBin8(uint8_t);

#endif //IMAGE_DEMO_RGB_PIXEL_LZW_COMPRESS_H
