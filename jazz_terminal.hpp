#pragma once
#include <gtkmm.h>
namespace Jazz
{
	class Terminal : public Gtk::ScrolledWindow
	{
	public:
		Terminal();
		bool Update(Glib::IOCondition, const Glib::ustring&);
		void Clear();
		void AddText(const Glib::ustring&);
	private:
		Gtk::TextView* text_view;
		Glib::RefPtr<Gtk::TextBuffer> buffer;
	};
}