
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_string.h"

////////////////////////////////
// string class
namespace HoeCore {
namespace string {

int vsnprintf(char *, size_t, const char *, va_list)
{
	not_implemented();
	return 0;
}

int vsnprintf(wchar_t *, size_t, const char *, va_list)
{
	not_implemented();
	return 0;
}

int vsnprintf(wchar_t *, size_t, const wchar_t *, va_list)
{
	not_implemented();
	return 0;
}

void copy(char * dest, const char * src, size_t cnt)
{
	strncpy(dest, src, cnt);
}

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

size_t utf8len(const wchar_t * s)
{
	size_t l = 0;
	while (*s)
	{
		l += utf8len(*s++);
	}
	return l;
}

void copy(wchar_t * dest, const char * src, size_t s)
{
	s--;
	while (s > 0)
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
			ch = *src++;
		*dest++ = ch;
		if (!ch)
			break;
		s--;
	}
	*dest = '\0';
}

/*
U+00000000 - U+0000007F	0xxxxxxx
U+00000080 - U+000007FF	110xxxxx 10xxxxxx
U+00000800 - U+0000FFFF	1110xxxx 10xxxxxx 10xxxxxx
*/

void copy(char * dest, const wchar_t * src, size_t n)
{
	n--;
	while (*src)
	{
		register wchar_t c = *src++;
		register size_t l = utf8len(c);
		if (n < l)
			break;
		n-=l;
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
	};
	*dest = 0;
}

int cmp(const char*, const wchar_t*)
{
	not_implemented();
	return 0;
}

} // end namespace

String::String()
{
	m_data = NULL;
}

String::~String()
{
	if (m_data)
		m_data->data.Unlock();
}

const String & String::operator = (const String& s)
{
	if (this == &s)
		return *this;
	if (m_data)
		m_data->data.Unlock();
	m_data = NULL;
	if (!s.IsEmpty())
	{
		m_data = s.m_data;
		m_data->data.Lock();
	}
}

const String & String::operator = (const char * s)
{
	size_t l = string::len(s);
	if (m_data) 
		m_data->data.Unlock();

	m_data = CreateStringData(l);
	string::copy(m_data->str, s, m_data->data.alloc);
	return *this;
}

const String & String::operator = (const wchar_t * s)
{
#ifdef _UNICODE
	size_t l = string::len(s);
#else
	size_t l = string::utf8len(s);
#endif
	if (m_data) 
		m_data->data.Unlock();

	m_data = CreateStringData(l);
	string::copy(m_data->str, s, m_data->data.alloc);
	return *this;
}

String::StringDataPtr* String::CreateStringData(size_t n)
{
	StringDataPtr * data = (StringDataPtr*)malloc(sizeof(StringData)+(n+1)*sizeof(tchar));
	data->data.ref = 1;
	data->data.alloc = (n+1);
	return data;
}

void String::PrepareForModify(size_t n)
{
	if (!m_data)
	{
		m_data = CreateStringData(n);
		m_data->str[0] = 0;
	}
	else if (m_data->data.IsShared())
	{
		StringDataPtr * data = CreateStringData(n);
		string::copy(data->str, m_data->str, n);
		m_data->data.Unlock();
		m_data = data;
	}
	else if (n > m_data->data.alloc)
	{
		m_data = (StringDataPtr*)realloc(m_data, sizeof(StringData)+(n+1)*sizeof(tchar));
		m_data->data.alloc = (n+1);
	}
}

void String::concat(const char * str)
{
	if (IsEmpty())
	{
		*this = str;
	}
	else
	{
		size_t s1 = string::len(m_data->str);
		size_t s2 = string::len(str);
		PrepareForModify(s1 + s2 + 1);
		string::copy(m_data->str + s1, str, m_data->data.alloc - s1);
	}
}


} // namespace HoeCore
