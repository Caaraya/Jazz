#include "jazz_sourceview.hpp"
#include <gtksourceview/gtksource.h>
#include <gtksourceview/gtksourcebuffer.h>

namespace Jazz
{
	SourceView::SourceView():
		Gtk::ScrolledWindow(), source_view(GTK_SOURCE_VIEW(gtk_source_view_new()))
	{	
		add(*Glib::wrap(GTK_WIDGET(source_view)));
		
		completion = gtk_source_view_get_completion(source_view);
		completion_words = gtk_source_completion_words_new("Suggestions", nullptr);
		gtk_source_completion_words_register(completion_words, gtk_text_view_get_buffer(GTK_TEXT_VIEW(source_view)));
		gtk_source_completion_add_provider(completion,
			GTK_SOURCE_COMPLETION_PROVIDER(completion_words), nullptr);
		
		show_all();
	}
	SourceView::~SourceView()
	{
		g_object_unref(completion_words);
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
