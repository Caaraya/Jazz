#include "jazz.hpp"

namespace Jazz
{
	WatchTree::WatchTree()
	{
		add(watch_tree_view);
		
		watch_tree_view.append_column("Filename", watch_columns.filename);
		watch_tree_view.append_column("Name", watch_columns.variable_name);
		watch_tree_view.append_column("Value", watch_columns.variable_value);
		
		watch_tree_store = Gtk::TreeStore::create(watch_columns);
		
		watch_tree_view.set_model(watch_tree_store);
		// apending some rows
		//Gtk::TreeModel::Row row = *(watch_tree_store->append());
		//row[watch_columns.filename] = "filename";
		//row[watch_columns.variable_name] = "variable";
		//row[watch_columns.variable_value] = "1";
		AddData("filename", "variable", "1");
		show_all();
	}
	WatchWindow::WatchWindow()
	{
		add(tree_view);
		show_all();
	}
	WatchWindow::~WatchWindow()
	{
		
	}
	void WatchTree::AddData(Glib::ustring filename, Glib::ustring variable_name, Glib::ustring variable_value)
	{
		Gtk::TreeModel::Row row = *(watch_tree_store->append());
		row[watch_columns.filename] = filename;
		row[watch_columns.variable_name] = variable_name;
		row[watch_columns.variable_value] = variable_value;
	}
}
