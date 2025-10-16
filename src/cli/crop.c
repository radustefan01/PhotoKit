#include <stdio.h>
#include <stdlib.h>
#include "defines.h"

void crop(pixel_t ***color, unsigned char ***grayscale, coordinates_t select,
		  int pbm_type, int *width, int *height)
{
	int new_height = select.end_y - select.start_y,
		new_width = select.end_x - select.start_x;

	if (pbm_type == 2 || pbm_type == 5) {
		unsigned char **new_grayscale = NULL;
		new_grayscale = calloc(new_height, sizeof(*new_grayscale));
		for (int i = 0; i < new_height; ++i)
			new_grayscale[i] = calloc(new_width, sizeof(**new_grayscale));

		for (int i = 0; i < new_height; ++i)
			for (int j = 0; j < new_width; ++j) {
				int i_image = i + select.start_y, j_image = j + select.start_x;
				new_grayscale[i][j] = (*grayscale)[i_image][j_image];
			}

		for (int i = 0; i < *height; ++i)
			free((*grayscale)[i]);
		free(*grayscale);
		*grayscale = new_grayscale;
	} else if (pbm_type == 3 || pbm_type == 6) {
		pixel_t **new_color = NULL;
		new_color = calloc(new_height, sizeof(pixel_t *));
		for (int i = 0; i < new_height; ++i)
			new_color[i] = calloc(new_width, sizeof(pixel_t **));

		for (int i = 0; i < new_height; ++i)
			for (int j = 0; j < new_width; ++j) {
				int i_image = i + select.start_y, j_image = j + select.start_x;
				new_color[i][j].red = (*color)[i_image][j_image].red;
				new_color[i][j].green = (*color)[i_image][j_image].green;
				new_color[i][j].blue = (*color)[i_image][j_image].blue;
			}

		for (int i = 0; i < *height; ++i)
			free((*color)[i]);
		free(*color);
		*color = new_color;
	}

	*width = new_width;
	*height = new_height;

	printf("Image cropped\n");
}
