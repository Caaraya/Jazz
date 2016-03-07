#include "jazz_flint.hpp"
#include <string>


namespace Jazz
{
    int FlintInstance::Lint()
    {
        std::vector<std::string> command = {
            std::string("flint++"),
            std::string("jazz.hpp")
        };
        GLib::spaun_async_with_pipes(
        std::string("./"),
                    Glib::ArrayHandle<std::string>(command),
                    Glib::SpawnFlags::SPAWN_SEARCH_PATH,
                    Glib::SlotSpawnChildSetup(),
                    &_child_pid,
                    &_flint_in,
                    &_flint_out,
                    &_flint_err  
        );
        
        #ifdef G_OS_WIN32
        Glib::RefPtr<Glib::IOChannel> out_ch = Glib::IOChannel::create_from_win32_fd(_flint_out);
        Glib::RefPtr<Glib::IOChannel> err_ch = Glib::IOChannel::create_from_win32_fd(_flint_err);
        #else
        Glib::RefPtr<Glib::IOChannel> out_ch = Glib::IOChannel::create_from_fd(_flint_out);
        Glib::RefPtr<Glib::IOChannel> err_ch = Glib::IOChannel::create_from_fd(_flint_err);
        #endif
        
        out_src = out_ch->create_watch(Glib::IO_IN);
        err_src = err_ch->create_watch(Glib::IO_IN);
        out_src->connect([out_ch, this](Glib::IOCondition cond){
            return this->HandleOutput(cond, out_ch);	
        });
        err_src->connect([err_ch, this](Glib::IOCondition cond){
            return this->HandleOutput(cond, err_ch);	
        });
        out_src->attach(Glib::MainContext::get_default());
        err_src->attach(Glib::MainContext::get_default());
        ready = true;
    }
    
    bool FlintInstance::HandleOutput(Glib::IOCondition cond, Glib::RefPtr<Glib::IOChannel> ch)
	{
		Glib::ustring text;
		ch->read_line(text);
		
		for(auto handler : _handlers)
			handler(cond, text);

		return true;
	}
    
    void FlintInstance::AddLineHandler(LineHandler lineCallback)
    {
        _handlers.push_back(lineCallback);
    }
    
    void FlintInstance::AddReturnHandler(ReturnHandler returnCallback)
    {
        _return_handlers.push_back(returnCallback);
    }
}
