#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "functions.h"

void start(void) {
	char command[NCOMANDA] = "";
	FILE *image = NULL;
	int pbm_type, width, height;
	coordinates_t select;
	pixel_t **color = NULL;
	unsigned char **grayscale = NULL;

	while (fgets(command, NCOMANDA, stdin)) {
		int nr_command = get_nr_command(command);
		//check if an image was loaded
		//only if the command isn't "LOAD"
		if (nr_command && !image)
			nr_command = -1;
		//free the pixel data before loading another image
		if (!nr_command)
			free_image_data(image, &grayscale, &color, height);
		if (check_invalid(image, nr_command))
			select_command(&image, &grayscale, &color, &width,
						   &height, &select, &pbm_type, command);
	}
	free_image_data(image, &grayscale, &color, height);
	return;
}

unsigned char **load_grayscale(FILE
*image, int width, int height, int pbm_type)
{
	unsigned char **image_data = NULL;
	image_data = calloc(height, sizeof(*image_data));

	if (!image_data) {
		free(image_data);
		perror("Calloc failed");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < height; ++i) {
		image_data[i] = calloc(width, sizeof(*image_data[i]));

		//checks if the row has been allocated in heap
		if (!check_calloc_grayscale(image_data, i))
			exit(EXIT_FAILURE);
	}

	if (pbm_type == 5) {
		for (int i = 0; i < height; ++i)
			fread(image_data[i], sizeof(*image_data[i]), width, image);
	} else {
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				fscanf(image, "%hhu", &image_data[i][j]);
	}

	return image_data;
}

pixel_t **load_color(FILE *image, int width, int height, int pbm_type)
{
	pixel_t **image_data = NULL;
	image_data = calloc(height, sizeof(pixel_t *));

	if (!image_data) {
		free(image_data);
		fprintf(stderr, "Calloc failed\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < height; ++i) {
		image_data[i] = calloc(width, sizeof(pixel_t));
		//checks if the row has been allocated in heap
		if (!check_calloc_color(image_data, i))
			exit(EXIT_FAILURE);
	}

	if (pbm_type == 6) {
		for (int i = 0; i < height; ++i)
			fread(image_data[i], sizeof(pixel_t), width, image);
	} else {
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				fscanf(image, "%hhu", &image_data[i][j].red);
				fscanf(image, "%hhu", &image_data[i][j].green);
				fscanf(image, "%hhu", &image_data[i][j].blue);
			}
	}

	return image_data;
}

FILE *open_file(char file_name[FLEN], int *pbm_type, int *width, int *height)
{
	FILE *image_ptr = NULL;
	char magic_type[3];
	int max_val;
	image_ptr = fopen(file_name, "r");

	if (!image_ptr) {
		fprintf(stdout, "Failed to load %s\n", file_name);
		return NULL;
	}

	fgets(magic_type, 3, image_ptr);
	//skip the newline after the maximum color value with %*c
	fscanf(image_ptr, "%d%d%d%*c", width, height, &max_val);
	*pbm_type = magic_type[1] - '0';

	if (*pbm_type == 5 || *pbm_type == 6) {
		fclose(image_ptr);
		image_ptr = fopen(file_name, "rb");
		if (!image_ptr) {
			printf("Failed to load %s\n", file_name);
			return NULL;
		}
		fgets(magic_type, 3, image_ptr);

		//skip the newline after the maximum color value with %*c
		fscanf(image_ptr, "%d%d%d%*c", width, height, &max_val);
	}
	return image_ptr;
}

void load(FILE *image, pixel_t ***color, unsigned char ***grayscale, char
		  file_name[FLEN], int pbm_type, int width, int height)
{
	if (pbm_type == 2 || pbm_type == 5)
		*grayscale = load_grayscale(image, width, height, pbm_type);

	if (pbm_type == 3 || pbm_type == 6)
		*color = load_color(image, width, height, pbm_type);

	printf("Loaded %s\n", file_name);
}
