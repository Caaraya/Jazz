#include "jazz_sourceview.hpp"
#include <gtksourceview/gtksourcebuffer.h>
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
		
			actual_view = scrolled_window;
			scrolled_win = GTK_SCROLLED_WINDOW(scrolled_window);
			source_view = GTK_SOURCE_VIEW(source_view_obj);
			return;
		}
		actual_view = source_view_obj;
		source_view = GTK_SOURCE_VIEW(source_view_obj);
	}
	GtkWidget* SourceView::gobj()
	{
		return actual_view;
	}
	GtkSourceBuffer* SourceView::get_buffer() const
	{
		return GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(source_view)));
	}
	GtkSourceView* SourceView::get_sourceview() const
	{
		return source_view;
	}
	GtkScrolledWindow* SourceView::get_scrolledwindow() const
	{
		return scrolled_win;
	}
	GtkWidget* SourceView::get_view() const
	{
		return actual_view;
	}
}
