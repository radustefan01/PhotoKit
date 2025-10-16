#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "functions.h"

int check_angle(int angle)
{
	if (angle % 90 != 0 || angle < -360 || angle > 360) {
		printf("Unsupported rotation angle\n");
		return 0;
	}
	return 1;
}

int check_square(coordinates_t select)
{
	int select_width = select.end_x - select.start_x;
	int select_height = select.end_y - select.start_y;
	if (select_width != select_height) {
		printf("The selection must be square\n");
		return 0;
	}
	return 1;
}

void rotate_select(pixel_t ***color, unsigned char
	 ***grayscale, coordinates_t select, int angle, int pbm_type)
{
	int select_width = select.end_x - select.start_x;
	int select_height = select.end_y - select.start_y;
	int original_angle = angle;
	if (check_angle(angle) && check_square(select)) {
		if (angle < 0)
			angle += 360;
		//we will only have these values: {0, 90, 180, 270}
		angle %= 360;
		if (!angle) {
			printf("Rotated %d\n", original_angle);
			//nothing changes
			return;
		}
		if (pbm_type == 2 || pbm_type == 5) {
			unsigned char **tmp = NULL;
			tmp = calloc(select_height, sizeof(*tmp));
			for (int i = 0; i < select_height; ++i)
				tmp[i] = calloc(select_width, sizeof(**tmp));
			for (int i = 0; i < select_height; ++i)
				for (int j = 0; j < select_width; ++j) {
					int i_image = i + select.start_y,
						j_image = j + select.start_x;
					tmp[i][j] = (*grayscale)[i_image][j_image];
				}
			for (int i = 0; i < select_height; ++i)
				for (int j = 0; j < select_width; ++j) {
					int new_x, new_y;
					if (angle == 90) {
						new_y = select.start_y + j;
						new_x = select.start_x + select_width - 1 - i;
					} else if (angle == 180) {
						new_y = select.start_y + select_height - 1 - i;
						new_x  = select.start_x + select_width - 1 - j;
					} else if (angle == 270) {
						new_y = select.start_y + select_height - 1 - j;
						new_x = select.start_x + i;
					}
					(*grayscale)[new_y][new_x] = tmp[i][j];
				}
			for (int i = 0; i < select_height; ++i)
				free(tmp[i]);
			free(tmp);
		} else {
			pixel_t **tmp = NULL;
			tmp = calloc(select_height, sizeof(pixel_t *));
			for (int i = 0; i < select_height; ++i)
				tmp[i] = calloc(select_width, sizeof(pixel_t));

			for (int i = 0; i < select_height; ++i)
				for (int j = 0; j < select_width; ++j) {
					int i_image = i + select.start_y,
						j_image = j + select.start_x;
					tmp[i][j] = (*color)[i_image][j_image];
				}

			for (int i = 0; i < select_height; ++i)
				for (int j = 0; j < select_width; ++j) {
					int new_x = 0, new_y = 0;
					if (angle == 90) {
						new_y = select.start_y + j;
						new_x = select.start_x + select_width - 1 - i;
					} else if (angle == 180) {
						new_y = select.start_y + select_height - 1 - i;
						new_x  = select.start_x + select_width - 1 - j;
					} else if (angle == 270) {
						new_y = select.start_y + select_height - 1 - j;
						new_x = select.start_x + i;
					}
					(*color)[new_y][new_x] = tmp[i][j];
				}

			for (int i = 0; i < select_height; ++i)
				free(tmp[i]);
			free(tmp);
		}
		printf("Rotated %d\n", original_angle);
	}
}

void rotate_full(pixel_t ***color, unsigned char ***grayscale,
				 int angle, int pbm_type, int *width, int *height)
{
	int original_angle = angle;
	if (check_angle(angle)) {
		if (angle < 0)
			angle += 360;
		//we will only have these values: {0, 90, 180, 270}
		angle %= 360;
		if (!angle) {
			printf("Rotated %d\n", original_angle);
			//nothing changes
			return;
		}
		int new_width = (angle == 90 || angle == 270) ? *height : *width;
		int new_height = (angle == 90 || angle == 270) ? *width : *height;

		if (pbm_type == 2 || pbm_type == 5) {
			unsigned char **rotated = NULL;
			rotated =  calloc(new_height, sizeof(*rotated));
			for (int i = 0; i < new_height; ++i)
				rotated[i] = calloc(new_width, sizeof(**rotated));

			for (int i = 0; i < *height; ++i)
				for (int j = 0; j < *width; ++j) {
					int new_x = 0, new_y = 0;
					if (angle == 90) {
						new_y = j;
						new_x = new_width - 1 - i;
					} else if (angle == 180) {
						new_y = new_height - 1 - i;
						new_x  = new_width - 1 - j;
					} else if (angle == 270) {
						new_y = new_height - 1 - j;
						new_x = i;
					}
					rotated[new_y][new_x] = (*grayscale)[i][j];
				}
			//free the old pixel data
			for (int i = 0; i < *height; ++i)
				free((*grayscale)[i]);
			free(*grayscale);

			*grayscale = rotated;
		} else {

			pixel_t **rotated = calloc(new_height, sizeof(pixel_t *));
			for (int i = 0; i < new_height; ++i)
				rotated[i] = calloc(new_width, sizeof(pixel_t));

			for (int i = 0; i < *height; ++i)
				for (int j = 0; j < *width; ++j) {
					int new_x = 0, new_y = 0;
					if (angle == 90) {
						new_y = j;
						new_x = new_width - 1 - i;
					} else if (angle == 180) {
						new_y = new_height - 1 - i;
						new_x  = new_width - 1 - j;
					} else if (angle == 270) {
						new_y = new_height - 1 - j;
						new_x = i;
					}

					rotated[new_y][new_x] = (*color)[i][j];
				}
			//free the old pixel data
			for (int i = 0; i < *height; ++i)
				free((*color)[i]);
			free(*color);

			*color = rotated;
		}

		*width = new_width;
		*height = new_height;

		printf("Rotated %d\n", original_angle);
	}
}

void rotate(pixel_t ***color, unsigned char ***grayscale, coordinates_t *select,
			int angle, int pbm_type, int *width, int *height)
{
	int select_height = select->end_y - select->start_y;
	int select_width = select->end_x - select->start_x;
	//rotate the entire image
	if (select_height == *height && select_width == *width) {
		rotate_full(color, grayscale, angle, pbm_type, width, height);
		select_all(select, *width, *height);
	}
	//rotate a part of it
	else
		rotate_select(color, grayscale, *select, angle, pbm_type);
}
