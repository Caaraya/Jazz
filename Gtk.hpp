#pragma once
#include <gtksourceview/gtksourceview.h>
#include <gtk/gtk.h>

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
			GtkWidget* Object() const
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
			
			Widget(GtkWidget* input)
			{
				widget = input;
			}
			
			Widget(const Widget& other)
			{
				widget = other.widget;
				g_object_ref(widget);
			}
			
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
			void Add(const Widget& other)
			{
				gtk_container_add(GTK_CONTAINER(widget),other.Object());
			}
			void Remove(const Widget& other)
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
	}
}