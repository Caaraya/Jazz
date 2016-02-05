#include "jazz_terminal.hpp"

namespace Jazz
{
	Terminal::Terminal(): 
		//Gtk::ScrolledWindow(),
		text_view(Gtk::manage(new Gtk::TextView)),
		buffer(text_view->get_buffer())
	{
		text_view->set_editable(false);
		text_view->set_wrap_mode(Gtk::WrapMode::WRAP_CHAR);
		text_view->set_monospace(true);
		
		add(*text_view);
		show_all();
	}
	bool Terminal::Update(Glib::IOCondition, const Glib::ustring& text)
	{
		AddText(text);
		return true;
	}
	void Terminal::Clear()
	{
		buffer->set_text("");
	}
	void Terminal::AddText(const Glib::ustring& text)
	{
		buffer->insert_at_cursor(text);
	}
}