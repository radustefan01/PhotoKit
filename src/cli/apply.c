#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "functions.h"

unsigned char clamp(double nr)
{
	if (nr < 0)
		return 0;
	if (nr > 255)
		return 255;
	return (unsigned char)nr;
}

int get_kernel_type(char kernel_name[NCOMANDA])
{
	if (!strcmp(kernel_name, "EDGE"))
		return 0;
	if (!strcmp(kernel_name, "SHARPEN"))
		return 1;
	if (!strcmp(kernel_name, "BLUR"))
		return 2;
	if (!strcmp(kernel_name, "GAUSSIAN_BLUR"))
		return 3;
	return -1;
}

void init_edge(double matrix[NKERNEL][NKERNEL])
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			matrix[i][j] = -1;
	matrix[1][1] = 8;
}

void init_sharpen(double matrix[NKERNEL][NKERNEL])
{
	matrix[0][1] = -1;
	matrix[1][0] = -1;
	matrix[1][2] = -1;
	matrix[2][1] = -1;
	matrix[1][1] = 5;
}

void init_blur(double matrix[NKERNEL][NKERNEL])
{
	double scalar = 1.0 / 9.0;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			matrix[i][j] = scalar;
}

void init_gaussian_blur(double matrix[NKERNEL][NKERNEL])
{
	double scalar = 1.0 / 16.0;
	matrix[0][0] = scalar;
	matrix[0][2] = scalar;
	matrix[2][0] = scalar;
	matrix[2][2] = scalar;
	matrix[0][1] = 2 * scalar;
	matrix[1][0] = 2 * scalar;
	matrix[1][2] = 2 * scalar;
	matrix[2][1] = 2 * scalar;
	matrix[1][1] = 4.0 * scalar;
}

void apply_kernel(double matrix[NKERNEL][NKERNEL], pixel_t
		***color, coordinates_t select, int height, int width)
{
	int select_width = select.end_x - select.start_x;
	int select_height = select.end_y - select.start_y;
	pixel_t **color_temp = NULL;
	color_temp = (pixel_t **)calloc(select_height, sizeof(pixel_t *));
	if (!color_temp) {
		perror("Calloc failed");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < select_height; ++i) {
		color_temp[i] = (pixel_t *)calloc(select_width, sizeof(pixel_t));
		//checks if the row has been allocated in heap
		if (!check_calloc_color(color_temp, i))
			exit(EXIT_FAILURE);
	}
	for (int i = 0; i < select_height; ++i)
		for (int j = 0; j < select_width; ++j) {
			int i_image = i + select.start_y, j_image = j + select.start_x;
			if (i_image == 0 || j_image == 0 ||
				i_image == height - 1 || j_image == width - 1) {
				color_temp[i][j] = (*color)[i_image][j_image];
			} else {
				double sum;
				sum = matrix[0][0] * (*color)[i_image - 1][j_image - 1].red +
					  matrix[0][1] * (*color)[i_image - 1][j_image].red +
					  matrix[0][2] * (*color)[i_image - 1][j_image + 1].red +
					  matrix[1][0] * (*color)[i_image][j_image - 1].red +
					  matrix[1][1] * (*color)[i_image][j_image].red +
					  matrix[1][2] * (*color)[i_image][j_image + 1].red +
					  matrix[2][0] * (*color)[i_image + 1][j_image - 1].red +
					  matrix[2][1] * (*color)[i_image + 1][j_image].red +
					  matrix[2][2] * (*color)[i_image + 1][j_image + 1].red;
				color_temp[i][j].red = clamp(round(sum));

				sum = matrix[0][0] * (*color)[i_image - 1][j_image - 1].green +
					  matrix[0][1] * (*color)[i_image - 1][j_image].green +
					  matrix[0][2] * (*color)[i_image - 1][j_image + 1].green +
					  matrix[1][0] * (*color)[i_image][j_image - 1].green +
					  matrix[1][1] * (*color)[i_image][j_image].green +
					  matrix[1][2] * (*color)[i_image][j_image + 1].green +
					  matrix[2][0] * (*color)[i_image + 1][j_image - 1].green +
					  matrix[2][1] * (*color)[i_image + 1][j_image].green +
					  matrix[2][2] * (*color)[i_image + 1][j_image + 1].green;
				color_temp[i][j].green = clamp(round(sum));

				sum = matrix[0][0] * (*color)[i_image - 1][j_image - 1].blue +
					  matrix[0][1] * (*color)[i_image - 1][j_image].blue +
					  matrix[0][2] * (*color)[i_image - 1][j_image + 1].blue +
					  matrix[1][0] * (*color)[i_image][j_image - 1].blue +
					  matrix[1][1] * (*color)[i_image][j_image].blue +
					  matrix[1][2] * (*color)[i_image][j_image + 1].blue +
					  matrix[2][0] * (*color)[i_image + 1][j_image - 1].blue +
					  matrix[2][1] * (*color)[i_image + 1][j_image].blue +
					  matrix[2][2] * (*color)[i_image + 1][j_image + 1].blue;
				color_temp[i][j].blue = clamp(round(sum));
			}
		}
	for (int i = 0; i < select_height; ++i)
		for (int j = 0; j < select_width; ++j) {
			//the coordinates from the image for
			//(i, j) coordinates of selected zone
			int i_image = i + select.start_y, j_image = j + select.start_x;
			(*color)[i_image][j_image] = color_temp[i][j];
		}
	for (int i = 0; i < select_height; ++i)
		free(color_temp[i]);
	free(color_temp);
}

void apply(char kernel_name[NCOMANDA],
		   pixel_t ***color, coordinates_t select, int height, int width)
{
	double matrix[NKERNEL][NKERNEL] = {0};
	int kernel_type = get_kernel_type(kernel_name);

	//initialize the kernel matrix
	switch (kernel_type) {
	case 0:
		init_edge(matrix);
		break;
	case 1:
		init_sharpen(matrix);
		break;
	case 2:
		init_blur(matrix);
		break;
	case 3:
		init_gaussian_blur(matrix);
		break;
	}

	if (kernel_type != -1) {
		apply_kernel(matrix, color, select, height, width);
		printf("APPLY %s done\n", kernel_name);
	} else
		printf("APPLY parameter invalid\n");
}
