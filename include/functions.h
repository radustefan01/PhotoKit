//Vasile Radu-Stefan, 314CA
#ifndef VECTOR
#define VECTOR

#include <stdio.h>
#include <stdlib.h>
#include "defines.h"

//coordinates.c

int check_coordinates(coordinates_t *select, int width, int height);

void select_all(coordinates_t *select, int width, int height);

//files.c

void start(void);

void load(FILE *image, pixel_t ***color, unsigned char ***grayscale, char
		  file_name[FLEN], int pbm_type, int width, int height);

FILE *open_file(char file_name[FLEN], int *pbm_type, int *width, int *height);

//save.c

void save(unsigned char **grayscale, pixel_t **color, int pbm_type, int width,
		  int height, char save_file_name[FLEN], char save_type[6]);

//memory.c

int check_calloc_grayscale(unsigned char **image_data, int n);

int check_calloc_color(pixel_t **image_data, int n);

void free_image_data(FILE *image, unsigned char
	   ***grayscale, pixel_t ***color, int height);

//command.c

int check_grayscale(int pbm_type);

int check_invalid(FILE *image, int nr_command);

void select_command(FILE **image, unsigned char ***grayscale, pixel_t
		  ***color, int *width, int *height,
		  coordinates_t *select, int *pbm_type, char command[NCOMANDA]);

void del_spaces(char command[NCOMANDA]);

int get_nr_command(char comanda[NCOMANDA]);

//crop.c

void crop(pixel_t ***color, unsigned char ***grayscale,
		  coordinates_t select, int pbm_type, int *width, int *height);

//apply.c

unsigned char clamp(double nr);

int get_kernel_type(char kernel_name[NCOMANDA]);

void apply(char kernel_name[NCOMANDA],
		   pixel_t ***color, coordinates_t select,
		   int height, int width);

void apply_kernel(double matrix[NKERNEL][NKERNEL], pixel_t
		***color, coordinates_t select, int height, int width);

void init_edge(double matrix[NKERNEL][NKERNEL]);

void init_sharpen(double matrix[NKERNEL][NKERNEL]);

void init_blur(double matrix[NKERNEL][NKERNEL]);

void init_gaussian_blur(double matrix[NKERNEL][NKERNEL]);

//histogram.c

void histogram(unsigned char ***grayscale, int
		width, int height, int max_stars, int max_bins);

int find_max(int frequency[256], int max_nr);

//equalize.c

void equalize(unsigned char ***grayscale, int width, int height);

//rotate.c

void rotate_select(pixel_t ***color, unsigned char
	 ***grayscale, coordinates_t select, int angle, int pbm_type);

void rotate_full(pixel_t ***color, unsigned char ***grayscale,
				 int angle, int pbm_type, int *width, int *height);

void rotate(pixel_t ***color, unsigned char ***grayscale, coordinates_t
	*select, int angle, int pbm_type, int *width, int *height);

#endif
