/**
 * Abdulwahab Alkharashi
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include <MaxSLiCInterface.h>

#include "ppmIO.h"

int main(void)
{
	printf("Loading image.\n");
	int32_t *the_image;
	int width = 0, height = 0;
	loadImage("lena.ppm", &the_image, &width, &height, 1);
	int dataSize = width * height * sizeof(int);
	int32_t *out_image = malloc(dataSize);
	printf("Running Kernel.\n");
	Mean(width * height, the_image, out_image);
	printf("Saving image.\n");
	writeImage("lena_mean.ppm", out_image, width, height, 1);
	printf("Exiting\n");
	return 0;
}
