//
// Created by apple on 2020/12/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "libbmp.h"
#include "png.h"
#include "jpeglib.h"
#include "gif_lib.h"
#include "getarg.h"

int decodeBMP(char *, unsigned char **);
int decodePNG(char *, unsigned char **);
int decodeJPG(char *, unsigned char **);
int decodeGIF(char *, unsigned char **);
int encodeGIF(unsigned char **RGBBuffers, int NumOfRGBBuffer, char *GIFFileName,
              int ExpNumOfColors, int Width, int Height);
int AppendExtension(const ExtensionBlock *ExtensionBlock,
                    GifFileType *GifFile, int ImageIndex);

// https://gist.github.com/niw/5963798
int main() {
    char *bmp = "/Users/hubin/Desktop/blog-20/Android.bmp";
    char *png = "/Users/hubin/Desktop/blog-20/Huawei.png";
    char *jpg = "/Users/hubin/Desktop/blog-20/Fuchsia.jpg";
    char *gif = "/Users/hubin/Desktop/blog-20/iOS.gif";

    unsigned char *bmpRGB = NULL;
    unsigned char *pngRGB = NULL;
    unsigned char *jpgRGB = NULL;
    unsigned char *gifRGB = NULL;

    printf("开始解码 BMP 文件！\n");
    decodeBMP(bmp, &bmpRGB);
    char *androidRGB = "/Users/hubin/Desktop/blog-20/Android.rgb";
    FILE *androidRGBFile = fopen(androidRGB, "wb");
    fwrite(bmpRGB, 400*400*3, 1, androidRGBFile);
    fclose(androidRGBFile);
    printf("\n\n");

    printf("开始解码 PNG 文件！\n");
    decodePNG(png, &pngRGB);
    char *huaweiRGB = "/Users/hubin/Desktop/blog-20/Huawei.rgb";
    FILE *huaweiRGBFile = fopen(huaweiRGB, "wb");
    fwrite(pngRGB, 400*400*3, 1, huaweiRGBFile);
    fclose(huaweiRGBFile);
    printf("\n\n");

    printf("开始解码 JPG 文件！\n");
    decodeJPG(jpg, &jpgRGB);
    char *fuchsiaRGB = "/Users/hubin/Desktop/blog-20/Fuchsia.rgb";
    FILE *fuchsiaRGBFile = fopen(fuchsiaRGB, "wb");
    fwrite(jpgRGB, 400*400*3, 1, fuchsiaRGBFile);
    fclose(fuchsiaRGBFile);
    printf("\n\n");

    printf("开始解码 GIF 文件！\n");
    decodeGIF(gif, &gifRGB);
    char *iOSRGB = "/Users/hubin/Desktop/blog-20/iOS.rgb";
    FILE *iOSRGBFile = fopen(iOSRGB, "wb");
    fwrite(gifRGB, 400*400*3, 1, iOSRGBFile);
    fclose(iOSRGBFile);
    printf("\n\n");

    unsigned char **rgbBuffers = malloc(4*sizeof(unsigned char *));
    rgbBuffers[0] = bmpRGB;
    rgbBuffers[1] = pngRGB;
    rgbBuffers[2] = jpgRGB;
    rgbBuffers[3] = gifRGB;
    encodeGIF(rgbBuffers, 4, "/Users/hubin/Desktop/image-to-gif.gif",
              7, 400, 400);


    free(bmpRGB);
    free(pngRGB);
    free(jpgRGB);
    free(gifRGB);

//     // PNG to BMP
//    char *png1 = "/Users/hubin/Desktop/Android.png";
//    decodePNG(png1, &pngRGB);
//    int width = 400, height = 400;
//    bmp_img img;
//    bmp_img_init_df (&img, width, height);
//    for (int i = 0; i < height; ++i) {
//        for (int j = 0; j < width; ++j) {
//            unsigned char R = pngRGB[i*width*3+j*3];
//            unsigned char G = pngRGB[i*width*3+j*3+1];
//            unsigned char B = pngRGB[i*width*3+j*3+2];
//            bmp_pixel_init (&img.img_pixels[i][j], R, G, B);
//        }
//    }
//    bmp_img_write (&img, "/Users/hubin/Desktop/Android.bmp");
//    bmp_img_free (&img);
    return 0;
}


