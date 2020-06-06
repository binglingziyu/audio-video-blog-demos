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
#include "lzw.h"
#include "lzw_bits.h"
#include "lzw_table.h"
#include "darray.h"
#include <stdio.h>
#include <stdlib.h>

void compress(unsigned char bit_size,
              unsigned long size,
              unsigned char *src,
              unsigned long *out_len,
              unsigned char **output,
              int gif_format) {
  unsigned char bit_width = bit_size + 1;
  struct lzw_bit_writer b;
  struct lzw_table ctable;
  struct darray *result;

  if (!output) return;
  if (!out_len) return;
  lzw_bw_init(&b, BIT_BUFFER, NULL);
  lzw_table_init(&ctable, LZW_TABLE_COMPRESS, bit_size);
  if (gif_format) lzw_bw_pack(&b, bit_width, 1 << bit_size);
  for (;;) {
    unsigned int code = 0;
    struct lzw_entry e = { 0 };

    /* build up the next dictionary entry */
    do {
      e.val = *src++;
      e.prev = code;
      e.len++;
      size--;
      if (!size) {
        lzw_bw_pack(&b, bit_width, code);
        code = *--src;
        break;
      }
    } while (lzw_table_lookup_entry(&ctable, &e, &code));
    lzw_table_add(&ctable, &e);
    lzw_bw_pack(&b, bit_width, code);
    if (ctable.n_entries > (1 << bit_width)) bit_width++;
    if (!size) break;
    size++;
    src--;
  }
  /* end of stream code for GIFs */
  if (gif_format) lzw_bw_pack(&b, bit_width, (1 << bit_size) + 1);
  lzw_table_deinit(&ctable);
  result = lzw_bw_result(&b);
  *out_len = dalen(result);
  *output = dapeel(result);
}

/* **************************************** */
/* Public */
/* **************************************** */

void lzw_compress(unsigned char bit_size,
                  unsigned long size,
                  unsigned char *src,
                  unsigned long *out_len,
                  unsigned char **result) {
  compress(bit_size, size, src, out_len, result, 0);
}

void lzw_compress_gif(unsigned char bit_size,
                      unsigned long size,
                      unsigned char *src,
                      unsigned long *out_len,
                      unsigned char **result) {
  compress(bit_size, size, src, out_len, result, 1);
}

void lzw_decompress(unsigned char bit_size,
                    unsigned long size,
                    unsigned char *src,
                    unsigned long *out_len,
                    unsigned char **result) {
  unsigned char bit_width = bit_size + 1;
  unsigned int code;
  struct lzw_bit_reader b;
  struct lzw_table dtable;
  struct darray *output;

  if (!src) return;
  if (!result) return;
  if (!out_len) return;
  output = danew(4096);
  lzw_table_init(&dtable, LZW_TABLE_DECOMPRESS, bit_size);
  lzw_br_init(&b, BIT_BUFFER, size, src);
  while (lzw_br_read(&b, bit_width, &code)) {
    struct lzw_entry cur;

    /* LZW clear code */
    if (code == (1 << bit_size)) {
      bit_width = bit_size + 1;
      dtable.n_entries = (1 << bit_size) + 2;
      continue;
    }
    /* explicit error code */
    if (code == (1 << bit_size) + 1) break;
    if (lzw_table_lookup_code(&dtable, code, &cur)) {
      unsigned int next_code;
      unsigned long i;
      struct darray *buf;
      struct lzw_entry new, next;

      lzw_table_str(&dtable, code, &buf);
      for (i = dalen(buf); i > 0; --i) {
        dapush(output, daptr(buf)[i -1 ]);
      }
      dafree(buf);
      if ((dtable.n_entries + 1 > (1 << bit_width)) &&
          (bit_width < LZW_MAX_ENTRY_EXP)) {
        bit_width++;
      }
      if (!lzw_br_peek(&b, bit_width, &next_code)) continue;
      new.len = cur.len + 1;
      if (lzw_table_lookup_code(&dtable, next_code, &next)) {
        new.val = lzw_entry_head(&dtable, &next);
      } else {
        new.val = lzw_entry_head(&dtable, &cur);
      }
      new.prev = code;
      lzw_table_add(&dtable, &new);
    }
  }
  lzw_table_deinit(&dtable);
  *out_len = dalen(output);
  *result = dapeel(output);
}
