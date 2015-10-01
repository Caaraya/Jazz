#pragma once
#include <gtkmm.h>
#include <string>

namespace Jazz
{
	class FileTreeModelColumns : public Gtk::TreeModel::ColumnRecord
	{
		public:
		FileTreeModelColumns()
		{add(filename);}
		Gtk::TreeModelColumn<Glib::ustring> filename;
	};
	class FileTree : public Gtk::ScrolledWindow
	{
	public:
		FileTree(const std::string& path);
	private:
		void DrawTree(const std::string& path, const Gtk::TreeModel::Row& parent);
		void OpenSelected(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn*);
	private:
		Glib::RefPtr<Gtk::TreeStore> tree_store;
		Gtk::TreeView tree_view;
		FileTreeModelColumns column;
		std::string path;
		Glib::RefPtr<Gio::Cancellable> cancellation_token;
	};
}
