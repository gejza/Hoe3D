
/**
   @file    hoe_console.h
   @date    Nov 2004
   @version $Revision: 1.5 $
   @brief   Konzole pro logov�n�.
*/

#ifndef _HOE_CONSOLE_H_
#define _HOE_CONSOLE_H_

#include "hoe_game.h"
#include "hoe_gui.h"

namespace HoeGame {

class HoeApp;

class BaseConsole : public XHoeConsole
{
private:
	static BaseConsole * m_shared;
public:
	BaseConsole();
	~BaseConsole();
	static void Printf(const char *, ...);
	static void Printf(const wchar_t *, ...);
	static void Printfarg(const char *, va_list argptr);
	static void Printfarg(const wchar_t *, va_list argptr);
};

class TextItem
{
	tchar * m_text;
public:
	TextItem()
	{
		m_text = NULL;
	}
	TextItem(const tchar * text)
	{
		m_text = NULL; SetText(text);
	}
	virtual ~TextItem()
	{
		SetText(T(""));
	}

	const tchar * GetText() const { return m_text ? m_text : T(""); }
	void SetText(const tchar * text);
};

inline void TextItem::SetText(const tchar * text)
{
	if (m_text) { delete [] m_text; m_text = NULL; }
	if (text && text[0])
	{
		size_t n = HoeCore::string::len(text) + 1;
		m_text = new tchar[n];
		memcpy(m_text, text, n * sizeof(tchar));
	}
}

/**
 * @brief Konzole
 */

class Console: public BaseConsole
{
protected:
	HoeCore::CircleList<TextItem, 100> m_lines;
	FILE * log;
	HoeApp * app_callback;

public:
	Console();
	~Console();
	void SetFileLogging(const char * flog);
	virtual void HOEAPI Con_Print(const tchar *);
	const HoeCore::CircleList<TextItem, 100> & GetLines() { return m_lines; }
	void SetCallback(HoeApp * app) { app_callback = app; }
};

// chtelo by to novou konzoli, s historii a napovedou


class GuiConsole : public Gui::TextDevice
{
public:
	enum EState
	{
		sClose,
		sOpen,
		sGoClose,
		sGoOpen
	};
protected:
	IHoeFont * font; 
	IHoePicture * background;
	IHoeEngine * engine;
	tchar cmdline[512];
	EState state;
	float height;
	float maxheight;
	XHoeKeyboard * prev;
	unsigned long m_fontcolor;
	Console & m_console;
	HoeCore::CircleList<TextItem, 20> m_history;
	int m_histptr;

	void Acquire();
	void Unacquire();
public:
	GuiConsole(Console &);
	virtual bool Load(IHoeEngine *);
	void Draw(IHoe2D *);

	// cmd edit functions
	virtual void AddChar(char c);
	virtual void Back();
	virtual bool Key(int k);

	bool IsActive(void)
	{
		return (state != sClose);
	}
	void Open(void);
	void Close(void);
	void Update(float dtime);

	void RegisterCommands(IHoeEngine *);

	static int c_openconsole(int argc, const tchar * argv[], void * param); 
	static int c_closeconsole(int argc, const tchar * argv[], void * param);
};

} // namespace HoeGame

#endif // _HOE_CONSOLE_H_

