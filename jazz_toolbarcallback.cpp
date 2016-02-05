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
				
			using std::placeholders::_1;
			using std::placeholders::_2;
			gdb.reset(new GdbInstance(path_to_exe.c_str()));
			gdb->AddOutHandler(std::bind(&Terminal::Update, &terminal, _1, _2));
			gdb->AddOutHandler(std::bind(&JazzIDE::HandleGDBOutput, this, _1, _2));
			
			if(project_doc.has("breakpoints"))
			{
				object breakpoints = project_doc["breakpoints"];
				
				for(auto breakpoint : breakpoints.as<array>())
					gdb->Command(breakpoint.as<string>().c_str());
			}
			//gdb->Command("b gdb_test.cpp:9");
			gdb->Command("r");
		}
		catch(std::exception& e)
		{
			ShowMessage(e.what());
		}
	}
	void JazzIDE::DebugContinueCmd()
	{
		gdb->Command("c");
	}
}