#include "jazz_newproj_dialog.hpp"
#include "jazz_msgbox.hpp"
#include "coralscript/include/jsonparse.hh"

#include <iostream>
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
		
		Glib::ustring directory_name = directory_entry->get_text();
		if(directory_name.empty())
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
		
		std::cout << proj_name << " " << directory_name << std::endl;
		
		using namespace coral::zircon;
		
		object document = table();
		document["project_name"] = std::string(proj_name);
		document["compiler"] = table();
		document["compiler"]["command"] = std::string(compiler_name);
		document["compiler"]["options"] = std::string("-std=c++14 -c -Os");
		document["linker"] = table();
		document["linker"]["command"] = std::string(compiler_name);
		
		json_savetofile(document, directory_name+"\\"+proj_name+".jazzproj");
		
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