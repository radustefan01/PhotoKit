#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "functions.h"

int check_invalid(FILE *image, int nr_command)
{
	if (nr_command == -1) {
		if (!image)
			printf("No image loaded\n");
		else
			printf("Invalid command\n");
		return 0;
	}
	return 1;
}

//delete spaces at the end of a string
void del_spaces(char command[NCOMANDA])
{
	int let = strlen(command) - 1;
	while (command[let] == ' ') {
		command[let] = '\0';
		let--;
	}
}

//check if the loaded image is grayscale
int check_grayscale(int pbm_type)
{
	if (pbm_type == 3 || pbm_type == 6) {
		printf("Black and white image needed\n");
		return 0;
	}
	return 1;
}

int get_nr_command(char command[NCOMANDA])
{
	int nr_var = 0;

	//remove the newline from the command
	if (command[strlen(command) - 1] == '\n')
		command[strlen(command) - 1] = '\0';

	//delete extra spaces at the end
	del_spaces(command);

	char command_name[NCOMANDA];
	nr_var = sscanf(command, "%s", command_name);

	if (!strcmp(command_name, "LOAD"))
		return 0;
	if (!strcmp(command_name, "SELECT")) {
		if (!strcmp("SELECT ALL", command))
			return 2;
		int x = -1, y = -1, z = -1, k = -1;
		nr_var = sscanf(command, "%*s%d%d%d%d", &x, &y, &z, &k);
		//check if we have the number of parameters we need
		//and if they all are integers
		if (nr_var != 4)
			return -1;
		return 1;
	}
	if (!strcmp(command_name, "ROTATE"))
		return 3;
	if (!strcmp(command_name, "EQUALIZE"))
		return 4;
	if (!strcmp(command_name, "CROP"))
		return 5;
	if (!strcmp(command_name, "APPLY")) {
		//command has no parameter
		if (strlen(command) == 5)
			return -1;
		return 6;
	}
	if (!strcmp(command_name, "SAVE"))
		return 7;
	if (!strcmp(command_name, "HISTOGRAM")) {
		int x = -1, y = -1, z = -1;
		int nr_parameters = sscanf(command, "%*s%d%d%d", &x, &y, &z);
		//check if we have too many parameters
		if (nr_parameters > 2)
			return -1;
		if (x < 0 || y < 0)
			return -1;
		return 8;
	}
	if (!strcmp(command_name, "EXIT"))
		return 9;

	//the command isn't valid
	return -1;
}

void select_command(FILE **image, unsigned char ***grayscale, pixel_t
		  ***color, int *width, int *height, coordinates_t *select,
		  int *pbm_type, char command[NCOMANDA])
{
	int nr_command = get_nr_command(command), read_var = 0;
	switch (nr_command) {
	case 0: {  //LOAD
		char file_name[FLEN] = "";
		//skip over the command name with *
		read_var = sscanf(command, "%*s%s", file_name);
		*image = open_file(file_name, pbm_type, width, height);
		if (*image) {
			load(*image, color, grayscale,
				 file_name, *pbm_type, *width, *height);
			select_all(select, *width, *height); //select all as default
		}
		break;
	}
	case 1: { //SELECT
		coordinates_t select_temp = {-1, -1, -1, -1};
		read_var = sscanf(command, "%*s%d%d%d%d", &select_temp.start_x,
						  &select_temp.start_y, &select_temp.end_x,
						  &select_temp.end_y);
		if (check_coordinates(&select_temp, *width, *height))
			*select = select_temp;
		break;
	}
	case 2: { //SELECT ALL
		select_all(select, *width, *height);
		printf("Selected ALL\n");
		break;
	}
	case 3: { //ROTATE
		int angle;
		read_var = sscanf(command, "%*s%d", &angle);
		if (!read_var)
			return;
		rotate(color, grayscale, select, angle, *pbm_type, width, height);
		break;
	}
	case 4: { //EQUALIZE
		if (check_grayscale(*pbm_type))
			equalize(grayscale, *width, *height);
		break;
	}
	case 5: { //CROP
		crop(color, grayscale, *select, *pbm_type, width, height);
		//select the entire image as default
		select_all(select, *width, *height);
		break;
	}
	case 6: { //APPLY
		char kernel_name[NCOMANDA];
		read_var = sscanf(command, "%*s%s", kernel_name);
		if (*pbm_type == 2 || *pbm_type == 5)
			printf("Easy, Charlie Chaplin\n");
		else
			apply(kernel_name, color, *select, *height, *width);
		break;
	}
	case 7: { //SAVE
		char save_file_name[FLEN] = "", save_type[6] = "";
		read_var = sscanf(command, "%*s%s%s", save_file_name, save_type);
		save(*grayscale, *color, *pbm_type, *width,
			 *height, save_file_name, save_type);
		break;
	}
	case 8: { //HISTOGRAM
		int max_stars, max_bins;
		read_var = sscanf(command, "%*s%d%d", &max_stars, &max_bins);
		if (check_grayscale(*pbm_type))
			histogram(grayscale, *width, *height, max_stars, max_bins);
		break;
	}
	case 9: { //EXIT
		free_image_data(*image, grayscale, color, *height);
		exit(EXIT_SUCCESS);
		break;
	}
	}
}
