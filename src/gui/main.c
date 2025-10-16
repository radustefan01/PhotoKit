#include "app_state.h"
#include "ui.h"

int main(int argc, char **argv)
{
	GtkApplication *app;
	int status;
	app_state_t *app_state;

	//allocate the AppState struct
	app_state = g_new0(app_state_t, 1);

	app = gtk_application_new("com.example.image_editor", G_APPLICATION_DEFAULT_FLAGS);

	//pass the app_state to the activate signal handler
	g_signal_connect(app, "activate", G_CALLBACK(activate), app_state);

	//run the application
	status = g_application_run(G_APPLICATION(app), argc, argv);

	//cleanup
	g_object_unref(app);

	//free the core image data
	free_image_data(NULL, &app_state->grayscale_data, &app_state->color_data, app_state->height);

	//free the state struct itself
	g_free(app_state);

	return status;
}