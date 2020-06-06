/* This file is a part of liblzw
 *
 * Copyright 2019, Jeffery Stager
 *
 * liblzw is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liblzw is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liblzw.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LZW_BITS_H
#define LZW_BITS_H

#include <stdio.h>

enum lzw_bit_resource_type {
  BIT_FILE,
  BIT_BUFFER
};

struct lzw_bit_writer {
  enum lzw_bit_resource_type type;
  unsigned long bits;
  unsigned char pos;
  struct darray *buf;
};

struct lzw_bit_reader {
  enum lzw_bit_resource_type type;
  unsigned long bits;
  struct {
    unsigned long pos;
    unsigned long size;
    unsigned char *data;
  } buf;
  unsigned char pos;
};

void lzw_bw_init(struct lzw_bit_writer *b,
                 enum lzw_bit_resource_type type,
                 void *dst);
void lzw_bw_pack(struct lzw_bit_writer *b,
                 unsigned char n_bits,
                 unsigned int bits);
struct darray *lzw_bw_result(struct lzw_bit_writer *b);
void lzw_br_init(struct lzw_bit_reader *r,
                 enum lzw_bit_resource_type type,
                 unsigned long src_size,
                 void *src);
int lzw_br_read(struct lzw_bit_reader *r,
                unsigned char n_bits,
                unsigned int *result);
int lzw_br_peek(struct lzw_bit_reader *r,
                unsigned char n_bits,
                unsigned int *result);

#endif
