#include <string>
#include <iostream>
extern int puts(const char*);

namespace Racc
{
#if defined(WIN32)
#else
#include <dirent.h>
	class Entry
	{
	public:
		Entry(Entry&& e) : parent(e.parent), ent(e.ent)
		{
			e.parent = nullptr;
			e.ent = nullptr;
		}
		Entry(DIR* p, dirent* e): parent(p), ent(e) { }
		std::string Name() const { return ent->d_name; }
		bool IsDir() const { return ent->d_type == DT_DIR; }
		bool IsNull() const { return ent == nullptr; }
	private:
		DIR* parent;
		dirent* ent;
	};
	class Directory
	{
	public:
		static const char Separator = '/';
	
		Directory(const char* directory)
		{
			dp = opendir(directory);
			beg = readdir(dp);
		}
		~Directory()
		{
			closedir(dp);
		}
		class Iterator
		{
		public:
			Iterator(DIR* dp, dirent* = nullptr);
			Iterator& operator++();
			Entry operator*();
			bool operator==(const Iterator&);
			bool operator!=(const Iterator&);
		private:
			DIR* dp;
			dirent* ep;
		};
	
		Iterator begin();
		Iterator end();
	private:
		friend class Iterator;
		DIR* dp;
		dirent* beg;
	};
#endif
}
int main()
{
	using namespace std;
	using namespace Racc;
	Directory dir("../Documents/Jazz");
	
	for(auto file : dir)
	{
		//puts(file.c_str());
		cout << file.Name() << (file.IsDir() ? " is a directory\n" : "\n");
	}
}
namespace Racc
{
#if defined(WIN32)
#else
Directory::Iterator::Iterator(DIR* dir, dirent* entry): dp(dir), ep(entry)
{
}
Directory::Iterator& Directory::Iterator::operator++()
{
	ep = readdir(dp);
	return *this;
}
Entry Directory::Iterator::operator*()
{
	//return std::string(ep->d_name);
	return Entry(dp, ep);
}
bool Directory::Iterator::operator==(const Iterator& other)
{
	return ep == other.ep;
}
bool Directory::Iterator::operator!=(const Iterator& other)
{
	return !(*this == other);
}
Directory::Iterator Directory::begin()
{
	return Iterator(dp, beg);
}
Directory::Iterator Directory::end()
{
	return Iterator(dp);
}
#endif
}
