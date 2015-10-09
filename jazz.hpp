#include <gtkmm.h>
#include <gtksourceview/gtksourceview.h>
#include "jazz_filetree.hpp"
namespace Jazz
{
	class JazzIDE : public Gtk::Window
	{
	public:
		JazzIDE();
		~JazzIDE();
	private:
		Gtk::Box			box;
		Gtk::Box			h_box;
		Gtk::Notebook notebook;
		Gtk::MenuBar*	menubar;
		Gtk::Toolbar*	toolbar;
		FileTree file_tree;
	private:
		void NewFile();
		void SaveFile();
		void OpenFile();
		void OpenFileFromTree(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
		void Quit();
		void ChooseFont();
		void SetNewPageFont();
		void BindChaiscript();
	private:
		GtkSourceLanguageManager* language_manager;
	};
}
