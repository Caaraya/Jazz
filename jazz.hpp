#include <gtkmm.h>
#include <gtksourceview/gtksourceview.h>
namespace Jazz
{
	class JazzIDE : public Gtk::Window
	{
	public:
		JazzIDE();
		~JazzIDE();
	private:
		Gtk::Box			box;
		Gtk::Notebook notebook;
		Gtk::MenuBar*	menubar;
		Gtk::Toolbar*	toolbar;
	private:
		void NewFile();
		void SaveFile();
		void OpenFile();
		void Quit();
		void ChooseFont();
		void SetNewPageFont();
	private:
		GtkSourceLanguageManager* language_manager;
	};
}