int decodeBMP(char *filename, unsigned char **bmpRGB) {
    bmp_img img;
    bmp_img_read(&img, filename);

    int width = img.img_header.biWidth;
    int height = img.img_header.biHeight;
    printf("Size: [%d, %d]\n", width, height);
    printf("BitCount: %d\n", img.img_header.biBitCount);
    printf("Compression: %d\n", img.img_header.biCompression);
    printf("SizeImage: %d\n", img.img_header.biSizeImage);

    *bmpRGB = malloc(width * height * 3);
    int x, y;
    unsigned char *BufferP;
    for (x = 0 ; x < height ; x++) {
        bmp_pixel *row = img.img_pixels[x];
        for (y = 0, BufferP = *bmpRGB+width*3*x; y < width; y++) {
            /* Get pixel's RGB values */
            bmp_pixel pixel = row[y];
            *BufferP++ = pixel.red;
            *BufferP++ = pixel.green;
            *BufferP++ = pixel.blue;
        }
    }
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


int decodeJPG(char *filename, unsigned char **jpgRGB) {
    FILE *fp = fopen(filename, "rb");

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, fp);

    jpeg_read_header(&cinfo, TRUE);

    printf("image_width = %d\n", cinfo.image_width);
    printf("image_height = %d\n", cinfo.image_height);
    printf("num_components = %d\n", cinfo.num_components);

    printf("enter scale M/N:\n");

//    cinfo.out_color_space = JCS_YCbCr;
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

    *jpgRGB = malloc(row_stride*cinfo.image_height);
    long counter = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(*jpgRGB + counter, buffer[0], row_stride);
        counter += row_stride;
    }
    printf("total size: %ld\n", counter);

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    return 0;
}


