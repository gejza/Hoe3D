
#ifndef _HOE_GAME_WIN32_H_
#define _HOE_GAME_WIN32_H_

#include "hoe_game.h"
#include "hoe_baseapp.h"
#include "hoe_console.h"

BEGIN_HOEGAME 

class HoeWin32 : public HoeBaseApp
{
protected:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int  m_cyClient;
	int  m_cyChar;

	IHoe3DEngine * m_enginstance;
public:
	HoeWin32(HOE_INSTANCE);
	virtual ~HoeWin32();
	
	virtual bool RegisterApp();
	virtual bool CreateWin(const tchar * title, int width, int height, bool fs);

	HOE_INSTANCE GetInstance() { return m_hInstance; }
	virtual const tchar * GetAppName() = 0;

	virtual void SetTitle(const tchar * title);

	virtual void HandleError();

	virtual void UpdateConsole();
	virtual int GetMsg(IHoe3DEngine *);
	virtual void Destroy();

	virtual int ShowMsg(const tchar * caption, const tchar * msg);

	virtual void OnSize(int width, int height) = 0;
	virtual void OnPaint() = 0;

	virtual void PaintConsole(Console *);

	LRESULT CALLBACK MsgProc (HWND,UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
};

END_HOEGAME

#endif // _HOE_GAME_WIN32_H_

