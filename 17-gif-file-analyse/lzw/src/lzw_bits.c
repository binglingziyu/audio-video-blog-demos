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

#include "lzw_bits.h"
#include "darray.h"
#include <stdlib.h>

static unsigned long gen_mask(unsigned int len) {
  unsigned long mask;

  switch (len) {
    case 0: mask = 0x0000; break;

    case 1: mask = 0x0001; break;
    case 2: mask = 0x0003; break;
    case 3: mask = 0x0007; break;
    case 4: mask = 0x000f; break;

    case 5: mask = 0x001f; break;
    case 6: mask = 0x003f; break;
    case 7: mask = 0x007f; break;
    case 8: mask = 0x00ff; break;

    case 9:  mask = 0x01ff; break;
    case 10: mask = 0x03ff; break;
    case 11: mask = 0x07ff; break;
    case 12: mask = 0x0fff; break;

    case 13: mask = 0x1fff; break;
    case 14: mask = 0x3fff; break;
    case 15: mask = 0x7fff; break;
    case 16: mask = 0xffff; break;

    case 17: mask = 0x0001ffff; break;
    case 18: mask = 0x0003ffff; break;
    case 19: mask = 0x0007ffff; break;
    case 20: mask = 0x000fffff; break;

    case 21: mask = 0x001fffff; break;
    case 22: mask = 0x003fffff; break;
    case 23: mask = 0x007fffff; break;
    case 24: mask = 0x00ffffff; break;

    case 25: mask = 0x01ffffff; break;
    case 26: mask = 0x03ffffff; break;
    case 27: mask = 0x07ffffff; break;
    case 28: mask = 0x0fffffff; break;

    case 29: mask = 0x1fffffff; break;
    case 30: mask = 0x3fffffff; break;
    case 31: mask = 0x7fffffff; break;
    case 32: mask = 0xffffffff; break;

    default: mask = 0x0000; break;
  }
  return mask;
}

/* **************************************** */
/* Public */
/* **************************************** */

void lzw_bw_init(struct lzw_bit_writer *b,
                 enum lzw_bit_resource_type type,
                 void *dst) {
  if (!b) return;
  b->type = type;
  b->bits = 0;
  b->pos = 0;
  b->buf = danew(1024);
}

void lzw_bw_pack(struct lzw_bit_writer *b,
                 unsigned char n_bits,
                 unsigned int bits) {
  unsigned long mask;

  if (!b) return;
  mask = gen_mask(n_bits);
  mask <<= b->pos;
  b->bits |= (bits << b->pos) & mask;
  b->pos += n_bits;
  while (b->pos >= 8) {
    dapush(b->buf, b->bits & 0xff);
    b->pos -= 8;
    b->bits = (b->bits >> 8) & gen_mask(b->pos);
  }
}

struct darray *lzw_bw_result(struct lzw_bit_writer *b) {
  if (!b) return NULL;
  if (b->pos) {
    dapush(b->buf, (unsigned char) (b->bits & gen_mask(b->pos)));
  }
  return b->buf;
}

void lzw_br_init(struct lzw_bit_reader *r,
                 enum lzw_bit_resource_type type,
                 unsigned long src_size,
                 void *src) {
  if (!r) return;
  if (!src) return;
  r->type = type;
  r->bits = 0;
  r->buf.pos = 0;
  r->buf.size = src_size;
  r->buf.data = (unsigned char *) src;
  r->pos = 0;
}

int lzw_br_read(struct lzw_bit_reader *r,
                unsigned char n_bits,
                unsigned int *result) {
  if (!r) return 0;
  if (!result) return 0;
  while (r->pos < n_bits) {
    unsigned long tmp;

    if (r->buf.pos >= r->buf.size) return 0;
    tmp = (unsigned long) (r->buf.data[r->buf.pos++]);
    r->bits |= (tmp & gen_mask(8)) << r->pos;
    r->pos += 8;
  }
  *result = r->bits & (unsigned int) gen_mask(n_bits);
  r->pos -= n_bits;
  r->bits = (r->bits >> n_bits) & gen_mask(r->pos);
  return 1;
}

int lzw_br_peek(struct lzw_bit_reader *r,
                unsigned char n_bits,
                unsigned int *result) {
  struct lzw_bit_reader copy;

  if (!r) return 0;
  if (!result) return 0;
  copy = *r;
  return lzw_br_read(&copy, n_bits, result);
}
