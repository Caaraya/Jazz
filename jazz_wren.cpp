#include "jazz.hpp"
extern "C" {
#include <wren.h>
}
#include <assert.h>
#include <algorithm>
namespace
{
	// These functions which are defined seperately are
	// used by multiple classes which follow an inheritance heirarchy that at
	// the time of writing doese not seem to be expressable by the api
	struct WidgetPtr { GtkWidget* widget = nullptr; };
	
	void alloc_widget(WrenVM* vm)
	{
		wrenAllocateForeign(vm, sizeof(WidgetPtr));
	}
	void binding_widget_from(WrenVM* vm)
	{
		WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm, 0);
		WidgetPtr* other = (WidgetPtr*)wrenGetArgumentValue(vm, 1);
		obj->widget = other->widget;
		wrenReturnValue(vm, wrenGetArgumentValue(vm, 0));
	}
	void widget_show(WrenVM* vm)
	{
		WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm, 0);
		gtk_widget_show(GTK_WIDGET(obj->widget));
	}
	void widget_show_all(WrenVM* vm)
	{
		WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm, 0);
		gtk_widget_show_all(GTK_WIDGET(obj->widget));
	}
	void widget_hide(WrenVM* vm)
	{
		WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm, 0);
		gtk_widget_hide(obj->widget);
	}
	void container_add(WrenVM* vm)
	{
		WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm, 0);
		WidgetPtr* other = (WidgetPtr*)wrenGetArgumentForeign(vm, 1);
		gtk_container_add(GTK_CONTAINER(obj->widget),
			GTK_WIDGET(other->widget));
	}
	void container_remove(WrenVM* vm)
	{
		WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm, 0);
		WidgetPtr* other = (WidgetPtr*)wrenGetArgumentForeign(vm, 1);
		gtk_container_remove(GTK_CONTAINER(obj->widget),
			GTK_WIDGET(other->widget));
	}
	void bin_get_child(WrenVM* vm)
	{
		WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm, 0);
		WidgetPtr* child = (WidgetPtr*)wrenAllocateForeign(vm, sizeof(WidgetPtr));
		child->widget = gtk_bin_get_child(GTK_BIN(obj->widget));
		wrenReturnValue(vm, (WrenValue*)child);
		//wrenReturnValue(vm, (WrenValue*)gtk_bin_get_child(GTK_BIN(obj->widget)));
	}
	
	WrenForeignClassMethods BindForeignClass(WrenVM*, const char* mod, const char* cls_name)
	{
		WrenForeignClassMethods widget_methods;
		
		widget_methods.allocate = alloc_widget;
		widget_methods.finalize = nullptr;
		
		if(strcmp(cls_name, "Window") == 0)
		{
			return widget_methods;
		}
		else if(strcmp(cls_name, "Button") == 0)
		{
			return widget_methods;
		}
		return WrenForeignClassMethods{nullptr, nullptr};
	}
	WrenForeignMethodFn BindForeignMethod(WrenVM* vm,const char* module, const char* className, bool isStatic, const char* signature)
	{
		if(strcmp(className, "Window") == 0)
		{
			if(strcmp(signature, "show()") == 0)			
				return widget_show;
			else if(strcmp(signature, "show_all()") == 0)
				return widget_show_all;
			else if(strcmp(signature, "hide()") == 0)
				return widget_hide;
			else if(strcmp(signature, "add(_)") == 0)
				return container_add;
			else if(strcmp(signature, "remove(_)") == 0)
				return container_remove;
			else if(strcmp(signature, "get_child()") == 0)
				return bin_get_child;
				
			else if(strcmp(signature, "init new(_)") == 0)
			{
				return [](WrenVM* vm2){
					WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm2, 0);
					if(strcmp(wrenGetArgumentString(vm2, 1), "TOP_LEVEL") == 0)
						obj->widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);	
					else if(strcmp(wrenGetArgumentString(vm2, 1), "POPUP") == 0)
						obj->widget = gtk_window_new(GTK_WINDOW_POPUP);
					else
						fprintf(stderr, "Invalid value: %s used for window constructor\n", wrenGetArgumentString(vm2, 1));
					
					wrenReturnValue(vm2, wrenGetArgumentValue(vm2,0));
				};
			}
			else if(strcmp(signature, "set_default_size(_,_)") == 0)
			{
				return [](WrenVM* vm2)
				{
					WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm2, 0);
					double width = wrenGetArgumentDouble(vm2, 1);
					double height= wrenGetArgumentDouble(vm2, 2);
					gtk_window_set_default_size(
						GTK_WINDOW(obj->widget),
						width, height
					);
				};
			}
		}
		else if(strcmp(className, "Button") == 0)
		{
			if(strcmp(signature, "init new(_)") == 0)
			{
				return [](WrenVM* vm2){
					WidgetPtr* obj = (WidgetPtr*)wrenGetArgumentForeign(vm2, 0);
					obj->widget = gtk_button_new_with_label(wrenGetArgumentString(vm2, 1));
					wrenReturnValue(vm2, wrenGetArgumentValue(vm2,0));
				};
			}
			else if(strcmp(signature, "init from(_)") == 0)
				return binding_widget_from;
			else if(strcmp(signature, "show()") == 0)			
				return widget_show;
			else if(strcmp(signature, "show_all()") == 0)
				return widget_show_all;
			else if(strcmp(signature, "hide()") == 0)
				return widget_hide;
		}
		return nullptr;
	}
}

namespace Jazz
{
	WrenConfiguration config;
	void JazzIDE::BindWren()
	{
		wrenInitConfiguration(&config);
		config.writeFn = [](WrenVM*, const char* text){
			fputs(text, stdout);
		};
		config.bindForeignClassFn = BindForeignClass;
		config.bindForeignMethodFn = BindForeignMethod;
		// typedef char* (*WrenLoadModuleFn)(WrenVM* vm, const char* name);
		config.loadModuleFn = [](WrenVM* vm, const char* name) -> char*
		{			
			std::string fname = name;
			std::replace(fname.begin(), fname.end(), '.', '/');
			fname += ".wren";
			
			FILE* f = fopen(fname.c_str(), "r");
			
			if(!f) {
				puts("File was not opened");
				return nullptr;
			}
			
			fseek(f, 0, SEEK_END);
			
			unsigned int len = ftell(f);
			
			fseek(f, 0, SEEK_SET);
			
			char* data = (char*)config.reallocateFn(nullptr, len);
			
			if(fread(data, 1, len, f) != len)
			{
				fprintf(stderr, "Failed to read whole file: %s\n", name);
			}
			
			fclose(f);
			
			return data;
		};
		
		vm = wrenNewVM(&config);
	}
}