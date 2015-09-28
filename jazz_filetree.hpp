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
		FileTree(const std::string&);
		DrawTree(const std::string&, Gtk::TreeModel::Row*);
	private:
		Glib::RefPtr<Gtk::TreeStore> tree_store;
		Gtk::TreeView tree_view;
		FileTreeModelColumns column;
		std::string path;
		Glib::RefPtr<Gio::Cancellable> cancellation_token;
	};
}
