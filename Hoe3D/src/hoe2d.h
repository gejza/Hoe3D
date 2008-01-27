
#ifndef _HOE2D_ENGINE_H_
#define _HOE2D_ENGINE_H_

#include "../include/hoe3d.h"

class Hoe2DEngine : public IHoeEngine
{
public:
	/** Ukazatel na funkci k ziskani casu */
	vfloat (*SysFloatTime)(); 

	/** Init funkce */
	virtual bool HOEAPI Init(THoeInitSettings *);

	/** Funkce co vytvari interface tridy */
	virtual IHoeInterface * HOEAPI Create(const tchar *);

	virtual IHoePicture * HOEAPI CreatePicture(int width, int height, dword * data);

	/** Funkce pro pristup k systemum hoe */
	virtual IHoeSystem * HOEAPI GetSystem(HOESYSTEMS sys);

	/** Slouzi ke spusteni prikazu */
	virtual int HOEAPI exec(const tchar *);

	/** Zaregistrovani noveho prikazu */
	virtual bool HOEAPI RegisterCmd(const tchar * cmd, HOE_CMDFUNC func, void * par);
	/** Zaregistrovani var */
	virtual bool HOEAPI RegisterVar(THoeVar * var);
	
	/** Vypocita kolize a input */
	virtual void HOEAPI Process(const double);
	/** Vykresli snimek */
	virtual bool HOEAPI Frame();
	/** Pro aktualizaci okna */
	virtual bool HOEAPI Resize(unsigned int width,unsigned int height);

	virtual IHoeScene * HOEAPI CreateScene(HOE_TYPE_SCENE type);

	virtual void HOEAPI SetActiveScene(IHoeScene * scene);
	virtual IHoeScene * HOEAPI GetActiveScene();

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


