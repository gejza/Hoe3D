
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

int find_last(const char* str,char f);

float GetReal(const char* str);
float GetReal(const wchar_t* str);
int GetHex(const char* str);
int GetHex(const wchar_t* str);
int GetNumber(const char* str);
int GetNumber(const wchar_t* str);


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

void concat(char* dest, size_t sizeb, char c);
void concat(char* dest, size_t sizeb, const char* src);

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

class String;

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
	//int printf(const char * szFormat, ...) { return 0; }
	/*{
		int ret;
		va_list args;
		va_start(args, szFormat);
		ret = Str::vsnprintf( m_str, maxsize-1, szFormat, args );
		va_end(args);
		return ret;
	}*/
	//int printf(const wchar_t * szFormat, ...) { return 0; }
	// obecne funkce
	bool IsEmpty() const { return m_str[0] == 0; }
	// operatory
	operator const tchar * () const { return this->GetPtr();}
	const tchar * GetPtr() const { return m_str;}
	operator const String () const;

	tchar& operator [](const int index)
	{
		static tchar fc = 0;
		if (index < 0 || index >= maxsize)
			return fc;
		return m_str[index];
	}
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
	int FindLast(char c) const
	{
		return string::find_last(m_str, c);
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

class CString
{
    const tchar* m_str;
public:
    CString(const tchar* str)
    {
        m_str = str;
    }
    CString(const CString& str)
    {
        m_str = str.m_str;
    }
	operator const tchar * () const { return GetPtr(); }
	const tchar * GetPtr() const { return m_str ? m_str:T(""); }
	size_t Length() const { return string::len(GetPtr()); }
#if defined(ENABLE_AUTOCONV_FUNCTIONS) || !defined(_UNICODE)
	bool operator == (const char * s) const
    {
        return string::cmp(m_str, s) == 0;
    }
#endif
#if defined(ENABLE_AUTOCONV_FUNCTIONS) || defined(_UNICODE)
	bool operator == (const wchar_t * s) const
    {
        return string::cmp(m_str, s) == 0;
    }
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
		tchar str[1];
	} * m_data;

	static StringDataPtr* CreateStringData(size_t n);
	void PrepareForModify(size_t n, bool canempty);
public:
	String();
	String(const String& s);
	template<int size>String(const String_s<size>& s)
    {
	    String((const tchar*)s);
    }
	~String();
	//int printf(const char * szFormat, ...) { return 0; }
	void Set(const String& s);
	const String & operator = (const String& s)
	{
		Set(s); return *this;
	}
	const String & operator = (const CString& s);
	bool IsEmpty() const { return !m_data || !m_data->str[0]; }
	operator bool () const { return !IsEmpty(); }
	size_t Length() const { return m_data ? string::len(m_data->str):0; }
	//void Export(char *, size_t size) {}
	const tchar * GetPtr() const { return m_data ? m_data->str:T(""); }
	operator const tchar * () const { return GetPtr(); }
	operator const CString () const { return CString(m_data ? m_data->str:T("")); }

	// utf section
#if defined(ENABLE_AUTOCONV_FUNCTIONS) || !defined(_UNICODE)
	String(const char* s);
	void Set(const char * s);
	bool operator == (const char* s) const
	{
		return string::cmp(GetPtr(), s) == 0;
	}
	const String & operator = (const char * s)
	{
		Set(s); return *this;
	}
	template<int size>const String & operator = (const String_s<size>& s)
	{
		Set(s); return *this;
	}
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
	int printf(const char * szFormat, ...);
	int vprintf(const char * szFormat, va_list);
	/** Nahrazeni znaku
	* @param f Hledany znak
	* @param r Nahrazeny znak
	* @return Pocet nahrazeni
	*/
	int Replace(char f, char r);
	int Find(char f);

#endif
	// unicode section
#if defined(ENABLE_AUTOCONV_FUNCTIONS) || defined(_UNICODE)
	String(const wchar_t* s);
	void Set(const wchar_t * s);
	bool operator == (const wchar_t* s) const
	{
		return string::cmp(GetPtr(), s) == 0;
	}
	const String & operator = (const wchar_t * s)
	{
		Set(s); return *this;
	}
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

	static String Empty;
};


template<size_t maxsize> HOE_INLINE  String_s<maxsize>::operator const String () const
{
    return String(this->GetPtr());
}





} // namespace HoeCore

#endif // _HOE_STRING_H_

