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
		GtkSourceView* GetSourceView() const;
		GtkSourceBuffer* GetSourceBuffer() const;
	private:
		GtkSourceView* source_view = nullptr;
		GtkSourceCompletion* completion = nullptr;
		GtkSourceCompletionWords* completion_words = nullptr;
	private:
		void UserAction();
	};
}
