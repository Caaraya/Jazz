#pragma once
#include <string>
#include <iterator>
#include <dirent.h>
/**
 * Example usage
 *
int main()
{
	using namespace std;
	rac::directory dir("./");
	
	for(auto file : dir)
	{
		cout << file.name() << (file.is_dir() ? " is a directory\n" : "\n");
	}
}
*/
namespace rac
{
class entry
	{
	public:
		entry(entry&& e);
		entry(DIR* p, dirent* e);
		
		virtual ~entry() { }
		
		std::string name() const { return ent->d_name; }
		bool is_dir() const { return ent->d_type == DT_DIR; }
		bool id_null() const { return ent == nullptr; }
	protected:
		DIR* parent;
		dirent* ent;
	};
	class directory
	{
	public:
		static const char Separator = '/';
	
		directory(const char* directory);
		~directory()
		{
			closedir(dp);
		}
		class iterator :
			std::iterator<std::input_iterator_tag, entry>
		{
		public:
			iterator(DIR* dp, dirent* = nullptr);
			iterator& operator++();
			entry operator*();
			bool operator==(const iterator&);
			bool operator!=(const iterator&);
		private:
			DIR* dp;
			dirent* ep;
		};
	
		iterator begin();
		iterator end();
	private:
		friend class iterator;
		DIR* dp;
		dirent* beg;
	};
}
