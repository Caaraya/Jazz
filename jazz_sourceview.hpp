#include <gtksourceview/gtksourceview.h>

namespace Jazz
{
	class SourceView
	{
	public:
		SourceView(bool scrolled = true);
		GtkWidget* gobj();
	private:
		GtkWidget* source_view = nullptr;
	};
}
