
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_string.h"
#include "../include/hoe_unicode.h"

////////////////////////////////
// string class
namespace HoeCore {
namespace string {

int vsnprintf(char *dest, size_t n, const char * fmt, va_list vl)
{
#ifdef _WIN32
	return ::vsprintf_s(dest, n, fmt, vl);
#else
	return vsnprintf(dest, n, fmt, vl);
#endif
}

int vsnprintf(wchar_t* dest, size_t n, const char *fmt, va_list vl)
{
	const int size_buff = 2512;
	if (n <= 2048)
	{
		char str[size_buff];
		int r = string::vsnprintf(str, size_buff, fmt, vl);
		string::copy(dest, str, n);
		return r;
	}
	else
	{
		char* str = new char[int(n * 1.3)];
		int r = string::vsnprintf(str, int(n * 1.3), fmt, vl);
		string::copy(dest, str, n);
		delete [] str;
		return r;
	}
}

int vsnprintf(char* dest, size_t n, const wchar_t *fmt, va_list vl)
{
	const int size_buff = 2512;
	if (n <= 2048)
	{
		wchar_t str[size_buff];
		int r = string::vsnprintf(str, size_buff, fmt, vl);
		string::copy(dest, str, n);
		return r;
	}
	else
	{
		wchar_t* str = new wchar_t[int(n * 1.3)];
		int r = string::vsnprintf(str, int(n * 1.3), fmt, vl);
		string::copy(dest, str, n);
		delete [] str;
		return r;
	}
}

int vsnprintf(wchar_t* dest, size_t n, const wchar_t* fmt, va_list vl)
{
#ifdef _WIN32
	return ::vswprintf_s(dest, n, fmt, vl);
#else
	return ::vswprintf(dest, n, fmt, vl);
#endif
}

void copy(char * dest, const char * src, size_t cnt)
{
	strncpy(dest, src, cnt);
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
		register wchar_t ch = utf2w(src);
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
		register size_t l = w2utf(dest, *src++, n);
		if (!l)
			break;
		n-=l;
	};
	*dest = 0;
}

int cmp(const char* str1, const wchar_t* str2)
{
	register wchar_t c1, c2;
	do
	{
		c1 = utf2w(str1);
		c2 = *str2++;
	} while (c1 && (c1 == c2));
	if (c1 < c2)
		return -1;
	else if (c1 > c2)
		return 1;
	else
		return 0;
}

float GetReal(const char* str) 
{
	return (float) ::atof(str);
}

float GetReal(const wchar_t* str) 
{  //TODO optimalizovat
	float p=0.f;
	// dec
	while (*str >= '0' && *str <= '9')
	{
		p = p * 10.f + (*str++ - '0');
	}
	if (*str++ != '.')
		return p;
	float e = 0.1f;
	while (*str >= '0' && *str <= '9')
	{
		p += e * (*str++ - '0');
		e /= 10.f;
	}
	return p;	
}

int GetHex(const char* str) 
{
	int n = 0;
	while (*str)
	{
		register char c = *str;
		if (c >= '0' && c <= '9')
			n = (n << 4) + (c - '0');
		else if (c >= 'A' && c <= 'F')
			n = (n << 4) + (c - 'A' + 10);
		else if (c >= 'a' && c <= 'f')
			n = (n << 4) + (c - 'a' + 10);
		else
			break;
		str++;
	}
	return n;
}

int GetHex(const wchar_t* str) 
{
	int n = 0;
	while (*str)
	{
		register wchar_t c = *str;
		if (c >= '0' && c <= '9')
			n = (n << 4) + (c - '0');
		else if (c >= 'A' && c <= 'F')
			n = (n << 4) + (c - 'A' + 10);
		else if (c >= 'a' && c <= 'f')
			n = (n << 4) + (c - 'a' + 10);
		else
			break;
		str++;
	}
	return n;
}

int GetNumber(const char* str)
{
	if (str[0] == '0' && str[1] == 'x')
		return GetHex(str+2);
	else
		return atoi(str);
}

int GetNumber(const wchar_t* str)
{
	if (str[0] == '0' && str[1] == 'x')
		return GetHex(str+2);

	int n = 0;
	while (*str)
	{
		register wchar_t c = *str;
		if (c >= '0' && c <= '9')
			n = (n * 10) + (c - '0');
		else
			break;
		str++;
	}
	return n;
}

} // end namespace string

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
	return *this;
}

const String & String::operator = (const CString& s)
{
	size_t l = string::len((const tchar*)s);
	if (m_data) 
		m_data->data.Unlock();

	m_data = CreateStringData(l);
	string::copy(m_data->str, (const tchar*)s, m_data->data.alloc);
	return *this;
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
