#pragma once
#include <gtkmm.h>
#include "coralscript/include/jsonparse.hh"
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
		ProjectTreeView();
		ProjectTreeView(coral::zircon::object&);
		void SetProject(coral::zircon::object&);
		Gtk::TreeView& TreeView()
		{
			return tree_view;
		}
		ProjectTreeModelColumns& Columns()
		{
			return column;
		}
		Glib::RefPtr<Gtk::TreeStore> TreeStore() const
		{
			return tree_store;
		}
	private:
		Glib::RefPtr<Gtk::TreeStore> tree_store;
		Gtk::TreeView tree_view;
		ProjectTreeModelColumns column;
		Glib::ustring path;
		Glib::RefPtr<Gio::Cancellable> cancellation_token;
	};
}