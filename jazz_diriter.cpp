#include "jazz_diriter.hpp"
#include <stdexcept>

namespace rac
{
entry::entry(entry&& e) : parent(e.parent), ent(e.ent)
{
	e.parent = nullptr;
	e.ent = nullptr;
}
entry::entry(DIR* p, dirent* e):
	parent(p), ent(e)
{
}
directory::directory(const char* directory)
{
	dp = opendir(directory);
	
	if(dp == nullptr)
		throw std::runtime_error("Invalid directory name passed");
	beg = readdir(dp);
}
directory::iterator::iterator(DIR* dir, dirent* entry): dp(dir), ep(entry)
{
}
directory::iterator& directory::iterator::operator++()
{
	ep = readdir(dp);
	return *this;
}
entry directory::iterator::operator*()
{
	//return std::string(ep->d_name);
	return entry(dp, ep);
}
bool directory::iterator::operator==(const iterator& other)
{
	return ep == other.ep;
}
bool directory::iterator::operator!=(const iterator& other)
{
	return !(*this == other);
}
directory::iterator directory::begin()
{
	return iterator(dp, beg);
}
directory::iterator directory::end()
{
	return iterator(dp);
}
}
