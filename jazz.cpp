#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksource.h>
#include <cstdio>
//#include <iostream>

GtkWidget* window 	= nullptr;
GtkWidget* toolbar 	= nullptr;
GtkWidget* swin 	= nullptr;
GtkWidget* source_view 	= nullptr;
GtkWidget* hbox 	= nullptr;
GtkWidget* notebook = nullptr;
GtkWidget* frame	= nullptr;
GtkWidget* label	= nullptr;
int frame_num = 1;
char bufferf[32];
char bufferl[32];
GtkSourceLanguageManager* language_manager = nullptr;

struct _Data
{
   GtkTextBuffer *buffer;
   GtkWindow     *parent;
};
typedef struct _Data Data;
static void new_file(GtkToolItem *button, Data *data)
{
	frame = gtk_frame_new(bufferf);
	//gtk_container_set_border_width(GTK_CONTAINER(frame), 10);
	//gtk_widget_set_size_request(frame, 400, 500);
	gtk_widget_show(frame);
	
	/*label = gtk_label_new (bufferf);
	gtk_container_add(GTK_CONTAINER(frame), label);
	gtk_widget_show(label);*/
	
	label = gtk_label_new(bufferl);
	gtk_notebook_prepend_page(GTK_NOTEBOOK(notebook), frame, label);
}
static void load_file(GtkToolItem *button, Data *data)
{
	static GtkWidget *dialog = nullptr;
	if (dialog == nullptr)
	{
		dialog = gtk_file_chooser_dialog_new("Open File",
			data->parent, GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
	}
	if( gtk_dialog_run( GTK_DIALOG(dialog) ) == GTK_RESPONSE_ACCEPT )
	{
		gchar *filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER( dialog ) );
		gchar *text_data;
		printf("Open: %s\n", filename);
		g_file_get_contents( filename, &text_data, NULL, NULL );

		auto s_lang = gtk_source_language_manager_guess_language(
			language_manager,
			filename,
			nullptr);
		
		// Append new page
		auto scrolled_win = gtk_scrolled_window_new(NULL, NULL);
		auto new_source_view = gtk_source_view_new();
		auto tab_label = gtk_label_new(filename);
		
		auto buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW( new_source_view ));
		gtk_text_buffer_set_text( buff, text_data, -1 );
		
		gtk_source_buffer_set_language(
			GTK_SOURCE_BUFFER(buff), s_lang);
		
		gtk_container_add(GTK_CONTAINER(scrolled_win), new_source_view);
		
		gtk_widget_show_all(GTK_WIDGET(scrolled_win));
		
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scrolled_win, tab_label);
		
		gtk_widget_show_all(notebook);
		
		g_free( filename );
		g_free( text_data );
	}
	gtk_widget_hide(dialog);
}
static void save_file(GtkToolItem *button, Data *data)
{
	static GtkWidget *dialog = nullptr;
	
	if(dialog == nullptr)
	{
		dialog = gtk_file_chooser_dialog_new("Save file",
			data->parent, GTK_FILE_CHOOSER_ACTION_SAVE,
			GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
			GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL);
	}
	if( gtk_dialog_run( GTK_DIALOG(dialog) ) == GTK_RESPONSE_ACCEPT )
	{
		gchar *filename;
		gchar *text_data;
		
		g_object_get( G_OBJECT( data->buffer ), "text",
			&text_data, NULL);
		
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER( dialog ));
		
		g_file_set_contents( filename, text_data, -1, NULL);
		g_free( filename );
		g_free( text_data );
	}
	gtk_widget_hide(dialog);
}
static void activate(GtkApplication* app, gpointer user_data)
{
	GtkTextBuffer *buffer;
	Data *data;
	// initialize
	notebook = gtk_notebook_new();
	data = g_slice_new(Data);
	window = gtk_application_window_new(app);
	toolbar = gtk_toolbar_new();
	hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	source_view = gtk_source_view_new();
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW( source_view ));
	swin = gtk_scrolled_window_new(NULL, NULL);
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
	g_signal_connect (save_button, "clicked", G_CALLBACK(save_file), data);
	g_signal_connect (open_button, "clicked", G_CALLBACK(load_file), data);
	g_signal_connect (new_button, "clicked", G_CALLBACK(new_file), data);
	// position
	gtk_box_pack_start(GTK_BOX(hbox), toolbar, FALSE, FALSE, 0);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook), GTK_POS_TOP);
	gtk_box_pack_end( GTK_BOX( hbox ), notebook, TRUE, TRUE, 0 );
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
	//gtk_container_add(GTK_CONTAINER(hbox), notebook);
	//gtk_container_add(GTK_CONTAINER(swin), notebook);
	//tk_container_add(GTK_CONTAINER(notebook), source_view);
	//gtk_container_add(GTK_CONTAINER(swin), sourceview);
	
	data->buffer = buffer;
	data->parent = GTK_WINDOW(window);
	//gtk_widget_show(toolbar);
	gtk_widget_show_all(window);
}

int main(int argc, char** argv)
{
	GtkApplication* app;
	
	int status;
	
	app = gtk_application_new("ca.chr.gtk.jazz", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), 0);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	
	return status;
}
