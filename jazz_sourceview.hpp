#include <gtksourceview/gtksourceview.h>
#include <gtkmm.h>
#include <gtksourceview/completion-providers/words/gtksourcecompletionwords.h>
namespace Jazz
{
	class SourceView : public Gtk::ScrolledWindow
	{
	public:
		SourceView();
		~SourceView();
		GtkSourceView* get_sourceview() const;
		GtkSourceBuffer* get_buffer() const;
	private:
		GtkSourceView* source_view = nullptr;
		GtkSourceCompletion* completion = nullptr;
		GtkSourceCompletionWords* completion_words = nullptr;
	private:
		void UserAction();
	};
}
