#pragma once
#include <glibmm.h>
#include <functional>
namespace Jazz
{
	class Command
	{
	public:
		typedef std::function<bool(Glib::IOCondition, const Glib::ustring&)> LineHandler;
		typedef std::function<void(int)> ReturnHandler;
	public:
		Command(const std::vector<std::string>& command_line);
	public:
		void AddLineHandler(LineHandler);
		void AddReturnHandler(ReturnHandler);
	private:
		std::vector<LineHandler> _line_handlers;
		std::vector<ReturnHandler> _return_handlers;
	}
}