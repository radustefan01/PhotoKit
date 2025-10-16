#include <stdio.h>
#include "defines.h"
#include "functions.h"

int find_max(int frequency[MAX_PIXEL], int max_nr)
{
	int max = -1;
	for (int i = 0; i < MAX_PIXEL; i += max_nr) {
		int bin_freq = 0;
		for (int j = i; j < i + max_nr; ++j) {
			bin_freq += frequency[j];
		}
		if (bin_freq > max)
			max = bin_freq;
	}
	return max;
}

void histogram(unsigned char ***grayscale, int width,
			   int height, int max_stars, int max_bins)
{
	int frequency[MAX_PIXEL] = {0};
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			frequency[(*grayscale)[i][j]]++;

	//number of values from a bin
	int max_nr = MAX_PIXEL / max_bins;

	//the maximum frequency
	int max_freq = find_max(frequency, max_nr);

	for (int i = 0; i < MAX_PIXEL; i += max_nr) {
		//number of values from the current bin
		double bin_freq = 0;
		for (int j = i; j < i + max_nr; ++j) {
			bin_freq += frequency[j];
		}
		//number of stars we need to print
		int nr_stars = (bin_freq / max_freq) * max_stars;
		printf("%d\t|\t", nr_stars);
		for (int j = 0; j < nr_stars; ++j)
			printf("*");
		printf("\n");
	}
}
