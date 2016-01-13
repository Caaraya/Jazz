#pragma once
#include <gtkmm.h>
#include "coralscript/include/jsonparse.hh"
namespace Jazz
{
	class NewProjectDialog : public Gtk::Dialog
	{
	public:
		NewProjectDialog(GtkDialog*, const Glib::RefPtr<Gtk::Builder>&);
		virtual ~NewProjectDialog();
	public:
		void OnOkClicked();
		void OnCancelClicked();
		coral::zircon::object GetProject();
	private:
		void ResetView();
	private:
		coral::zircon::object project_document;
		Gtk::Entry* name_entry = nullptr;
		Gtk::FileChooserButton* directory_entry = nullptr;
		Gtk::ComboBoxText* compiler_selection = nullptr;
		Gtk::ComboBoxText* debugger_selection = nullptr;
		Gtk::CheckButton* create_proj_dir = nullptr;
	};
}