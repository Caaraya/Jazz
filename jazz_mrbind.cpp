#include "jazz.hpp"
#include "mrubybind/mrubybind.h"

namespace Jazz
{
	Gtk::Window* new_window() { return new Gtk::Window(); }
	void JazzIDE::MrbBind()
	{
		RClass* mod = mrb_define_module(mrb, "Gtk");
		
		mrubybind::MrubyBind binder(mrb, mod);
		
		binder.bind_class("Window", new_window);
		binder.bind_instance_method("Window", "show_all", &Gtk::Window::show_all);
		binder.bind_instance_method("Window", "set_default_size", &Gtk::Window::set_default_size);
	}
}
