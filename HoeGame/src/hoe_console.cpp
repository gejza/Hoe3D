

#include "StdAfx.h"
#include <hoe3d.h>
#include "../include/hoe_console.h"
#include "../include/hoe_app.h"

BEGIN_HOEGAME 

#ifdef _UNICODE
#define strlen wcslen
#endif

BaseConsole * BaseConsole::m_shared = NULL;

BaseConsole::BaseConsole()
{
	assert(m_shared == NULL);
	m_shared = this;
}

BaseConsole::~BaseConsole()
{
	assert(m_shared == this);
	m_shared = NULL;
}

void BaseConsole::Printfarg(const char * szFormat, va_list args)
{
	static tchar szBuff[1024];
	HoeCore::string::vsnprintf( szBuff, 1024, szFormat, args );
	if (m_shared)
		m_shared->Con_Print(szBuff);
	/*!!!*/
#if (defined(DEBUG) || defined(_DEBUG)) && defined(_WIN32_WINNT) 
	OutputDebugString( szBuff );OutputDebugString( T("\n") );
#endif
}

void BaseConsole::Printfarg(const wchar_t * szFormat, va_list args)
{
	static tchar szBuff[1024];
	HoeCore::string::vsnprintf( szBuff, 1024, szFormat, args );
	if (m_shared)
		m_shared->Con_Print(szBuff);
	/*!!!*/
#if (defined(DEBUG) || defined(_DEBUG)) && defined(_WIN32_WINNT) 
	OutputDebugString( szBuff );OutputDebugString( T("\n") );
#endif
}

void BaseConsole::Printf(const char * szFormat, ...)
{
	va_list args;

	va_start(args, szFormat);
	Printfarg(szFormat,args);
	va_end(args);
}

void BaseConsole::Printf(const wchar_t * szFormat, ...)
{
	va_list args;

	va_start(args, szFormat);
	Printfarg(szFormat,args);
	va_end(args);
}


///////////////////////////////////////////////////////////////

Console::Console()
{
	log = NULL;
	app_callback = NULL;
}

Console::~Console()
{
	if (log)
        fclose(log);
}

void Console::SetFileLogging(const char * flog)
{
	if (log)
        fclose(log);
	log = fopen(flog,"wt");
	// ulozit UTF-8
	if (log)
	{
		byte utf[] = { 0xef, 0xbb, 0xbf };
		fwrite(utf, 1, 3, log);
	}
}

void Console::Con_Print(const tchar * str)
{
	// repeated
	/*static dword lastid = 0;
	static int num = 0;
	dword id = HoeCore::HashString(str);
	if (id != lastid)
	{
		if (num)
		{
			char repeat[1024];
			sprintf(repeat, "Last message was repeated %d times.",num);
			PrintMessage(repeat);
		}
		num=0;
		lastid=id;
		PrintMessage(str);
	}
	else
		num++;*/

	fprintf(stderr,"%s\n",str);
	if (log)
	{
        fprintf(log,"%s\n",str);
		fflush(log);
	}

	m_lines.AddTail().SetText(str);
	if (app_callback)
		app_callback->UpdateConsole();

}

////////////////////////////////////////////////////////////////

GuiConsole::GuiConsole(Console & con)
: m_console(con)
{
	font = NULL;
	cmdline[0] = '\0';
	state = sClose;
	maxheight = 240;
	height = 0.f;
	prev = NULL;
	m_histptr = 0;
}

bool GuiConsole::Load(IHoe3DEngine *eng)
{
	engine = eng;
	font = (IHoeFont *)eng->Create(T("font '../data/font.ttf' 12"));
	if (!font)
		return false;
	background = (IHoePicture *)eng->Create(T("picture lake"));
	if (!background)
		return false;

	return true;
}

