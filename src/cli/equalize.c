#include <math.h>
#include "defines.h"
#include "functions.h"

void equalize(unsigned char ***grayscale, int width, int height)
{
	int frequency[MAX_PIXEL] = {0};
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			frequency[(*grayscale)[i][j]]++;

	int area = width * height;

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j) {
			int sum = 0;
			for (int k = 0; k <= (*grayscale)[i][j]; ++k)
				sum += frequency[k];
			double new_value = MAX_PIXEL - 1;
			new_value /= area;
			new_value = new_value * sum;

			(*grayscale)[i][j] = clamp(round(new_value));
		}

	printf("Equalize done\n");
}
