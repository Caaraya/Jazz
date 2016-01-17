#include <gtkmm.h>

namespace Jazz
{
	class Terminal : Gtk::TextView
	{
	public:
		Terminal();
		void Update();
		void Clear();
	private:
		Glib::RefPtr<Gtk::TextBuffer> buffer;
	};
}