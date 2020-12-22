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


static void LoadRGB(char *FileName,
                    int OneFileFlag,
                    GifByteType **RedBuffer,
                    GifByteType **GreenBuffer,
                    GifByteType **BlueBuffer,
                    int Width, int Height);
static void SaveGif(GifByteType *OutputBuffer,
                    int Width, int Height,
                    int ExpColorMapSize, ColorMapObject *OutputColorMap);

/******************************************************************************
 Load RGB file into internal frame buffer.
******************************************************************************/
static void LoadRGB(char *FileName,
                    int OneFileFlag,
                    GifByteType **RedBuffer,
                    GifByteType **GreenBuffer,
                    GifByteType **BlueBuffer,
                    int Width, int Height)
{
    int i;
    unsigned long Size;
    GifByteType *RedP, *GreenP, *BlueP;
    FILE *rgbfp[3];

    Size = ((long) Width) * Height * sizeof(GifByteType);

    if ((*RedBuffer = (GifByteType *) malloc((unsigned int) Size)) == NULL ||
        (*GreenBuffer = (GifByteType *) malloc((unsigned int) Size)) == NULL ||
        (*BlueBuffer = (GifByteType *) malloc((unsigned int) Size)) == NULL)
    GIF_EXIT("Failed to allocate memory required, aborted.");

    RedP = *RedBuffer;
    GreenP = *GreenBuffer;
    BlueP = *BlueBuffer;

    if (FileName != NULL) {
        if (OneFileFlag) {
            if ((rgbfp[0] = fopen(FileName, "rb")) == NULL)
            GIF_EXIT("Can't open input file name.");
        }
        else {
            static char *Postfixes[] = { ".R", ".G", ".B" };
            char OneFileName[80];

            for (i = 0; i < 3; i++) {
                strncpy(OneFileName, FileName, sizeof(OneFileName)-1);
                strncat(OneFileName, Postfixes[i],
                        sizeof(OneFileName) - 1 - strlen(OneFileName));

                if ((rgbfp[i] = fopen(OneFileName, "rb")) == NULL)
                GIF_EXIT("Can't open input file name.");
            }
        }
    }
    else {
        OneFileFlag = true;

#ifdef _WIN32
        _setmode(0, O_BINARY);
#endif /* _WIN32 */

        rgbfp[0] = stdin;
    }

    GifQprintf("\n%s: RGB image:     ", PROGRAM_NAME);

    if (OneFileFlag) {
        GifByteType *Buffer, *BufferP;

        if ((Buffer = (GifByteType *) malloc(Width * 3)) == NULL)
        GIF_EXIT("Failed to allocate memory required, aborted.");

        for (i = 0; i < Height; i++) {
            int j;
            GifQprintf("\b\b\b\b%-4d", i);
            if (fread(Buffer, Width * 3, 1, rgbfp[0]) != 1)
            GIF_EXIT("Input file(s) terminated prematurly.");
            for (j = 0, BufferP = Buffer; j < Width; j++) {
                *RedP++ = *BufferP++;
                *GreenP++ = *BufferP++;
                *BlueP++ = *BufferP++;
            }
        }

        free((char *) Buffer);
        fclose(rgbfp[0]);
    }
    else {
        for (i = 0; i < Height; i++) {
            GifQprintf("\b\b\b\b%-4d", i);
            if (fread(RedP, Width, 1, rgbfp[0]) != 1 ||
                fread(GreenP, Width, 1, rgbfp[1]) != 1 ||
                fread(BlueP, Width, 1, rgbfp[2]) != 1)
            GIF_EXIT("Input file(s) terminated prematurly.");
            RedP += Width;
            GreenP += Width;
            BlueP += Width;
        }

        fclose(rgbfp[0]);
        fclose(rgbfp[1]);
        fclose(rgbfp[2]);
    }
}

/******************************************************************************
 Save the GIF resulting image.
******************************************************************************/
static void SaveGif(GifByteType *OutputBuffer,
                    int Width, int Height,
                    int ExpColorMapSize, ColorMapObject *OutputColorMap)
{
    int i, Error;
    GifFileType *GifFile;
    GifByteType *Ptr = OutputBuffer;

    /* Open stdout for the output file: */
//    if ((GifFile = EGifOpenFileHandle(1, &Error)) == NULL) {
//        PrintGifError(Error);
//        exit(EXIT_FAILURE);
//    }

    if ((GifFile = EGifOpenFileName("/Users/apple/Desktop/rainbow0.gif", true, &Error)) == NULL) {
        PrintGifError(Error);
        exit(EXIT_FAILURE);
    }

    if (EGifPutScreenDesc(GifFile,
                          Width, Height, ExpColorMapSize, 0,
                          OutputColorMap) == GIF_ERROR ||
        EGifPutImageDesc(GifFile,
                         0, 0, Width, Height, false, NULL) == GIF_ERROR) {
        PrintGifError(Error);
        exit(EXIT_FAILURE);
    }

    GifQprintf("\n%s: Image 1 at (%d, %d) [%dx%d]:     ",
               PROGRAM_NAME, GifFile->Image.Left, GifFile->Image.Top,
               GifFile->Image.Width, GifFile->Image.Height);

    for (i = 0; i < Height; i++) {
        if (EGifPutLine(GifFile, Ptr, Width) == GIF_ERROR)
            exit(EXIT_FAILURE);
        GifQprintf("\b\b\b\b%-4d", Height - i - 1);

        Ptr += Width;
    }

    if (EGifCloseFile(GifFile, &Error) == GIF_ERROR) {
        PrintGifError(Error);
        exit(EXIT_FAILURE);
    }
}

/******************************************************************************
 Close output file (if open), and exit.
******************************************************************************/
static void RGB2GIF(bool OneFileFlag, int NumFiles, char *FileName,
                    int ExpNumOfColors, int Width, int Height)
{
    int ColorMapSize;

    GifByteType *RedBuffer = NULL, *GreenBuffer = NULL, *BlueBuffer = NULL,
            *OutputBuffer = NULL;
    ColorMapObject *OutputColorMap = NULL;

    ColorMapSize = 1 << ExpNumOfColors;

    if (NumFiles == 1) {
        LoadRGB(FileName, OneFileFlag,
                &RedBuffer, &GreenBuffer, &BlueBuffer, Width, Height);
    }
    else {
        LoadRGB(NULL, OneFileFlag,
                &RedBuffer, &GreenBuffer, &BlueBuffer, Width, Height);
    }

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

    printf("保存 GIF。\n");
    SaveGif(OutputBuffer, Width, Height, ExpNumOfColors, OutputColorMap);
}

int main(int argc, char **argv) {

    RGB2GIF(true, 1, "/Users/apple/Desktop/rainbow0.rgb", 7, 700, 700);
    return 0;
}

