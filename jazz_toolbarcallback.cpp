#include "jazz.hpp"

namespace Jazz
{
	void JazzIDE::ExecuteProject()
	{
		using namespace coral::zircon;
		
		try
		{
			object target = project_doc.get("target");
			std::string path = target["path"].as<string>();
			std::string name = target["name"].as<string>();
		}
		catch(std::exception& e)
		{
			puts(e.what());
		}
	}
}