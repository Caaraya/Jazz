#include "jazz_newproj_dialog.hpp"
#include "jazz_msgbox.hpp"
#include <iostream>
using namespace coral::zircon;
namespace Jazz
{
	NewProjectDialog::NewProjectDialog(GtkDialog* dialog, const Glib::RefPtr<Gtk::Builder>& builder):
		Gtk::Dialog(dialog)
	{
		Gtk::Button* button = nullptr;
		
		builder->get_widget("new_proj_ok", button);
		button->signal_clicked().connect(sigc::mem_fun(*this, &NewProjectDialog::OnOkClicked));
		
		builder->get_widget("new_proj_cancel", button);
		button->signal_clicked().connect(sigc::mem_fun(*this, &NewProjectDialog::OnCancelClicked));
		
		builder->get_widget("name_entry", name_entry);
		builder->get_widget("directory_entry", directory_entry);
		builder->get_widget("debugger_selection", debugger_selection);
		builder->get_widget("create_proj_dir_check", create_proj_dir);
		
		builder->get_widget("compiler_selection", compiler_selection);
	}
	NewProjectDialog::~NewProjectDialog()
	{
	}
	void NewProjectDialog::OnOkClicked()
	{
		// Validate the input to the project
		Glib::ustring proj_name = name_entry->get_text();
		if(proj_name.empty())
		{
			ShowMessage("Project name must not be empty");
			return;
		}
		
		Glib::ustring directory_path = directory_entry->get_filename();
		if(directory_path.empty())
		{
			ShowMessage("Project location must not be empty");
			return;
		}
		
		if(compiler_selection->get_active_row_number() == -1)
		{
			ShowMessage("A primary compiler must be selected");
			return;
		}
		Glib::ustring compiler_name = compiler_selection->get_active_text();
		
		Glib::ustring debugger_name = "";
		if(debugger_selection->get_active_row_number() != -1)
			debugger_name = debugger_selection->get_active_text();
		
		std::cout << proj_name << " " << directory_path << std::endl;
		
		project_document = object(table());
		project_document["project_name"] = std::string(proj_name);
		project_document["compiler"] = table();
		project_document["compiler"]["command"] = std::string(compiler_name);
		project_document["compiler"]["options"] = std::string("-std=c++14 -c -O -g");
		project_document["linker"] = table();
		project_document["linker"]["command"] = std::string(compiler_name);
		project_document["debugger"] = table();
		project_document["debugger"]["command"] = std::string(debugger_name);
		if(debugger_name == "gdb")
			project_document["debugger"]["options"] = std::string("--interpreter=mi");
		
		if(create_proj_dir->get_active())
		{
			directory_path+="/"+proj_name;
			Glib::RefPtr<Gio::File> dir = Gio::File::create_for_path(directory_path);
			if(!dir->query_exists())
				dir->make_directory();
		}
		
		json_savetofile(project_document, directory_path+"/"+proj_name+".jazzproj");
		
		puts(compiler_selection->get_active_text().c_str());
		response(0);
		hide();
	}
	void NewProjectDialog::OnCancelClicked()
	{
		response(-1);
		hide();
	}
}