#include <functional>
#include <vector>
namespace Jazz
{
	class GdbInstance
	{
	public:
		typedef std::function<bool(Glib::IOCondition, const Glib::ustring&)> HandlerType;
	public:
		GdbInstance(const char* path_to_exec);
		~GdbInstance();
		void Command(const std::string&);
		
		// Register a callback object of handler type
		// Your handler will recieve output line by line
		void AddOutHandler(HandlerType);
		bool Ready() const
		{
			return ready;
		}
	private:
		bool HandleOutput(Glib::IOCondition, Glib::RefPtr<Glib::IOChannel>);
	private:
		bool ready = false;
		int _gdb_in;
		int _gdb_out;
		int _gdb_err;
		std::vector<HandlerType> _handlers;
		Glib::RefPtr<Glib::IOSource> out_src;
		Glib::RefPtr<Glib::IOSource> err_src;
		Glib::Pid _child_pid;
	};
}