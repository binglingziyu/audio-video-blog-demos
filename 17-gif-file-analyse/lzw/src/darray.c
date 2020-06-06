/* This file is a part of darray
 *
 * Copyright 2019, Jeffery Stager
 *
 * darray is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * darray is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with darray.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "darray.h"
#include <stdlib.h>
#include <string.h>

struct darray {
  unsigned long len;
  unsigned long cap;
  unsigned char *ptr;
};

static int increase_mem(struct darray *d) {
  unsigned char *tmp;
  unsigned long old_cap;

  old_cap = d->cap;
  d->cap <<= 1;
  tmp = realloc(d->ptr, d->cap);
  if (!tmp) {
    d->cap = old_cap;
    return -1;
  }
  d->ptr = tmp;
  return 0;
}

/* **************************************** */
/* Public */
/* **************************************** */

struct darray *danew(unsigned long cap) {
  struct darray *out;

  out = malloc(sizeof(struct darray));
  if (!out) return NULL;
  out->len = 0;
  out->cap = cap;
  out->ptr = malloc(cap);
  if (!out->ptr) {
    free(out);
    return NULL;
  }
  return out;
}

void dafree(struct darray *d) {
  if (!d) return;
  free(d->ptr);
  free(d);
}

unsigned char *dapeel(struct darray *d) {
  unsigned char *result;

  if (!d) return NULL;
  result = daptr(d);
  free(d);
  return result;
}

int dapush(struct darray *d, unsigned char byte) {
  if (!d) return -1;
  if (d->len >= d->cap) {
    if (increase_mem(d)) return -1;
  }
  d->ptr[d->len++] = byte;
  return 0;
}

int dapushn(struct darray *d, unsigned long len, unsigned char *bytes) {
  if (!d) return -1;
  if (!bytes) return -1;
  while ((d->len + len) >= d->cap) {
    if (increase_mem(d)) return -1;
  }
  memcpy(d->ptr + d->len, bytes, len);
  d->len += len;
  return 0;
}

unsigned long dalen(struct darray *d) {
  return d->len;
}

unsigned long dacap(struct darray *d) {
  return d->cap;
}

unsigned char *daptr(struct darray *d) {
  return d->ptr;
}
