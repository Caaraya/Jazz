#include "jazz.hpp"
#include "jazz_tablabel.hpp"
#include "jazz_sourceview.hpp"
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksource.h>

namespace Jazz
{
	void JazzIDE::NewFile()
	{
		SourceView sourceview;
	
		Gtk::Widget* sourceview_wrap = Glib::wrap(sourceview.gobj());
	
		TabLabel* tablabel = Gtk::manage(new TabLabel(*sourceview_wrap));
	
		notebook.append_page(*sourceview_wrap, *tablabel);
		
		notebook.set_current_page(-1);
	}
	void JazzIDE::OpenFile()
	{		
		// Fill out the buffer
		GtkWidget* dialog = gtk_file_chooser_dialog_new("Open File",	GTK_WINDOW(gobj()), 				GTK_FILE_CHOOSER_ACTION_OPEN,
			"_Open", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
			
		if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		{
			gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			
			std::string shortname = "";
			std::string title = filename;
			if(title.find("/")!=std::string::npos)
				shortname = title.substr(title.find_last_of("/")+1);
			else
				shortname = title;

			SourceView sourceview;
		
			GtkSourceBuffer* buff = sourceview.get_buffer();
	
			Gtk::Widget* sourceview_wrap = Glib::wrap(sourceview.gobj());
	
			TabLabel* tablabel = Gtk::manage(new TabLabel(shortname, *sourceview_wrap));
	
			notebook.append_page(*sourceview_wrap, *tablabel);
			
			// ----------------------------------------------------------
			// Open the file and load it with the sourcefileloader object
			// ----------------------------------------------------------
			
			GFile* new_file = g_file_new_for_path(filename);
			GtkSourceFile* new_source_file = gtk_source_file_new();
		
			gtk_source_file_set_location(new_source_file, new_file);
		
			GtkSourceFileLoader* loader = gtk_source_file_loader_new(
				buff, new_source_file);
			
			gtk_source_file_loader_load_async(
			loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL,	NULL,
			[](GObject* source_obj, GAsyncResult* res, gpointer loader) -> void {
				GError* error = nullptr;
				gboolean success = gtk_source_file_loader_load_finish(
					(GtkSourceFileLoader*)loader, res, &error);
				if(success)
					puts("Successfully opened file");
				else
					printf("Failed to open file: %i, %s\n",
						error->code,error->message);
				}, 
				// Pass the loader as the user data, so that we can just keep
				// the lambda function as is
				(gpointer)loader);

			auto s_lang = gtk_source_language_manager_guess_language(
				language_manager,
				filename,
				nullptr);
				
			gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(buff), s_lang);
			
			g_free( filename );
			g_object_unref(new_file);
			
			notebook.set_current_page(-1);
		}
		gtk_widget_destroy(dialog);
	}
	void JazzIDE::SaveFile()
	{
		GtkWidget* dialog = gtk_file_chooser_dialog_new("Save File",	GTK_WINDOW(gobj()), 				GTK_FILE_CHOOSER_ACTION_SAVE,
			"_Save", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
		if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		{
			Gtk::Widget* page = notebook.get_nth_page(notebook.get_current_page());
			
			if(auto scrld = dynamic_cast<Gtk::ScrolledWindow*>(page))
			{
				page = scrld->get_child();
				puts("Scrolled Window");
				printf("Page new val: %p\n", page);
			}
			else
				puts("Text View");
			
			GtkSourceBuffer* s_buffer = GTK_SOURCE_BUFFER(static_cast<Gtk::TextView*>(page)->get_buffer()->gobj());
			
			gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER( dialog ));
			
			GFile* new_file = g_file_new_for_path(filename);
			
			GtkSourceFile* new_source_file = gtk_source_file_new();
		
			gtk_source_file_set_location(new_source_file, new_file);
		
			GtkSourceFileSaver* new_srcfile_saver = gtk_source_file_saver_new_with_target(s_buffer, new_source_file, new_file);
		
			puts("B4 the async launch");
			gtk_source_file_saver_save_async(
			new_srcfile_saver, G_PRIORITY_DEFAULT, NULL, NULL, NULL,	NULL,
			[](GObject* source_obj, GAsyncResult* res, gpointer new_srcfile_saver) -> void {
				GError* error = nullptr;
				puts("Ansync launch b4 finish call");
				gboolean success = gtk_source_file_saver_save_finish(
					(GtkSourceFileSaver*)new_srcfile_saver, res, &error);
				if(success)
					puts("Successfully saved file");
				else
					printf("Failed to save file: %i, %s\n",
						error->code,error->message);
				},
				// Pass the loader as the user data, so that we can just keep
				// the lambda function as is
				(gpointer)new_srcfile_saver);
			
			puts("After the async launch");

			std::string filenm = filename;
			puts(filename);
			std::string shortname = filenm.substr(filenm.find_last_of("/")+1);
			
			Gtk::Box* box = static_cast<Gtk::Box*>(notebook.get_tab_label(*page->get_parent()));
			puts("after creation of box");
			
			if(auto boxxy = dynamic_cast<Gtk::Box*>(notebook.get_tab_label(*page)))
			{
				puts("we got a box");
				printf("box children size: %i\n", boxxy->get_children().size());
			}
			else
			{
				puts("we did not get a box?!?"); 
				printf("Ptr: %p\n", notebook.get_tab_label(*page));
			}
			
			Gtk::Label* label = static_cast<Gtk::Label*>(box->get_children()[0]);
			puts("b4 set text");
			label->set_text(shortname);
			puts("after set text");
			g_object_unref(new_file);
			g_free( filename );
		}
		gtk_widget_destroy(dialog);
	}
}
