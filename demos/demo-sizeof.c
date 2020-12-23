//
// Created by apple on 2020/12/23.
//

#include <stdio.h>
#include "strings.h"

#define GIF_STAMP "GIFVER"          /* First chars in file - GIF stamp.  */
#define GIF_STAMP_LEN sizeof(GIF_STAMP) - 1
typedef unsigned char *GifRowType;

int main() {
    // 6
    printf("GIF_STAMP_LEN: %lu\n", GIF_STAMP_LEN);

    char *name = "binglingziyu";

    // 8
    printf("sizeOf name: %lu\n", sizeof(name));

    // 13
    printf("sizeOf string: %lu\n", sizeof("binglingziyu"));

    // 12
    printf("strlen name: %lu\n", strlen(name));

    // 8
    printf("sizeOf GifRowType: %lu\n", sizeof(GifRowType));

    return 0;
}