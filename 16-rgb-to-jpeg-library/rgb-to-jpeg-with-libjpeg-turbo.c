//
// Created by hubin on 2020/2/28.
//

#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <stdbool.h>
#include <turbojpeg.h>
#include "util.h"

struct ImageData {
     unsigned char *pixels;
     long  width;
     long height;
};

int decode_JPEG_file(char *inJpegName, char *outRgbName) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE * infile;
    FILE * outfile;

    if ((infile = fopen(inJpegName, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", inJpegName);
        return -1;
    }
    if ((outfile = fopen(outRgbName, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", outRgbName);
        return -1;
    }

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, infile);

    jpeg_read_header(&cinfo, TRUE);

    printf("image_width = %d\n", cinfo.image_width);
    printf("image_height = %d\n", cinfo.image_height);
    printf("num_components = %d\n", cinfo.num_components);

    printf("enter scale M/N:\n");

    cinfo.out_color_space = JCS_YCbCr;
    printf("scale to : %d/%d\n", cinfo.scale_num, cinfo.scale_denom);

    jpeg_start_decompress(&cinfo);

    //输出的图象的信息
    printf("output_width = %d\n", cinfo.output_width);
    printf("output_height = %d\n", cinfo.output_height);
    printf("output_components = %d\n", cinfo.output_components);

    int row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */
    JSAMPARRAY buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW));
    buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);

    struct ImageData imageData = {
        .width =  cinfo.image_width,
        .height = cinfo.image_height,
        .pixels = malloc(row_stride*cinfo.image_height)
    };
    long counter = 0;

//    tjDecompressToYUV()

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(imageData.pixels + counter, buffer[0], row_stride);
        counter += row_stride;
//        /* jpeg_read_scanlines expects an array of pointers to scanlines.
//        * Here the array is only one element long, but you could ask for
//        * more than one scanline at a time if that's more convenient.
//        */
//        jpeg_read_scanlines(&cinfo, buffer, 1);
//        /* Assume put_scanline_someplace wants a pointer and sample count. */
//        //put_scanline_someplace(buffer[0], row_stride);
//
//        fwrite(buffer, row_stride*cinfo.num_components, 1, outfile);
    }

    printf("total size: %ld\n", counter);
    fwrite(imageData.pixels,  counter, 1, outfile);


    jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    fclose(infile);

    return 0;
}


int encode_JPEG_file(char *strImageName, int image_height, int image_width, int quality) {
//    unsigned char *image_buffer = NULL;    /* Points to large array of R,G,B-order data */
//    int image_height = 780; /* Number of rows in image */
//    int image_width = 1040;  /* Number of columns in image */

    /* This struct contains the JPEG compression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    * It is possible to have several such structures, representing multiple
    * compression/decompression processes, in existence at once.  We refer
    * to any one struct (and its associated working data) as a "JPEG object".
    */
    struct jpeg_compress_struct cinfo;

    /* This struct represents a JPEG error handler.  It is declared separately
    * because applications often want to supply a specialized error handler
    * (see the second half of this file for an example).  But here we just
    * take the easy way out and use the standard error handler, which will
    * print a message on stderr and call exit() if compression fails.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE * outfile;     /* target file */
    JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */
    int row_stride;     /* physical row width in image buffer */

    /* Step 1: allocate and initialize JPEG compression object */

    /* We have to set up the error handler first, in case the initialization
    * step fails.  (Unlikely, but it could happen if you are out of memory.)
    * This routine fills in the contents of struct jerr, and returns jerr's
    * address which we place into the link field in cinfo.
    */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */

    /* Here we use the library-supplied code to send compressed data to a
    * stdio stream.  You can also write your own code to do something else.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to write binary files.
    */
    if ((outfile = fopen(strImageName, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", strImageName);
        //exit(1);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */
    /* First we supply a description of the input image.
    * Four fields of the cinfo struct must be filled in:
    */
    cinfo.image_width = image_width;    /* image width and height, in pixels */
    cinfo.image_height = image_height;
    cinfo.input_components = 3;     /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB;     /* colorspace of input image */
    /* Now use the library's routine to set default compression parameters.
    * (You must set at least cinfo.in_color_space before calling this,
    * since the defaults depend on the source color space.)
    */
    jpeg_set_defaults(&cinfo);
    /* Now you can set any non-default parameters you wish to.
    * Here we just illustrate the use of quality (quantization table) scaling:
    */
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */
    /* TRUE ensures that we will write a complete interchange-JPEG file.
    * Pass TRUE unless you are very sure of what you're doing.
    */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */
    /* Here we use the library's state variable cinfo.next_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    * To keep things simple, we pass one scanline per call; you can pass
    * more if you wish, though.
    */
    row_stride = image_width * 3;   /* JSAMPLEs per row in image_buffer */

//    image_buffer[row_stride * cinfo.image_height];
//    image_buffer = malloc(row_stride * cinfo.image_height);
//    memset(image_buffer, 0xff, row_stride * cinfo.image_height);
//    memset(image_buffer, rgbData, row_stride * cinfo.image_height);
//    unsigned char *image_buffer = NULL;
    uint8_t image_buffer[image_width*image_height*3];
    genRGB24Data(image_buffer, image_width, image_height);


    int line = 0;
    //while (cinfo.next_scanline < cinfo.image_height) {
    while (line < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could pass
        * more than one scanline at a time if that's more convenient.
        */
        //row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
        row_pointer[0] = &image_buffer[line * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);

        line++;
    }

    //free(image_buffer);

    /* Step 6: Finish compression */
    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    fclose(outfile);

    /* Step 7: release JPEG compression object */
    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    return 0;
}



int main(int argc, char* argv[])
{
    char *inJpegName = "/Users/hubin/Desktop/WX20200321-193524@2x.jpg";
    char *outRgbName = "/Users/hubin/Desktop/WX20200321-193524@2x.rgb24";
    int flag1 = decode_JPEG_file(inJpegName, outRgbName);
    if (flag1 == 0) {
        printf("decode ok!\n");
    } else {
        printf("decode error!\n");
    }


    int width = 70, height = 70;
    char *strImageName = "/Users/hubin/Desktop/rainbow-rgb23.jpeg";
    int flag2 = encode_JPEG_file(strImageName, width, height, 80);
    if (flag2 == 0)
        printf("encode ok!\n");
    else
        printf("encode error!\n");


    return 0;
}