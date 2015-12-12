#include <gtksourceview/gtksource.h>
#include "jazz.hpp"
#include "jazz_tablabel.hpp"
#include "jazz_sourceview.hpp"
#include "jazz_filetree.hpp"
#include <iostream>

using coral::zircon::json_loadfile;
namespace Jazz
{
JazzIDE::JazzIDE(): box(Gtk::ORIENTATION_VERTICAL, 1),
	h_box(Gtk::ORIENTATION_HORIZONTAL, 1), file_tree("./"),
   project_doc(json_loadfile("bin/test.jazzproj")),
	project_tree(),
	language_manager(gtk_source_language_manager_get_default())
{
   builder = Gtk::Builder::create_from_file("jazz_menubar.ui");
	builder->add_from_file("jazz_project_dialog.ui");
   builder->get_widget("jazz_menubar", menubar);
   menubar->show();
   
	set_default_size(800, 600);
	
	add(box);
	box.pack_end(h_box, true, true);
	
	right_pane.set_tab_pos(Gtk::PositionType::POS_BOTTOM);
   right_pane.append_page(project_tree, *Gtk::manage(new Gtk::Label("Project")));
   right_pane.append_page(file_tree, *Gtk::manage(new Gtk::Label("File")));
	
   h_box.pack_end(right_pane, false, false);
   h_box.pack_end(notebook, true, true);
   
	GtkWidget* tool_bar = gtk_toolbar_new();
	
	GtkToolItem* new_button = gtk_tool_button_new(nullptr, nullptr);
	GtkToolItem* open_button = gtk_tool_button_new(nullptr, nullptr);
	GtkToolItem* save_button = gtk_tool_button_new(nullptr, nullptr);
	GtkToolItem* compile_button = gtk_tool_button_new(nullptr, nullptr);
	
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(new_button), "document-new");
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(open_button), "document-open");
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(save_button), "document-save");
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(compile_button), "system-run");
	
	GtkToolItem* seperator = gtk_separator_tool_item_new();
	
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), new_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), open_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), save_button, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), seperator, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(tool_bar), compile_button, -1);
	
	toolbar = static_cast<Gtk::Toolbar *>(Glib::wrap(tool_bar));
	
	box.pack_start(*menubar,false,false);
	box.pack_start(*toolbar, false, false);
	// Connect the new menu button to our member function for it
   Gtk::MenuItem* menu_item = nullptr;
   
   builder->get_widget("filemenunewfile", menu_item);
   menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::NewFile));
	
	builder->get_widget("filemenunewproject", menu_item);
	menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::NewProject));
   
   builder->get_widget("filemenuopen", menu_item);
   menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::OpenFile));
   
   builder->get_widget("filemenusave", menu_item);
   menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::SaveFile));
   
   builder->get_widget("filemenusaveas", menu_item);
   menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::SaveFileAs));
   
   builder->get_widget("filemenuexit", menu_item);
   menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::Quit));
	
	builder->get_widget("editmenufont", menu_item);
	menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::ChooseFont));

	Gtk::ToolButton* new_bttn_wrapper = static_cast<Gtk::ToolButton*>(Glib::wrap(new_button));
	new_bttn_wrapper->signal_clicked().connect(sigc::mem_fun(*this,&JazzIDE::NewFile));
	new_bttn_wrapper->set_tooltip_text("New File");
	
	Gtk::ToolButton* open_bttn_wrapper = static_cast<Gtk::ToolButton*>(Glib::wrap(open_button));
	open_bttn_wrapper->signal_clicked().connect(sigc::mem_fun(*this,&JazzIDE::OpenFile));
	open_bttn_wrapper->set_tooltip_text("Open File");
	
	Gtk::ToolButton* save_bttn_wrapper = static_cast<Gtk::ToolButton*>(Glib::wrap(save_button));
	save_bttn_wrapper->signal_clicked().connect(sigc::mem_fun(*this,&JazzIDE::SaveFile));
	save_bttn_wrapper->set_tooltip_text("Save As");
	
	file_tree.TreeView().signal_row_activated().connect(sigc::mem_fun(*this, &JazzIDE::OpenFileFromTree));

	project_tree.SetProject(project_doc);

	show_all_children();
	
	using std::placeholders::_1;
	using std::placeholders::_2;
	gdb->AddOutHandler(std::bind(&JazzIDE::HandleGDBOutput, this, _1, _2));
	gdb->Command("help");
	gdb->Command("b gdb_test.cpp:9");
	gdb->Command("r");
}
JazzIDE::~JazzIDE()
{
	delete gdb;
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
bool JazzIDE::HandleGDBOutput(Glib::IOCondition, const Glib::ustring& thing)
{
	if(thing[0] == '*')
	{
		if(thing.find("breakpoint-hit") !=  Glib::ustring::npos)
		{
			// find the fullname property and open that file
			auto new_pos = thing.rfind("fullname=")+10U;
			auto str = thing.substr(new_pos);
			str = str.substr(0, str.find('"'));
			printf("I need to open %s\n", str.c_str());
			
			AddFileToNotebook(str, [this](bool success){
				if(success)
				{
					puts("Breakpoint hit and we opened a file");
					SourceView* page = static_cast<SourceView*>(notebook.get_nth_page(notebook.get_current_page()));
					GtkSourceView* source_view = page->GetSourceView();
					GtkSourceBuffer* source_buffer = page->GetSourceBuffer();
				}
				else
				{
					// In the future we should use lib notify to inform the user that this operation didn't work
					puts("Breakpoint hit but we didn't open the file successfully");
				}
			});
		}
	}
	return true;
}
}

