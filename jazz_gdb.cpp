#include <string>
#include <glibmm.h>
#include "jazz_gdb.hpp"

namespace Jazz
{
	GdbInstance::GdbInstance(const char* path_to_exec)
	{
		std::vector<std::string> command = {
			std::string("/usr/bin/gdb"),
			std::string("-interpreter=mi"),
			std::string(path_to_exec)
		};
		Glib::spawn_async_with_pipes(
			std::string("./"),
			Glib::ArrayHandle<std::string>(command),
			Glib::SpawnFlags::SPAWN_SEARCH_PATH,
			Glib::SlotSpawnChildSetup(),
			nullptr,
			&_gdb_in,
			&_gdb_out,
			&_gdb_err
		);
	}
}