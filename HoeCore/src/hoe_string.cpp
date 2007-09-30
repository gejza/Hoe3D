
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_string.h"

////////////////////////////////
// string class
namespace HoeCore {

int vsnprintf(char *, size_t, const char *, va_list);

const String & String::operator = (const char * s)
{
	return *this;
}


} // namespace HoeCore
