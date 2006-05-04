
/**
   @file    hoe_console.h
   @date    Nov 2004
   @version $Revision: 1.5 $
   @brief   Konzole pro logování.
*/
/*
	$Id: hoe_console.h,v 1.5 2005/04/05 00:48:39 gejza Exp $
	$Author: gejza $
	$Date: 2005/04/05 00:48:39 $
	$Revision: 1.5 $
*/ 

#ifndef _HOE_CONSOLE_H_
#define _HOE_CONSOLE_H_

#include "hoe_game.h"
#include "hoe_gui.h"

#define MAX_NUM_LINES 30

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

/**
 * @brief Konzole
 */

class Console : public BaseConsole
{
	int count;
	int ptr;
	char buff[MAX_NUM_LINES][512];
	FILE * log;
	HoeApp * app_callback;

	void AddLine(const char *);
public:
	Console();
	~Console();
	void SetFileLogging(const char * flog);
	virtual void HOEAPI Con_Print(const char *);
	int GetCount() { return (count > MAX_NUM_LINES) ? MAX_NUM_LINES:count; }
	char * GetLine(int n);
	void SetCallback(HoeApp * app) { app_callback = app; }
};

class GuiConsole : public TextDevice, public Console
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

	void Acquire();
	void Unacquire();
public:
	GuiConsole();
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

