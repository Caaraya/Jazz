#include <cstring>
#include <string>
#include <glibmm.h>
#include <unistd.h>
#include "jazz_gdb.hpp"

namespace sigc {
  SIGC_FUNCTORS_DEDUCE_RESULT_TYPE_WITH_DECLTYPE
}
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
	}
	GdbInstance::~GdbInstance()
	{
		out_src->destroy();
		err_src->destroy();
		Glib::spawn_close_pid(_child_pid);
	}
	void GdbInstance::Command(const std::string& command)
	{
		auto cmd=command+'\n';
		write(_gdb_in, cmd.c_str(), cmd.size());
	}
	bool GdbInstance::HandleOutput(Glib::IOCondition cond, Glib::RefPtr<Glib::IOChannel> ch)
	{
		Glib::ustring text;
		ch->read_line(text);
		if(text[0] == '*')
			puts(text.c_str());
		return true;
	}
}