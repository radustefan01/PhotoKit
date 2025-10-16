#include <stdio.h>
#include <string.h>
#include "defines.h"

void save(unsigned char **grayscale, pixel_t **color, int
pbm_type, int width, int height, char save_file_name[FLEN], char save_type[6])
{
	int save_pbm_type;

	if (strlen(save_type)) {

		FILE *image = fopen(save_file_name, "w");

		if (pbm_type == 2 || pbm_type == 5) {
			save_pbm_type = 2;

			fprintf(image, "P%d\n%d %d\n255\n", save_pbm_type, width, height);
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j)
					fprintf(image, "%hhu ", grayscale[i][j]);
			}
		} else if (pbm_type == 3 || pbm_type == 6) {
			save_pbm_type = 3;

			fprintf(image, "P%d\n%d %d\n255\n", save_pbm_type, width, height);
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j)
					fprintf(image, "%hhu %hhu %hhu ",
							color[i][j].red,
							color[i][j].green,
							color[i][j].blue);
			}
		}

		if (image) {
			fclose(image);
			image = NULL;
		}

	} else {

		FILE *image = fopen(save_file_name, "wb");

		if (pbm_type == 2 || pbm_type == 5) {
			save_pbm_type = 5;

			fprintf(image, "P%d\n%d %d\n255\n", save_pbm_type, width, height);

			for (int i = 0; i < height; ++i)
				fwrite(grayscale[i], sizeof(*grayscale[i]), width, image);

		} else if (pbm_type == 3 || pbm_type == 6) {
			save_pbm_type = 6;

			fprintf(image, "P%d\n%d %d\n255\n", save_pbm_type, width, height);

			for (int i = 0; i < height; ++i)
				fwrite(color[i], sizeof(*color[i]), width, image);
		}

		if (image) {
			fclose(image);
			image = NULL;
		}

	}

	printf("Saved %s\n", save_file_name);
}
