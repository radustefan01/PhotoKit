//Vasile Radu-Stefan, 314CA
#ifndef APP_STATE_H
#define APP_STATE_H

#include <gtk/gtk.h>
#include "functions.h" // From the core logic

// This struct holds all the application's data, making it accessible
// to every callback function.
typedef struct {
    // Image data
    pixel_t **color_data;
    unsigned char **grayscale_data;
    int width;
    int height;
    int pbm_type;
    coordinates_t selection;

    // Pointers to important GTK widgets
    GtkWidget *window;
    GtkWidget *image_display; // The widget that will show the image
    GtkWidget *filter_dropdown;
    GtkWidget *crop_x1_entry, *crop_y1_entry, *crop_x2_entry, *crop_y2_entry;
    GtkWidget *hist_stars_entry, *hist_bins_entry;

} app_state_t;

#endif // APP_STATE_H