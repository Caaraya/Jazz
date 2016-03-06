#include "jazz.hpp"
#include "jazz_sourceview.hpp"
namespace Jazz
{
	bool JazzIDE::HandleGDBOutput(Glib::IOCondition, const Glib::ustring& thing)
	{
		if(thing[0] == '*')
		{
			if(thing.find("breakpoint-hit") !=  Glib::ustring::npos)
			{
				// Find the fullname property and open that file
				auto new_pos = thing.rfind("fullname=")+10U;
				auto str = thing.substr(new_pos);
				str = str.substr(0, str.find('"'));
				
				new_pos = thing.rfind("line=")+6U;
				int line_num = std::stoi(thing.substr(new_pos, thing.find('"', new_pos)));  
				
				// Add the file into the notebook
				AddFileToNotebook(str, [this, str, line_num](FileOpened success, int which){
					switch(success)
					{
						case FileOpened::Success:
						{
							SourceView* page = static_cast<SourceView*>(notebook.get_nth_page(notebook.get_current_page()));
							GtkTextBuffer* buffer = GTK_TEXT_BUFFER(page->GetSourceBuffer());
							BreakpointCallbackData* data = new BreakpointCallbackData{line_num, 0U, page};
							data->signal_id = g_signal_connect(GTK_WIDGET(page->GetSourceView()), "size-allocate",
								G_CALLBACK(OnSizeAllocate), (gpointer)data); 
							page->ScrollToLine(line_num);
							GdkRGBA bg_color = {.85,0.30,0.30,1.0};
							auto tag = gtk_text_buffer_create_tag(buffer, "current-break",
								"background-rgba", &bg_color, NULL);
							auto line1 = page->GetTextIterAtLine(line_num-1);
							auto line2 = page->GetTextIterAtLine(line_num);
							gtk_text_buffer_apply_tag(buffer, tag, &line1, &line2);
						}break;
						case FileOpened::Failure:
						{
							// In the future we should use lib notify to inform the user that this operation didn't work
							ShowMessage("Breakpoint hit but could not open"
											" the file" + str + " successfully", this);
							return;
						}break;
						case FileOpened::WasOpen:
						{
							notebook.set_current_page(which);
							SourceView* page = static_cast<SourceView*>(notebook.get_nth_page(notebook.get_current_page()));
							page->ScrollToLine(line_num);
						}break;
					}
				});
				// Update watch window
			}
		}
		return true;
	}
}