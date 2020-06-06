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

#ifndef LZW_H
#define LZW_H

void lzw_compress(unsigned char bit_size,
                  unsigned long size,
                  unsigned char *src,
                  unsigned long *out_len,
                  unsigned char **output);
void lzw_compress_gif(unsigned char bit_size,
                      unsigned long size,
                      unsigned char *src,
                      unsigned long *out_len,
                      unsigned char **output);
void lzw_decompress(unsigned char bit_size,
                    unsigned long size,
                    unsigned char *src,
                    unsigned long *out_len,
                    unsigned char **output);

#endif