int decodeGIF(char *filename, unsigned char **gifRGB) {
    int	i, j, Size, Row, Col, Width, Height, ExtCode, Count;
    GifRecordType RecordType;
    GifByteType *Extension;
    GifRowType *ScreenBuffer;
    GifFileType *GifFile;
    int InterlacedOffset[] = { 0, 4, 2, 1 }, /* The way Interlaced image should. */
        InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */
    int ImageNum = 0;
    ColorMapObject *ColorMap;
    int Error;

    if ((GifFile = DGifOpenFileName(filename, &Error)) == NULL) {
        printf("Open File Error.\n");
        return -1;
    }

    if (GifFile->SHeight == 0 || GifFile->SWidth == 0) {
        printf("Image of width or height 0\n");
        return -1;
    }

    /*
     * Allocate the screen as vector of column of rows. Note this
     * screen is device independent - it's the screen defined by the
     * GIF file parameters.
     */
    if ((ScreenBuffer = (GifRowType *)
            malloc(GifFile->SHeight * sizeof(GifRowType))) == NULL) {
        printf("Failed to allocate memory required, aborted.\n");
        return -1;
    }

    Size = GifFile->SWidth * sizeof(GifPixelType);/* Size in bytes one row.*/
    if ((ScreenBuffer[0] = (GifRowType) malloc(Size)) == NULL) { /* First row. */
        printf("Failed to allocate memory required, aborted.\n");
        return -1;
    }

    for (i = 0; i < GifFile->SWidth; i++)  /* Set its color to BackGround. */
        ScreenBuffer[0][i] = GifFile->SBackGroundColor;
    for (i = 1; i < GifFile->SHeight; i++) {
        /* Allocate the other rows, and set their color to background too: */
        if ((ScreenBuffer[i] = (GifRowType) malloc(Size)) == NULL) {
            printf("Failed to allocate memory required, aborted.\n");
            return -1;
        }
        memcpy(ScreenBuffer[i], ScreenBuffer[0], Size);
    }

    int screenIndex = 0;
    /* Scan the content of the GIF file and load the image(s) in: */
    do {
        if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR) {
            printf("DGifGetRecordType Error.\n");
            return -1;
        }
        switch (RecordType) {
            case IMAGE_DESC_RECORD_TYPE:
                if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
                    printf("DGifGetImageDesc Error.\n");
                    return -1;
                }
                Row = GifFile->Image.Top; /* Image Position relative to Screen. */
                Col = GifFile->Image.Left;
                Width = GifFile->Image.Width;
                Height = GifFile->Image.Height;
                if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
                    GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) {
                    printf("Image %d is not confined to screen dimension, aborted.\n",ImageNum);
                    return -1;
                }
                if (GifFile->Image.Interlace) {
                    /* Need to perform 4 passes on the images: */
                    for (Count = i = 0; i < 4; i++)
                        for (j = Row + InterlacedOffset[i]; j < Row + Height;
                             j += InterlacedJumps[i]) {
                            if (DGifGetLine(GifFile, &ScreenBuffer[j][Col],
                                            Width) == GIF_ERROR) {
                                printf("DGifGetLine Error.\n");
                                return -1;
                            }
                        }
                }
                else {
                    for (i = 0; i < Height; i++) {
                        if (DGifGetLine(GifFile, &ScreenBuffer[Row++][Col],
                                        Width) == GIF_ERROR) {
                            printf("DGifGetLine Error.\n");
                            return -1;
                        }
                    }
                }

                /* Lets dump it - set the global variables required and do it: */
                ColorMap = (GifFile->Image.ColorMap
                            ? GifFile->Image.ColorMap
                            : GifFile->SColorMap);
                if (ColorMap == NULL) {
                    fprintf(stderr, "Gif Image does not have a colormap\n");
                    exit(EXIT_FAILURE);
                }

                /* check that the background color isn't garbage (SF bug #87) */
                if (GifFile->SBackGroundColor < 0 || GifFile->SBackGroundColor >= ColorMap->ColorCount) {
                    printf("Background color out of range for colormap\n");
                    return -1;
                }

                GifRowType GifRow;
                GifColorType *ColorMapEntry;
                unsigned char *BufferP;

                *gifRGB = malloc(GifFile->SWidth*GifFile->SHeight*3);

                for (i = 0; i <  GifFile->SHeight; i++) {
                    GifRow = ScreenBuffer[i];
                    for (j = 0, BufferP = *gifRGB+GifFile->SWidth*3*i; j < GifFile->SWidth; j++) {
                        ColorMapEntry = &ColorMap->Colors[GifRow[j]];
                        *BufferP++ = ColorMapEntry->Red;
                        *BufferP++ = ColorMapEntry->Green;
                        *BufferP++ = ColorMapEntry->Blue;
                    }
                }

                break;
            case EXTENSION_RECORD_TYPE:
                /* Skip any extension blocks in file: */
                if (DGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR) {
                    printf("DGifGetExtension Error.\n");
                    return -1;
                }
                while (Extension != NULL) {
                    if (DGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR) {
                        printf("DGifGetExtensionNext Error.\n");
                        return -1;
                    }
                }
                break;
            case TERMINATE_RECORD_TYPE:
                break;
            default:		    /* Should be trapped by DGifGetRecordType. */
                break;
        }
    } while (RecordType != TERMINATE_RECORD_TYPE);

    (void)free(ScreenBuffer);

    if (DGifCloseFile(GifFile, &Error) == GIF_ERROR) {
        printf("DGifCloseFile Error.\n");
        return -1;
    }
    return 0;
}

