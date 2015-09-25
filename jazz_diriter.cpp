#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif
#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include "jazz_diriter.hpp"
#include <sstream>
#include <stdexcept>
using namespace std;

namespace rac
{
	directory::~directory()
	{
		for(auto child : _children)
			delete child;
	}

#if defined(WIN32)
	// thx to http://stackoverflow.com/a/32684078
	std::string toMultibyte(const wchar_t* src) {
		size_t length = WideCharToMultiByte(CP_OEMCP, 0, src, -1, NULL, 0, NULL, NULL) + 1;
		char* buff = (char *)_alloca(length);
		WideCharToMultiByte(CP_OEMCP, 0, src, -1, buff, length, NULL, NULL);
		return std::string(buff);
	}
	
	std::string toMultibyte(const std::wstring& src) {
		return toMultibyte(src.c_str());
	}

	file::file(const std::wstring& path, bool dir):
		_name(path), _is_dir(dir) { }
	
	directory::directory(const std::wstring& path):
		file(path, true)
	{
		// thx to http://stackoverflow.com/a/2315808
		WIN32_FIND_DATAW fdFile;
    	HANDLE hFind = NULL;
		
		if((hFind = FindFirstFileW((path+L"\\*.*").c_str(), &fdFile)) == INVALID_HANDLE_VALUE) 
		{ 
			throw runtime_error(toMultibyte(path+L" could not be opened"));
		} 
		do
    	{ 
			//Find first file will always return "."
			//    and ".." as the first two directories. 
			if(wcscmp(fdFile.cFileName, L".") != 0
					&& wcscmp(fdFile.cFileName, L"..") != 0) 
			{
				// Is a directory
				if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) 
            	{
					std::wostringstream wos(L"");
					wos << path << fdFile.cFileName << L'/';
					_children.push_back(new directory(wos.str()));
				}
				else
				{
					_children.push_back(new file(path+fdFile.cFileName, false));
				}
			}
		}
		while(FindNextFileW(hFind, &fdFile));
		
		FindClose(hFind);
	}
#else
// *n[iu]x
#endif
}
