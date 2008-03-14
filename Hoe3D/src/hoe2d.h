
#ifndef _HOE2D_ENGINE_H_
#define _HOE2D_ENGINE_H_

#include "hoe.h"

class Hoe2DEngine : public HoeEngine
{
	/** Funkce co vytvari interface tridy */
	IHoeInterface * CreatePic(const tchar *);
	IHoeInterface * CreateFont(const tchar *);
public:
	Hoe2DEngine(int flags) : HoeEngine(flags) {}
	/** Init funkce */
	virtual bool HOEAPI Init(THoeInitSettings *);

	/** Funkce co vytvari interface tridy */
	virtual IHoeInterface * HOEAPI Create(const tchar *);

	/** Funkce pro pristup k systemum hoe */
	virtual IHoeSystem * HOEAPI GetSystem(HOESYSTEMS sys);

	/** Vypocita kolize a input */
	virtual void HOEAPI Process(const double);
	/** Vykresli snimek */
	virtual bool HOEAPI Frame();
	/** Pro aktualizaci okna */
	virtual bool HOEAPI Resize(unsigned int width,unsigned int height);

	virtual IHoeScene * HOEAPI CreateScene(HOE_TYPE_SCENE type);

	/** Zrusi engine */
	virtual void HOEAPI Destroy(); // vymazani

	#ifdef _WIN32
	/** Metoda pro vstup pøes okenní zprávy. */
	virtual LRESULT CALLBACK HOEAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	#endif // _WIN32
	#ifdef _LINUX
	/** Matoda pro zpravy Xwindows */
	virtual bool HOEAPI XProc(XEvent * event);	
	#endif // _LINUX
};

#endif // _HOE2D_ENGINE_H_


