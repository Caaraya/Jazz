#include "jazz_sourceview.hpp"
#include <gtksourceview/gtksourcebuffer.h>
namespace Jazz
{
	SourceView::SourceView():
		Gtk::ScrolledWindow(), source_view(GTK_SOURCE_VIEW(gtk_source_view_new()))
	{	
		add(*Glib::wrap(GTK_WIDGET(source_view)));
		
		show_all();
	}
	GtkSourceBuffer* SourceView::get_buffer() const
	{
		return GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(source_view)));
	}
	GtkSourceView* SourceView::get_sourceview() const
	{
		return source_view;
	}
}
