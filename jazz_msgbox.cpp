#include <gtkmm.h>
namespace Jazz
{
	void ShowMessage(const Glib::ustring& s)
	{
		Gtk::MessageDialog* msg = new Gtk::MessageDialog(s);
		msg->run();
		delete msg;
	}
}