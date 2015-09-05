#include <gtksourceview/gtksourceview.h>

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
			
			Widget(Widget&& other)
			{
				widget = other.widget;
				other.widget = nullptr;
			}
			
			~Widget()
			{
				if(widget != nullptr)
				{
					g_obj_unref(widget);
					this->widget = nullptr;
				}
			}
			operator GtkWidget*() { return widget; }
			bool Valid() { return widget != nullptr; }
		protected:
			GtkWidget* widget = nullptr;
		};
		class Container : public Widget
		{
		public:
			Container(GtkWidget* cont):Widget(cont){}
			//
			void Add(GtkWidget& other)
			{
				gtk_container_add(other);
			}
			void Remove(GtkWidget& other)
			{
				gtk_container_remove(other);
			}
		}
		class Bin : public Container
		{
		public:
			Bin(GtkWidget* bin): Container(bin) {}
			//
			Widget Child() const
			{	
				return gtk_bin_get_child(bin);
			}
		};
		class ScrolledWindow: public Bin
		{
		public:
			ScrolledWindow(): Bin(gtk_scrolled_window_new(nullptr,nullptr)) {}
		}
	}
}