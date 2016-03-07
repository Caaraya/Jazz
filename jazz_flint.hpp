#include <glibmm.h>
#include <functional>
namespace Jazz
{
    typedef std::function<bool(Glib::IOCondition, const Glib::ustring&)> LineHandler;
    typedef std::function<int()> ReturnHandler;
    
    
    
    class FlintInstance
    {
    private: 
        int _flint_in;
        int _flint_out;
        int _flint_err;
        bool ready = false;
        std::vector<LineHandler> _handlers;
        std::vector<ReturnHandler> _return_handlers;
		Glib::RefPtr<Glib::IOSource> out_src;
		Glib::RefPtr<Glib::IOSource> err_src;
		Glib::Pid _child_pid;        
    public:
        int Lint();
        void AddLineHandler(LineHandler);
        void AddReturnHandler(ReturnHandler);
        bool HandleOutput(Glib::IOCondition, Glib::RefPtr<Glib::IOChannel>);
    }
}
