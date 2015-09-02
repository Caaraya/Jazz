#include <gtksourceview/gtksourceview.h>
#include <cstdio>

struct _Data
{
   GtkTextBuffer *buffer;
   GtkWindow     *parent;
};
typedef struct _Data Data;

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
		gchar *filename;
		gchar *text;
		
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER( dialog ));
		g_file_get_contents(filename, &text, NULL, NULL);
		gtk_text_buffer_set_text(data->buffer, text, -1);
		g_free( filename );
		g_free( text );
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
		gchar *text;
		
		g_object_get( G_OBJECT( data->buffer ), "text",
			&text, NULL);
		
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER( dialog ));
		
		g_file_set_contents( filename, text, -1, NULL);
		g_free( filename );
		g_free( text );
	}
	gtk_widget_hide(dialog);
}
static void activate(GtkApplication* app, gpointer user_data)
{
	GtkWidget* window;
	GtkWidget* toolbar;
	GtkWidget* hbox;
	GtkWidget* sview;
	GtkWidget* scrollwin;
	GtkWidget* text;
	GtkTextBuffer* buffer;
	Data* data;
	
	data = g_slice_new( Data );
	window = gtk_application_window_new(app);
	toolbar = gtk_toolbar_new();
	hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	sview = gtk_source_view_new();
	scrollwin = gtk_scrolled_window_new(0,0);
	
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), 0);
	
	GtkToolItem *new_button = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
	GtkToolItem *open_button = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	GtkToolItem *save_button = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	
	gtk_scrolled_window_set_policy(	GTK_SCROLLED_WINDOW(scrollwin),	GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
	
	gtk_box_pack_start(GTK_BOX(hbox),toolbar,FALSE,FALSE,0);
	
	//gtk_scrolled_window_set_policy(	GTK_SCROLLED_WINDOW(scrollwin),	GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
	
	gtk_container_add( GTK_CONTAINER(scrollwin), GTK_WIDGET(sview) );
	
	gtk_box_pack_end(GTK_BOX(hbox),scrollwin,TRUE,TRUE,0);
	
	text = gtk_text_view_new();
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW( text ));
	
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_button, -1);
	gtk_window_set_title(GTK_WINDOW(window), "Window");
	
	g_signal_connect (save_button, "clicked", G_CALLBACK(save_file), data);
	g_signal_connect (open_button, "clicked", G_CALLBACK(load_file), data);
	
	
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
	//gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), TOOLBAR_ICONS)
	//gtk_container_add(GTK_CONTAINER(window), toolbar);
	gtk_container_add( GTK_CONTAINER(window), hbox );
	
	data->buffer = buffer;
	data->parent = GTK_WINDOW(window);
	
	gtk_widget_show_all(window);
	
	g_slice_free(Data, data);
}

int main(int argc, char** argv)
{
	GtkApplication* app;
	
	int status;
	
	app = gtk_application_new("ca.chr.gtk", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), 0);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	
	return status;
}