/**************************************************************

	The program reads an 8BPP BMP image file and creates a
	new image showing the input file's palette's colors in a
	16x16 grid.

**************************************************************/

#include <stdio.h>
#include <memory.h>

#include "qdbmp.h"


#define	PALETTE_SIZE		256						/* Number of colors in an 8 bit palette */
#define OUTPUT_WIDTH		512						/* Width of output image (pixels) */
#define OUTPUT_HEIGHT		512						/* Height of output image (pixels) */
#define	RECTANGLE_WIDTH		( OUTPUT_WIDTH / 16 )	/* Width of a color rectangle in the output image */
#define	RECTANGLE_HEIGHT	( OUTPUT_HEIGHT / 16 )	/* Height of a color rectangle in the output image */


int main( int argc, char* argv[] )
{
	BMP*	input;
	BMP*	output;
	UINT 	x, y;
	UCHAR 	r, g, b;
	int		i;

	/* Check arguments */
	if ( argc != 3 )
	{
		fprintf( stderr, "Usage: %s <input file> <output file>\n", argv[ 0 ] );
		return -1;
	}


	/* Open input BMP */
	input = BMP_ReadFile( argv[ 1 ] );
	BMP_CHECK_ERROR( stderr, -2 );		/* Check if any errors have occurred */


	/* Input image's depth must be 8 bits per pixel */
	if ( BMP_GetDepth( input ) != 8 )
	{
		fprintf( stderr, "ERROR: Input image's depth must be 8 bits per pixel.\n" );
		return -1;
	}


	/* Create a new image from scratch. This is the output image that
	 * will contain an illustration of the input image's palette.
	 * The new BMP is also 8 BPP. */
	output = BMP_Create( OUTPUT_WIDTH, OUTPUT_HEIGHT, 8 );
	BMP_CHECK_ERROR( stderr, -3 );


	/* The new image's palette is a copy of the input image's palette */
	for ( i = 0 ; i < PALETTE_SIZE ; ++i )
	{
		BMP_GetPaletteColor( input, i, &r, &g, &b );

		BMP_SetPaletteColor( output, i, r, g, b );
	}


	/* The input image is no longer required and may be freed */
	BMP_Free( input );


	/* Render the output image. For each palette color, draw a rectangle. */
	for ( i = 0 ; i < PALETTE_SIZE ; ++i )
	{
		for ( x = ( i % 16 ) * RECTANGLE_WIDTH ; x < ( i % 16 ) * RECTANGLE_WIDTH + RECTANGLE_WIDTH ; ++x )
		{
			for ( y = ( i / 16 ) * RECTANGLE_HEIGHT ; y < ( i / 16 ) * RECTANGLE_HEIGHT + RECTANGLE_HEIGHT ; ++y )
			{
				BMP_SetPixelIndex( output, x, y, i );
			}
		}
	}


	/* Save output image */
	BMP_WriteFile( output, argv[ 2 ] );
	BMP_CHECK_ERROR( stderr, -5 );


	/* Free output image memory */
	BMP_Free( output );


	return 0;
}
