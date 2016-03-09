#pragma once
#include <gtkmm.h>

namespace Jazz
{
	class WatchTreeView : public Gtk::ScrolledWindow
	{
	public:
		WatchTreeView();
		void AddData(Glib::ustring filename, Glib::ustring var_name, Glib::ustring var_value);
	private:
		class TreeView : public Gtk::TreeView
		{
		public:
			TreeView();
		public:
			void AddRow(const Glib::ustring&, const Glib::ustring&, const Glib::ustring&);
		private:
			// Override signal handlere
			bool on_button_press_event(GdkEventButton*) override;
			// Signal handler for  popup menu items
			void on_menu_file_popup_generic();
		private:
			struct ColumnModel : public Gtk::TreeModel::ColumnRecord
			{
				ColumnModel();
				Gtk::TreeModelColumn<Glib::ustring> filename;
				Gtk::TreeModelColumn<Glib::ustring> variable_name;
				Gtk::TreeModelColumn<Glib::ustring> variable_value;
			};
		private:
			ColumnModel watch_columns;
			Glib::RefPtr<Gtk::TreeStore> watch_tree_store;
			Gtk::Menu ctxt_menu;
		};
	private:
		TreeView watch_tree_view;
	};
	class WatchWindow : public Gtk::Window
	{
	public:
		WatchWindow();
	private:
		WatchTreeView tree_view;
	};
}