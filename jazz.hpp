#pragma once
#include <gtkmm.h>
#include <gtksourceview/gtksourceview.h>
#include "jazz_filetree.hpp"
#include "jazz_watch.hpp"
#include "jazz_gdb.hpp"
#include "jazz_projecttree.hpp"
#include "jazz_terminal.hpp"
#include "jazz_tablabel.hpp"
#include "coralscript/include/jsonparse.hh"

namespace Jazz
{
	class JazzIDE : public Gtk::Window
	{
	public:
		JazzIDE();
		~JazzIDE();
	private:
		Gtk::Box box;
		Gtk::Box h_box;
		Gtk::Notebook notebook;
		Gtk::Notebook right_pane;
		Gtk::MenuBar* menubar;
		Gtk::Toolbar* toolbar;
		Glib::RefPtr<Gtk::Builder> builder;
		FileTree file_tree;
		WatchWindow watch_window;
		coral::zircon::object project_doc;
		ProjectTreeView project_tree;
		Terminal terminal;
	private:
		// Menu callbacks
		TabLabel* GetTabLabel();
		void Save(const Glib::ustring&);
		void NewFile();
		void NewProject();
		void SaveFile();
		void SaveFileAs();
		void OpenFile();
		// String filename
		// Call back with a bool representing success
		void AddFileToNotebook(const Glib::ustring&, std::function<void(bool)>);
		void OpenProject();
		void OpenFileFromTree(const Gtk::TreeModel::Path&, Gtk::TreeViewColumn*);
		void Quit();
		void ChooseFont();
		void SetNewPageFont();
		bool HandleGDBOutput(Glib::IOCondition, const Glib::ustring&);
		void ExecuteProject();
		void DebugContinueCmd();
	private:
		GtkSourceLanguageManager* language_manager;
		std::unique_ptr<GdbInstance> gdb;
	};
}
