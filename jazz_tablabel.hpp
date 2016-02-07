#pragma once
#include <gtkmm.h>

namespace Jazz
{
	class TabLabel : public Gtk::Box
	{
	public:
		TabLabel(Gtk::Widget&);
		TabLabel(const std::string&, Gtk::Widget&, const Glib::ustring&);
	public:
		Gtk::Button& CloseButton();
		Gtk::Label&	 TextLabel();
		const Glib::ustring& FullPath() const
		{
			return full_path;
		}
		void FullPath(const Glib::ustring& path)
		{
			full_path = path;
		}
	private:
		Gtk::Button close_button;
		Gtk::Label	text_label;
		Gtk::Widget*  child_ref;
		Glib::ustring full_path;
	private:
		void Close();
	private:
		static int nth_tab;
	};
}
