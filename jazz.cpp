#include "jazz.hpp"
int main(int argc, char** argv)
{
	puts("Welcome to jazz");
	auto app = Gtk::Application::create(argc, argv,
		"ca.chr.apps.jazz");
	
	Jazz::JazzIDE jazz;
	
	return app->run(jazz);
}
