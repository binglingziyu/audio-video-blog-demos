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

#define PROGRAM_NAME	"gif2rgb"

/******************************************************************************
 The real screen dumping routine.
******************************************************************************/
static void DumpScreen2RGB(char *FileName, int OneFileFlag,
                           ColorMapObject *ColorMap,
                           GifRowType *ScreenBuffer,
                           int ScreenWidth, int ScreenHeight)
{
    int i, j;
    GifRowType GifRow;
    GifColorType *ColorMapEntry;
    FILE *rgbfp[3];

    if (FileName != NULL) {
        if (OneFileFlag) {
            if ((rgbfp[0] = fopen(FileName, "a+")) == NULL)
            GIF_EXIT("Can't open input file name.");
        } else {
            static char *Postfixes[] = { ".R", ".G", ".B" };
            char OneFileName[80];

            for (i = 0; i < 3; i++) {
                strncpy(OneFileName, FileName, sizeof(OneFileName)-1);
                strncat(OneFileName, Postfixes[i],
                        sizeof(OneFileName) - 1 - strlen(OneFileName));

                if ((rgbfp[i] = fopen(OneFileName, "wb")) == NULL) {
                    GIF_EXIT("Can't open input file name.");
                }
            }
        }
    } else {
        OneFileFlag = true;

#ifdef _WIN32
        _setmode(1, O_BINARY);
#endif /* _WIN32 */

        rgbfp[0] = stdout;
    }

    if (ColorMap == NULL) {
        fprintf(stderr, "Color map pointer is NULL.\n");
        exit(EXIT_FAILURE);
    }

    if (OneFileFlag) {
        unsigned char *Buffer, *BufferP;

        if ((Buffer = (unsigned char *) malloc(ScreenWidth * 3)) == NULL)
        GIF_EXIT("Failed to allocate memory required, aborted.");
        for(int k = 0; k < 1; k++) {
            for (i = 0; i < ScreenHeight; i++) {
                GifRow = ScreenBuffer[i];
                GifQprintf("\b\b\b\b%-4d", ScreenHeight - i);
                for (j = 0, BufferP = Buffer; j < ScreenWidth; j++) {
                    ColorMapEntry = &ColorMap->Colors[GifRow[j]];
                    *BufferP++ = ColorMapEntry->Red;
                    *BufferP++ = ColorMapEntry->Green;
                    *BufferP++ = ColorMapEntry->Blue;
                }
                if (fwrite(Buffer, ScreenWidth * 3, 1, rgbfp[0]) != 1)
                GIF_EXIT("Write to file(s) failed.");
            }
        }

        free((char *) Buffer);
        fclose(rgbfp[0]);
    } else {
        unsigned char *Buffers[3];

        if ((Buffers[0] = (unsigned char *) malloc(ScreenWidth)) == NULL ||
            (Buffers[1] = (unsigned char *) malloc(ScreenWidth)) == NULL ||
            (Buffers[2] = (unsigned char *) malloc(ScreenWidth)) == NULL)
        GIF_EXIT("Failed to allocate memory required, aborted.");

        for (i = 0; i < ScreenHeight; i++) {
            GifRow = ScreenBuffer[i];
            GifQprintf("\b\b\b\b%-4d", ScreenHeight - i);
            for (j = 0; j < ScreenWidth; j++) {
                ColorMapEntry = &ColorMap->Colors[GifRow[j]];
                Buffers[0][j] = ColorMapEntry->Red;
                Buffers[1][j] = ColorMapEntry->Green;
                Buffers[2][j] = ColorMapEntry->Blue;
            }
            if (fwrite(Buffers[0], ScreenWidth, 1, rgbfp[0]) != 1 ||
                fwrite(Buffers[1], ScreenWidth, 1, rgbfp[1]) != 1 ||
                fwrite(Buffers[2], ScreenWidth, 1, rgbfp[2]) != 1)
            GIF_EXIT("Write to file(s) failed.");
        }

        free((char *) Buffers[0]);
        free((char *) Buffers[1]);
        free((char *) Buffers[2]);
        fclose(rgbfp[0]);
        fclose(rgbfp[1]);
        fclose(rgbfp[2]);
    }
}

