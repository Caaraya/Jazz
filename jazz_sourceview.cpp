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
		gtk_source_buffer_set_highlight_matching_brackets(GetSourceBuffer(), TRUE);
		ShowLineNumbers(true);
		
		show_all();
	}
	SourceView::SourceView(GtkSourceBuffer* source_buffer):
		Gtk::ScrolledWindow(), source_view(GTK_SOURCE_VIEW(gtk_source_view_new_with_buffer(GTK_SOURCE_BUFFER(source_buffer))))
	{	
		add(*Glib::wrap(GTK_WIDGET(source_view)));
		
		completion = gtk_source_view_get_completion(source_view);
		completion_words = gtk_source_completion_words_new("Suggestions", nullptr);
		gtk_source_completion_words_register(completion_words, gtk_text_view_get_buffer(GTK_TEXT_VIEW(source_view)));
		gtk_source_completion_add_provider(completion,
			GTK_SOURCE_COMPLETION_PROVIDER(completion_words), nullptr);
		gtk_source_buffer_set_highlight_matching_brackets(GetSourceBuffer(), TRUE);
		ShowLineNumbers(true);
		
		show_all();
	}
	SourceView::~SourceView()
	{
		g_object_unref(completion_words);
	}
	
	GtkSourceBuffer* SourceView::GetSourceBuffer() const
	{
		return GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(source_view)));
	}
	GtkSourceView* SourceView::GetSourceView() const
	{
		return source_view;
	}
	GtkTextIter SourceView::GetTextIterAtLine(const int num)
	{
		GtkTextIter iter;
		gtk_text_buffer_get_iter_at_line(
			GTK_TEXT_BUFFER(GetSourceBuffer()),
			&iter, num
		);
		return iter;
	}
	void SourceView::ScrollToLine(int line)
	{
		GtkTextIter iter = GetTextIterAtLine(line);
		gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(GetSourceView()), &iter, 0.0, FALSE, 1.0, 1.0);
	}
}
