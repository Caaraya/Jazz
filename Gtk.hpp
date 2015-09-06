// Gtk.hpp by Steve Phillips
// Racinettee on github.
// 
// Helpful links:
// https://www.gnu.org/software/guile-gnome/docs/gtk/html/GtkObject.html
// https://developer.gnome.org/gtk3/stable/GtkWidget.html
//
// 'Never call g-object-unref unless you have previously called g-object-ref, even if you created the <gtk-object>.'
//
#pragma once
#include <gtksourceview/gtksourceview.h>
#include <gtk/gtk.h>
#include <string>

namespace Jazz
{
	namespace Gtk
	{
		// Widget assumes ownership of the pointer you give it
		// use release to get its ownership
		class Widget
		{
		public:
			// Interface
			GtkWidget* Object()
			{
				return widget;
			}
			void Show() const
			{
				gtk_widget_show(widget);
			}
			void ShowAll() const
			{
				gtk_widget_show_all(widget);
			}
			Widget Parent() const
			{
				return Widget(gtk_widget_get_parent(widget));
			}
			void Destroy() const
			{
				gtk_widget_destroy(widget);
			}
			GtkWidget* Release()
			{
				auto result = widget;
				widget = nullptr;
				return result;
			}
			
			Widget() = delete;
			
			// New reference to existing object
			Widget(GtkWidget* input)
			{
				widget = input;
				g_object_ref(widget);
			}
			// Reference an instance
			Widget(const Widget& other)
			{
				widget = other.widget;
				g_object_ref(widget);
			}
			// Using the move constructor we take ownership
			// therefore do not ref count up
			Widget(Widget&& other)
			{
				widget = other.widget;
				other.widget = nullptr;
			}
			
			~Widget()
			{
				if(widget != nullptr)
				{
					g_object_unref(widget);
					this->widget = nullptr;
				}
			}
			operator GtkWidget*() const { return widget; }
			bool Valid() { return widget != nullptr; }
		protected:
			GtkWidget* widget = nullptr;
		};
		class Container : public Widget
		{
		public:
			Container(GtkWidget* cont):Widget(cont){}
			//
			void Add(Widget& other)
			{
				gtk_container_add(GTK_CONTAINER(widget),other.Object());
			}
			void Remove(Widget& other)
			{
				gtk_container_remove(GTK_CONTAINER(widget),other.Object());
			}
		};
		class Bin : public Container
		{
		public:
			Bin(GtkWidget* bin): Container(bin) {}
			//
			Widget Child() const
			{	
				return gtk_bin_get_child(GTK_BIN(widget));
			}
		};
		class ScrolledWindow: public Bin
		{
		public:
			ScrolledWindow(): Bin(gtk_scrolled_window_new(nullptr,nullptr)) {}
		};
		class Notebook : public Container
		{
		public:
			Notebook(): Container(GTK_WIDGET(gtk_notebook_new())) { }
			Notebook(GtkWidget* nb): Container(nb) { }
			//
			gint CurrentPageNum()
			{
				return gtk_notebook_get_current_page(GTK_NOTEBOOK(widget));
			}
			Widget CurrentPage()
			{
				return gtk_notebook_get_nth_page(GTK_NOTEBOOK(widget), CurrentPageNum());
			}
			Widget GetNthPage(gint num)
			{
				return gtk_notebook_get_nth_page(GTK_NOTEBOOK(widget), num);
			}
			gint PageCount()
			{
				return gtk_notebook_get_n_pages(GTK_NOTEBOOK(widget));
			}
		};
		
		class Buffer
		{
		public:
			GtkTextBuffer* Object() { return buffer; }
			Buffer() : buffer(gtk_text_buffer_new(nullptr))
			{
				g_object_ref(buffer);
			}
			Buffer(GtkTextBuffer* buff)
			{
				buffer = buff;
				g_object_ref(buffer);
			}
			Buffer(Buffer&& other)
			{
				buffer = other.buffer;
				other.buffer = nullptr;
			}
			~Buffer()
			{
				g_object_unref(buffer);
			}
			void SetText(const std::string& text)
			{
				gtk_text_buffer_set_text(buffer, text.c_str(), text.size());
			}
			const std::string GetText()
			{
				GtkTextIter* start_iter = nullptr;
				GtkTextIter* end_iter = nullptr;
				
				gtk_text_buffer_get_start_iter(buffer, start_iter);
				gtk_text_buffer_get_end_iter(buffer, end_iter);
				
				return std::string(gtk_text_buffer_get_text(buffer, start_iter, end_iter, false));
			}
			gint LineCount() const
			{
				return gtk_text_buffer_get_line_count(buffer);
			}
			gint CharCount() const
			{
				return gtk_text_buffer_get_char_count(buffer);
			}
			void InsertAtCursor(const std::string& text)
			{
				gtk_text_buffer_insert_at_cursor(buffer, text.c_str(), text.size());
			}
		protected:
			GtkTextBuffer* buffer;
		};
		class SourceBuffer : public Buffer
		{
		public:
			SourceBuffer() : Buffer(GTK_TEXT_BUFFER(gtk_source_buffer_new(nullptr))) { }
			void HighlightSyntax(bool is_set)
			{
				gtk_source_buffer_set_highlight_syntax(GTK_SOURCE_BUFFER(buffer), static_cast<gboolean>(is_set));
			}
			bool HighlightSyntax()
			{
				return static_cast<bool>(gtk_source_buffer_get_highlight_syntax(GTK_SOURCE_BUFFER(buffer)));
			}
		};
		
		class TextView : public Container
		{
		public:
			TextView() : Container(gtk_text_view_new()) { }
			TextView(GtkWidget* new_widget): Container(new_widget) { }
			TextView(Buffer buffer) : Container(gtk_text_view_new_with_buffer(buffer.Object())) { }
			
			Buffer GetBuffer()
			{
				return Buffer(gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget)));
			}
			void SetBuffer(Buffer buff)
			{
				gtk_text_view_set_buffer(GTK_TEXT_VIEW(widget), buff.Object());
			}
		};
		class SourceView : public TextView
		{
		public:
			SourceView(): TextView(GTK_WIDGET(gtk_source_view_new())) { }
			SourceView(SourceBuffer buffer): TextView(GTK_WIDGET(gtk_source_view_new_with_buffer(GTK_SOURCE_BUFFER(buffer.Object())))) { }
			SourceView(GtkWidget* wid): TextView(wid) { }
		};
	}
}