static void GIF2RGB(int NumFiles, char *FileName,
                    bool OneFileFlag,
                    char *OutFileName)
{
    int	i, j, Size, Row, Col, Width, Height, ExtCode, Count;
    GifRecordType RecordType;
    GifByteType *Extension;
    GifRowType *ScreenBuffer;
    GifFileType *GifFile;
    int
            InterlacedOffset[] = { 0, 4, 2, 1 }, /* The way Interlaced image should. */
    InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */
    int ImageNum = 0;
    ColorMapObject *ColorMap;
    int Error;

    if (NumFiles == 1) {
        int Error;
        if ((GifFile = DGifOpenFileName(FileName, &Error)) == NULL) {
            PrintGifError(Error);
            exit(EXIT_FAILURE);
        }
    }
    else {
        int Error;
        /* Use stdin instead: */
        if ((GifFile = DGifOpenFileHandle(0, &Error)) == NULL) {
            PrintGifError(Error);
            exit(EXIT_FAILURE);
        }
    }

    if (GifFile->SHeight == 0 || GifFile->SWidth == 0) {
        fprintf(stderr, "Image of width or height 0\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Allocate the screen as vector of column of rows. Note this
     * screen is device independent - it's the screen defined by the
     * GIF file parameters.
     */
    if ((ScreenBuffer = (GifRowType *)
            malloc(GifFile->SHeight * sizeof(GifRowType))) == NULL)
    GIF_EXIT("Failed to allocate memory required, aborted.");

    Size = GifFile->SWidth * sizeof(GifPixelType);/* Size in bytes one row.*/
    if ((ScreenBuffer[0] = (GifRowType) malloc(Size)) == NULL) /* First row. */
    GIF_EXIT("Failed to allocate memory required, aborted.");

    for (i = 0; i < GifFile->SWidth; i++)  /* Set its color to BackGround. */
        ScreenBuffer[0][i] = GifFile->SBackGroundColor;
    for (i = 1; i < GifFile->SHeight; i++) {
        /* Allocate the other rows, and set their color to background too: */
        if ((ScreenBuffer[i] = (GifRowType) malloc(Size)) == NULL)
        GIF_EXIT("Failed to allocate memory required, aborted.");

        memcpy(ScreenBuffer[i], ScreenBuffer[0], Size);
    }

    int screenIndex = 0;
    /* Scan the content of the GIF file and load the image(s) in: */
    do {
        if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR) {
            PrintGifError(GifFile->Error);
            exit(EXIT_FAILURE);
        }
        switch (RecordType) {
            case IMAGE_DESC_RECORD_TYPE:
                if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
                    PrintGifError(GifFile->Error);
                    exit(EXIT_FAILURE);
                }
                Row = GifFile->Image.Top; /* Image Position relative to Screen. */
                Col = GifFile->Image.Left;
                Width = GifFile->Image.Width;
                Height = GifFile->Image.Height;
                GifQprintf("\n%s: Image %d at (%d, %d) [%dx%d]:     ",
                           PROGRAM_NAME, ++ImageNum, Col, Row, Width, Height);
                if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
                    GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) {
                    fprintf(stderr, "Image %d is not confined to screen dimension, aborted.\n",ImageNum);
                    exit(EXIT_FAILURE);
                }
                if (GifFile->Image.Interlace) {
                    /* Need to perform 4 passes on the images: */
                    for (Count = i = 0; i < 4; i++)
                        for (j = Row + InterlacedOffset[i]; j < Row + Height;
                             j += InterlacedJumps[i]) {
                            GifQprintf("\b\b\b\b%-4d", Count++);
                            if (DGifGetLine(GifFile, &ScreenBuffer[j][Col],
                                            Width) == GIF_ERROR) {
                                PrintGifError(GifFile->Error);
                                exit(EXIT_FAILURE);
                            }
                        }
                }
                else {
                    for (i = 0; i < Height; i++) {
                        GifQprintf("\b\b\b\b%-4d", i);
                        if (DGifGetLine(GifFile, &ScreenBuffer[Row++][Col],
                                        Width) == GIF_ERROR) {
                            PrintGifError(GifFile->Error);
                            exit(EXIT_FAILURE);
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
                    fprintf(stderr, "Background color out of range for colormap\n");
                    exit(EXIT_FAILURE);
                }

//                char *name = malloc(255*sizeof(char));
//                char *n1 = "/Users/apple/Desktop/rainbow";
//                char *n2 = ".rgb";
//                sprintf(name, "%s-%d%s", n1, screenIndex++, n2);
//                printf("Final File Name: %s\n", name);

//                DumpScreen2RGB(OutFileName, OneFileFlag,
//                               ColorMap,
//                               ScreenBuffer,
//                               GifFile->SWidth, GifFile->SHeight);
                break;
            case EXTENSION_RECORD_TYPE:
                /* Skip any extension blocks in file: */
                if (DGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR) {
                    PrintGifError(GifFile->Error);
                    exit(EXIT_FAILURE);
                }
                while (Extension != NULL) {
                    if (DGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR) {
                        PrintGifError(GifFile->Error);
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case TERMINATE_RECORD_TYPE:
                break;
            default:		    /* Should be trapped by DGifGetRecordType. */
                break;
        }
    } while (RecordType != TERMINATE_RECORD_TYPE);

    char* name = "/Users/apple/Desktop/rainbow0.rgb";
    DumpScreen2RGB(name, OneFileFlag,
                   ColorMap,
                   ScreenBuffer,
                   GifFile->SWidth, GifFile->SHeight);

    (void)free(ScreenBuffer);

    if (DGifCloseFile(GifFile, &Error) == GIF_ERROR) {
        PrintGifError(Error);
        exit(EXIT_FAILURE);
    }

}


int main(int argc, char **argv) {
    GIF2RGB(1, "/Users/apple/Desktop/rainbow.gif", true, "/Users/apple/Desktop/rainbow.rgb");
    return 0;
}

