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
		gtk_source_buffer_set_highlight_matching_brackets(source_buffer, TRUE);
		gtk_source_view_set_show_line_marks(source_view, TRUE);
		ShowLineNumbers(true);
		
		show_all();
	}
	SourceView::SourceView(GtkSourceBuffer* source_buffer):
		Gtk::ScrolledWindow(), source_view(GTK_SOURCE_VIEW(gtk_source_view_new_with_buffer(
			GTK_SOURCE_BUFFER(source_buffer)))), source_buffer(GTK_SOURCE_BUFFER(source_buffer))
	{	
		add(*Glib::wrap(GTK_WIDGET(source_view)));
		
		completion = gtk_source_view_get_completion(source_view);
		completion_words = gtk_source_completion_words_new("Suggestions", nullptr);
		gtk_source_completion_words_register(completion_words, gtk_text_view_get_buffer(GTK_TEXT_VIEW(source_view)));
		gtk_source_completion_add_provider(completion,
			GTK_SOURCE_COMPLETION_PROVIDER(completion_words), nullptr);
		gtk_source_buffer_set_highlight_matching_brackets(source_buffer, TRUE);
		gtk_source_view_set_show_line_marks(source_view, TRUE);
		ShowLineNumbers(true);
		
		show_all();
	}
	SourceView::~SourceView()
	{
		g_object_unref(completion_words);
	}
	void SourceView::ShowLineNumbers(bool show)
	{
		gtk_source_view_set_show_line_numbers(source_view, static_cast<gboolean>(show));
	}
	bool SourceView::IsShowingLineNumbers() const
	{
		return static_cast<bool>(gtk_source_view_get_show_line_numbers(source_view));
	}
	void SourceView::ShowRightMargin(bool show)
	{
		gtk_source_view_set_show_right_margin(source_view, static_cast<gboolean>(show));
	}
	bool SourceView::IsShowingRightMargin() const
	{
		return static_cast<bool>(gtk_source_view_get_show_right_margin(source_view));
	}
	void SourceView::HighlightCurrentLine(bool light)
	{
		gtk_source_view_set_highlight_current_line(source_view, static_cast<gboolean>(light));
	}
	bool SourceView::IsHighlightingCurrentLine() const
	{
		return static_cast<bool>(gtk_source_view_get_highlight_current_line(source_view));
	}
	void SourceView::ScrollToLine(int line)
	{
		GtkTextIter iter = GetTextIterAtLine(line);
		gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(GetSourceView()),
			&iter, 0.0, FALSE, 1.0, 1.0);
	}
	GtkSourceBuffer* SourceView::GetSourceBuffer() const
	{
		return source_buffer;
	}
	GtkSourceView* SourceView::GetSourceView() const
	{
		return source_view;
	}
	GtkTextIter SourceView::GetTextIterAtLine(const int num)
	{
		GtkTextIter iter;
		gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(source_buffer), &iter, num);
		return iter;
	}
	GtkSourceMark* SourceView::CreateMarkAtLine(
		const int line,
		const Glib::ustring& name,
		const Glib::ustring& category)
	{
		auto text_iter = GetTextIterAtLine(line);
		return gtk_source_buffer_create_source_mark(
			GetSourceBuffer(), name.c_str(), category.c_str(), &text_iter);
	}
}
