#include <stdio.h>
#include "defines.h"

int check_number(int nr, int max)
{
	if (nr < 0 || nr > max)
		return 0;
	return 1;
}

int check_coordinates(coordinates_t *select, int width, int height)
{
	if (!check_number(select->start_x, width) ||
		!check_number(select->end_x, width) ||
		!check_number(select->start_y, height) ||
		!check_number(select->end_y, height)) {
		printf("Invalid set of coordinates\n");
		return 0;
	}
	if (select->start_x == select->end_x ||
		select->start_y == select->end_y) {
		printf("Invalid set of coordinates\n");
		return 0;
		}
	//interchange the values if needed
	if (select->start_x > select->end_x) {
		int tmp = select->end_x;
		select->end_x = select->start_x;
		select->start_x = tmp;
	}
	if (select->start_y > select->end_y) {
		int tmp = select->end_y;
		select->end_y = select->start_y;
		select->start_y = tmp;
	}
	printf("Selected %d %d %d %d\n",
		   select->start_x,
		   select->start_y,
		   select->end_x,
		   select->end_y);
	return 1;
}

void select_all(coordinates_t *select, int width, int height)
{
	(*select).start_x = 0;
	(*select).end_x = width;
	(*select).start_y = 0;
	(*select).end_y = height;
}
