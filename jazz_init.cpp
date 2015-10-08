#include <gtksourceview/gtksource.h>
#include "jazz.hpp"
#include "jazz_tablabel.hpp"
#include "jazz_sourceview.hpp"
#include "jazz_filetree.hpp"
#include <iostream>
// ----------------------------------
// mruby header
// ----------------
#include <mruby/compile.h>
// -------------------------
namespace Jazz
{
JazzIDE::JazzIDE(): box(Gtk::ORIENTATION_VERTICAL, 1),
	h_box(Gtk::ORIENTATION_HORIZONTAL, 1), file_tree("./"),
	language_manager(gtk_source_language_manager_get_default())
{
	mrb = mrb_open();

	set_default_size(600, 500);
	
	add(box);
	box.pack_end(h_box, true, true);
	
	h_box.pack_end(file_tree, false, false);
	file_tree.set_size_request(150);
	h_box.pack_end(notebook, true, true);

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
	
	menubar = static_cast<Gtk::MenuBar *>(Glib::wrap(menu_bar));
	toolbar = static_cast<Gtk::Toolbar *>(Glib::wrap(tool_bar));
	
	box.pack_start(*menubar,false,false);
	box.pack_start(*toolbar, false, false);

	puts("Success constructor");
	
	// Connect the new menu button to our member function for it
	Gtk::MenuItem* new_item_wrapper = static_cast<Gtk::MenuItem*>(Glib::wrap(new_item));
	new_item_wrapper->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::NewFile));
	
	Gtk::MenuItem* open_item_wrapper = static_cast<Gtk::MenuItem*>(Glib::wrap(open_item));
	open_item_wrapper->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::OpenFile));
	
	Gtk::MenuItem* save_item_wrapper = static_cast<Gtk::MenuItem*>(Glib::wrap(save_item));
	save_item_wrapper->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::SaveFile));
	
	Gtk::MenuItem* font_item_wrapper = static_cast<Gtk::MenuItem*>(Glib::wrap(font_item));
	font_item_wrapper->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::ChooseFont));
	
	Gtk::MenuItem* quit_item_wrapper = static_cast<Gtk::MenuItem*>(Glib::wrap(quit_item));
	quit_item_wrapper->signal_activate().connect(sigc::mem_fun(*this, &JazzIDE::Quit));

	Gtk::ToolButton* new_bttn_wrapper = static_cast<Gtk::ToolButton*>(Glib::wrap(new_button));
	new_bttn_wrapper->signal_clicked().connect(sigc::mem_fun(*this,&JazzIDE::NewFile));
	
	Gtk::ToolButton* open_bttn_wrapper = static_cast<Gtk::ToolButton*>(Glib::wrap(open_button));
	open_bttn_wrapper->signal_clicked().connect(sigc::mem_fun(*this,&JazzIDE::OpenFile));
	
	Gtk::ToolButton* save_bttn_wrapper = static_cast<Gtk::ToolButton*>(Glib::wrap(save_button));
	save_bttn_wrapper->signal_clicked().connect(sigc::mem_fun(*this,&JazzIDE::SaveFile));
	
	file_tree.TreeView().signal_row_activated().connect(sigc::mem_fun(*this, &JazzIDE::OpenFileFromTree));
	
	show_all_children();
}
JazzIDE::~JazzIDE()
{
	mrb_close(mrb);
}
void JazzIDE::OpenFileFromTree(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn*)
{
	Gtk::TreeModel::iterator iter = file_tree.TreeStore()->get_iter(path);
	if(iter)
	{
		Gtk::TreeModel::Row row = *iter;
		std::cout <<"Row activated: filename= " << row[file_tree.Columns().filename] << std::endl;
	}
}
}

