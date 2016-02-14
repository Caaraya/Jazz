#include <gtkmm.h>
namespace Jazz
{
	void ShowMessage(const Glib::ustring& s, Gtk::Window* parent)
	{
		Gtk::MessageDialog* msg = new Gtk::MessageDialog(s);
		if(parent)
			msg->set_transient_for(*parent);
		msg->run();
		delete msg;
	}
	bool FileExists(const Glib::ustring& filepath)
	{
		Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(filepath);
		return file->query_exists();
	}
}