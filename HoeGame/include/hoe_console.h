
/**
   @file    hoe_console.h
   @date    Nov 2004
   @version $Revision: 1.5 $
   @brief   Konzole pro logování.
*/

#ifndef _HOE_CONSOLE_H_
#define _HOE_CONSOLE_H_

#include "hoe_game.h"
#include "hoe_gui.h"

BEGIN_HOEGAME

class HoeApp;

class BaseConsole : public XHoeConsole
{
private:
	static BaseConsole * m_shared;
public:
	BaseConsole();
	~BaseConsole();
	static void Printf(const char *, ...);
	static void Printfarg(const char *, va_list argptr);
};

class TextItem
{
	char * m_text;
public:
	TextItem()
	{
		m_text = NULL;
	}
	TextItem(const char * text)
	{
		m_text = NULL; SetText(text);
	}
	virtual ~TextItem()
	{
		SetText("");
	}

	const char * GetText() const { return m_text ? m_text : ""; }
	void SetText(const char * text);
};

inline void TextItem::SetText(const char * text)
{
	if (m_text) { delete [] m_text; m_text = NULL; }
	if (text && text[0])
	{
		size_t n = strlen(text) + 1;
		m_text = new char[n];
		memcpy(m_text, text, n);
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
	virtual void HOEAPI Con_Print(const char *);
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
	IHoe3DEngine * engine;
	char cmdline[512];
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
	virtual bool Load(IHoe3DEngine *);
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

	void RegisterCommands(IHoe3DEngine *);

	static int c_openconsole(int argc, const char * argv[], void * param); 
	static int c_closeconsole(int argc, const char * argv[], void * param);
};

END_HOEGAME

#endif // _HOE_CONSOLE_H_

