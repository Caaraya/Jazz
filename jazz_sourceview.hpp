#include <gtksourceview/gtksourceview.h>
#include <gtkmm.h>
namespace Jazz
{
	class SourceView : public Gtk::ScrolledWindow
	{
	public:
		SourceView();
		GtkSourceView* get_sourceview() const;
		GtkSourceBuffer* get_buffer() const;
	private:
		GtkSourceView* source_view = nullptr;
	};
}
