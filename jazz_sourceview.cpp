#include "jazz_sourceview.hpp"

namespace Jazz
{
	SourceView::SourceView(bool scrolled)
	{
		GtkWidget* source_view_obj = gtk_source_view_new();
	
		if(scrolled)
		{
			//Jazz::Gtk::ScrolledWindow scrolled_window;
			GtkWidget* scrolled_window = gtk_scrolled_window_new(0,0);
			
			//scrolled_window.Add(source_view);
			//scrolled_window.ShowAll();
			gtk_container_add(GTK_CONTAINER(scrolled_window), source_view_obj);
			gtk_widget_show_all(scrolled_window);
		
			source_view = scrolled_window;
		}
		source_view = source_view_obj;
	}
	GtkWidget* SourceView::gobj()
	{
		return source_view;
	}
}
