
#ifndef _HOE_STRING_H_
#define _HOE_STRING_H_

////////////////////////////////
// string class
namespace HoeCore {

template<size_t maxsize> class String_s
{
	char m_str[maxsize];
public:
	// konstruktory
	String_s()
	{
		m_str[0] = 0;
	}
	// funkce ze stdio (proto jsou v jinem coding stylu)
	int printf(const char * szFormat, ...)
	{
		int ret;
		va_list args;
		va_start(args, szFormat);
		ret = vsnprintf( m_str, maxsize-1, szFormat, args );
		va_end(args);
		return ret;
	}
	// obecne funkce
	bool IsEmpty() const { return m_str[0] == 0; }
	// operatory
	operator const char * () const { return m_str;}
	const String_s & operator = (const char * s)
	{
		strncpy(m_str, s, maxsize-1);
		return *this;
	}
};

class String
{
public:
	int printf(const char * szFormat, ...);
};

} // namespace HoeCore

#endif // _HOE_STRING_H_

