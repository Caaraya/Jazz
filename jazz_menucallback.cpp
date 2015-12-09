#include "jazz.hpp"
#include "jazz_sourceview.hpp"
#include "jazz_newproj_dialog.hpp"
#include <gtksourceview/gtksource.h>

namespace Jazz
{
	void JazzIDE::SetNewPageFont()
	{
		int pages = notebook.get_n_pages();
		
		if(pages > 1)
		{
			Gtk::Widget* page = notebook.get_nth_page(-1);
			
			Gtk::Widget* first_page = notebook.get_nth_page(1);
			
			if(auto scrld = dynamic_cast<Gtk::ScrolledWindow*>(page))
				page = scrld->get_child();
				
			if(auto scrld = dynamic_cast<Gtk::ScrolledWindow*>(first_page))
				first_page = scrld->get_child();
				
			auto context = first_page->get_pango_context();
			
			auto font_des = context->get_font_description();
			
			page->override_font(font_des);
		}
	}
	void JazzIDE::NewFile()
	{
		SourceView* source_view = Gtk::manage(new SourceView);
	
		TabLabel* tablabel = Gtk::manage(new TabLabel(*source_view));
	
		notebook.append_page(*source_view, *tablabel);
		
		notebook.set_current_page(-1);
		
		SetNewPageFont();
	}
	void JazzIDE::OpenFile()
	{		
		// Open a file or cancel it :P
		GtkWidget* dialog = gtk_file_chooser_dialog_new("Open File",
			GTK_WINDOW(gobj()), GTK_FILE_CHOOSER_ACTION_OPEN, "_Open",
			GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
			
		// If a user chose a file and selected "Open"
		if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		{
			gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			
			// Pear down the name to just the short version of the filename eg. this.txt
			std::string shortname = "";
			std::string title = filename;
			if(title.find("/")!=std::string::npos)
				shortname = title.substr(title.find_last_of("/")+1);
			else
				shortname = title;

			// Create a new source view that is scrolled, then get the buffer
			SourceView* source_view = Gtk::manage(new SourceView);
			
			GtkSourceBuffer* buff = source_view->get_buffer(); //sourceview.get_buffer();
	
			// Create a new tab label (box containing label and button) contains a
			// reference to the child the notebook has for it
			TabLabel* tablabel = Gtk::manage(new TabLabel(shortname, *source_view));
			
			tablabel->filename = filename;
	
			// Add the two objects to the notebook
			notebook.append_page(*source_view, *tablabel);
			
			// ----------------------------------------------------------
			// Open the file and load it with the sourcefileloader object
			// ----------------------------------------------------------
			GFile* new_file = g_file_new_for_path(filename);
			GtkSourceFile* new_source_file = gtk_source_file_new();
		
			gtk_source_file_set_location(new_source_file, new_file);
		
			GtkSourceFileLoader* loader = gtk_source_file_loader_new(
				buff, new_source_file);
			
			// Launch an async call to load the buffer with the contents of the file
			gtk_source_file_loader_load_async(
			loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL,	NULL,
			// Function to 'finish' the loading routine
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

			// Set the language of the source buffer for syntax highlighting
			auto s_lang = gtk_source_language_manager_guess_language(
				language_manager,
				filename,
				nullptr);
				
			gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(buff), s_lang);
			
			// Free certain resources
			g_free( filename );
			g_object_unref(new_file);
			
			// Switch the the newly opened page
			notebook.set_current_page(-1);
			
			//Set the font of the newly opened page
			SetNewPageFont();
			
		}
		gtk_widget_destroy(dialog);
	}
    
    TabLabel* JazzIDE::GetTabLabel()
    {
        int pages = notebook.get_n_pages();
		
		if(pages <= 0) return nullptr;
		
		Gtk::Widget* page = notebook.get_nth_page(notebook.get_current_page());
		
		Gtk::Box* box = static_cast<Gtk::Box*>(notebook.get_tab_label(*page));
		
		TabLabel* tablabel = static_cast<TabLabel*>(box);
        
        return tablabel;
    }
    
	void JazzIDE::SaveFile()
	{
		
		TabLabel* tabLabel = GetTabLabel();
        
        if(tabLabel == nullptr) return;
		
		if(!tabLabel->filename.empty())
		{
			Save(tabLabel->filename);
			return;
		}
		SaveFileAs();
		
	}
	void JazzIDE::SaveFileAs()
	{
		TabLabel* tabLabel = GetTabLabel();
        
        if (tabLabel == nullptr) return;
		
		GtkWidget* dialog = gtk_file_chooser_dialog_new("Save File",
			GTK_WINDOW(gobj()), GTK_FILE_CHOOSER_ACTION_SAVE, "_Save",
			GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
		if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		{			
			
			gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER( dialog ));
			
			Save(filename);

			std::string filenm = filename;
			
			tabLabel->filename = filename;

			std::string shortname = filenm.substr(filenm.find_last_of(
				#if defined _WIN32
				"\\"
				#else
				"/"
				#endif
				)+1);
				
			tabLabel->TextLabel().set_text(shortname);

			g_free( filename );
		}
		gtk_widget_destroy(dialog);
	}
	void JazzIDE::Save(const Glib::ustring& filename)
	{
		Gtk::Widget* page = notebook.get_nth_page(notebook.get_current_page());
			
		//Gtk::Box* box = static_cast<Gtk::Box*>(notebook.get_tab_label(*page));
		
		if(auto scrld = dynamic_cast<Gtk::ScrolledWindow*>(page))
			{
				page = scrld->get_child();
				puts("Scrolled Window");
				printf("Page new val: %p\n", page);
			}
			else
				puts("Text View");
		
		GFile* current_file = g_file_new_for_path(filename.c_str());
				
		GtkSourceFile* new_source_file = gtk_source_file_new();
	
		gtk_source_file_set_location(new_source_file, current_file);
		
		GtkSourceBuffer* s_buffer = GTK_SOURCE_BUFFER(
			static_cast<Gtk::TextView*>(page)->get_buffer()->gobj());
		
		GtkSourceFileSaver* new_srcfile_saver = gtk_source_file_saver_new_with_target(
			s_buffer, new_source_file, current_file);
	
		struct save_user_data
		{
			GFile* current_file;
			GtkSourceFileSaver* saver;
		};
		save_user_data* user_data = new save_user_data{current_file, new_srcfile_saver};
		puts("B4 the async launch");
		gtk_source_file_saver_save_async(
		new_srcfile_saver, G_PRIORITY_DEFAULT, NULL, NULL, NULL,	NULL,
		[](GObject* source_obj, GAsyncResult* res, gpointer user_data) -> void {
			GError* error = nullptr;
			save_user_data* data = static_cast<save_user_data*>(user_data);
			gboolean success = gtk_source_file_saver_save_finish(data->saver, res, &error);
			if(success)
				puts("Successfully saved file");
			else
				printf("Failed to save file: %i, %s\n",	error->code,error->message);
					
			g_object_unref(data->current_file);
			delete data;
		},
		// Pass the loader as the user data, so that we can just keep
		// the lambda function as is
		(gpointer)user_data);
			
			//g_object_unref(current_file);
	}
	void JazzIDE::Quit()
	{
		close();
	}
	void JazzIDE::ChooseFont()
	{
		int pages = notebook.get_n_pages();
		
		Gtk::FontChooserDialog dialog("Choose Font", *this);
		
		if(dialog.run() == Gtk::RESPONSE_OK)
		{
			Glib::ustring font_name = dialog.get_font();
			for(int i = 0; i < pages; i++)
			{
				Gtk::Widget* cur_page = notebook.get_nth_page(i);
				if(auto scrld = dynamic_cast<Gtk::ScrolledWindow*>(cur_page))
				{
					cur_page = scrld->get_child();
				}
				cur_page->override_font(Pango::FontDescription(font_name));
			}
		}				
	}
	void JazzIDE::NewProject()
	{
		NewProjectDialog* project_dialog = nullptr;
		builder->get_widget_derived("new_proj_dialog", project_dialog);
		project_dialog->set_transient_for(*this);
		
		int response = project_dialog->run();
		
		switch(response)
		{
		case 0: // Project settings made
			puts("Project configuration created");
			break;
		case -1: // Cancel
			puts("New project canceled");
			break;
		}
	}
	void JazzIDE::AddFileToNotebook(const Glib::ustring&, std::function<void(int)> callback)
	{
		gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		
		// Pear down the name to just the short version of the filename eg. this.txt
		std::string shortname = "";
		std::string title = filename;
		if(title.find("/")!=std::string::npos)
			shortname = title.substr(title.find_last_of("/")+1);
		else
			shortname = title;

		// Create a new source view that is scrolled, then get the buffer
		SourceView* source_view = Gtk::manage(new SourceView);
		
		GtkSourceBuffer* buff = source_view->get_buffer(); //sourceview.get_buffer();

		// Create a new tab label (box containing label and button) contains a
		// reference to the child the notebook has for it
		TabLabel* tablabel = Gtk::manage(new TabLabel(shortname, *source_view));

		// Add the two objects to the notebook
		notebook.append_page(*source_view, *tablabel);
		
		// ----------------------------------------------------------
		// Open the file and load it with the sourcefileloader object
		// ----------------------------------------------------------
		GFile* new_file = g_file_new_for_path(filename);
		GtkSourceFile* new_source_file = gtk_source_file_new();
	
		gtk_source_file_set_location(new_source_file, new_file);
	
		GtkSourceFileLoader* loader = gtk_source_file_loader_new(
			buff, new_source_file);
		
		// Launch an async call to load the buffer with the contents of the file
		gtk_source_file_loader_load_async(
		loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL,	NULL,
		// Function to 'finish' the loading routine
		[](GObject* source_obj, GAsyncResult* res, gpointer loader) -> void {
			GError* error = nullptr;
			gboolean success = gtk_source_file_loader_load_finish(
				(GtkSourceFileLoader*)loader, res, &error);
			if(success)
			{
				puts("Successfully opened file");
				// Switch the the newly opened page
				notebook.set_current_page(-1);
				callback(note)
			}
			else
				printf("Failed to open file: %i, %s\n",
					error->code,error->message);
		}, 
		// Pass the loader as the user data, so that we can just keep
		// the lambda function as is
		(gpointer)loader);

		// Set the language of the source buffer for syntax highlighting
		auto s_lang = gtk_source_language_manager_guess_language(
			language_manager,
			filename,
			nullptr);
			
		gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(buff), s_lang);
		
		// Free certain resources
		g_free( filename );
		g_object_unref(new_file);
		
		//Set the font of the newly opened page
		SetNewPageFont();
	}
}
