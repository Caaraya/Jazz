#include <gtkmm.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksource.h>
#include <cstdio>
#include <string>
#include <iostream>

#include "jazz.hpp"
//using namespace Jazz;
/*
GtkWidget* toolbar 	= nullptr;
GtkWidget* swin 	= nullptr;
GtkWidget* source_view 	= nullptr;
GtkWidget* hbox 	= nullptr;
GtkWidget* notebook = nullptr;
GtkWidget* file_menu	= nullptr;
GtkWidget* edit_menu	= nullptr;
GtkWidget* menu_bar	= nullptr;
GtkWidget* choose_font	=nullptr;
//GtkWidget* frame	= nullptr;
//GtkWidget* label	= nullptr;
int tab_num = 1;
char bufferf[32];
char bufferl[32];
GtkSourceLanguageManager* language_manager = nullptr;

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

GtkWidget* new_sourceview(bool scrollable = true)
{
	Jazz::Gtk::SourceView source_view;
	
	if(scrollable)
	{
		Jazz::Gtk::ScrolledWindow scrolled_window;
		
		scrolled_window.Add(source_view);
		scrolled_window.ShowAll();
		
		return scrolled_window.Object();
	}
	return source_view.Object();
}

static void new_file(GtkToolItem *button, void*)
{
	std::string str = "Page ";
	str += std::to_string(tab_num); //char* str = "Page";
	
	auto new_source_view = new_sourceview();
	auto new_tab_thing = new_tab_label(str, new_source_view);
	
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), new_source_view, new_tab_thing); //scrolled_win, box);//tab_label);//new_source_view, new_tab_thing); 
	
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), -1);
	
	gtk_widget_show_all(notebook);
	//std::cout << tab_num << std::endl;
	tab_num++;
}
static void font_chooser(GtkWidget *font, void*)
{
	choose_font = gtk_font_chooser_dialog_new("Choose Font", GTK_WINDOW(window->gobj()));
	
	gtk_font_chooser_set_font(GTK_FONT_CHOOSER(choose_font), "Monospace 10");
	/*responce = gtk_dialog_run(GTK_DIALOG(choose_font));
	gchar* new_font = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(choose_font));
	gtk_widget_destroy(choose_font);
	//if(responce == GTK_RESPONSE_ACCEPT)
		// whatever
}
static void load_file(GtkToolItem *button, void*)
{
	static GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",	GTK_WINDOW(window->gobj()), GTK_FILE_CHOOSER_ACTION_OPEN,
			"_Open", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
			
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
		
		// the pointer is to a scrolled window containing a sourceview because we didnt pass false
		auto new_sview = new_sourceview();
		// Append new page
		auto tab_label = new_tab_label(filename, new_sview);
		
		auto source = gtk_bin_get_child (GTK_BIN(new_sview));
		
		auto buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW( source ));
		gtk_text_buffer_set_text( buff, text_data, -1 );
		
		gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(buff), s_lang);
		
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook), new_sview, tab_label);
		
		gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), -1);
		
		gtk_widget_show_all(notebook);
		
		g_free( filename );
		g_free( text_data );
		tab_num++;
	}
	//gtk_widget_hide(dialog);
	gtk_widget_destroy(dialog);
}
static void save_file(GtkToolItem *button, void*)
{
	static GtkWidget *dialog = gtk_file_chooser_dialog_new("Save file", GTK_WINDOW(window->gobj()), GTK_FILE_CHOOSER_ACTION_SAVE,
			"_Save", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
			
	if( gtk_dialog_run( GTK_DIALOG(dialog) ) == GTK_RESPONSE_ACCEPT )
	{
		gchar *filename;
		gchar *text_data;
		
		Jazz::Gtk::Notebook the_notebook(notebook);
		
		//gint page_num = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
		GtkWidget* page = the_notebook.CurrentPage().Object();//gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), page_num);
		GtkWidget* label = gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), page);
		
		GtkWidget* the_child = the_notebook.CurrentPage().Object();
		GtkSourceView* the_sourceview = nullptr;
		
		if(GTK_IS_SCROLLED_WINDOW(the_child))
			the_sourceview = GTK_SOURCE_VIEW(gtk_bin_get_child(GTK_BIN(the_child)));
		else
			the_sourceview = GTK_SOURCE_VIEW(the_child);
			
		Jazz::Gtk::SourceView sourceview(GTK_WIDGET(the_sourceview));
		
		Jazz::Gtk::Buffer the_buffer = sourceview.GetBuffer();		
		
		//Gtk::Buffer the_buffer(the_notebook.CurrentPage());
		
		g_object_get( G_OBJECT( the_buffer.Object() ), "text",
			&text_data, NULL);
		
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER( dialog ));
		
		std::string filenm = filename;
		
		std::string shortname = filenm.substr(filenm.find_last_of("\\")+1);
		
		GList* children = gtk_container_get_children(GTK_CONTAINER(label));
		
		GList* label_child = g_list_first(children);
		
		gtk_label_set_text(GTK_LABEL(label_child->data), shortname.c_str()); //children->data, shortname);
		
		g_file_set_contents( filename, text_data, -1, NULL);
		g_free( filename );
		g_free( text_data );
	}
	//gtk_widget_hide(dialog);
	gtk_widget_destroy(dialog);
}
static void activate()
{
	puts("Activate called");
	window = new Gtk::Window;
	window->set_default_size(600, 500);
	// initialize
	notebook = gtk_notebook_new();
	menu_bar = gtk_menu_bar_new();
	//data = g_slice_new(Data);
	file_menu = gtk_menu_new();
	edit_menu = gtk_menu_new();
	//window = gtk_application_window_new(app);
	toolbar = gtk_toolbar_new();
	hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	//create menu items
	GtkWidget* new_item = gtk_menu_item_new_with_label("New");
	GtkWidget* open_item = gtk_menu_item_new_with_label("Open");
	GtkWidget* save_item = gtk_menu_item_new_with_label("Save");
	GtkWidget* quit_item = gtk_menu_item_new_with_label("Quit");
	GtkWidget* file_item = gtk_menu_item_new_with_label("File");
	GtkWidget* font_item = gtk_menu_item_new_with_label("Font");
	GtkWidget* edit_item = gtk_menu_item_new_with_label("Edit");
	// add to menu
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), new_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), font_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit_item);
	
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
	g_signal_connect(G_OBJECT(window->gobj()), "destroy", G_CALLBACK( gtk_main_quit ), NULL);
	g_signal_connect (save_button, "clicked", G_CALLBACK(save_file), nullptr);
	g_signal_connect (open_button, "clicked", G_CALLBACK(load_file), nullptr);
	g_signal_connect (new_button, "clicked", G_CALLBACK(new_file), nullptr);
	g_signal_connect (G_OBJECT(new_item), "activate", G_CALLBACK(new_file), nullptr);
	g_signal_connect (G_OBJECT(open_item), "activate", G_CALLBACK(load_file), nullptr);
	g_signal_connect (G_OBJECT(save_item), "activate", G_CALLBACK(save_file), nullptr);
	g_signal_connect (G_OBJECT(font_item), "activate", G_CALLBACK(font_chooser), nullptr);
	g_signal_connect_swapped (G_OBJECT(quit_item), "activate", G_CALLBACK(gtk_widget_destroy), window->gobj());
	// position
	gtk_box_pack_start(GTK_BOX(hbox), menu_bar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), toolbar, FALSE, FALSE, 0);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook), GTK_POS_TOP);
	gtk_box_pack_end( GTK_BOX( hbox ), notebook, TRUE, TRUE, 0 );
	//
	gtk_widget_show( open_item );
	gtk_widget_show( save_item );
	gtk_widget_show( new_item );
	gtk_widget_show( quit_item );
	gtk_widget_show( font_item );
	//
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_item), edit_menu);
	// insert toolbar items
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), seperator, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), compile_button, -1);
	// window set
	//gtk_window_set_title(GTK_WINDOW(window), "Window");
	window->set_title("Window");
//	gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
	// connections
	gtk_container_add(GTK_CONTAINER(window->gobj()), hbox);
	
	gtk_widget_show(menu_bar);
	gtk_widget_show(file_item);
	//gtk_widget_show_all(GTK_WIDGET(window->gobj()));
	window->show_all();
	puts("activate success");
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
