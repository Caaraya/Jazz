#pragma once
#include <glibmm/ustring.h>
#include <vector>
/**
 * Example usage
 *
#include <iostream>
int main()
{
	using namespace std;
	rac::directory dir("./");
	
	for(auto file : dir)
	{
		cout << file->name() << (file->is_dir() ? " is a directory\n" : "\n");
	}
}
*/
namespace rac
{	
	class file
	{
	public:
		file(const std::string&, bool);
		const std::string& name() const
		{
			return _name;
		}
		bool is_dir() const
		{
			return _is_dir;
		}
		virtual ~file() { }
	private:
		std::string _name;
		bool _is_dir;
	};
	
	class directory : public file
	{
	public:
		directory(const std::string&);
		std::vector<file*>& children() const;
		
		std::vector<file*>::iterator begin()
		{
			return _children.begin();
		}
		std::vector<file*>::iterator end()
		{
			return _children.end();
		}
		~directory();
	private:
		std::vector<file*> _children;
	};
}
