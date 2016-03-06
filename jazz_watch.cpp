#include "jazz.hpp"

namespace Jazz
{
	WatchTreeView::WatchTreeView()
	{
		add(watch_tree_view);
		watch_tree_view.append_column("Filename", watch_columns.filename);
		watch_tree_view.append_column("Name", watch_columns.variable_name);
		watch_tree_view.append_column("Value", watch_columns.variable_value);
		watch_tree_store = Gtk::TreeStore::create(watch_columns);
		watch_tree_view.set_model(watch_tree_store);
		AddData("filename", "variable", "1");
		show_all();
	}
	WatchWindow::WatchWindow()
	{
		add(tree_view);
		show_all();
	}
	void WatchTreeView::AddData(Glib::ustring filename, Glib::ustring variable_name, Glib::ustring variable_value)
	{
		Gtk::TreeModel::Row row = *(watch_tree_store->append());
		row[watch_columns.filename] = filename;
		row[watch_columns.variable_name] = variable_name;
		row[watch_columns.variable_value] = variable_value;
	}
	WatchTreeView::ColumnModel::ColumnModel()
	{
		add(filename);
		add(variable_name);
		add(variable_value);
	}
	WatchTreeView::TreeView::TreeView()
	{
		auto item = Gtk::manage(new Gtk::MenuItem("_Edit", true));
		item->signal_activate().connect(
			sigc::mem_fun(*this, &TreeView::on_menu_file_popup_generic));
		ctxt_menu.append(*item);
		
		item = Gtk::manage(new Gtk::MenuItem("_Process", true));
		item->signal_activate().connect(
    		sigc::mem_fun(*this, &TreeView::on_menu_file_popup_generic));
		
		ctxt_menu.accelerate(*this);
		ctxt_menu.show_all();
	}
	bool WatchTreeView::TreeView::on_button_press_event(GdkEventButton* btn)
	{
		bool return_value = false;
		return_value = Gtk::TreeView::on_button_press_event(btn);
		if((btn->type == GDK_BUTTON_PRESS) && (btn->button == 3))
			ctxt_menu.popup(btn->button, btn->time);
		return return_value;
	}
	void WatchTreeView::TreeView::on_menu_file_popup_generic()
	{
		puts("A popup menu item was selected.");
		auto selection = get_selection();
		if(selection)
		{
			auto iter = selection->get_selected();
			if(iter)
			{
				//int id = (*iter)[]
			}
		}
		
	}
}
