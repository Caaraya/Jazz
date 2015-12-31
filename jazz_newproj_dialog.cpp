#include "jazz_newproj_dialog.hpp"
#include "jazz_msgbox.hpp"

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
		Glib::ustring text = name_entry->get_text();
		if(text.empty())
		{
			ShowMessage("Project name must not be empty");
			return;
		}
		
		text = directory_entry->get_text();
		if(text.empty())
		{
			ShowMessage("Project location must not be empty");
			return;
		}
		
		if(compiler_selection->get_active_row_number() == -1)
		{
			ShowMessage("A primary compiler must be selected");
			return;
		}
		
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