#pragma once
#include <gtkmm.h>

namespace Jazz
{
	class ProjectTreeModelColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:
		ProjectTreeModelColumns();
		Gtk::TreeModelColumn<Glib::ustring> filename;
	};
	class ProjectTreeView : public Gtk::ScrolledWindow
	{
	public:
		ProjectTreeView(const Glib::ustring& path);
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
	private:
		void DrawTree(const std::string& path, const Gtk::TreeModel::Row& parent);
	private:
		Glib::RefPtr<Gtk::TreeStore> tree_store;
		Gtk::TreeView tree_view;
		FileTreeModelColumns column;
		Glib::ustring path;
		Glib::RefPtr<Gio::Cancellable> cancellation_token;
	};
}