
#ifndef _HOE_STRING_H_
#define _HOE_STRING_H_

////////////////////////////////
// string class
namespace HoeCore {

namespace string {

#undef vsnprintf

int vsnprintf(char *, size_t, const char *, va_list);
int vsnprintf(char *, size_t, const wchar_t *, va_list);
int vsnprintf(wchar_t *, size_t, const char *, va_list);
int vsnprintf(wchar_t *, size_t, const wchar_t *, va_list);
void copy(tchar *, const char *, size_t);
void copy(tchar *, const wchar_t *, size_t);

inline int cmp(const char*s1, const char*s2)
{
	return ::strcmp(s1,s2);
}
int cmp(const char*, const wchar_t*);
inline int cmp(const wchar_t* s1, const char* s2)
{
	return -cmp(s2,s1);
}
inline int cmp(const wchar_t* s1, const wchar_t* s2)
{
#ifdef _LINUX
    return 0;
#else
	return ::wcscmp(s1,s2);
#endif
}

inline size_t len(const tchar * s)
{
#ifdef UNICODE
	return wcslen(s);
#else
	return strlen(s);
#endif
}


inline bool ifbegin(const tchar* begin, const tchar* str) { return false; }

inline float atof(const tchar* str) { return 0; }
inline int atoi(const tchar* str) { return 0; }

inline int scanf(const char *, const char *, ...)
{
    assert(0);
    return 0;
}

} // end namespace

template<size_t maxsize> class String_s
{
	tchar m_str[maxsize];
public:
	// konstruktory
	String_s()
	{
		m_str[0] = 0;
	}
	// funkce ze stdio (proto jsou v jinem coding stylu)
	int printf(const char * szFormat, ...) { return 0; }
	/*{
		int ret;
		va_list args;
		va_start(args, szFormat);
		ret = Str::vsnprintf( m_str, maxsize-1, szFormat, args );
		va_end(args);
		return ret;
	}*/
	int printf(const wchar_t * szFormat, ...) { return 0; }
	void cat(const char *) {}
	void cat(const wchar_t *) {}
	// obecne funkce
	bool IsEmpty() const { return m_str[0] == 0; }
	// operatory
	operator const tchar * () const { return m_str;}
	const String_s & operator = (const char * s)
	{
		string::copy(m_str, s, maxsize-1);
		return *this;
	}
	const String_s & operator = (const wchar_t * s)
	{
		string::copy(m_str, s, maxsize-1);
		return *this;
	}
	bool operator == (const char * s) const;
	bool operator == (const wchar_t * s) const;
};

class String
{
public:
	int printf(const char * szFormat, ...) { return 0; }
	const String & operator = (const char * s) {  return *this; }
	const String & operator = (const wchar_t * s)  {  return *this; }
	operator bool () { return false; }
	size_t Length() { return 0; }
	void Export(char *, size_t size) {}
	operator const tchar * () const { return T(""); }
};

} // namespace HoeCore

#endif // _HOE_STRING_H_

