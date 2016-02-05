#include <gtkmm.h>
namespace Jazz
{
	void ShowMessage(const Glib::ustring& s)
	{
		Gtk::MessageDialog* msg = new Gtk::MessageDialog(s);
		msg->run();
		delete msg;
	}
	bool FileExists(const Glib::ustring& filepath)
	{
		Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(filepath);
		return file->query_exists();
	}
}