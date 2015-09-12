#include <gtkmm.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksource.h>
#include <cstdio>
#include <string>
#include <iostream>

namespace Jazz
{
class JazzIDE : public Gtk::Window
{
public:
	JazzIDE();
	~JazzIDE();
private:
	Gtk::Box			box;
	Gtk::Notebook notebook;
	Gtk::MenuBar*	menubar;
	Gtk::Toolbar 	toolbar;
private:
	void NewFile();
	void SaveFile();
	void OpenFile();
private:
	GtkWidget* new_tab_label(const std::string&, GtkWidget* child_held);
	GtkWidget* new_sourceview(bool scrollable = true);
private:
	int nth_tab = 1;
};
}
