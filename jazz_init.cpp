#include <gtksourceview/gtksource.h>
#include "jazz.hpp"
#include "jazz_tablabel.hpp"
#include "jazz_sourceview.hpp"
#include "jazz_filetree.hpp"
#include "jazz_util.hpp"
#include <iostream>

using coral::zircon::json_loadfile;
namespace Jazz
{
JazzIDE::JazzIDE(): box(Gtk::ORIENTATION_VERTICAL, 1),
	h_box(Gtk::ORIENTATION_HORIZONTAL, 1), file_tree("./"),
   project_doc(json_loadfile("test.jazzproj")),
	project_tree(),
	terminal(),
	language_manager(gtk_source_language_manager_get_default())
{
	builder = Gtk::Builder::create_from_file("jazz_menubar.ui");
	builder->add_from_file("jazz_project_dialog.ui");
	builder->add_from_file("jazz_toolbar.ui");
	builder->get_widget("jazz_menubar", menubar);
	menubar->show();
	
	set_default_size(800, 600);
	
	add(box);
	box.pack_end(terminal, true, true);
	box.pack_end(h_box, true, true);
	
	right_pane.set_tab_pos(Gtk::PositionType::POS_BOTTOM);
	right_pane.append_page(project_tree, *Gtk::manage(new Gtk::Label("Project")));
	right_pane.append_page(file_tree, *Gtk::manage(new Gtk::Label("File")));
	
	h_box.pack_end(right_pane, false, false);
	h_box.pack_end(notebook, true, true);
	
	Gtk::ToolButton* titem = nullptr;
	
	builder->get_widget("tb_newfile", titem);
	titem->signal_clicked().connect(sigc::mem_fun(*this, &JazzIDE::NewFile));
	
	builder->get_widget("tb_openfile", titem);
	titem->signal_clicked().connect(sigc::mem_fun(*this, &JazzIDE::OpenFile));
	titem->set_tooltip_text("Open File");
	
	builder->get_widget("tb_savefile", titem);
	titem->signal_clicked().connect(sigc::mem_fun(*this, &JazzIDE::SaveFile));
	titem->set_tooltip_text("Save As");
	
	builder->get_widget("tb_execute", titem);
	titem->signal_clicked().connect(sigc::mem_fun(*this, &JazzIDE::ExecuteProject));
	
	builder->get_widget("tb_resumedbg", titem);
	titem->signal_clicked().connect(sigc::mem_fun(*this, &JazzIDE::DebugContinueCmd));
	titem->set_tooltip_text("Continue");
	
	builder->get_widget("jazz_toolbar", toolbar);
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
	menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::SaveFile));
	
	builder->get_widget("filemenuexit", menu_item);
	menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::Quit));
	
	builder->get_widget("editmenufont", menu_item);
	menu_item->signal_activate().connect(sigc::mem_fun(*this,&JazzIDE::ChooseFont));
	
	file_tree.TreeView().signal_row_activated().connect(sigc::mem_fun(*this, &JazzIDE::OpenFileFromTree));

	project_tree.SetProject(project_doc);

	show_all_children();
}
JazzIDE::~JazzIDE()
{
	//delete gdb;
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
struct BreakpointCallbackData
{
	int line;
	gulong signal_id;
	SourceView* source_view;
};
static void OnSizeAllocate(GtkTextView* view, GdkRectangle*, gpointer user_data)
{
	BreakpointCallbackData* callback = static_cast<BreakpointCallbackData*>(user_data);
	callback->source_view->ScrollToLine(callback->line);
	
	// We need to come up with a way to delete the user data after the signal is disconnected at some point
	//g_signal_handler_disconnect(view, callback->signal_id);
	//delete callback;
}
bool JazzIDE::HandleGDBOutput(Glib::IOCondition, const Glib::ustring& thing)
{
	if(thing[0] == '*')
	{
		if(thing.find("breakpoint-hit") !=  Glib::ustring::npos)
		{
			// Find the fullname property and open that file
			auto new_pos = thing.rfind("fullname=")+10U;
			auto str = thing.substr(new_pos);
			str = str.substr(0, str.find('"'));
			
			new_pos = thing.rfind("line=")+6U;
			int line_num = std::stoi(thing.substr(new_pos, thing.find('"', new_pos)));  
			
			AddFileToNotebook(str, [this, str, line_num](FileOpened success){
				switch(success)
				{
					case FileOpened::Success:
					{
						SourceView* page = static_cast<SourceView*>(notebook.get_nth_page(notebook.get_current_page()));
						BreakpointCallbackData* data = new BreakpointCallbackData{line_num, 0U, page};
						data->signal_id = g_signal_connect(GTK_WIDGET(page->GetSourceView()), "size-allocate",
							G_CALLBACK(OnSizeAllocate), (gpointer)data); 
						page->ScrollToLine(line_num);
						puts("File opened successfully in jazz init callback");
					}break;
					case FileOpened::Failure:
					{
						// In the future we should use lib notify to inform the user that this operation didn't work
						ShowMessage("Breakpoint hit but could not open"
										" the file" + str + " successfully");
					}break;
					case FileOpened::WasOpen:
					{
						SourceView* page = static_cast<SourceView*>(notebook.get_nth_page(notebook.get_current_page()));
						page->ScrollToLine(line_num);
						puts("File was already open in jazz init callback");
					}break;
				}
			});
		}
	}
	return true;
}
}
