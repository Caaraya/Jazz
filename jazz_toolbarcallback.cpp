#include "jazz.hpp"
#include "jazz_util.hpp"

namespace Jazz
{
	void JazzIDE::ExecuteProject()
	{
		using namespace coral::zircon;
		
		try
		{
			object target = project_doc.get("target");
			Glib::ustring path_to_exe = target.get("path_to").as<string>();
			path_to_exe += target.get("executable_name").as<string>();
			
			if(!FileExists(path_to_exe))
				throw std::runtime_error(path_to_exe+" could not be found");
		}
		catch(std::exception& e)
		{
			ShowMessage(e.what());
		}
	}
}