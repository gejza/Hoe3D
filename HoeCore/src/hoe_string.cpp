
#include "StdAfx.h"
#include "../include/HoeCore/hoe_core.h"
#include "../include/HoeCore/hoe_string.h"
#include "../include/HoeCore/hoe_unicode.h"

#ifdef _WIN32
#define va_copy(dest, src) (dest = src)
#endif

////////////////////////////////
// string class
namespace HoeCore {
namespace string {

int snprintf(char *dest, size_t n, const char * fmt, ...)
{
    int ret;
	va_list args;
	va_start(args, fmt);
	ret = string::vsnprintf(dest, n, fmt, args);
	va_end(args);
    return ret;
}

int vsnprintf(char *dest, size_t n, const char * fmt, va_list vl)
{
    int ret;
#if defined _WIN32_WINNT
	ret = ::vsnprintf_s(dest, n, n-1, fmt, vl);
#elif defined _WIN32_WCE
	ret = ::_vsnprintf(dest, n, fmt, vl);
#else
	ret = ::vsnprintf(dest, n, fmt, vl);
#endif
    if (ret < 0)
        return n;
    return ret;
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

int snprintf(wchar_t *dest, size_t n, const char * fmt, ...)
{
    int ret;
	va_list args;
	va_start(args, fmt);
	ret = string::vsnprintf(dest, n, fmt, args);
	va_end(args);
    return ret;
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

int snprintf(char *dest, size_t n, const wchar_t * fmt, ...)
{
    int ret;
	va_list args;
	va_start(args, fmt);
	ret = string::vsnprintf(dest, n, fmt, args);
	va_end(args);
    return ret;
}



int vsnprintf(wchar_t* dest, size_t n, const wchar_t* fmt, va_list vl)
{
    int ret;
#if defined _WIN32
	ret = ::_vsnwprintf(dest, n, fmt, vl);
#else
	ret = ::vswprintf(dest, n, fmt, vl);
#endif
    if (ret < 0)
        return n;
	return ret;
}

int snprintf(wchar_t *dest, size_t n, const wchar_t * fmt, ...)
{
    int ret;
	va_list args;
	va_start(args, fmt);
	ret = string::vsnprintf(dest, n, fmt, args);
	va_end(args);
    return ret;
}



int copy(char * dest, const char * src, size_t cnt)
{
	size_t len = ::strlen(src) + 1;
	if (cnt > len)
	{
		cnt = len;
	}
	::strncpy(dest, src, cnt);
	return cnt;
}

int copy(wchar_t * dest, const wchar_t * src, size_t cnt)
{
	size_t len = ::wcslen(src) + 1;
	if (cnt > len)
	{
		cnt = len;
	}
	::wcsncpy(dest, src, cnt);
	return cnt;
}

int copy(wchar_t * dest, const char * src, size_t s)
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
	return s;

}

/*
U+00000000 - U+0000007F	0xxxxxxx
U+00000080 - U+000007FF	110xxxxx 10xxxxxx
U+00000800 - U+0000FFFF	1110xxxx 10xxxxxx 10xxxxxx
*/

int copy(char * dest, const wchar_t * src, size_t n)
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
	return n; //todo udelat spravne pocitani u vsech copy funkci
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

void concat(char* dest, size_t sizeb, char c)
{
	char src[2] = { c, 0 };
	::strncat(dest, src, sizeb);
}

void concat(char* dest, size_t sizeb, const char* src)
{
	::strncat(dest, src, sizeb);
}

int find_last(const char* str,char f)
{
	int finded = -1;
	for (int t=0;str[t];t++)
		if (str[t] == f)
			finded = t;
	return finded;
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

// util func
template<typename CHARP, typename CHARS> bool wmatch(const CHARP* pat, const CHARS* str)
{
	// todo: spravne porovnani char a wchar <-
	const CHARS* s;
	const CHARP* p;
	bool star = false;
loopStart:
	for (s = str, p = pat; *s; ++s, ++p) 
	{
		switch (*p) 
		{
		 case '?':
			if (*s == '.') goto starCheck;
			break;
		 case '*':
			star = true;
			str = s, pat = p;
			if (!*++pat) return true;
			goto loopStart;
		 default:
			if (*s != *p)
			   goto starCheck;
			break;
		} /* endswitch */
	} /* endfor */
	if (*p == '*') ++p;
	return (!*p);

starCheck:
	if (!star) return false;
	str++;
	goto loopStart;
}

bool wmatch(const wchar_t* pattern, const wchar_t* str)
{
	return wmatch<wchar_t, wchar_t>(pattern, str);
}
bool wmatch(const char* pattern, const char* str)
{
	return wmatch<char, char>(pattern, str);
}
bool wmatch(const wchar_t* pattern, const char* str)
{
	return wmatch<wchar_t, char>(pattern, str);
}
bool wmatch(const char* pattern, const wchar_t* str)
{
	return wmatch<char, wchar_t>(pattern, str);
}

int join(char* str, size_t s, const SetBase<const char*>& n, const char* sep, int limit)
{
	int w = 0;
	s--; // pojistka zapsani posledniho znaku
	size_t ls = sep ? string::len(sep):0;
	if (limit == 0 || limit > n.Count())
		limit = n.Count();

	for (int i=0;i < limit;i++)
	{
		if (i && sep)
		{
			if (str && s >= ls)
			{
				string::copy(str, sep, ls);
				str += ls; s -= ls; 
			}
			else
				str = NULL;
			w++;
		}

		// name
		size_t l = string::len(n[i]);
		if (s >= l)
		{
			string::copy(str, n[i], l);
			str += l; s -= l; 
		}
		w += l;
	}
	*str = 0; return w;
}

int join(wchar_t* str, size_t s, const List<const wchar_t*>& n, const wchar_t* sep, int limit)
{
	int w = 0;
	s--; // pojistka zapsani posledniho znaku
	size_t ls = sep ? string::len(sep):0;
	if (limit == 0 || limit > n.Count())
		limit = n.Count();
	for (int i=0;i < limit;i++)
	{
		if (i && sep)
		{
			if (str && s >= ls)
			{
				string::copy(str, sep, ls);
				str += ls; s -= ls; 
			}
			else
				str = NULL;
			w++;
		}

		// name
		size_t l = string::len(n[i]);
		if (s >= l)
		{
			string::copy(str, n[i], l);
			str += l; s -= l; 
		}
		w += l;
	}
	*str = 0; return w;
}


} // end namespace string

// CString
String CString::Sub(int pos, int last) const
{
	int t = (int)Length();
	if (pos > t)
		return "";
	if (last == -1 || last > t)
		last = t;
	if (pos >= last)
		return "";
	String s;
	s.PrepareForModify(last-pos+1, true);
	string::copy(s.m_data->str, GetPtr()+pos, (size_t)(last-pos));
	s.m_data->str[last-pos] = '\0';
	return s;
}

// String
String String::Empty;

String::String()
{
	m_data = NULL;
}

String::String(const String& s)
{
	m_data = NULL;
	Set(s);
}

String::String(const char* str)
{
	m_data = NULL;
	Set(str);
}

String::String(const wchar_t* str)
{
	m_data = NULL;
	Set(str);
}

String::~String()
{
	if (m_data)
		m_data->data.Unlock();
}

void String::Set(const String& s)
{
	if (this == &s)
		return;
	if (m_data)
		m_data->data.Unlock();
	m_data = NULL;
	if (!s.IsEmpty())
	{
		m_data = s.m_data;
		m_data->data.Lock();
	}
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

void String::Set(const char * s)
{
	size_t l = string::len(s);
	if (m_data) 
		m_data->data.Unlock();

	m_data = CreateStringData(l);
	string::copy(m_data->str, s, m_data->data.alloc);
}

void String::Strip(size_t max)
{
	if (Length() <= max)
		return;
	PrepareForModify(max, false);
	m_data->str[max] = 0;
}

void String::Set(const wchar_t * s)
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
}

String::StringDataPtr* String::CreateStringData(size_t n)
{
	const size_t an = sizeof(StringData) + (n+1)*sizeof(tchar);
	StringDataPtr * data = (StringDataPtr*)malloc(an);
	data->data.ref = 1;
	data->data.alloc = (n+1);
	return data;
}

void String::PrepareForModify(size_t n, bool canempty)
{
	if (!m_data)
	{
		m_data = CreateStringData(n);
		m_data->str[0] = 0;
	}
	else if (m_data->data.IsShared() || n > m_data->data.alloc)
	{
		StringDataPtr * data = CreateStringData(n);
		if (!canempty)
			string::copy(data->str, m_data->str, n);
		m_data->data.Unlock();
		m_data = data;
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
		PrepareForModify(s1 + s2 + 1, false);
		string::copy(m_data->str + s1, str, m_data->data.alloc - s1);
	}
}

void String::concat(const wchar_t * str)
{
	if (IsEmpty())
	{
		*this = str;
	}
	else
	{
		size_t s1 = string::len(m_data->str);
		size_t s2 = string::len(str);
		PrepareForModify(s1 + s2 + 1, false);
		string::copy(m_data->str + s1, str, m_data->data.alloc - s1);
	}
}

int String::printf(const char * szFormat, ...)
{
	int ret;
	va_list args;
	va_start(args, szFormat);
	ret = this->vprintf(szFormat, args);
	va_end(args);
	return ret;
}

int String::vprintf(const char * szFormat, va_list vl)
{
	// todo optimalizovat
	size_t max = string::len(szFormat);
	max = size_t(max * 1.7);
	int ret = 0;
	while (1)
	{
		PrepareForModify(max+1, false);
		hoe_assert(m_data->str);
		va_list vnl;
		va_copy(vnl, vl);
		ret = string::vsnprintf(m_data->str, max, szFormat, vnl);
		va_end(vnl);
		if (ret >= 0 && ret < max)
		    break;
		if (ret > max)
		    max = ret + 1;
		else
			max = size_t(max * 1.5) + 1;
	}
	return ret;
}

int String::printf(const wchar_t * szFormat, ...)
{
	int ret;
	va_list args;
	va_start(args, szFormat);
	ret = this->vprintf(szFormat, args);
	va_end(args);
	return ret;
}

int String::vprintf(const wchar_t * szFormat, va_list vl)
{
	// todo optimalizovat
	size_t max = string::len(szFormat);
	max = size_t(max * 1.7);
	int ret = 0;
	while (1)
	{
		PrepareForModify(max+1, false);
        	hoe_assert(m_data->str);
		va_list vnl;
		va_copy(vnl, vl);
		ret = string::vsnprintf(m_data->str, max, szFormat, vl);
		va_end(vnl);
		if (ret >= 0 && ret < max)
		    break;
		if (ret > max)
		    max = ret + 1;
		else
			max = size_t(max * 1.5) + 1;
	}
	return ret;
}

int String::Replace(tchar f, tchar r)
{
	// todo optimalizovat
	int n = 0;
	if (!m_data)
		return 0;
	PrepareForModify(m_data->data.alloc,false);
	tchar* s = m_data->str;
	while (*s)
	{
		if (*s == f)
		{
			*s = r;
			n++;
		}
		s++;
	}
	return n;
}

void String::Short(int max)
{
	// todo optimalizovat
	if (!m_data || Length() <= max)
		return;
	PrepareForModify(m_data->data.alloc,false);
	m_data->str[max] = T('\0');
}

bool String::wmatch(const wchar_t* pattern)
{
	return string::wmatch(pattern, this->GetPtr());
}

bool String::wmatch(const char* pattern)
{
	return string::wmatch(pattern, this->GetPtr());
}


} // namespace HoeCore
