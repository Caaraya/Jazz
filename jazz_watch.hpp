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
		class TreeView : public Gtk::TreeView
		{
		public:
			TreeView();
		private:
			// Override signal handlere
			bool on_button_press_event(GdkEventButton*) override;
			// Signal handler for  popup menu items
			void on_menu_file_popup_generic();
		private:
			Gtk::Menu ctxt_menu;	
		};
	private:
		Glib::RefPtr<Gtk::TreeStore> watch_tree_store;
		TreeView watch_tree_view;
		ColumnModel watch_columns;
		Gtk::Menu ctxt_menu;	
	};
	class WatchWindow : public Gtk::Window
	{
	public:
		WatchWindow();
	private:
		WatchTreeView tree_view;
	};
}