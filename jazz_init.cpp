#include <gtkmm.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksource.h>
#include <cstdio>
#include <string>
#include <iostream>
#include "jazz.hpp"
#include "jazz_tablabel.hpp"
#include "jazz_sourceview.hpp"

namespace Jazz
{

JazzIDE::JazzIDE(): box(Gtk::ORIENTATION_VERTICAL, 1),
	language_manager(gtk_source_language_manager_get_default())
{
	set_default_size(600, 500);
	
	add(box);
	box.pack_end(notebook, true, true);
	
	GtkWidget* menu_bar = gtk_menu_bar_new();
	GtkWidget* file_menu = gtk_menu_new();
	GtkWidget* edit_menu = gtk_menu_new();
	
	GtkWidget* tool_bar = gtk_toolbar_new();
	
	GtkWidget* new_item = gtk_menu_item_new_with_label("New");
	GtkWidget* open_item = gtk_menu_item_new_with_label("Open");
	GtkWidget* save_item = gtk_menu_item_new_with_label("Save");
	GtkWidget* quit_item = gtk_menu_item_new_with_label("Quit");
	GtkWidget* file_item = gtk_menu_item_new_with_label("File");
	GtkWidget* font_item = gtk_menu_item_new_with_label("Font");
	GtkWidget* edit_item = gtk_menu_item_new_with_label("Edit");
	
	GtkToolItem* new_button = gtk_tool_button_new(nullptr, nullptr);
	GtkToolItem* open_button = gtk_tool_button_new(nullptr, nullptr);
	GtkToolItem* save_button = gtk_tool_button_new(nullptr, nullptr);
	GtkToolItem* compile_button = gtk_tool_button_new(nullptr, nullptr);
	
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(new_button), "document-new");
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(open_button), "document-open");
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(save_button), "document-save");
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(compile_button), "system-run");
	
	GtkToolItem* seperator = gtk_separator_tool_item_new();
	
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), new_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), font_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit_item);
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_item), edit_menu);
	
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), new_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), open_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), save_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), seperator, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), compile_button, -1);
	
	gtk_widget_show(menu_bar);
	gtk_widget_show(file_item);
	
	auto menbar = Glib::wrap(menu_bar);
	auto toolbar = Glib::wrap(tool_bar);
	
	box.pack_start(*menbar,false,false);
	box.pack_start(*toolbar, false, false);

	puts("Success constructor");
	
	// Connect the new menu button to our member function for it
	Gtk::MenuItem* new_item_wrapper = static_cast<Gtk::MenuItem*>(Glib::wrap(new_item));
	new_item_wrapper->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::NewFile));
	
	Gtk::MenuItem* open_item_wrapper = static_cast<Gtk::MenuItem*>(Glib::wrap(open_item));
	open_item_wrapper->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::OpenFile));
	
	Gtk::MenuItem* save_item_wrapper = static_cast<Gtk::MenuItem*>(Glib::wrap(save_item));
	save_item_wrapper->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::SaveFile));

	Gtk::ToolButton* new_bttn_wrapper = static_cast<Gtk::ToolButton*>(Glib::wrap(new_button));
	new_bttn_wrapper->signal_clicked().connect(sigc::mem_fun(*this,&JazzIDE::NewFile));
	
	Gtk::ToolButton* open_bttn_wrapper = static_cast<Gtk::ToolButton*>(Glib::wrap(open_button));
	open_bttn_wrapper->signal_clicked().connect(sigc::mem_fun(*this,&JazzIDE::OpenFile));
	
	Gtk::ToolButton* save_bttn_wrapper = static_cast<Gtk::ToolButton*>(Glib::wrap(save_button));
	save_bttn_wrapper->signal_clicked().connect(sigc::mem_fun(*this,&JazzIDE::SaveFile));
	
	show_all_children();
	/*
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
	*/
}
JazzIDE::~JazzIDE()
{
}
}

