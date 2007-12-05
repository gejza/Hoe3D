
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_string.h"

////////////////////////////////
// string class
namespace HoeCore {


namespace string {

int vsnprintf(char *, size_t, const char *, va_list)
{
	return 0;
}

int vsnprintf(wchar_t *, size_t, const char *, va_list)
{
	return 0;
}

int vsnprintf(wchar_t *, size_t, const wchar_t *, va_list)
{
	return 0;
}

void copy(char *, const char *, size_t)
{
	
}

void copy(wchar_t *, const char *, size_t)
{
	
}

void copy(tchar * dest, const wchar_t * src, size_t n)
{
#ifdef UNICODE
	wcscpy(dest, src);
#else
	assert(0);
#endif
}

int cmp(const char*, const wchar_t*)
{
	assert(0);
	return 0;
}

} // end namespace

/*const String & String::operator = (const char * s)
{
	return *this;
}*/

} // namespace HoeCore
