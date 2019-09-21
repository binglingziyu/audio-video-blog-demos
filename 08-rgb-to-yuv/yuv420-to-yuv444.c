//
// Created by hubin on 2019/9/17.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>

void yuv420ToYuv444(uint8_t *inbuf, uint8_t *outbuf, int w, int h) {
    uint8_t *srcY = NULL, *srcU = NULL, *srcV = NULL;
    uint8_t *desY = NULL, *desU = NULL, *desV = NULL;
    srcY = inbuf;//Y
    srcU = srcY + w * h;//U
    srcV = srcU + w * h / 4;;//V

    desY = outbuf;
    desU = desY + w * h;
    desV = desU + w * h;
    memcpy(desY, srcY, w * h * sizeof(uint8_t));//Y分量直接拷贝即可
    //UV分量转换
    int i, j;
    for (i = 0; i < h; i += 2) {//行
        for (j = 0; j < w; j += 2) {//列
            //U
            desU[i * w + j] = srcU[i / 2 * w / 2 + j / 2];
            desU[i * w + j + 1] = srcU[i / 2 * w / 2 + j / 2];
            desU[(i + 1) * w + j] = srcU[i / 2 * w / 2 + j / 2];
            desU[(i + 1) * w + j + 1] = srcU[i / 2 * w / 2 + j / 2];
            //V
            desV[i * w + j] = srcV[i / 2 * w / 2 + j / 2];
            desV[i * w + j + 1] = srcV[i / 2 * w / 2 + j / 2];
            desV[(i + 1) * w + j] = srcV[i / 2 * w / 2 + j / 2];
            desV[(i + 1) * w + j + 1] = srcV[i / 2 * w / 2 + j / 2];
        }
    }
}

int main() {
    int width = 700, height = 700;
    uint8_t yuv444pData[width*height*3];
    uint8_t yuv420pData[width*height*3/2];

    FILE *yuv420File = fopen("/Users/hubin/Desktop/rainbow-700x700-yuv420p-yu12.yuv", "rb");
    fread(yuv420pData, sizeof(yuv420pData), 1, yuv420File);

    yuv420ToYuv444(yuv420pData, yuv444pData, width, height);

    FILE *yuv444File = fopen("/Users/hubin/Desktop/rainbow-700x700-yuv420-to-yuv444.yuv", "wb");
    fwrite(yuv444pData, sizeof(yuv444pData), 1, yuv444File);

    fclose(yuv420File);
    fclose(yuv444File);

    return 0;
}
