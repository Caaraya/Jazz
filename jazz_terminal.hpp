#pragma once
#include <gtkmm.h>
namespace Jazz
{
	class Terminal : public Gtk::ScrolledWindow
	{
	public:
		Terminal(int max_lines = -1, int remove_lines = -1);
		bool Update(Glib::IOCondition, const Glib::ustring&);
		void Clear();
		void AddText(const Glib::ustring&);
		void SetCutoff(int buffer_no_lines, int no_lines_remove = -1);
	private:
		Gtk::TextView* text_view;
		Glib::RefPtr<Gtk::TextBuffer> buffer;
	private:
		int buffer_max_lines = -1;
		int buffer_remove_lines = -1;
	};
}