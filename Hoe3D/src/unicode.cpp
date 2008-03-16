#include "StdAfx.h"
#include "shared.h"
#include "cmdexec.h"
#include "unicode.h"

CodePage::CodePage()
{
	memset(m_pages, 0, sizeof(m_pages));
	memset(m_codes, 0, sizeof(m_codes));
	m_num = 0;

}

CodePage::~CodePage()
{
	// vymazani tabulek
	for (int i=0;i < 256;i++)
	{
		if (m_pages[i])
			delete [] m_pages[i];
	}
}

int CodePage::AddChar(wchar_t ch)
{
	// aaa
	// addchar
	assert(m_num < 0xfe);
	AddAliasChar(ch, m_num);
	m_codes[m_num] = ch;
	return m_num++;
}

void CodePage::AddAliasChar(wchar_t ch, int index)
{
	register int page = (ch >> 8) & 0xff;
	if (m_pages[page]==NULL)
	{
		m_pages[page] = new byte[256];
		memset(m_pages[page], 0, 256);
	}
	else if (m_pages[page][ch&0xff])
		return;
	m_pages[page][ch&0xff] = index;
}

wchar_t CodePage::UTFtoUnicode(const char *&p)
{
	register wchar_t ch = 0;
	// pokud *p < 0x80
	if (!(*p & 0x80)) return *p++;
	if ((*p & 0xE0) == 0xC0)
	{
		ch |= ((*p) & 0x1F) << 6;p++;
		ch |= (*p) & 0x3F; p++;
		return ch;
	}
	if ((*p & 0xF0) == 0xE0)
	{
		ch |= (*p & 0xF) << 12;p++;
		ch |= (*p & 0x3F) << 6;p++;
		ch |= (*p) & 0x3F; p++;
		return ch;
	}	
	assert("char is not in UTF-8");
	p++;
	return 'X';
}

MainCodePage::MainCodePage()
{
	SET_SHARED_PTR(codepage);

	// rozsireni tabulky o zakladni ascii znaky
	for (wchar_t c = ' '; c <= 'z';c++)
		AddChar(c);

	GetExec()->Register(T("specialchars"), MainCodePage::c_specialchars, NULL, T("Register next unicode chars."));
	//Register("quit",c_quit,NULL,"quit program");
	//Register("quit!",c_fquit,NULL,"force quit");
	//Register("help",c_help,NULL);
	//Register("list",c_list,NULL,"List all commands.");
}

MainCodePage::~MainCodePage()
{
	UNSET_SHARED_PTR(codepage);
}

int MainCodePage::c_specialchars(int argc, const tchar * argv[], void * param)
{
	for (int i=1;i < argc;i++)
	{
		const tchar * chars = argv[i];
		while (*chars)
		{
#ifdef _UNICODE
			GetCodePage()->AddChar(*chars++);
#else
			GetCodePage()->AddChar(UTFtoUnicode(chars));
#endif
		}
	}
	return 0;
}

