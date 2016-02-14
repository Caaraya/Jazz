#pragma once
#include <gtkmm.h>
#include <string>

namespace Jazz
{
	struct FileTreeModelColumns : public Gtk::TreeModel::ColumnRecord
	{
		FileTreeModelColumns()
		{
			add(filename);
		}
		Gtk::TreeModelColumn<Glib::ustring> filename;
	};
	class FileTree : public Gtk::ScrolledWindow
	{
	public:
		FileTree(const Glib::ustring& path);
		Gtk::TreeView& TreeView()
		{
			return tree_view;
		}
		FileTreeModelColumns& Columns()
		{
			return column;
		}
		Glib::RefPtr<Gtk::TreeStore> TreeStore() const
		{
			return tree_store;
		}
        const Glib::ustring& CurrentPath() const
        {
            return path;
        }
		Gtk::TreeModel::Row Root()
		{
			return root;
		}
        void SetPath(const Glib::ustring&, bool clear = true);
	private:
		void DrawTree(const std::string& path, const Gtk::TreeModel::Row& parent);
	private:
		Glib::RefPtr<Gtk::TreeStore> tree_store;
		Gtk::TreeView tree_view;
		FileTreeModelColumns column;
        Glib::ustring path;
		Glib::RefPtr<Gio::Cancellable> cancellation_token;
		Gtk::TreeModel::Row root;
	};
}
