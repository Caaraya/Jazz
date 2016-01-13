#include "jazz.hpp"
int main(int argc, char** argv) try
{
	puts("Welcome to jazz");
	auto app = Gtk::Application::create(argc, argv,
		"ca.chr.apps.jazz");
	
	Jazz::JazzIDE jazz;
	
	return app->run(jazz);
}
catch(std::exception& e)
{
	puts(e.what());
	return 1;
}
catch(const Glib::FileError& e)
{
	printf("FileError: %s\n", e.what().c_str());
	return 1;
}
catch(const Glib::MarkupError& e)
{
	printf("MarkupError: %s\n", e.what().c_str());
	return 1;
}
catch(const Gtk::BuilderError& e)
{
	printf("BuilderError: %s\n", e.what().c_str());
	return 1;
}
