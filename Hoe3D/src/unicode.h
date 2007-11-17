
#ifndef _HOE_CODEPAGE_UNICODE_H_
#define _HOE_CODEPAGE_UNICODE_H_

class CodePage
{
protected:
	byte * m_pages[256];
	wchar_t m_codes[256]; // predelat na pohyblive pole
	int m_num;
public:
	CodePage();
	~CodePage();
	inline int GetIndex(wchar_t unicode) { 
		register int page = (unicode >> 8) & 0xff;
		if (m_pages[page])return (m_pages[page])[unicode&0xff];
		else
			return 'x';
	}
	inline wchar_t GetChar(int index) { return m_codes[index]; }
	void AddChar(wchar_t ch);
	static wchar_t UTFtoUnicode(const char *&p);
	int StringToIndex(const char *&p) { return GetIndex(UTFtoUnicode(p)); }
	int StringToIndex(const wchar_t *&p) 
	{ 
		int ret = GetIndex(*p);
		p++; return ret;
	}
	int GetNumChars() { return m_num; } 
	static int c_specialchars(int argc, const tchar * argv[], void * param);
};

#endif // _HOE_CODEPAGE_UNICODE_H_