void GuiConsole::Draw(IHoe2D * h2d)
{
	if (state == sClose)
		return;

	float h = height * maxheight;
	h2d->SetRect(640,480);
	h2d->BltFast(0,640,h - maxheight,h,background);

	bool blink = (((int)(engine->SysFloatTime() * 2)) % 2) == 0;

	float y = h - 3 - 14;
	if (blink)
		AddChar('_');
	font->DrawText(5,y,m_fontcolor,cmdline);
	if (blink)
		Back();

	y -= 14;
	
	int n = 0;
	
	while (y > -14)
	{
		const tchar * str = m_console.GetLines().GetLine(n++).GetText();
		font->DrawText(5,y,m_fontcolor,str);
		y -= 14;
	}
	
}

bool GuiConsole::Key(int k)
{
	switch (k)
	{
	case HK_RETURN:
		m_console.Con_Print(cmdline);
		Unacquire();
		engine->exec(cmdline);
		// history? yes
		m_history.AddTail().SetText(cmdline);
		m_histptr = -1;
		cmdline[0] = 0; 
		Acquire();
		return false;
	case HK_UP:
		if (m_histptr < (m_history.Count()-1))
		{
			HoeCore::string::copy(cmdline, m_history.GetLine(++m_histptr).GetText(),sizeof(cmdline));
		}
		return false;
	case HK_DOWN:
		if (m_histptr > 0)
		{
			HoeCore::string::copy(cmdline, 
				m_history.GetLine(--m_histptr).GetText(), sizeof(cmdline));
		}
		return false;
	case HK_ESCAPE:
	case HK_GRAVE:
		if (state == sOpen || state == sGoOpen)
            Close();
		else
			Open();
		return false;
	/*default:
		if (k < g_charmapsize && g_charmap[k].text)
			con->Printf("push: %s",g_charmap[k].text);
		else
			con->Printf("push: 0x%x",k); */
	};
	return true;
}

void GuiConsole::AddChar(char c)
{
	if (strlen(cmdline) > (sizeof(cmdline) - 5))
		return;

	int lastchar = strlen(cmdline);
	cmdline[lastchar] = c;
	cmdline[lastchar+1] = 0; 
}

void GuiConsole::Back()
{
	int lastchar = strlen(cmdline);
	if (lastchar > 0)
	{
		cmdline[lastchar-1] = 0; 
	}
}

void GuiConsole::Open(void)
{
	if (state == sClose || state == sGoClose)
	{
		Acquire();
		state = sGoOpen;
	}
}

void GuiConsole::Close(void)
{
	if (state == sOpen || state == sGoOpen)
	{
		Unacquire();
		state = sGoClose;
	}
}

void GuiConsole::Acquire()
{
		// chytit input
		prev = HoeGetInput(engine)->RegisterKeyboard(this);
}

void GuiConsole::Unacquire()
{
#ifdef _DEBUG
		XHoeKeyboard * actrl = HoeGetInput(engine)->RegisterKeyboard(prev);
		assert(actrl == this);
#else
		HoeGetInput(engine)->RegisterKeyboard(prev);
#endif

}

void GuiConsole::Update(float dtime)
{
	if (state == sGoOpen)
	{
		height += dtime * 3.f;
		if (height > 1.f)
		{
			height = 1.f;
			state = sOpen;
		}
	}
	if (state == sGoClose)
	{
		height -= dtime * 3.f;
		if (height < 0.f)
		{
			height = 0.f;
			state = sClose;
		}
	}
}

void GuiConsole::RegisterCommands(IHoe3DEngine * engine)
{
	engine->RegisterCmd(T("openconsole"),GuiConsole::c_openconsole,this);
	engine->RegisterCmd(T("closeconsole"),GuiConsole::c_closeconsole,this);
}

int GuiConsole::c_openconsole(int argc, const tchar * argv[], void * param)
{
	assert(param != NULL);
	reinterpret_cast<GuiConsole*>(param)->Open();
	return 0;
}

int GuiConsole::c_closeconsole(int argc, const tchar * argv[], void * param)
{
	assert(param != NULL);
	reinterpret_cast<GuiConsole*>(param)->Close();
	return 0;

}

END_HOEGAME



