#pragma once
#include <gtkmm.h>

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
	private:
		Gtk::Entry* name_entry = nullptr;
		Gtk::Entry* directory_entry = nullptr;
		Gtk::ComboBoxText* compiler_selection = nullptr;
	};
}