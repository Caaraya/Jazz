#pragma once
namespace Jazz
{
	void ShowMessage(const Glib::ustring&, Gtk::Window* = nullptr);
	bool FileExists(const Glib::ustring&);
}