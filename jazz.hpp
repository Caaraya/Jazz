#pragma once
#include <gtkmm.h>
#include <gtksourceview/gtksourceview.h>
#include "jazz_filetree.hpp"
#include "jazz_projecttree.hpp"
#include  "jazz_tabLabel.hpp"
#include "coralscript/include/jsonparse.hh"
#include "jazz_watch.hpp"
#include "jazz_gdb.hpp"

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
		WatchWindow watch_window;
	private:
        TabLabel* GetTabLabel();
		void Save(const Glib::ustring&);
		// Menu callbacks
		void NewFile();
		void NewProject();
		void SaveFile();
		void SaveFileAs();
		void OpenFile();
        void OpenProject();
		// String filename
		// Call back with an int representing the new tab
		void AddFileToNotebook(const Glib::ustring&, std::function<void(bool)>);
		void OpenFileFromTree(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
		void Quit();
		void ChooseFont();
		void SetNewPageFont();
		void BindWren();
		bool HandleGDBOutput(Glib::IOCondition, const Glib::ustring&);
	private:
		GtkSourceLanguageManager* language_manager;
		
		GdbInstance* gdb = new GdbInstance("./test");
	};
}
