#include <gtkmm.h>

namespace Jazz
{
	class Terminal : public Gtk::ScrolledWindow
	{
	public:
		Terminal();
		void Update();
		void Clear();
		void AddText(const Glib::ustring&);
	private:
		Gtk::TextView* text_view;
		Glib::RefPtr<Gtk::TextBuffer> buffer;
	};
}