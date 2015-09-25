#pragma once
#include <string>
#include <iterator>
#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dirent.h>
#endif
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
#if defined(WIN32)
	class entry
	{
	public:
		entry(entry&& e);
		entry(WIN32_FIND_DATA p, HANDLE e);
		
		virtual ~entry() { }
		
		std::string name() const { return file_data.cFileName; }
		bool is_dir() const
			{ return file_data.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY; }
		bool is_null() const { return file_data.cFileName == nullptr; }
	protected:
		WIN32_FIND_DATA file_data;
		HANDLE h_find;
	};
	class directory
	{
	public:
		static const char Separator = '/';
	
		directory(const char* directory);
		~directory()
		{
			FindClose(h_find);
		}
		class iterator :
			std::iterator<std::input_iterator_tag, entry>
		{
		public:
			iterator(HANDLE, WIN32_FIND_DATA);
			iterator& operator++();
			entry operator*();
			bool operator==(const iterator&);
			bool operator!=(const iterator&);
		private:
			//DIR* dp;
			WIN32_FIND_DATA fileData;
			HANDLE h_find;
		};
	
		iterator begin();
		iterator end();
	private:
		friend class iterator;
		WIN32_FIND_DATA firstFileData;
		HANDLE hFind;
	};
#else
	class entry
	{
	public:
		entry(entry&& e);
		entry(DIR* p, dirent* e);
		
		virtual ~entry() { }
		
		std::string name() const { return ent->d_name; }
		bool is_dir() const { return ent->d_type == DT_DIR; }
		bool is_null() const { return ent == nullptr; }
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
#endif
}
