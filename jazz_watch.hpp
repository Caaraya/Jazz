#pragma once
#include <gtkmm.h>

namespace Jazz
{
	class WatchTreeView : public Gtk::ScrolledWindow
	{
	public:
		WatchTreeView();
		void AddData(Glib::ustring filename, 
		Glib::ustring variable_name, Glib::ustring variable_value);
	private:
		struct ColumnModel : public Gtk::TreeModel::ColumnRecord
		{
			ColumnModel();
			Gtk::TreeModelColumn<Glib::ustring> filename;
			Gtk::TreeModelColumn<Glib::ustring> variable_name;
			Gtk::TreeModelColumn<Glib::ustring> variable_value;
		};
	private:
		Glib::RefPtr<Gtk::TreeStore> watch_tree_store;
		Gtk::TreeView watch_tree_view;
		ColumnModel watch_columns;	
	};
	class WatchWindow : public Gtk::Window
	{
	public:
		WatchWindow();
	private:
		WatchTreeView tree_view;
	};
}