#include <stdlib.h>
#include "callbacks.h"
#include "app_state.h"
#include "image_utils.h"

static void on_load_dialog_response(GtkNativeDialog *dialog, int response_id, gpointer user_data);
static void on_save_dialog_response(GtkNativeDialog *dialog, int response_id, gpointer user_data);

void on_load_button_clicked(GtkWidget *widget, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;
	GtkFileChooserNative *native;

	(void)widget; //suppress unused parameter warning

	native = gtk_file_chooser_native_new("Open Image",
										   GTK_WINDOW(app_state->window),
										   GTK_FILE_CHOOSER_ACTION_OPEN,
										   "_Open", "_Cancel");
	g_signal_connect(native, "response", G_CALLBACK(on_load_dialog_response), app_state);
	gtk_native_dialog_show(GTK_NATIVE_DIALOG(native));
}

void on_save_button_clicked(GtkWidget *widget, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;
	GtkFileChooserNative *native;

	(void)widget;

	//if no image is loaded, do nothing
	if (!app_state->color_data && !app_state->grayscale_data)
		return;

	native = gtk_file_chooser_native_new("Save Image As...",
										   GTK_WINDOW(app_state->window),
										   GTK_FILE_CHOOSER_ACTION_SAVE,
										   "_Save", "_Cancel");
	g_signal_connect(native, "response", G_CALLBACK(on_save_dialog_response), app_state);
	gtk_native_dialog_show(GTK_NATIVE_DIALOG(native));
}

void on_rotate_clicked(GtkWidget *widget, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;

	(void)widget;

	if (!app_state->color_data && !app_state->grayscale_data)
		return;

	//call the core logic function
	rotate(&app_state->color_data, &app_state->grayscale_data, &app_state->selection, 90,
		   app_state->pbm_type, &app_state->width, &app_state->height);
	
	//update the display to show the result
	update_image_display(app_state);
}

void on_apply_filter_clicked(GtkWidget *widget, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;
	guint index;
	GtkStringList *string_list;
	const char *kernel_name;

	(void)widget;

	//apply only works on color images per function prototype
	if (!app_state->color_data) {
		g_print("Apply filter only works on color images.\n");
		return;
	}

	//get selected filter name from dropdown
	index = gtk_drop_down_get_selected(GTK_DROP_DOWN(app_state->filter_dropdown));
	g_object_get(app_state->filter_dropdown, "model", &string_list, NULL);
	kernel_name = gtk_string_list_get_string(string_list, index);

	//call the core logic function
	apply((char *)kernel_name, &app_state->color_data, app_state->selection, app_state->height, app_state->width);
	
	//update the display to show the result
	update_image_display(app_state);
}

void on_equalize_clicked(GtkWidget *widget, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;

	(void)widget;

	//equalize only works on grayscale images
	if (!app_state->grayscale_data) {
		g_print("Equalize only works on grayscale images.\n");
		return;
	}
	
	//call the core logic function
	equalize(&app_state->grayscale_data, app_state->width, app_state->height);

	//update the display to show the result
	update_image_display(app_state);
}

void on_crop_clicked(GtkWidget *widget, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;
	int x1, y1, x2, y2;

	(void)widget;

	if (!app_state->color_data && !app_state->grayscale_data)
		return;

	//read coordinates from entry boxes
	x1 = atoi(gtk_editable_get_text(GTK_EDITABLE(app_state->crop_x1_entry)));
	y1 = atoi(gtk_editable_get_text(GTK_EDITABLE(app_state->crop_y1_entry)));
	x2 = atoi(gtk_editable_get_text(GTK_EDITABLE(app_state->crop_x2_entry)));
	y2 = atoi(gtk_editable_get_text(GTK_EDITABLE(app_state->crop_y2_entry)));

	//basic validation
	if (x1 >= x2 || y1 >= y2) {
		g_print("Invalid crop coordinates.\n");
		return;
	}

	//update selection in AppState
	app_state->selection.start_x = x1;
	app_state->selection.start_y = y1;
	app_state->selection.end_x = x2;
	app_state->selection.end_y = y2;

	//call the core logic function
	crop(&app_state->color_data, &app_state->grayscale_data, app_state->selection,
		 app_state->pbm_type, &app_state->width, &app_state->height);

	//update the display to show the result
	update_image_display(app_state);
}

void on_histogram_clicked(GtkWidget *widget, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;
	int stars, bins;

	(void)widget;

	//histogram only works on grayscale images
	if (!app_state->grayscale_data) {
		g_print("Histogram only works on grayscale images.\n");
		return;
	}

	//read parameters from entry boxes
	stars = atoi(gtk_editable_get_text(GTK_EDITABLE(app_state->hist_stars_entry)));
	bins = atoi(gtk_editable_get_text(GTK_EDITABLE(app_state->hist_bins_entry)));

	if (stars <= 0 || bins <= 0) {
		g_print("Invalid histogram parameters.\n");
		return;
	}

	//function prints directly to console
	g_print("\n--- Histogram ---\n");
	histogram(&app_state->grayscale_data, app_state->width, app_state->height, stars, bins);
	g_print("--- End Histogram ---\n");
}

static void on_load_dialog_response(GtkNativeDialog *dialog, int response_id, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;
	GFile *file;
	char *file_path;
	FILE *image_file;

	//if the user clicked "Accept"
	if (response_id == GTK_RESPONSE_ACCEPT) {
		file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));
		file_path = g_file_get_path(file);

		//free any previously loaded image data
		free_image_data(NULL, &app_state->grayscale_data, &app_state->color_data, app_state->height);

		//use core functions to open and load the image
		image_file = open_file(file_path, &app_state->pbm_type, &app_state->width, &app_state->height);
		if (image_file) {
			load(image_file, &app_state->color_data, &app_state->grayscale_data, file_path,
				 app_state->pbm_type, app_state->width, app_state->height);
			
			//select all by default when loading
			select_all(&app_state->selection, app_state->width, app_state->height);

			//update the display after loading
			update_image_display(app_state);
		}
		g_free(file_path);
		g_object_unref(file);
	}
	g_object_unref(dialog);
}

static void on_save_dialog_response(GtkNativeDialog *dialog, int response_id, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;
	GFile *file;
	char *file_path;

	if (response_id == GTK_RESPONSE_ACCEPT) {
		file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));
		file_path = g_file_get_path(file);

		//save as ascii by default
		save(app_state->grayscale_data, app_state->color_data, app_state->pbm_type,
			 app_state->width, app_state->height, file_path, "ascii");

		g_free(file_path);
		g_object_unref(file);
	}
	g_object_unref(dialog);
}

