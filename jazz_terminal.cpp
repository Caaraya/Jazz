#include "jazz_terminal.hpp"

namespace Jazz
{
	Terminal::Terminal(): Gtk::TextView(), buffer(get_buffer())
	{
		set_editable(false);
		set_wrap_mode(Gtk::WrapMode::WRAP_CHAR);
		set_monospace(true);
	}
	void Terminal::Update()
	{
		
	}
	void Terminal::Clear()
	{
		buffer->set_text("");
	}
}