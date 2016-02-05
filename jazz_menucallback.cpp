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
			
			AddFileToNotebook(filename, [this](bool success){
				if(success)
					notebook.set_current_page(-1);
			});
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
	// Pass in a function object as the callback, the callback will accept a bool indicating success
	void JazzIDE::AddFileToNotebook(const Glib::ustring& fname, std::function<void(bool)> callback)
	{		
		GtkSourceBuffer* buff = gtk_source_buffer_new(NULL);	
		// ----------------------------------------------------------
		// Open the file and load it with the sourcefileloader object
		// ----------------------------------------------------------
		GFile* new_file = g_file_new_for_path(fname.c_str());
		GtkSourceFile* new_source_file = gtk_source_file_new();
		gtk_source_file_set_location(new_source_file, new_file);
		GtkSourceFileLoader* loader = gtk_source_file_loader_new(buff, new_source_file);
		// -------------------------------------------------------------------------
		// This is the data we need to keep a copy of for the finalization callback
		// ------------------------------------------------------------------------
		struct load_async_finish_data
		{
			GtkSourceFileLoader* loader;
			std::function<void(bool)> callback;
			Glib::ustring file_name;
			GtkSourceBuffer* source_buffer;
			JazzIDE* jazz_ide;
			GFile* the_file;
			GtkSourceLanguageManager* lang_manager;
			Gtk::Notebook* notebook;
			std::function<void()> set_new_page_font;
		};
		
		//load_async_finish_data* finish_data = new load_async_finish_data{loader, callback, fname, buff, this, new_file, language_manager, &notebook, std::bind(&JazzIDE::SetNewPageFont, this)};
		// Launch an async call to load the buffer with the contents of the file
		gtk_source_file_loader_load_async(loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL, NULL,
		// Function to finish the loading routine
		[](GObject* source_obj, GAsyncResult* res, gpointer input_data) -> void {
			GError* error = nullptr;
			load_async_finish_data* final_data = static_cast<load_async_finish_data*>(input_data);
			gboolean success = gtk_source_file_loader_load_finish(final_data->loader, res, &error);
			
			if(success)
			{
				printf("%s successfully opened\n", final_data->file_name.c_str());
				
				// Generate the name to be shown in the notebook tab
				std::string shortname = "";
				std::string title = final_data->file_name.c_str();
				if(title.find("/")!=std::string::npos)
					shortname = title.substr(title.find_last_of("/")+1);
				else
					shortname = title;
					
				// Set the source language on the buffer
				auto s_lang = gtk_source_language_manager_guess_language(
					final_data->lang_manager,
					final_data->file_name.c_str(),
					nullptr);
			
				gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(final_data->source_buffer), s_lang);
				
				// If the buffer was successfully loaded then create a new source view to put it in
				SourceView* source_view = Gtk::manage(new SourceView(final_data->source_buffer));
				TabLabel* tablabel = Gtk::manage(new TabLabel(shortname, *source_view, final_data->file_name));
				final_data->notebook->append_page(*source_view, *tablabel);
				final_data->set_new_page_font();
			}
			else
			{
				printf("Failed to open file: %s, error: %i, %s\n", final_data->file_name.c_str(), error->code, error->message);
				// Unref the object to destroy it because its not going to be owned by any widget
				g_object_unref(final_data->source_buffer);
			}
			// Free the file that we were working with since we're done
			g_object_unref(final_data->the_file);
			// Call the users callback function
			final_data->callback(static_cast<bool>(success));
			// Delete the instantation of the data we needed to keep a copy of
			delete final_data;
		}, 
		// Pass the loader as the user data, so that we can just keep
		// the lambda function as is
		(gpointer)new load_async_finish_data{loader, callback, fname, buff, this, new_file, language_manager, &notebook, std::bind(&JazzIDE::SetNewPageFont, this)}); //finish_data);
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
}
