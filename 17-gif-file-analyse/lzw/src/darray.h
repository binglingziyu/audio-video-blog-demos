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

#ifndef DARRAY_H
#define DARRAY_H

#include <stddef.h>

struct darray;

#define dapusht(d, t) dapushn(d, sizeof(t), (unsigned char *) &(t));

struct darray *danew(unsigned long cap);
void dafree(struct darray *d);
unsigned char *dapeel(struct darray *d);
int dapush(struct darray *d, unsigned char byte);
int dapushn(struct darray *d, unsigned long len, unsigned char *byte);
size_t dalen(struct darray *d);
unsigned char *daptr(struct darray *d);

#endif
