#include <gtksourceview/gtksourceview.h>
#include <gtkmm.h>
#include <gtksourceview/completion-providers/words/gtksourcecompletionwords.h>
namespace Jazz
{
	class SourceView : public Gtk::ScrolledWindow
	{
	public:
		SourceView();
		SourceView(GtkSourceBuffer*);
		~SourceView();
		void ShowLineNumbers(bool show)
		{
			gtk_source_view_set_show_line_numbers(source_view, static_cast<gboolean>(show));
		}
		bool IsShowingLineNumbers() const
		{
			return static_cast<bool>(gtk_source_view_get_show_line_numbers(source_view));
		}
		void ShowRightMargin(bool show)
		{
			gtk_source_view_set_show_right_margin(source_view, static_cast<gboolean>(show));
		}
		bool IsShowingRightMargin() const
		{
			return static_cast<bool>(gtk_source_view_get_show_right_margin(source_view));
		}
		void HighlightCurrentLine(bool light)
		{
			gtk_source_view_set_highlight_current_line(source_view, static_cast<gboolean>(light));
		}
		bool IsHighlightingCurrentLine() const
		{
			return static_cast<bool>(gtk_source_view_get_highlight_current_line(source_view));
		}
		void ScrollToLine(int);
		GtkSourceView* GetSourceView() const;
		GtkSourceBuffer* GetSourceBuffer() const;
		GtkTextIter GetTextIterAtLine(const int);
		// Which line, a name and a category
		GtkSourceMark* CreateMarkAtLine(const int, const Glib::ustring&, const Glib::ustring&);
	private:
		GtkSourceView* source_view = nullptr;
		GtkSourceCompletion* completion = nullptr;
		GtkSourceCompletionWords* completion_words = nullptr;
	private:
		void UserAction();
	};
}
