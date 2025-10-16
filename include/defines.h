//Vasile Radu-Stefan, 314CA
#ifndef DEFINES
#define DEFINES

#define NCOMANDA 100
#define FLEN 1000
#define TEXT 0
#define BINARY 1
#define NOFILE -1
#define NKERNEL 3
#define MAX_PIXEL 256

//data type for the RGB channels of a pixel
typedef struct {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} pixel_t;

//data type for the coordinates returned by the select function
typedef struct {
	int start_x;
	int start_y;
	int end_x;
	int end_y;
} coordinates_t;

#endif
