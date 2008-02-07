
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

#ifdef _WIN32_WCE
typedef IDirectDrawSurface DirectDrawSurface;
typedef IDirectDraw DirectDraw;
typedef DDSURFACEDESC DDSurfaceDesc;
#else
typedef IDirectDrawSurface7 DirectDrawSurface;
typedef IDirectDraw7 DirectDraw;
typedef DDSURFACEDESC2 DDSurfaceDesc;
#endif

class RefSurface : public RefSurfaceBase
{
public:
	DirectDrawSurface* m_srf; 
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
	DirectDraw*	m_pDD;        // DirectDraw object
	DirectDrawSurface* m_pDDSPrimary;// DirectDraw primary surface
	DirectDrawSurface* m_pDDSBack;   // DirectDraw back surface
	int m_width;
	int m_height;
	bool m_usebb;
	bool m_useflip;
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

	DirectDraw* GetDD() { return m_pDD; } 
	DirectDrawSurface* GetSurface() { return m_pDDSBack; }
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
