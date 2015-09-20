#pragma once
#include <gtkmm.h>

namespace Jazz
{
	class FileTree : public Gtk::ScrolledWindow
	{
	public:
		FileTree(const std::string&);
	private:
		Glib::RefPtr<Gtk::TreeStore> tree_store;
		Gtk::TreeView tree_view;
	};
}
