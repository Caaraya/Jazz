#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksource.h>
#include <cstdio>
#include <string>
#include <iostream>
#include "Gtk.hpp"
using namespace Jazz;

GtkWidget* window 	= nullptr;
GtkWidget* toolbar 	= nullptr;
GtkWidget* swin 	= nullptr;
GtkWidget* source_view 	= nullptr;
GtkWidget* hbox 	= nullptr;
GtkWidget* notebook = nullptr;
GtkWidget* menu		= nullptr;
GtkWidget* menu_bar	= nullptr;
//GtkWidget* frame	= nullptr;
//GtkWidget* label	= nullptr;
int tab_num = 1;
char bufferf[32];
char bufferl[32];
GtkSourceLanguageManager* language_manager = nullptr;
/*
struct _Data
{
   GtkTextBuffer *buffer;
   GtkWindow     *parent;
};
typedef struct _Data Data;
*/
static void close_tab(GtkButton* button, GtkWidget* child_obj)
{	
	gint pagenum = gtk_notebook_page_num(GTK_NOTEBOOK(notebook), child_obj);
	
	gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), pagenum);
}

GtkWidget* new_tab_label(const std::string& title, GtkWidget* child_held)
{
	std::string shortname = "";
	if(title.find("\\")!=std::string::npos)
		shortname = title.substr(title.find_last_of("\\")+1);
	else
		shortname = title;
	// Create widgets that go in the new tab
	auto image = gtk_image_new_from_icon_name("window-close",GTK_ICON_SIZE_SMALL_TOOLBAR);
	auto box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	auto tab_close_button = gtk_button_new();
	auto tab_label = gtk_label_new(shortname.c_str());
	// making close button
	gtk_button_set_relief(GTK_BUTTON(tab_close_button),GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click(GTK_BUTTON(tab_close_button), false);
	gtk_button_set_image(GTK_BUTTON(tab_close_button), image);
	// pack the label on the left, and the button
	gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(tab_label), TRUE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(box), tab_close_button, TRUE, TRUE, 0);
	//
	g_signal_connect (tab_close_button, "clicked", G_CALLBACK(close_tab), child_held);//tab_label);
	// the box shows its widgets
	gtk_widget_show_all(GTK_WIDGET(box));
	
	return box;
}

Gtk::Widget new_sourceview(bool scrollable = true)
{
	Gtk::SourceView source_view;
	
	if(scrollable)
	{
		Gtk::ScrolledWindow scrolled_window;
		
		scrolled_window.Add(source_view);
		scrolled_window.ShowAll();
		
		return scrolled_window;
	}
	return source_view;
}

