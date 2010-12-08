/**
 * Linux x86
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * This program converts a monochrome bitmap with format 128 x 64 to an array
 * of data that can be used to display on a GLCD as in example displayBitmap.c
 * See http://en.wikipedia.org/wiki/BMP_file_format for info on the BMP header.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    int charRow, pixRow, pixCol, charCol;
    unsigned int rgb;
    FILE *bitmap;
    char dump[8][128] = {};

    // open bitmap
    bitmap = fopen("image.bmp", "r");
    if (bitmap == NULL) {
        printf("File \"image.bmp\" could not be read!\n");
        return (EXIT_FAILURE);
    }

    // skip bitmap header
    for (pixCol = 0; pixCol < 62; pixCol++) {
        getc(bitmap);
    }

    // initialize counters
    charRow = 0;   // step +1 = character row, there are 8 rows
    pixRow  = 128; // step /2 = pixel row in character, there are 8 pixels
    pixCol  = 0;   // step +8 = pixel column, there are 128 columns per charRow
    charCol = 7;   // step -1 = character column, there are 8 columns per pixCol

    // read the entire bitmap, every read is 8 bits
    rgb = getc(bitmap);
    while (rgb != EOF, charRow < 8) {
        for (charCol = 7; charCol >= 0; charCol--) {
            dump[charRow][pixCol + 7 - charCol] +=
                (rgb & (int) pow(2, charCol)) == pow(2, charCol) ? pixRow : 0;
        }

        // change counters to position to the pixels in result set
        pixCol += 8;
        if (pixCol >= 128) {
            pixRow /= 2;
            pixCol = 0;
        }
        if (pixRow < 1) {
            pixRow = 128;
            charRow++;
        }

        // read next
        rgb = getc(bitmap);
    }

    // print results as C code, format code with breaks and spaces
    printf("    const code char image[1024] = {");
    for (charRow = 7; charRow >= 0; charRow--) {
        for (pixCol = 0; pixCol < 128; pixCol++) {
            if (pixCol % 14 == 0) {
                printf("\n        ");
            }
            printf("%d, ", 255 - dump[charRow][pixCol]);
        }
    }
    printf("\n    };\n");

    return (EXIT_SUCCESS);
}

