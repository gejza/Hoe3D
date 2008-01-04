
#ifndef _HOE_STRING_H_
#define _HOE_STRING_H_

#define ENABLE_AUTOCONV_FUNCTIONS 1
////////////////////////////////
// string class
namespace HoeCore {
namespace string {

#undef vsnprintf

int vsnprintf(char *, size_t, const char *, va_list);
int vsnprintf(wchar_t *, size_t, const wchar_t *, va_list);
void copy(char *, const char *, size_t);
void copy(wchar_t *, const wchar_t *, size_t);

inline int cmp(const char*s1, const char*s2)
{
	return ::strcmp(s1,s2);
}
inline int cmp(const wchar_t* s1, const wchar_t* s2)
{
	return ::wcscmp(s1,s2);
}

inline size_t len(const char * s)
{
	return strlen(s);
}

inline size_t len(const wchar_t * s)
{
	return wcslen(s);
}

size_t utf8len(const wchar_t * s);

inline bool ifbegin(const tchar* begin, const tchar* str) { return false; }

inline float atof(const char* str) 
{
	return (float) ::atof(str);
}
inline float atof(const wchar_t* str) 
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

inline int xtoi(const tchar* str) 
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


inline int atoi(const char* str) 
{ 
	return ::atoi(str); 
}

inline int atoi(const wchar_t* str);

inline int strtoi(const tchar* str)
{
	if (str[0] == '0' && str[1] == 'x')
		return xtoi(str+2);
	else
		return atoi(str);
}


inline int scanf(const char *, const char *, ...)
{
    assert(0);
    return 0;
}
inline int scanf(const tchar *, const wchar_t *, ...)
{
    assert(0);
    return 0;
}

inline void concat(char* dest, size_t sizeb, char c)
{
	char src[2] = { c, 0 };
	::strncat(dest, src, sizeb);
}

inline void concat(char* dest, size_t sizeb, const char* src)
{
	::strncat(dest, src, sizeb);
}

#ifdef ENABLE_AUTOCONV_FUNCTIONS
int vsnprintf(char *, size_t, const wchar_t *, va_list);
int vsnprintf(wchar_t *, size_t, const char *, va_list);
void copy(char *, const wchar_t *, size_t);
void copy(wchar_t *, const char *, size_t);
int cmp(const char*, const wchar_t*);
inline int cmp(const wchar_t* s1, const char* s2)
{
	return -cmp(s2,s1);
}

#endif // ENABLE_AUTOCONV_FUNCTIONS

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
	String_s(const tchar * s)
	{
		string::copy(m_str, s, maxsize-1);
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
	// obecne funkce
	bool IsEmpty() const { return m_str[0] == 0; }
	// operatory
	operator const tchar * () const { return m_str;}
#if defined(ENABLE_AUTOCONV_FUNCTIONS) || !defined(_UNICODE)
	bool operator == (const char * s) const;
	const String_s & operator = (const char * s)
	{
		string::copy(m_str, s, maxsize-1);
		return *this;
	}
	const String_s& operator += (char c)
	{
		concat(c); return *this;
	}
	const String_s& operator += (const char* str)
	{
		concat(str); return *this;
	}
	void concat(const char * str)
	{
		string::concat(m_str, maxsize, str);
	}
	void concat(char c)
	{
		string::concat(m_str, maxsize, c);
	}
#endif
#if defined(ENABLE_AUTOCONV_FUNCTIONS) || defined(_UNICODE)
	const String_s & operator = (const wchar_t * s)
	{
		string::copy(m_str, s, maxsize-1);
		return *this;
	}
	bool operator == (const wchar_t * s) const;
	void concat(const wchar_t *);
#endif

};

class String
{
	struct StringData
	{
		mutable int ref;
		size_t alloc;

		void Init(int num);
		int Lock() { return ++ref; }
		inline void Unlock() { if (!--ref) free(this); }
		bool IsShared() { return ref > 1; }
	};

	struct StringDataPtr 
	{
		StringData data;
		tchar str[];
	} * m_data;

	static StringDataPtr* CreateStringData(size_t n);
	void PrepareForModify(size_t n);
public:
	String();
	String(const String& s);
	~String();
	//int printf(const char * szFormat, ...) { return 0; }
	const String & operator = (const String& s);
	bool IsEmpty() const { return !m_data || !m_data->str[0]; }
	operator bool () const { return !IsEmpty(); }
	size_t Length() const { return m_data ? string::len(m_data->str):0; }
	//void Export(char *, size_t size) {}
	operator const tchar * () const { return m_data ? m_data->str:T(""); }
#if defined(ENABLE_AUTOCONV_FUNCTIONS) || !defined(_UNICODE)
	const String & operator = (const char * s);
	const String& operator += (char c)
	{
		concat(c); return *this;
	}
	const String& operator += (const char* str)
	{
		concat(str); return *this;
	}
	void concat(const char * str);
	void concat(char c)
	{
		char src[2] = { c, 0 };
		concat(src);
	}
#endif
#if defined(ENABLE_AUTOCONV_FUNCTIONS) || defined(_UNICODE)
	const String & operator = (const wchar_t * s);
	const String& operator += (wchar_t c)
	{
		concat(c); return *this;
	}
	const String& operator += (const wchar_t* str)
	{
		concat(str); return *this;
	}
	void concat(const wchar_t * str);
	void concat(wchar_t c)
	{
		wchar_t src[2] = { c, 0 };
		concat(src);
	}
#endif
};

} // namespace HoeCore

#endif // _HOE_STRING_H_

