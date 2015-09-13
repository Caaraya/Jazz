#include <gtksourceview/gtksourceview.h>

namespace Jazz
{
	class SourceView
	{
	public:
		SourceView(bool scrolled = true);
		GtkWidget* gobj();
		GtkSourceView* get_sourceview() const;
		GtkScrolledWindow* get_scrolledwindow() const;
		GtkWidget* get_view() const;
		GtkSourceBuffer* get_buffer() const;
	private:
		GtkWidget* actual_view = nullptr;
		GtkSourceView* source_view = nullptr;
		GtkScrolledWindow* scrolled_win = nullptr;
	};
}
