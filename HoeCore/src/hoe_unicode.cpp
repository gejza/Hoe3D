
#include "StdAfx.h"
#include "../include/HoeCore/hoe_unicode.h"
#include "../include/HoeCore/hoe_string.h"

////////////////////////////////
// string class
namespace HoeCore {
namespace string {

size_t utf8cont(const char* utf, size_t num)
{
	size_t t = num-1;
	while (t>0)
	{
		size_t l = utf8len(utf[t]);
		if (l) return t + l - num;
		t--;
	}
	return 0;
}

size_t utf2w(wchar_t* w, const char* utf, size_t num)
{
	// najit pokracovani
	size_t nw = 0;
	while (num)
	{
		register size_t p = utf8len(*utf);
		if (p > num)
			return -1;
		num -= p;
		w[nw++] = utf2w(utf);
	}
	return nw;
}

} // end namespace string
} // namespace HoeCore
