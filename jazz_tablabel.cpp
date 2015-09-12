#include <gtkmm.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksource.h>
#include <cstdio>
#include <string>
#include <iostream>
#include "jazz_tablabel.hpp"

namespace Jazz
{
	int TabLabel::nth_tab = 1;
	
	TabLabel::TabLabel(Gtk::Widget& child_ref):
		TabLabel(std::string("Page: ")+std::to_string(nth_tab++), child_ref)
	{
	}
	
	TabLabel::TabLabel(const std::string& title, Gtk::Widget& child_ref) :
		Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL)
	{
		std::string shortname = "";
		if(title.find("\\")!=std::string::npos)
			shortname = title.substr(title.find_last_of("\\")+1);
		else
			shortname = title;
				
		text_label.set_text(shortname);
		
		close_button.set_image_from_icon_name("window-close");
		close_button.set_relief(Gtk::ReliefStyle::RELIEF_NONE);
		close_button.set_focus_on_click(false);
		
		pack_start(text_label, true, true);
		pack_end(close_button, true, true);
		
		show_all();		
	}
}
