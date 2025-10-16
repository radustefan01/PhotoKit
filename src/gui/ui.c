#include "ui.h"
#include "app_state.h"
#include "callbacks.h"

void activate(GtkApplication *app, gpointer user_data)
{
	app_state_t *app_state = (app_state_t *)user_data;
	GtkWidget *main_box, *top_controls_box, *bottom_controls_box;
	GtkWidget *load_button, *save_button, *rotate_button, *equalize_button;
	GtkWidget *apply_button, *crop_button, *hist_button;
	GtkWidget *crop_box, *hist_box;
	const char *filters[] = {"EDGE", "SHARPEN", "BLUR", "GAUSSIAN_BLUR", NULL};

	//create main window and layout
	app_state->window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(app_state->window), "Image Editor");

	// Set window icon
	GError *error = NULL;
	gtk_window_set_icon_from_file(GTK_WINDOW(app_state->window), "data/PhotoKit.jpeg", &error);
	if (error) {
		g_warning("Could not set window icon: %s", error->message);
		g_error_free(error);
	}

	gtk_window_set_default_size(GTK_WINDOW(app_state->window), 800, 700);

	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_window_set_child(GTK_WINDOW(app_state->window), main_box);

	//create image display area
	app_state->image_display = gtk_picture_new();
	gtk_widget_set_hexpand(app_state->image_display, TRUE);
	gtk_widget_set_vexpand(app_state->image_display, TRUE);
	gtk_box_append(GTK_BOX(main_box), app_state->image_display);

	//create control sections
	top_controls_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_widget_set_halign(top_controls_box, GTK_ALIGN_CENTER);
	gtk_box_append(GTK_BOX(main_box), top_controls_box);

	bottom_controls_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_widget_set_halign(bottom_controls_box, GTK_ALIGN_CENTER);
	gtk_box_append(GTK_BOX(main_box), bottom_controls_box);

	//basic operations
	load_button = gtk_button_new_with_label("Load Image");
	save_button = gtk_button_new_with_label("Save As...");
	rotate_button = gtk_button_new_with_label("Rotate 90");
	equalize_button = gtk_button_new_with_label("Equalize");
	gtk_box_append(GTK_BOX(top_controls_box), load_button);
	gtk_box_append(GTK_BOX(top_controls_box), save_button);
	gtk_box_append(GTK_BOX(top_controls_box), rotate_button);
	gtk_box_append(GTK_BOX(top_controls_box), equalize_button);

	//apply filter section
	app_state->filter_dropdown = gtk_drop_down_new_from_strings(filters);
	apply_button = gtk_button_new_with_label("Apply Filter");
	gtk_box_append(GTK_BOX(top_controls_box), app_state->filter_dropdown);
	gtk_box_append(GTK_BOX(top_controls_box), apply_button);

	//crop section
	crop_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	app_state->crop_x1_entry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(app_state->crop_x1_entry), "x1"); gtk_widget_set_size_request(app_state->crop_x1_entry, 40, -1);
	app_state->crop_y1_entry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(app_state->crop_y1_entry), "y1"); gtk_widget_set_size_request(app_state->crop_y1_entry, 40, -1);
	app_state->crop_x2_entry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(app_state->crop_x2_entry), "x2"); gtk_widget_set_size_request(app_state->crop_x2_entry, 40, -1);
	app_state->crop_y2_entry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(app_state->crop_y2_entry), "y2"); gtk_widget_set_size_request(app_state->crop_y2_entry, 40, -1);
	crop_button = gtk_button_new_with_label("Crop");
	gtk_box_append(GTK_BOX(bottom_controls_box), gtk_label_new("Crop:"));
	gtk_box_append(GTK_BOX(crop_box), app_state->crop_x1_entry);
	gtk_box_append(GTK_BOX(crop_box), app_state->crop_y1_entry);
	gtk_box_append(GTK_BOX(crop_box), app_state->crop_x2_entry);
	gtk_box_append(GTK_BOX(crop_box), app_state->crop_y2_entry);
	gtk_box_append(GTK_BOX(bottom_controls_box), crop_box);
	gtk_box_append(GTK_BOX(bottom_controls_box), crop_button);

	//histogram section
	hist_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	app_state->hist_stars_entry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(app_state->hist_stars_entry), "stars"); gtk_widget_set_size_request(app_state->hist_stars_entry, 50, -1);
	app_state->hist_bins_entry = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(app_state->hist_bins_entry), "bins"); gtk_widget_set_size_request(app_state->hist_bins_entry, 50, -1);
	hist_button = gtk_button_new_with_label("Show Histogram");
	gtk_box_append(GTK_BOX(bottom_controls_box), gtk_label_new("Hist:"));
	gtk_box_append(GTK_BOX(hist_box), app_state->hist_stars_entry);
	gtk_box_append(GTK_BOX(hist_box), app_state->hist_bins_entry);
	gtk_box_append(GTK_BOX(bottom_controls_box), hist_box);
	gtk_box_append(GTK_BOX(bottom_controls_box), hist_button);

	//connect signals
	g_signal_connect(load_button, "clicked", G_CALLBACK(on_load_button_clicked), app_state);
	g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_button_clicked), app_state);
	g_signal_connect(rotate_button, "clicked", G_CALLBACK(on_rotate_clicked), app_state);
	g_signal_connect(equalize_button, "clicked", G_CALLBACK(on_equalize_clicked), app_state);
	g_signal_connect(apply_button, "clicked", G_CALLBACK(on_apply_filter_clicked), app_state);
	g_signal_connect(crop_button, "clicked", G_CALLBACK(on_crop_clicked), app_state);
	g_signal_connect(hist_button, "clicked", G_CALLBACK(on_histogram_clicked), app_state);

	gtk_window_present(GTK_WINDOW(app_state->window));
}
