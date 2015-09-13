#include <gtkmm.h>
#include "jazz.hpp"
/*

static void font_chooser(GtkWidget *font, void*)
{
	choose_font = gtk_font_chooser_dialog_new("Choose Font", GTK_WINDOW(window->gobj()));
	
	gtk_font_chooser_set_font(GTK_FONT_CHOOSER(choose_font), "Monospace 10");
	responce = gtk_dialog_run(GTK_DIALOG(choose_font));
	gchar* new_font = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(choose_font));
	gtk_widget_destroy(choose_font);
	//if(responce == GTK_RESPONSE_ACCEPT)
		// whatever
}
*/
int main(int argc, char** argv)
{
	puts("Welcome to jazz");
	auto app = Gtk::Application::create(argc, argv,
		"ca.chr.apps.jazz");
	
	Jazz::JazzIDE jazz;
	
	return app->run(jazz);
}
