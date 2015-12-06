namespace Jazz
{
	class GdbInstance
	{
	public:
		GdbInstance(const char* path_to_exec);
		~GdbInstance();
		void Command(const char*);
	private:
		bool HandleOutput(Glib::IOCondition, Glib::RefPtr<Glib::IOChannel>);
	private:
		int _gdb_in;
		int _gdb_out;
		int _gdb_err;
		Glib::RefPtr<Glib::IOChannel> out_ch;
		Glib::RefPtr<Glib::IOChannel> err_ch;
		Glib::RefPtr<Glib::IOSource> out_src;
		Glib::RefPtr<Glib::IOSource> err_src;
		Glib::Pid _child_pid;
	};
}