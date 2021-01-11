//
// Created by apple on 2020/12/21.
//

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#endif /* _WIN32 */

#include "gif_lib.h"
#include "getarg.h"

#define PROGRAM_NAME	"rgb2gif"

/******************************************************************************
 Load RGB file into internal frame buffer.
******************************************************************************/
static void LoadRGB(char *FileName,
                    GifByteType **RedBuffer,
                    GifByteType **GreenBuffer,
                    GifByteType **BlueBuffer,
                    int Width, int Height)
{
    int i;
    unsigned long Size;
    GifByteType *RedP, *GreenP, *BlueP;
    FILE *rgbfp;

    Size = ((long) Width) * Height * sizeof(GifByteType);

    if ((*RedBuffer = (GifByteType *) malloc((unsigned int) Size)) == NULL ||
        (*GreenBuffer = (GifByteType *) malloc((unsigned int) Size)) == NULL ||
        (*BlueBuffer = (GifByteType *) malloc((unsigned int) Size)) == NULL)
    GIF_EXIT("Failed to allocate memory required, aborted.");

    RedP = *RedBuffer;
    GreenP = *GreenBuffer;
    BlueP = *BlueBuffer;

    if (FileName != NULL) {
        if ((rgbfp = fopen(FileName, "rb")) == NULL)
            GIF_EXIT("Can't open input file name.");
    }
    else {
        GIF_EXIT("File name is NULL.");
    }

    GifQprintf("\n%s: RGB image:     ", PROGRAM_NAME);

    GifByteType *Buffer, *BufferP;

    if ((Buffer = (GifByteType *) malloc(Width * 3)) == NULL)
    GIF_EXIT("Failed to allocate memory required, aborted.");

    for (i = 0; i < Height; i++) {
        int j;
        GifQprintf("\b\b\b\b%-4d", i);
        if (fread(Buffer, Width * 3, 1, rgbfp) != 1)
        GIF_EXIT("Input file(s) terminated prematurly.");
        for (j = 0, BufferP = Buffer; j < Width; j++) {
            *RedP++ = *BufferP++;
            *GreenP++ = *BufferP++;
            *BlueP++ = *BufferP++;
        }
    }

    free((char *) Buffer);
    fclose(rgbfp);
}

/******************************************************************************
 Close output file (if open), and exit.
******************************************************************************/
static void RGB2GIF(char **RGBFileNames, int NumOfRGBFile, char *GIFFileName,
                    int ExpNumOfColors, int Width, int Height)
{
    int ColorMapSize;
    GifByteType *RedBuffer = NULL, *GreenBuffer = NULL, *BlueBuffer = NULL, *OutputBuffer = NULL;
    ColorMapObject *OutputColorMap = NULL;

    // 打开输出的 GIF 文件
    int Error;
    GifFileType *GifFile;
    if ((GifFile = EGifOpenFileName(GIFFileName, false, &Error)) == NULL) {
        PrintGifError(Error);
        exit(EXIT_FAILURE);
    }

    GifFile->SWidth = Width;
    GifFile->SHeight = Height;
    GifFile->SColorResolution = 1;
    GifFile->SBackGroundColor = 0;
    GifFile->SColorMap = NULL;

    for(int i = 0; i < NumOfRGBFile; i++) {
        ColorMapSize = 1 << ExpNumOfColors;
        printf("读取 RGB 文件：%d\n", i);
        LoadRGB(RGBFileNames[i], &RedBuffer, &GreenBuffer, &BlueBuffer, Width, Height);
        if ((OutputColorMap = GifMakeMapObject(ColorMapSize, NULL)) == NULL ||
            (OutputBuffer = (GifByteType *) malloc(Width * Height *
                                                   sizeof(GifByteType))) == NULL)
        GIF_EXIT("Failed to allocate memory required, aborted.");

        if (GifQuantizeBuffer(Width, Height, &ColorMapSize,
                              RedBuffer, GreenBuffer, BlueBuffer,
                              OutputBuffer, OutputColorMap->Colors) == GIF_ERROR)
            exit(EXIT_FAILURE);
        free((char *) RedBuffer);
        free((char *) GreenBuffer);
        free((char *) BlueBuffer);

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

        GraphicsControlBlock *GCB = (GraphicsControlBlock *) malloc(sizeof(GraphicsControlBlock));
        GCB->DisposalMode = DISPOSAL_UNSPECIFIED;
        GCB->DelayTime = 50;
        GCB->UserInputFlag = false;
        GCB->TransparentColor = NO_TRANSPARENT_COLOR;

        printf("GCBToSaved：%d\n", i);
        EGifGCBToSavedExtension(GCB, GifFile, i);
    }

    printf("输出文件。");
    // 输出文件
    EGifSpew(GifFile);

}

int main(int argc, char **argv) {
    char *rgbFiles[] = {
            "/Users/hubin/Desktop/rainbow-0.rgb",
            "/Users/hubin/Desktop/rainbow-1.rgb",
            "/Users/hubin/Desktop/rainbow-2.rgb",
            "/Users/hubin/Desktop/rainbow-3.rgb",
            "/Users/hubin/Desktop/rainbow-4.rgb",
            "/Users/hubin/Desktop/rainbow-5.rgb",
            "/Users/hubin/Desktop/rainbow-6.rgb",
    };

    RGB2GIF(rgbFiles, 7, "/Users/hubin/Desktop/rainbow0.gif", 3, 700, 700);
    return 0;
}

