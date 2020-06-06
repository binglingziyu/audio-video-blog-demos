//
// Created by hubin on 2020/6/5.
//

#include "lzw-compress.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>


#define TRUE 1
#define FALSE 0
//Info at: http://www.matthewflickinger.com/lab/whatsinagif/bits_and_bytes.asp

int n_bits;
int* output_sequence;
unsigned char minCodeSize_gbl;

struct {
    sequence_ptr entries;
    int n_entries;
} dictionary;

struct {
    uint8_t* bytes;
    int bits_unused;
    int bytes_used;
    int index;
} bit_stream;

void freeDic() {
    int i;
    for (i = 0; i < dictionary.n_entries; i++) {
        free(dictionary.entries[i].chars);
    }
    free(dictionary.entries);
}

void printDic() {
    int i;

    printf("\n[Dictionary:]\n\n");
    printf("-------------\n");

    for (i = 0; i < 81; i++) {
        printf("| %02d |", i);
        printSequence(&dictionary.entries[i]);
    }
    printf("-------------\n");
}

void printSequence(sequence_ptr sequence) {

    int i;

    for (i = 0; i < sequence->length; i++) {
        printf(" 0x%02X", sequence->chars[i] & 0xff);
    }
    printf("\n");

}

sequence_ptr createNewSequence(unsigned char array[], int length) {

    sequence_ptr new_sequence;

    new_sequence = (sequence_ptr)malloc(sizeof(sequence));
    new_sequence->length = length;
    if (length > 0) {
        new_sequence->chars = (unsigned char*)malloc(length*sizeof(unsigned char));
        memcpy(new_sequence->chars, array, length*sizeof(unsigned char));
    } else {
        new_sequence->chars = NULL;
    }

    return new_sequence;
}
void resetDic () {
    int i;
    addToBitStream((uint16_t)((int)pow(2.0, (double)minCodeSize_gbl)));
    for (i = 0; i < dictionary.n_entries; i++) {
        free(dictionary.entries[i].chars);
    }
    free(dictionary.entries);
    populateDic(minCodeSize_gbl);
}

void populateDic(unsigned char minCodeSize) {
    int i;
    int i_max;
    unsigned char temp_char[1];
    sequence_ptr new_sequence;

    n_bits = minCodeSize_gbl;

    dictionary.n_entries = 0;
    i_max = (int)pow(2.0, (double)minCodeSize);
    dictionary.entries = (sequence_ptr)malloc(sizeof(sequence) * (i_max+2));

    for (i = 0; i < i_max; i++) {
        temp_char[0] = (unsigned char)i;
        new_sequence = createNewSequence(temp_char, 1);
        addToDic(new_sequence);
    }

    temp_char[0] = (unsigned char)i_max;
    new_sequence = createNewSequence(temp_char, 1);
    addToDic(new_sequence);      //Clear-Code
    temp_char[0] = (unsigned char)(i_max+1);
    new_sequence = createNewSequence(temp_char, 1);
    addToDic(new_sequence);      //EoI-Code
}

int addToDic(sequence_ptr new_sequence) {

    dictionary.n_entries += 1;
    dictionary.entries = realloc(dictionary.entries, sizeof(sequence) * dictionary.n_entries);
    dictionary.entries[dictionary.n_entries-1] = *new_sequence;

    if (dictionary.n_entries > (int)pow(2.0, n_bits)) {
        if (n_bits < 12) {
            n_bits++;
        } else {
            return -1;
        }
    }
    return 0;
}

int sequenceCmp(sequence_ptr seq_1, sequence_ptr seq_2) {

    int i;

    if (seq_1->length == seq_2->length) {
        for (i = 0; i < seq_1->length; i++) {
            if (seq_1->chars[i] != seq_2->chars[i]) {
                return -1;
            }
        }
        return 0;
    }
    return -1;
}

int lookInDic(sequence_ptr cmp_sequence) {
    int i = 0;
    int was_found = FALSE;
    int index = -1;

    do {
        if (sequenceCmp(cmp_sequence, &dictionary.entries[i]) == 0) {
            index = i;
            was_found = TRUE;
        }
        i++;
    } while (was_found == FALSE && i < dictionary.n_entries);

    return index;
}

void initBitStream(unsigned char minCodeSize) {
    bit_stream.bytes_used = 0;
    bit_stream.index = 0;
    bit_stream.bits_unused = 0;
    bit_stream.bytes = (uint8_t*)malloc(sizeof(uint8_t));
    addToBitStream((uint16_t)pow(2.0, (double)minCodeSize) & 0xffff);
}

uint8_t reverseBin8(uint8_t code) {
    uint8_t temp_overflow = 0x00;
    uint8_t temp_mask = 0x01;
    uint8_t new_code = 0x00;
    int i;

    for (i = 0; i < 8; i++) {
        new_code = new_code << 1;
        temp_overflow = (uint8_t)(temp_mask & code);
        new_code = (uint8_t)(new_code | temp_overflow);
        code = code >> 1;
    }

    return new_code;
}

uint16_t reverseBin16(uint16_t code) {
    uint16_t temp_overflow = 0x0000;
    uint16_t temp_mask = 0x0001;
    uint16_t new_code = 0x0000;
    int i;

    for (i = 0; i < 16; i++) {
        new_code = new_code << 1;
        temp_overflow = (uint16_t)(temp_mask & code);
        new_code = (uint16_t)(new_code | temp_overflow);
        code = code >> 1;
    }

    new_code = (uint16_t)(new_code >> (16-n_bits));

    return new_code;
}

