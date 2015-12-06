#include <cstring>
#include <string>
#include <glibmm.h>
#include <unistd.h>
#include "jazz_gdb.hpp"
namespace Jazz
{
	GdbInstance::GdbInstance(const char* path_to_exec)
	{
		puts("Gonna spawn a process!!");
		std::vector<std::string> command = {
			std::string("gdb"),
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
		
		g_child_watch_add(_child_pid,
		[](GPid pid, gint status, gpointer user_data)
		{
			// do something if the process exits
		},
		nullptr);
		// Create channels that will be used to read data from pipes
		#ifdef G_OS_WIN32
		out_ch = Glib::IOChannel::create_from_win32_fd(_gdb_out);//g_io_channel_win32_new_fd( _gdb_out );
    	err_ch = Glib::IOChannel::create_from_win32_fd(_gdb_err);//g_io_channel_win32_new_fd( _gdb_err );
		#else
		out_ch = Glib::IOChannel::create_from_fd(_gdb_out);//g_io_channel_unix_new( _gdb_out );
    	err_ch = Glib::IOChannel::create_from_fd(_gdb_err);//g_io_channel_unix_new( _gdb_err );
		#endif
		out_src = out_ch->create_watch(Glib::IO_IN);
		err_src = err_ch->create_watch(Glib::IO_IN);
		out_src->connect([this](Glib::IOCondition cond){
			return HandleOutput(cond, this->out_ch);	
		});
		out_src->attach(Glib::MainContext::get_default());
		err_src->connect([this](Glib::IOCondition cond){
			return HandleOutput(cond, this->err_ch);
		});
		err_src->attach(Glib::MainContext::get_default());
	}
	GdbInstance::~GdbInstance()
	{
		Glib::spawn_close_pid(_child_pid);
	}
	void GdbInstance::Command(const char* command)
	{
		write(_gdb_in, command, strlen(command)+1);
	}
	bool GdbInstance::HandleOutput(Glib::IOCondition cond, Glib::RefPtr<Glib::IOChannel> ch)
	{
		Glib::ustring text;
		ch->read_line(text);
		puts(text.c_str());
		return true;
	}
}