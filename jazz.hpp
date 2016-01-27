#include <gtkmm.h>
#include <gtksourceview/gtksourceview.h>
#include "jazz_filetree.hpp"
#include "jazz_projecttree.hpp"
#include "coralscript/include/jsonparse.hh"
typedef struct WrenVM WrenVM;

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
      Gtk::Notebook right_pane;
		Gtk::MenuBar*	menubar;
		Gtk::Toolbar*	toolbar;
      Glib::RefPtr<Gtk::Builder> builder;
		FileTree file_tree;
		coral::zircon::object project_doc;
      ProjectTreeView project_tree;
	private:
		void Save(const Glib::ustring&);
		void NewFile();
		void NewProject();
		void SaveFile();
		void SaveFileAs();
		void OpenFile();
        void OpenProject();
		void OpenFileFromTree(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
		void Quit();
		void ChooseFont();
		void SetNewPageFont();
		void BindWren();
	private:
		GtkSourceLanguageManager* language_manager;
	};
}
