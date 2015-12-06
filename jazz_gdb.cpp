#include <cstring>
#include <string>
#include <glibmm.h>
#include <unistd.h>
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
			&_child_pid,
			&_gdb_in,
			&_gdb_out,
			&_gdb_err
		);
		printf("Pipes: in %i & out %i\n", _gdb_in, _gdb_out);
		// Create channels that will be used to read data from pipes
		#ifdef G_OS_WIN32
		
		#else
		#endif
	}
	GdbInstance::~GdbInstance()
	{
		Glib::spawn_close_pid(_child_pid);
	}
	void GdbInstance::Command(const char* command)
	{
		int bytes_read = 0;
		write(_gdb_in, command, strlen(command)+1);
		char buffer[2000];
		bytes_read = read(_gdb_out, buffer, sizeof(buffer));
		puts(buffer);
	}
}