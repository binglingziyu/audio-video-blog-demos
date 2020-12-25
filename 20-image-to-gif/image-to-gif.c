//
// Created by apple on 2020/12/25.
//

#include <stdio.h>
#include <png.h>
#include <stdlib.h>
#include <strings.h>

int decodePNG(char *, unsigned char **);

//https://gist.github.com/niw/5963798

int main() {
    char *bmp = "/Users/apple/Desktop/Android.bmp";
    char *png = "/Users/apple/Desktop/Huawei.png";
    char *jpg = "/Users/apple/Desktop/Fuchsia.jpg";
    char *gif = "/Users/apple/Desktop/iOS.gif";

    unsigned char *pngRGB = NULL;

    printf("开始读取文件！\n");
    decodePNG(png, &pngRGB);

    char *huaweiRGB = "/Users/apple/Desktop/Huawei.rgb";
    FILE *huaweiRGBFile = fopen(huaweiRGB, "wb");
    fwrite(pngRGB, 400*400*3, 1, huaweiRGBFile);
    fclose(huaweiRGBFile);
    return 0;
}


int decodePNG(char *filename, unsigned char **pngRGB) {
    FILE *fp = fopen(filename, "rb");
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) {
        fclose(fp);
        return -1;
    }

    png_infop info = png_create_info_struct(png);
    if(!info)  {
        fclose(fp);
        return -1;
    }

    if(setjmp(png_jmpbuf(png))) {
        fclose(fp);
        return -1;
    }

    png_init_io(png, fp);

    png_read_info(png, info);

    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers = NULL;

    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);


    printf("PNG 图片尺寸：【%d, %d】\n", width, height);
    printf("颜色类型：%d, 位深：%d\n", color_type, bit_depth);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
       color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);
    int rowByteCount = png_get_rowbytes(png,info);
    printf("rowByteCount: %d\n", rowByteCount);

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(rowByteCount);
    }

    png_read_image(png, row_pointers);

    *pngRGB = malloc(width*height*3);
    int counter = 0;
    for(int i = 0; i < height; i++) {
        if(color_type == 6) { // 带有透明 RGBA
            for(int j = 0; j < rowByteCount; j+=4) {
                memcpy(*pngRGB+counter, row_pointers[i]+j, 3);
                counter+=3;
            }
        } else {
            memcpy(*pngRGB+rowByteCount, row_pointers[i], rowByteCount);
        }
    }

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);

    return 0;
}