static void new_file(GtkToolItem *button, void*)
{
	std::string str = "Page ";
	str += std::to_string(tab_num); //char* str = "Page";
	
	Gtk::Widget new_source_view = new_sourceview();
	Gtk::Widget new_tab_thing = new_tab_label(str, new_source_view.Object());
	
	Gtk::Notebook the_notebook(notebook);
	
	the_notebook.AddPage(new_source_view, new_tab_thing);
	
	the_notebook.SetCurrentPage(-1);
	
	the_notebook.ShowAll();

	tab_num++;
}
static void load_file(GtkToolItem *button, void*)
{
	GtkWidget* dialog = gtk_file_chooser_dialog_new("Open File",	GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN,
			"_Open", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
			
	if( gtk_dialog_run( GTK_DIALOG(dialog) ) == GTK_RESPONSE_ACCEPT )
	{
		gchar* filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER( dialog ) );
		printf("Open: %s\n", filename);
		
		// Construct the new text view, the pointer is to a scrolled
		// window containing a sourceview because we didnt pass false
		auto new_sview = new_sourceview();
		auto tab_label = new_tab_label(filename, new_sview);
		auto source = gtk_bin_get_child (GTK_BIN(new_sview.Object()));
		auto buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW( source ));
		
		GFile* new_file = g_file_new_for_path(filename);
		GtkSourceFile* new_source_file = gtk_source_file_new();
		
		gtk_source_file_set_location(new_source_file, new_file);
		
		GtkSourceFileLoader* loader = gtk_source_file_loader_new(
			GTK_SOURCE_BUFFER(buff), new_source_file);

		auto s_lang = gtk_source_language_manager_guess_language(
			language_manager,
			filename,
			nullptr);
			
		gtk_source_file_loader_load_async(
			loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL,	NULL,
			[](GObject* source_obj, GAsyncResult* res, gpointer loader) -> void {
				GError* error = nullptr;
				gboolean success = gtk_source_file_loader_load_finish(
					(GtkSourceFileLoader*)loader, res, &error);
				if(success)
					puts("Successfully opened file");
				else
					printf("Failed to open file: %i, %s\n",
						error->code,error->message);
			}, 
			// Pass the loader as the user data, so that we can just keep
			// the lambda function as is
			(gpointer)loader);	
		
		gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(buff), s_lang);
		
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook), new_sview, tab_label);
		
		gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), -1);
		
		gtk_widget_show_all(notebook);
		
		g_free( filename );
		g_object_unref(new_file);

		tab_num++;
	}
	//gtk_widget_hide(dialog);
	gtk_widget_destroy(dialog);
}
static void save_file(GtkToolItem *button, void*)
{
	GtkWidget *dialog = gtk_file_chooser_dialog_new("Save file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE,
			"_Save", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
			
	if( gtk_dialog_run( GTK_DIALOG(dialog) ) == GTK_RESPONSE_ACCEPT )
	{
		// First thing is to aquire the text buffer of the current tab
		Gtk::Notebook the_notebook(notebook);

		Gtk::Widget page = the_notebook.CurrentPage();
		GtkWidget* label = the_notebook.TabForChild(page).Object();
		
		GtkSourceView* the_sourceview = nullptr;
		
		if(GTK_IS_SCROLLED_WINDOW(page.Object()))
			the_sourceview = GTK_SOURCE_VIEW(gtk_bin_get_child(GTK_BIN(page.Object())));
		else
			the_sourceview = GTK_SOURCE_VIEW(page.Object());//the_child);
			
		Gtk::SourceView sourceview(GTK_WIDGET(the_sourceview));
		
		Gtk::Buffer the_buffer = sourceview.GetBuffer();		

		gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER( dialog ));
		
		GFile* new_file = g_file_new_for_path(filename);
		GtkSourceFile* new_source_file = gtk_source_file_new();
		
		gtk_source_file_set_location(new_source_file, new_file);
		
		GtkSourceFileSaver* new_srcfile_saver = gtk_source_file_saver_new_with_target(GTK_SOURCE_BUFFER(the_buffer.Object()), new_source_file, new_file);
		
		gtk_source_file_saver_save_async(
			new_srcfile_saver, G_PRIORITY_DEFAULT, NULL, NULL, NULL,	NULL,
			[](GObject* source_obj, GAsyncResult* res, gpointer saver) -> void {
				GError* error = nullptr;
				gboolean success = gtk_source_file_saver_save_finish(
					(GtkSourceFileSaver*)saver, res, &error);
				if(success)
					puts("Successfully saved file");
				else
					printf("Failed to save file: %i, %s\n",
						error->code,error->message);
			}, 
			// Pass the loader as the user data, so that we can just keep
			// the lambda function as is
			(gpointer)new_srcfile_saver);
		
		std::string filenm = filename;
		
		std::string shortname = filenm.substr(filenm.find_last_of("\\")+1);
		
		GList* children = gtk_container_get_children(GTK_CONTAINER(label));
		
		GList* label_child = g_list_first(children);
		
		gtk_label_set_text(GTK_LABEL(label_child->data), shortname.c_str());
		
		g_object_unref(new_file);
		g_free( filename );
	}
	gtk_widget_destroy(dialog);
}
static void activate(GtkApplication* app, gpointer user_data)
{
	// initialize
	notebook = gtk_notebook_new();
	menu_bar = gtk_menu_bar_new();
	//data = g_slice_new(Data);
	menu = gtk_menu_new();
	window = gtk_application_window_new(app);
	toolbar = gtk_toolbar_new();
	hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	//create menu items
	GtkWidget* new_item = gtk_menu_item_new_with_label("New");
	GtkWidget* open_item = gtk_menu_item_new_with_label("Open");
	GtkWidget* save_item = gtk_menu_item_new_with_label("Save");
	//GtkMenuItem* quit_item = gtk_menu_item_new_with_label("Quit");
	GtkWidget* file_item = gtk_menu_item_new_with_label("File");
	// add to menu
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), new_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), open_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), save_item);
	//gtk_menu_append(GTK_MENU(menu), quit_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_item);
	//
	language_manager = gtk_source_language_manager_get_default ();
	//
	GtkToolItem* new_button = gtk_tool_button_new(nullptr, nullptr);
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(new_button), "document-new");
	//
	GtkToolItem* open_button = gtk_tool_button_new(nullptr, nullptr);
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(open_button), "document-open");
	//
	GtkToolItem* save_button = gtk_tool_button_new(nullptr, nullptr);
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(save_button), "document-save");
	//
	GtkToolItem* seperator = gtk_separator_tool_item_new();
	//
	GtkToolItem* compile_button = gtk_tool_button_new(nullptr, nullptr);
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(compile_button), "system-run");
	//
	// signal connect
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK( gtk_main_quit ), NULL);
	g_signal_connect (save_button, "clicked", G_CALLBACK(save_file), nullptr);
	g_signal_connect (open_button, "clicked", G_CALLBACK(load_file), nullptr);
	g_signal_connect (new_button, "clicked", G_CALLBACK(new_file), nullptr);
	g_signal_connect (G_OBJECT(new_item), "activate", G_CALLBACK(new_file), nullptr);
	g_signal_connect (G_OBJECT(open_item), "activate", G_CALLBACK(load_file), nullptr);
	g_signal_connect (G_OBJECT(save_item), "activate", G_CALLBACK(save_file), nullptr);
	//g_signal_connect (G_OBJECT(quit_item), "clicked", GTK_SIGNAL_FUNC(destroy), (gpointer) "file.quit");
	// position
	gtk_box_pack_start(GTK_BOX(hbox), menu_bar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), toolbar, FALSE, FALSE, 0);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook), GTK_POS_TOP);
	gtk_box_pack_end( GTK_BOX( hbox ), notebook, TRUE, TRUE, 0 );
	//
	gtk_widget_show( open_item );
	gtk_widget_show( save_item );
	gtk_widget_show( new_item );
	//gtk_widget_show( quit_item );
	//
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), menu);
	// insert toolbar items
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), seperator, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), compile_button, -1);
	// window set
	gtk_window_set_title(GTK_WINDOW(window), "Window");
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
	// connections
	gtk_container_add(GTK_CONTAINER(window), hbox);
	
	gtk_widget_show(menu_bar);
	gtk_widget_show(file_item);
	gtk_widget_show_all(window);
}

int main(int argc, char** argv)
{
	GtkApplication* app;
	
	app = gtk_application_new("ca.chr.gtk.jazz", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), 0);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	
	return status;
}
