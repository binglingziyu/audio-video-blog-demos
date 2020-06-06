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

#include "lzw_table.h"
#include "darray.h"
#include <stdlib.h>

static const unsigned char pearson_table[] = {
  185,  93, 140, 180, 191, 107,  29, 203,
    3, 115, 171,  37,  27,  57, 201, 240,
  177,  36, 233, 178, 188,  72, 202, 109,
  239,  32,  49, 235, 164,  45, 101, 129,
   69,   1, 125,  92,  84, 128, 103, 245,
  210, 189,  14,   9, 170, 237, 243, 249,
  182, 221, 187, 142, 132, 139, 205,  50,
  225, 175,  85, 122, 111, 181, 130, 226,
  227, 198, 176,  51,  80, 165,  98,  96,
  193,  39, 156,  61,  62, 246, 136, 146,
  121, 147, 116, 138, 124, 145, 135, 159,
   79,   7,  76, 250, 254,  55,  56, 172,
   44, 154, 223, 195, 204, 255,  94, 212,
  220,  77,   4, 253,  68,  74, 232, 184,
   71,  20,  19, 224,  13, 137, 160,  81,
  236,  53,  21, 214, 228, 163,  91, 117,
  231,  16,  34,  90,  40, 192,  33,  83,
  219, 100, 251, 216, 208,  12,  86, 247,
   54, 113,   0,  17,  73, 244,  70, 123,
   59,  89,  46,  31, 179,  15,  75,  65,
  106,  95, 108,  42, 230, 105, 119, 168,
  218, 104, 151,   5, 112,  64, 174, 110,
  229, 144,  67, 183, 149, 134, 194, 155,
    6,  97, 242,  87,  30,  60, 238, 158,
  206,  63, 150,  25, 222, 200,  35,  22,
  133,  38, 186, 173,  52, 252, 114, 152,
   23,  58,  99, 196, 118, 241,  47, 248,
  162,   2,  24, 120,  41, 169, 141, 207,
  211, 153, 143,  26,  78,  43,  11,  48,
  209, 127, 131, 234, 190, 197, 102, 167,
   28,   8, 161,  88, 148, 126, 215, 199,
  217, 166,  18,  10, 213, 157,  66,  82
};

static unsigned int hash16(struct lzw_entry *e) {
  unsigned char c[5], result[5];
  unsigned int h = 0, hash = 0;

  if (!e) return 0;
  c[0] = e->len & 0xff;
  c[1] = (e->len >> 8) & 0xff;
  c[2] = e->len & 0xff;
  c[3] = (e->prev >> 8) & 0xff;
  c[4] = e->val;
  h = result[0] = pearson_table[h ^ c[0]];
  h = result[1] = pearson_table[h ^ c[1]];
  h = result[2] = pearson_table[h ^ c[2]];
  h = result[3] = pearson_table[h ^ c[3]];
  h = result[4] = pearson_table[h ^ c[4]];
  hash = (unsigned int) (result[0] << 8) | result[1];
  hash ^= (unsigned int) (result[2] << 8) | result[3];
  hash ^= (unsigned int) (result[4] << 8) | result[4];
  return hash;
}

void lzw_table_init(struct lzw_table *t,
                    enum lzw_table_type type,
                    unsigned char bit_size) {
  unsigned int i, len = (1 << bit_size);

  if (!t) return;
  t->type = type;
  t->n_entries = 0;
  if (t->type == LZW_TABLE_COMPRESS) {
    t->size = LZW_MAX_ENTRIES * 4 / 3;
    t->initialized = malloc(t->size);
    t->entries = malloc(sizeof(struct lzw_entry) * t->size);
    for (i = 0; i < t->size; ++i) {
      t->initialized[i] = 0;
    }
  } else {
    t->entries = malloc(sizeof(struct lzw_entry) * LZW_MAX_ENTRIES);
  }
  for (i = 0; i < len + 2; ++i) {
    struct lzw_entry e;

    e.len = 1;
    e.prev = 0;
    e.code = i;
    e.val = (unsigned char) i;
    lzw_table_add(t, &e);
  }
}

void lzw_table_deinit(struct lzw_table *t) {
  if (!t) return;
  if (t->type == LZW_TABLE_COMPRESS) free(t->initialized);
  free(t->entries);
}

void lzw_table_add(struct lzw_table *t, struct lzw_entry *e) {
  if (!t) return;
  if (!e) return;
  if (t->type == LZW_TABLE_COMPRESS) {
    if (t->n_entries < LZW_MAX_ENTRIES) {
      size_t i;

      i = (size_t) hash16(e) % t->size;
      while (t->initialized[i]) i = (i + 1) % t->size;
      e->code = t->n_entries;
      t->entries[i] = *e;
      t->initialized[i] = 1;
      t->n_entries++;
    }
  } else {
    if (t->n_entries < LZW_MAX_ENTRIES) {
      e->code = t->n_entries;
      t->entries[t->n_entries++] = *e;
    }
  }
}

int lzw_table_lookup_entry(struct lzw_table *t,
                           struct lzw_entry *e,
                           unsigned int *out_code) {
  unsigned int i;

  if (!t) return 0;
  if (!e) return 0;
  if (!out_code) return 0;
  for (i = hash16(e) % t->size; t->initialized[i] != 0; i = (i + 1) % t->size) {
    if (t->entries[i].len == e->len) {
      if (t->entries[i].prev == e->prev && t->entries[i].val == e->val) {
        *out_code = t->entries[i].code;
        return 1;
      }
    }
  }
  return 0;
}

int lzw_table_lookup_code(struct lzw_table *t,
                          unsigned int code,
                          struct lzw_entry *out_e) {
  if (!t) return 0;
  if (code < t->n_entries) {
    *out_e = t->entries[code];
    return 1;
  }
  return 0;
}

void lzw_table_str(struct lzw_table *t,
                   unsigned int code,
                   struct darray **out_buf) {
  unsigned long i, size;
  struct lzw_entry e;

  if (!t) return;
  if (!out_buf) return;
  if (code > t->n_entries) return;
  if (!lzw_table_lookup_code(t, code, &e)) return;
  size = e.len;
  *out_buf = danew(size);
  for (i = 0; i < size; ++i) {
    dapush(*out_buf, e.val);
    lzw_table_lookup_code(t, e.prev, &e);
  }
}

unsigned char lzw_entry_head(struct lzw_table *t, struct lzw_entry *e) {
  unsigned char result;
  unsigned long len;
  struct lzw_entry tmp;

  if (!t) return 0;
  if (!e) return 0;
  tmp = *e;
  len = tmp.len;
  while (len--) {
    result = tmp.val;
    lzw_table_lookup_code(t, tmp.prev, &tmp);
  }
  return result;
}
