//Vasile Radu-Stefan, 314CA
#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

void on_load_button_clicked(GtkWidget *widget, gpointer user_data);
void on_save_button_clicked(GtkWidget *widget, gpointer user_data);
void on_rotate_clicked(GtkWidget *widget, gpointer user_data);
void on_apply_filter_clicked(GtkWidget *widget, gpointer user_data);
void on_equalize_clicked(GtkWidget *widget, gpointer user_data);
void on_crop_clicked(GtkWidget *widget, gpointer user_data);
void on_histogram_clicked(GtkWidget *widget, gpointer user_data);

#endif // CALLBACKS_H