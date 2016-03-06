#include "jazz_terminal.hpp"

namespace Jazz
{
	Terminal::Terminal(int max_lines, int remove_lines): 
		text_view(Gtk::manage(new Gtk::TextView)),
		buffer(text_view->get_buffer()),
		buffer_max_lines(max_lines), buffer_remove_lines(remove_lines)
	{
		text_view->set_editable(false);
		text_view->set_wrap_mode(Gtk::WrapMode::WRAP_CHAR);
		text_view->set_monospace(true);
		
		add(*text_view);
		show_all();
	}
	bool Terminal::Update(Glib::IOCondition, const Glib::ustring& text)
	{
		if(buffer_max_lines > 0 && buffer->get_line_count() > buffer_max_lines)
		{
			buffer->erase(buffer->begin(), buffer_remove_lines > 0 ?
				buffer->get_iter_at_line(buffer_remove_lines) :
				buffer->get_iter_at_line(buffer->get_line_count()/2));
		}
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
	void Terminal::SetCutoff(int buffer_no_lines, int no_lines_remove)
	{
		buffer_max_lines = buffer_no_lines;
		buffer_remove_lines = no_lines_remove;
	}
}