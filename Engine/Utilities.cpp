#include "Utilities.h"




bool FileExists(char* filename)
{
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}


bool FileExists(WCHAR* filename)
{
	size_t s = wcslen(filename);
	char* newstr = new char[s + 1];
	memset(newstr, 0, s + 1);
	wcstombs(newstr, filename, s);
	return FileExists(newstr);
}