int encodeGIF(unsigned char **RGBBuffers, int NumOfRGBBuffer, char *GIFFileName,
             int ExpNumOfColors, int Width, int Height) {
    int ColorMapSize;
    GifByteType *RedBuffer = NULL, *GreenBuffer = NULL, *BlueBuffer = NULL, *OutputBuffer = NULL;
    ColorMapObject *OutputColorMap = NULL;

    // 打开输出的 GIF 文件
    int Error;
    GifFileType *GifFile;
    if ((GifFile = EGifOpenFileName(GIFFileName, false, &Error)) == NULL) {
        PrintGifError(Error);
        printf("EGifOpenFileName Error.\n");
        return -1;
    }

    GifFile->SWidth = Width;
    GifFile->SHeight = Height;
    GifFile->SColorResolution = 1;
    GifFile->SBackGroundColor = 0;
    GifFile->SColorMap = NULL;

    unsigned long Size;
    GifByteType *RedP, *GreenP, *BlueP;
    GifByteType *Buffer, *BufferP;

    Size = ((long) Width) * Height * sizeof(GifByteType);
    if ((RedBuffer = (GifByteType *) malloc((unsigned int) Size)) == NULL ||
        (GreenBuffer = (GifByteType *) malloc((unsigned int) Size)) == NULL ||
        (BlueBuffer = (GifByteType *) malloc((unsigned int) Size)) == NULL) {
        return -1;
    }
    if ((Buffer = (GifByteType *) malloc(Width * 3)) == NULL) {
        return -1;
    }

    for(int i = 0; i < NumOfRGBBuffer; i++) {
        ColorMapSize = 1 << ExpNumOfColors;
        RedP = RedBuffer;
        GreenP = GreenBuffer;
        BlueP = BlueBuffer;
        int pointer = 0;
        for (int j = 0; j < Height; j++) {
            int k;
            memcpy(Buffer, RGBBuffers[i]+pointer, Width * 3);
            pointer+=Width*3;
            for (k = 0, BufferP = Buffer; k < Width; k++) {
                *RedP++ = *BufferP++;
                *GreenP++ = *BufferP++;
                *BlueP++ = *BufferP++;
            }
        }
        if ((OutputColorMap = GifMakeMapObject(ColorMapSize, NULL)) == NULL ||
            (OutputBuffer = (GifByteType *) malloc(Width * Height *
                                                   sizeof(GifByteType))) == NULL) {
            printf("Failed to allocate memory required, aborted.\n");
            return -1;
        }

        if (GifQuantizeBuffer(Width, Height, &ColorMapSize,
                              RedBuffer, GreenBuffer, BlueBuffer,
                              OutputBuffer, OutputColorMap->Colors) == GIF_ERROR) {
            printf("GifQuantizeBuffer Error.\n");
            return -1;
        }

        printf("MakeSavedImage：%d\n", i);
        SavedImage *image = GifMakeSavedImage(GifFile, NULL);

        GifImageDesc *imageDesc = (GifImageDesc *) malloc(sizeof(GifImageDesc));
        imageDesc->Left = 0;
        imageDesc->Top = 0;
        imageDesc->Width = Width;
        imageDesc->Height = Height;
        imageDesc->Interlace = false;
        imageDesc->ColorMap = OutputColorMap;

        image->ImageDesc = *imageDesc;
        image->RasterBits = OutputBuffer;

//        // 添加 Application Extension 控制 GIF 循环
//        if(i == 0) {
//            printf("添加 Application Extension\n");
//            uint8_t Extension[] = {0x4E, 0x45, 0x54, 0x53, 0x43, 0x41, 0x50, 0x45, 0x32, 0x2E, 0x30, 0x03, 0x01, 0x00, 0x00};
//
//            ExtensionBlock *ExtensionBlock = malloc(sizeof(ExtensionBlock));
//            ExtensionBlock->Function = APPLICATION_EXT_FUNC_CODE;
//            ExtensionBlock->ByteCount = 15;
//            ExtensionBlock->Bytes = Extension;
//
//            if(AppendExtension(
//                    ExtensionBlock,
//                    GifFile,
//                    i) == GIF_ERROR) {
//                printf("添加 Application Extension 失败\n");
//                return -1;
//            }
//        }

        GraphicsControlBlock *GCB = (GraphicsControlBlock *) malloc(sizeof(GraphicsControlBlock));
        GCB->DisposalMode = DISPOSAL_UNSPECIFIED;
        GCB->DelayTime = 100;
        GCB->UserInputFlag = false;
        GCB->TransparentColor = NO_TRANSPARENT_COLOR;

        printf("GCBToSaved：%d\n", i);
        EGifGCBToSavedExtension(GCB, GifFile, i);
    }
    free((char *) RedBuffer);
    free((char *) GreenBuffer);
    free((char *) BlueBuffer);

    printf("输出 GIF 文件。\n");
    // 输出文件
    EGifSpew(GifFile);
    return 0;
}

int AppendExtension(const ExtensionBlock *ExtensionBlock,
                            GifFileType *GifFile, int ImageIndex)
{
    GifByteType buf[sizeof(GraphicsControlBlock)]; /* a bit dodgy... */

    if (ImageIndex < 0 || ImageIndex > GifFile->ImageCount - 1)
        return GIF_ERROR;

    if (GifAddExtensionBlock(&GifFile->SavedImages[ImageIndex].ExtensionBlockCount,
                             &GifFile->SavedImages[ImageIndex].ExtensionBlocks,
                             ExtensionBlock->Function,
                             ExtensionBlock->ByteCount,
                             ExtensionBlock->Bytes) == GIF_ERROR)
        return (GIF_ERROR);

    return (GIF_OK);
}