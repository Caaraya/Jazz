#include <cstring>
#include <string>
#include <glibmm.h>
#include <giomm.h>
#include <unistd.h>
#include "jazz_gdb.hpp"

namespace sigc {
  SIGC_FUNCTORS_DEDUCE_RESULT_TYPE_WITH_DECLTYPE
}
namespace Jazz
{
	GdbInstance::GdbInstance(const char* path_to_exec)
	{
		try
		{
			{
				Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(path_to_exec);
				if(!file->query_exists())
				{
					Glib::ustring msg = "File doesn't exist: " + Glib::ustring(path_to_exec);
					throw Glib::SpawnError(Glib::SpawnError::Code::NOEXEC, msg);
				}
			}
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
			
			// Create channels that will be used to read data from pipes
			#ifdef G_OS_WIN32
			Glib::RefPtr<Glib::IOChannel> out_ch = Glib::IOChannel::create_from_win32_fd(_gdb_out);
			Glib::RefPtr<Glib::IOChannel> err_ch = Glib::IOChannel::create_from_win32_fd(_gdb_err);
			#else
			Glib::RefPtr<Glib::IOChannel> out_ch = Glib::IOChannel::create_from_fd(_gdb_out);
			Glib::RefPtr<Glib::IOChannel> err_ch = Glib::IOChannel::create_from_fd(_gdb_err);
			#endif
			out_src = out_ch->create_watch(Glib::IO_IN);
			err_src = err_ch->create_watch(Glib::IO_IN);
			out_src->connect([out_ch, this](Glib::IOCondition cond){
				return this->HandleOutput(cond, out_ch);	
			});
			out_src->attach(Glib::MainContext::get_default());
			err_src->connect([err_ch, this](Glib::IOCondition cond){
				return this->HandleOutput(cond, err_ch);
			});
			err_src->attach(Glib::MainContext::get_default());
			ready = true;
		}
		catch(const Glib::SpawnError& e)
		{
			std::printf("Error %i, creating gdb process: %s\n", e.code(), e.what().c_str());
		}
	}
	GdbInstance::~GdbInstance()
	{
		if(ready)
		{
			out_src->destroy();
			err_src->destroy();
			Glib::spawn_close_pid(_child_pid);
		}
	}
	void GdbInstance::Command(const std::string& command)
	{
		if(!ready)
			return;
		auto cmd=command+'\n';
		int wrote = write(_gdb_in, cmd.c_str(), cmd.size());
		if(wrote == 0)
			printf("Failed to write command: %s to gdb\n", command.c_str());
	}
	bool GdbInstance::HandleOutput(Glib::IOCondition cond, Glib::RefPtr<Glib::IOChannel> ch)
	{
		Glib::ustring text;
		ch->read_line(text);
		
		for(auto handler : _handlers)
			handler(cond, text);

		return true;
	}
	void GdbInstance::AddOutHandler(GdbInstance::HandlerType fn)
	{
		_handlers.push_back(fn);
	}
}