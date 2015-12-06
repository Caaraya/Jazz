namespace Jazz
{
	class GdbInstance
	{
	public:
		GdbInstance(const char* path_to_exec);
		~GdbInstance();
		void Command(const char*);
	private:
		int _gdb_in;
		int _gdb_out;
		int _gdb_err;
		Glib::Pid _child_pid;
	};
}