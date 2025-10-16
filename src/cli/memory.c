#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "functions.h"

//checks if a row has been allocated for grayscale images
int check_calloc_grayscale(unsigned char **image_data, int n)
{
	if (!image_data[n]) {
		perror("Calloc");
		//free previous rows if one row isn't allocated
		for (int i = 0; i < n; ++i)
			free(image_data[i]);
		free(image_data);
		return 0;
	}
	return 1;
}

//checks if a row has been allocated for color images
int check_calloc_color(pixel_t **image_data, int n)
{
	if (!image_data[n]) {
		perror("Calloc");
		//free previous rows if one row isn't allocated
		for (int i = 0; i < n; ++i)
			free(image_data[i]);
		free(image_data);
		return 0;
	}
	return 1;
}

void free_image_data(FILE *image, unsigned char
	   ***grayscale, pixel_t ***color, int height)
{
	if (*grayscale) {
		for (int i = 0; i < height; ++i)
			free((*grayscale)[i]);
		free(*grayscale);
	}

	if (*color) {
		for (int i = 0; i < height; ++i)
			free((*color)[i]);
		free(*color);
	}

	if (image) {
		fclose(image);
		image = NULL;
	}

	*grayscale = NULL;
	*color = NULL;
	image = NULL;
}
