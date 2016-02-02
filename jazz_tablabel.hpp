#pragma once
#include <gtkmm.h>

namespace Jazz
{
	class TabLabel : public Gtk::Box
	{
	public:
		TabLabel(Gtk::Widget&);
		TabLabel(const std::string&, Gtk::Widget&);
	public:
		Gtk::Button& CloseButton();
		Gtk::Label&	 TextLabel();
	public:
		Glib::ustring filename = "";
	private:
		Gtk::Button close_button;
		Gtk::Label	text_label;
		Gtk::Widget*child_ref;
	private:
		void Close();
	private:
		static int nth_tab;
	};
}
