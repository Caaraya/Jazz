#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif
#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <sys/types.h>
#include <dirent.h>
#endif
#include "jazz_diriter.hpp"
#include <stdexcept>
using namespace std;

namespace rac
{

	file::file(const std::string& path, bool dir):
		_name(path), _is_dir(dir) { }
		
	directory::~directory()
	{
		for(auto child : _children)
			delete child;
	}

#if defined(WIN32)
	// thx to http://stackoverflow.com/a/32684078
	/*std::string toMultibyte(const wchar_t* src) {
		size_t length = WideCharToMultiByte(CP_OEMCP, 0, src, -1, NULL, 0, NULL, NULL) + 1;
		char* buff = (char *)_alloca(length);
		WideCharToMultiByte(CP_OEMCP, 0, src, -1, buff, length, NULL, NULL);
		return std::string(buff);
	}
	
	std::string toMultibyte(const std::wstring& src) {
		return toMultibyte(src.c_str());
	}*/
	
	directory::directory(const std::string& path):
		file(path, true)
	{
		// thx to http://stackoverflow.com/a/2315808
		WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
		
		if((hFind = FindFirstFile((path+"\\*.*").c_str(), &fdFile)) == INVALID_HANDLE_VALUE) 
		{ 
			throw runtime_error(path+" could not be opened");
		} 
		do
    	{ 
			//Find first file will always return "."
			//    and ".." as the first two directories. 
			if(strcmp(fdFile.cFileName, ".") != 0
					&& strcmp(fdFile.cFileName, "..") != 0) 
			{
				// Is a directory
				if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) 
            	{
					std::ostringstream wos(L"");
					wos << path << fdFile.cFileName << '/';
					_children.push_back(new directory(wos.str()));
				}
				else
				{
					_children.push_back(new file(path+fdFile.cFileName, false));
				}
			}
		}
		while(FindNextFile(hFind, &fdFile));
		
		FindClose(hFind);
	}
#else
// *n[iu]x
	directory::directory(const std::string& path):
		file(path, true)
	{
		//auto dir = Gio::File::create_for_path(path);
		/*DIR* dir = opendir(path.c_str());
		
		assert(dir != nullptr);
		
		for(dirent* dir_entry = readdir(dir); dir_entry != nullptr; dir_entry = readdir(dir))
		{
			if(dir_entry->d_type == DT_DIR)
			{
				_children.push_back(new directory(dir_entry->d_name));
			}
			else
			{
				_children.push_back(new file(dir_entry->d_name, false));
			}
		}
		closedir(dir);*/
	}
#endif
}
