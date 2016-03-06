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
		void ShowLineNumbers(bool show);
		bool IsShowingLineNumbers() const;
		void ShowRightMargin(bool show);
		bool IsShowingRightMargin() const;
		void HighlightCurrentLine(bool light);
		bool IsHighlightingCurrentLine() const;
		void ScrollToLine(int);
		GtkSourceView* GetSourceView() const;
		GtkSourceBuffer* GetSourceBuffer() const;
		GtkTextIter GetTextIterAtLine(const int);
		// Which line, a name and a category
		GtkSourceMark* CreateMarkAtLine(const int, const Glib::ustring&, const Glib::ustring&);
	private:
		GtkSourceView* source_view = nullptr;
		GtkSourceBuffer* source_buffer = nullptr;
		GtkSourceCompletion* completion = nullptr;
		GtkSourceCompletionWords* completion_words = nullptr;
	private:
		void UserAction();
	};
}