uint8_t* partitionCode(uint16_t code) {
    uint8_t* partition;
    uint8_t temp_overflow;
    int i;

    code = reverseBin16(code);

    partition = (uint8_t*)malloc(3*sizeof(uint8_t));
    code = code << (16 - n_bits);
    partition[0] = 0x00;
    partition[1] = (uint8_t)((code & 0xff00) >> 8);
    partition[2] = (uint8_t)(code & 0x00ff);

    //shift whole partition
    for (i = 0; i < 2; i++) {
        temp_overflow = 0xff;
        temp_overflow = (uint8_t)(temp_overflow << (8-bit_stream.bits_unused));
        temp_overflow = (uint8_t)(temp_overflow & partition[1+i]);
        temp_overflow = (uint8_t)(temp_overflow >> (8-bit_stream.bits_unused));

        partition[i] = (uint8_t)(partition[i] | temp_overflow);
        partition[1+i] = (uint8_t)(partition[1+i] << bit_stream.bits_unused);
    }

    return partition;
}

void addToBitStream(uint16_t code) {
    uint8_t* code_parts;
    int extra_bytes_needed = 0;
    int i;

    code_parts = partitionCode(code);
    bit_stream.bytes[bit_stream.index] = (uint8_t)(bit_stream.bytes[bit_stream.index] | code_parts[0]);

    if (bit_stream.bits_unused < n_bits) {
        extra_bytes_needed += 1;
        bit_stream.bytes[bit_stream.index] = reverseBin8( bit_stream.bytes[bit_stream.index]);
        if (n_bits - bit_stream.bits_unused > 8) {
            extra_bytes_needed += 1;
        }
    }

    bit_stream.bits_unused = (1+extra_bytes_needed)*8 - ((8 - bit_stream.bits_unused) + n_bits);

    for (i = 0; i < extra_bytes_needed; i++) {
        if (bit_stream.bytes_used == 255) {
            bit_stream.bytes[bit_stream.index - bit_stream.bytes_used] = (uint8_t)(0xff);
            bit_stream.index += 1;
            bit_stream.bytes_used = 0;
            bit_stream.bytes = realloc(bit_stream.bytes, sizeof(uint8_t) * (bit_stream.index+1));
        }
        bit_stream.bytes_used += 1;
        bit_stream.index += 1;
        bit_stream.bytes = realloc(bit_stream.bytes, sizeof(uint8_t) * (bit_stream.index+1));
        if (extra_bytes_needed == 2 && i == 0) {
            bit_stream.bytes[bit_stream.index] = reverseBin8((uint8_t)(0x00 | code_parts[1+i]));
        }
        else
        {
            bit_stream.bytes[bit_stream.index] = (uint8_t)(0x00 | code_parts[1+i]);
        }
    }

}

void addPixel(unsigned char pixel, sequence_ptr sequence) {

    if (sequence->chars == NULL) {
        sequence->chars = (unsigned char*)malloc(sizeof(unsigned char));
        sequence->length = 0;
    } else {
        sequence->chars = (unsigned char*)realloc(sequence->chars, (sequence->length + 1) * sizeof(unsigned char));
    }
    sequence->chars[sequence->length] = pixel;
    sequence->length += 1;
}

void resetSequence(sequence_ptr* sequence) {
    if (sequence == NULL) {
        *sequence = createNewSequence(NULL, 0);
    } else {
        free(*sequence);
        *sequence = createNewSequence(NULL, 0);
    }
}

void LZWCompress(FILE* file, unsigned char minCodeSize, unsigned char pixels[], int n_elem) {

    int pixel_index = 0;
    sequence_ptr cur_sequence = NULL;        //janela de pixels a verificar
    sequence_ptr prev_sequence = NULL;       //janela de pixeis verificados na iteracao anterior

    minCodeSize_gbl = minCodeSize;

    populateDic(minCodeSize);
    initBitStream(minCodeSize);

    resetSequence(&cur_sequence);
    resetSequence (&prev_sequence);
    addPixel(pixels[pixel_index], cur_sequence);

    while (pixel_index < n_elem) {
        if (lookInDic(cur_sequence) == -1) {

            addToBitStream((uint16_t)lookInDic(prev_sequence));

            if (addToDic(cur_sequence) == -1) {
                resetDic();
            }

            resetSequence(&cur_sequence);
            resetSequence(&prev_sequence);
            addPixel(pixels[pixel_index], cur_sequence);
        } else {
            addPixel(pixels[pixel_index], prev_sequence);
            if (pixel_index < n_elem) {
                pixel_index++;
                addPixel(pixels[pixel_index], cur_sequence);
            }
        }
    }

    if (cur_sequence->length > 1) {
        if (lookInDic(cur_sequence) == -1) {
            addToBitStream(lookInDic(prev_sequence));
        } else {
            addToBitStream(lookInDic(cur_sequence));
        }
    } else if (cur_sequence->length == 1) {
        addToBitStream(lookInDic(cur_sequence));
    }
    free(cur_sequence);
    free(prev_sequence);

    bit_stream.bytes[bit_stream.index - bit_stream.bytes_used] = bit_stream.bytes_used;       //write the size of the sub-block

    fwrite(bit_stream.bytes, 1, bit_stream.index + 1, file);
    free(bit_stream.bytes);
    freeDic();

}