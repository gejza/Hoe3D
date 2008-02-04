
/**
   @file    ref_dd.h
   @date    Mar 2008
   @version 1.0
   @brief   Hlavni soubor definice DIRECTDRAW.
*/

#ifndef _HOE_DD_
#define _HOE_DD_

#ifndef HOE2D
#error "DirectDraw can be used only with 2D engine."
#endif

#include <ddraw.h>
#include "ref_base.h"

struct THoeInitSettings;

class RefSurface : public RefSurfaceBase
{
public:
	LPDIRECTDRAWSURFACE7 m_srf; 
	bool Lock(LockRect* l);
	void Unlock();

	friend class RefDD;
};

/**
* @brief Trida s implementaci zakladnich vlastnosti <b>D3D8</b>
*/
class RefDD : public RefBase
{
protected:
	HWND m_hWnd;  ///< Handle na okno
	LPDIRECTDRAW7	m_pDD;        // DirectDraw object
	LPDIRECTDRAWSURFACE7 m_pDDSPrimary;// DirectDraw primary surface
	LPDIRECTDRAWSURFACE7 m_pDDSBack;   // DirectDraw back surface
	int m_width;
	int m_height;
public:
	/**
	* Konstruktor
	*/
	RefDD();
	/**
	* Destruktor
	*/
	virtual ~RefDD() {}
	/**
	* Inicializace DirectDraw
	* @param his vyplnena struktura THoeInitSettings
	* @return Pokud se zdari inicializace, vraci true, jinak false.
	*/
	bool Init(THoeInitSettings * his);
	/**
	* Zacatek sceny. viz DIRECT3D help.
	*/
	bool Begin();
	/**
	* Konec vykreslovani sceny. viz DIRECT3D help.
	*/
	void End();

	LPDIRECTDRAW7 GetDD() { return m_pDD; } 
	LPDIRECTDRAWSURFACE7 GetSurface() { return m_pDDSBack; }
	/**
	* Vymazani bufferu
	* @param target Smaze target buffer
	* @param depth Smaze depth buffer
	*/
	void ClearBuffers(bool target, bool depth);

	/*
	* Color fill, vyplneni obdelniku
	*/
	void ColorFill(const vfloat l,const vfloat r,
		const vfloat t,const vfloat b,unsigned long color,bool full);

	bool CreateSurface(RefSurface* surf, uint width, uint height);
	void Blt(RefSurface& surf, const THoeRect * dest, const THoeRect * src, int method);
	/**
	* Zruseni DIRECT3D
	*/
	void Destroy();

	// utils
	static HOEFORMAT GetFormat(DDPIXELFORMAT& pf);
	static const tchar * GetErrorString(HRESULT hRes);

};



#endif // _HOE_D3D8_
