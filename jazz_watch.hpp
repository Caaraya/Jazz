#pragma once
#include <gtkmm.h>

namespace Jazz
{
	class WatchWindowColumnModel : public Gtk::TreeModel::ColumnRecord
	{
	public:
		WatchWindowColumnModel()
		{
			add(filename);
			add(variable_name);
			add(variable_value);
		}
		Gtk::TreeModelColumn<Glib::ustring> filename;
		Gtk::TreeModelColumn<Glib::ustring> variable_name;
		Gtk::TreeModelColumn<Glib::ustring> variable_value;
	};
	class WatchTree : public Gtk::ScrolledWindow
	{
	public:
		WatchTree();
		void AddData(Glib::ustring filename, 
		Glib::ustring variable_name, Glib::ustring variable_value);
	private:
		Glib::RefPtr<Gtk::TreeStore> watch_tree_store;
		Gtk::TreeView watch_tree_view;
		WatchWindowColumnModel watch_columns;	
	};
	class WatchWindow : public Gtk::Window
	{
	public:
		WatchWindow();
		~WatchWindow();
	private:
		WatchTree tree_view;
	};
}