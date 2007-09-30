
#ifndef _HOE_STRING_H_
#define _HOE_STRING_H_

////////////////////////////////
// string class
namespace HoeCore {

namespace Str {

int vsnprintf(tchar *, size_t, const char *, va_list);
int vsnprintf(tchar *, size_t, const wchar_t *, va_list);
void strncpy(tchar *, const char *, size_t);
void strncpy(tchar *, const wchar_t *, size_t);

} // end namespace

namespace string {

int vsnprintf(tchar *, size_t, const char *, va_list);
int vsnprintf(tchar *, size_t, const wchar_t *, va_list);
void copy(tchar *, const char *, size_t);
void copy(tchar *, const wchar_t *, size_t);

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
	int printf(const char * szFormat, ...);
	/*{
		int ret;
		va_list args;
		va_start(args, szFormat);
		ret = Str::vsnprintf( m_str, maxsize-1, szFormat, args );
		va_end(args);
		return ret;
	}*/
	int wprintf(const wchar_t * szFormat, ...);
	void cat(const char *);
	void cat(const wchar_t *);
	// obecne funkce
	bool IsEmpty() const { return m_str[0] == 0; }
	// operatory
	operator const tchar * () const { return m_str;}
	const String_s & operator = (const char * s)
	{
		Str::strncpy(m_str, s, maxsize-1);
		return *this;
	}
	const String_s & operator = (const wchar_t * s)
	{
		Str::strncpy(m_str, s, maxsize-1);
		return *this;
	}
	bool operator == (const char * s) const;
	bool operator == (const wchar_t * s) const;
};

class String
{
public:
	int printf(const char * szFormat, ...);
	const String & operator = (const char * s);
	operator bool ();
	size_t Length();
	void Export(char *, size_t size);
	operator const tchar * () const;
};

} // namespace HoeCore

#endif // _HOE_STRING_H_

