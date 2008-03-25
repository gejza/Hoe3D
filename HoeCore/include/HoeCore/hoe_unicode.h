
#ifndef _HOE_CORE_UNICODE_H_
#define _HOE_CORE_UNICODE_H_

////////////////////////////////
// string class
namespace HoeCore {
namespace string {

inline size_t utf8len(const wchar_t c)
{
	if (c <= 0x7F)
		return 1;
	else if (c <= 0x7FF)
		return 2;
	else if (c <= 0xFFFF)
		return 3;
	else if (c <= 0x1FFFFF)
		return 4;
	else if (c <= 0x3FFFFFF)
		return 5;
	else
		return 6;
}

inline size_t utf8len(const char c)
{
	if ((c & 0x80) == 0)
		return 1;
	if ((c & 0xC0) == 0x80)
		return 0;
	if ((c & 0xE0) == 0xC0)
		return 2;
	if ((c & 0xF0) == 0xE0)
		return 3;
	if ((c & 0xF8) == 0xF0)
		return 4;
	if ((c & 0xFC) == 0xF8)
		return 5;
	if ((c & 0xFE) == 0xFC)
		return 6;
	return 1;
}

inline wchar_t utf2w(const char *& src)
{
	register wchar_t ch = 0;
	// pokud *p < 0x80
	if (!(*src & 0x80)) ch = *src++;
	else if ((*src & 0xE0) == 0xC0)
	{
		ch |= ((*src) & 0x1F) << 6;src++;
		ch |= (*src) & 0x3F; src++;
	}
	else if ((*src & 0xF0) == 0xE0)
	{
		ch |= (*src & 0xF) << 12;src++;
		ch |= (*src & 0x3F) << 6;src++;
		ch |= (*src) & 0x3F; src++;
	}	
	else
	{
		src++;
		return L'?';
	}
	return ch;
}

inline size_t w2utf(char *& dest, wchar_t c, size_t n)
{
	register size_t l = utf8len(c);
	if (n < l)
		return 0;
	switch (l)
	{
	case 1:
		*dest++ = (char)c;
		break;
	case 2:
		*dest++ = 0xA0 | (c >> 6);
		*dest++ = 0x80 | (c & 0x3F);
		break;
	case 3:
		*dest++ = 0xE0 | (c >> 12);
		*dest++ = 0x80 | ((c&0xFA0) >> 6);
		*dest++ = 0x80 | (c & 0x3f);
		break;
	default:
		*dest++ = '?';
	};
	return l;
}

size_t utf8cont(const char* utf, size_t num);
size_t utf8len(const wchar_t * s);
size_t utf2w(wchar_t* w, const char* utf, size_t num);

} // end namespace
} // namespace HoeCore

#endif // _HOE_CORE_UNICODE_H_

