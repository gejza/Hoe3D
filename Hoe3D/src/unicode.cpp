
#include "system.h"
#include "shared.h"
#include "cmdexec.h"
#include "unicode.h"

CodePage::CodePage()
{
	SET_SHARED_PTR(codepage);

	memset(m_pages, 0, sizeof(m_pages));
	memset(m_codes, 0, sizeof(m_codes));
	m_num = 0;

	// rozsireni tabulky o zakladni ascii znaky
	for (wchar_t c = ' '; c <= 'z';c++)
		AddChar(c);

	GetExec()->Register("specialchars", CodePage::c_specialchars, NULL, "Register next unicode chars.");
	//Register("quit",c_quit,NULL,"quit program");
	//Register("quit!",c_fquit,NULL,"force quit");
	//Register("help",c_help,NULL);
	//Register("list",c_list,NULL,"List all commands.");
}

CodePage::~CodePage()
{
	// vymazani tabulek
	for (int i=0;i < 256;i++)
	{
		if (m_pages[i])
			delete [] m_pages[i];
	}

	UNSET_SHARED_PTR(codepage);
}

void CodePage::AddChar(wchar_t ch)
{
	// aaa
	register int page = (ch >> 8) & 0xff;
	if (m_pages[page]==NULL)
	{
		m_pages[page] = new byte[256];
		memset(m_pages[page], 0, 256);
	}
	else if (m_pages[page][ch&0xff])
		return;
	// addchar
	assert(m_num < 0xfe);
	m_pages[page][ch&0xff] = m_num;
	m_codes[m_num] = ch;
	m_num++;
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
	return 'X';
}

int CodePage::c_specialchars(int argc, const char * argv[], void * param)
{
	for (int i=1;i < argc;i++)
	{
		const char * chars = argv[i];
		while (*chars)
		{
			GetCodePage()->AddChar(UTFtoUnicode(chars));
		}
	}
	return 0;
}

