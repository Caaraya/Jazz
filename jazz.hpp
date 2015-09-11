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
private:
	Gtk::Box			box;
	Gtk::Notebook notebook;
	Gtk::MenuBar*	menubar;
	Gtk::Menu			filemenu;
	Gtk::Menu			editmenu;
	Gtk::Toolbar 	toolbar;
private:
	void NewFile();
	void SaveFile();
	void OpenFile();
};
}
