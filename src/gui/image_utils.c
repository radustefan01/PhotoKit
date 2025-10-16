#include "image_utils.h"

void update_image_display(app_state_t *app_state)
{
	int width, height, channels;
	guchar *pixels;
	int i, j, pos;
	GdkPixbuf *pixbuf;

	//if no image is loaded, do nothing
	if (!app_state->color_data && !app_state->grayscale_data)
		return;

	width = app_state->width;
	height = app_state->height;
	channels = 3;

	//GdkPixbuf requires a flat 1D array of pixel data
	pixels = g_malloc(width * height * channels);

	//convert 2D data into the flat 1D array
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			pos = (i * width + j) * channels;
			//if it's a color image
			if (app_state->color_data) {
				pixels[pos]     = app_state->color_data[i][j].red;
				pixels[pos + 1] = app_state->color_data[i][j].green;
				pixels[pos + 2] = app_state->color_data[i][j].blue;
			} else {
				//if it's a grayscale image, duplicate the value for R, G, and B
				guchar val = app_state->grayscale_data[i][j];
				pixels[pos]     = val;
				pixels[pos + 1] = val;
				pixels[pos + 2] = val;
			}
		}
	}

	//create a GdkPixbuf from the raw pixel data
	pixbuf = gdk_pixbuf_new_from_data(pixels, GDK_COLORSPACE_RGB, FALSE, 8,
											width, height, width * channels,
											(GdkPixbufDestroyNotify)g_free, NULL);

	//tell our GtkPicture widget to display the new pixbuf
	gtk_picture_set_pixbuf(GTK_PICTURE(app_state->image_display), pixbuf);
	g_object_unref(pixbuf);
}