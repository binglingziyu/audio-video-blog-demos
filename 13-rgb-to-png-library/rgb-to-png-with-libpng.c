//
// Created by hubin on 2019/11/3.
//

#include <stdint.h>
#include <stdlib.h>
#include <png.h>

// 彩虹的七种颜色
uint32_t rainbowColors[] = {
        0XFF0000, // 红
        0XFFA500, // 橙
        0XFFFF00, // 黄
        0X00FF00, // 绿
        0X007FFF, // 青
        0X0000FF, // 蓝
        0X8B00FF  // 紫
};

uint8_t* getRainbowRGB24Data(uint8_t *rgb24Data, int width, int height) {
    for (int i = 0; i < width; ++i) {
        // 当前颜色
        uint32_t currentColor = rainbowColors[0];
        if(i < 100) {
            currentColor = rainbowColors[0];
        } else if(i < 200) {
            currentColor = rainbowColors[1];
        } else if(i < 300) {
            currentColor = rainbowColors[2];
        } else if(i < 400) {
            currentColor = rainbowColors[3];
        } else if(i < 500) {
            currentColor = rainbowColors[4];
        } else if(i < 600) {
            currentColor = rainbowColors[5];
        } else if(i < 700) {
            currentColor = rainbowColors[6];
        }
        // 当前颜色 R 分量
        uint8_t R = (currentColor & 0xFF0000) >> 16;
        // 当前颜色 G 分量
        uint8_t G = (currentColor & 0x00FF00) >> 8;
        // 当前颜色 B 分量
        uint8_t B = currentColor & 0x0000FF;

        for (int j = 0; j < height; ++j) {
            int currentPixelIndex = 3*(i*height + j);
            // 按 BGR 顺序写入一个像素 RGB24 到文件中
            rgb24Data[currentPixelIndex] = R;
            rgb24Data[currentPixelIndex+1] = G;
            rgb24Data[currentPixelIndex+2] = B;
        }
    }
    return rgb24Data;
}

void testRainbow() {
    int width = 700, height = 700, bit_depth = 8;
    uint8_t rgb24Data[width*height*3];
    png_structp png_ptr;
    png_infop info_ptr;

    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\lodepng-rgb-rainbow.png", "wb+");
    FILE *png_file = fopen("/Users/hubin/Desktop/libpng-rgb-rainbow.png", "wb");
    if (!png_file) {
        return ;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(png_ptr == NULL) {
        printf("ERROR:png_create_write_struct/n");
        fclose(png_file);
        return ;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL) {
        printf("ERROR:png_create_info_struct/n");
        png_destroy_write_struct(&png_ptr, NULL);
        return ;
    }
    png_init_io(png_ptr, png_file);
    png_set_IHDR(
            png_ptr,
            info_ptr,
            width,
            height,
            bit_depth,
            PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE);


    png_colorp palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
    if (!palette) {
        fclose(png_file);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return ;
    }
    png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
    png_write_info(png_ptr, info_ptr);
    png_set_packing(png_ptr);

    getRainbowRGB24Data(rgb24Data, width, height);
    //这里就是图像数据了
    png_bytepp rows = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (int i = 0; i < height; ++i)
    {
        rows[i] = (png_bytep)(rgb24Data + (i) * width * 3);
    }

    png_write_image(png_ptr, rows);

    // delete[] rows;
    png_write_end(png_ptr, info_ptr);

    png_free(png_ptr, palette);
    palette=NULL;
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(png_file);
}

void test01() {
    int width = 512, height = 512, bit_depth = 8;
    png_structp png_ptr;
    png_infop info_ptr;

    // FILE *file = fopen("C:\\Users\\Administrator\\Desktop\\lodepng-test01.png", "wb+");
    FILE *png_file = fopen("/Users/hubin/Desktop/libpng-test01.png", "wb");
    if (!png_file) {
        return ;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(png_ptr == NULL) {
        printf("ERROR:png_create_write_struct/n");
        fclose(png_file);
        return ;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL) {
        printf("ERROR:png_create_info_struct/n");
        png_destroy_write_struct(&png_ptr, NULL);
        return ;
    }
    png_init_io(png_ptr, png_file);
    png_set_IHDR(
            png_ptr,
            info_ptr,
            width,
            height,
            bit_depth,
            PNG_COLOR_TYPE_RGB_ALPHA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE);


    png_colorp palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
    if (!palette) {
        fclose(png_file);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return ;
    }
    png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
    png_write_info(png_ptr, info_ptr);
    png_set_packing(png_ptr);

    /*generate some image*/
    unsigned char* image = malloc(width * height * 4);
    unsigned x, y;
    for(y = 0; y < height; y++)
        for(x = 0; x < width; x++) {
            image[4 * width * y + 4 * x + 0] = 255 * !(x & y);
            image[4 * width * y + 4 * x + 1] = x ^ y;
            image[4 * width * y + 4 * x + 2] = x | y;
            image[4 * width * y + 4 * x + 3] = 255;
        }
    //这里就是图像数据了
    png_bytepp rows = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (int i = 0; i < height; ++i) {
        rows[i] = (png_bytep)(image + (i) * width * 4);
    }

    png_write_image(png_ptr, rows);

    // delete[] rows;
    png_write_end(png_ptr, info_ptr);

    png_free(png_ptr, palette);
    palette=NULL;
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(png_file);
}

int main(int c, char** v) {
    test01();
    testRainbow();
    return 0;
}