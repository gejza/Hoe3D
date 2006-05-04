
/**
   @file    ref_d3d8.h
   @date    Mar 2006
   @version 1.0
   @brief   Hlavni soubor definice Direct3D8.
*/

#ifndef _HOE_D3D8_
#define _HOE_D3D8_
#define _HOE_D3D_

#include <d3d8.h>
#include "ref_base.h"

#define SetFVF SetVertexShader

#define RESERVE_PAR
typedef BYTE** D3DLOCKTYPE;

typedef IDirect3DDevice8 * SysDevice;
typedef IDirect3DTexture8 * SysTexture;
typedef IDirect3DIndexBuffer8 * SysIndexBuffer;
typedef IDirect3DVertexBuffer8 * SysVertexBuffer;

typedef D3DMATERIAL8 D3DMaterial;
typedef D3DVIEWPORT8 D3DViewport;
typedef D3DLIGHT8 D3DLight;

struct THoeInitSettings;

/**
* @brief Trida s implementaci zakladnich vlastnosti <b>D3D8</b>
*/
class RefD3D8 : public RefBase
{
protected:
	HWND m_hWnd;  ///< Handle na okno
	uint m_Adapter; ///< Cislo adapteru
	IDirect3D8 * m_pD3D; ///< Ukazatel na Direct3D
	static IDirect3DDevice8 * m_Dev; ///< Device

public:
	/**
	* Konstruktor
	*/
	RefD3D8();
	/**
	* Destruktor
	*/
	virtual ~RefD3D8() {}
	/**
	* Inicializace DirectD3
	* @param his vyplnena struktura THoeInitSettings
	* @return Pokud se zdari inicializace, vraci true, jinak false.
	*/
	bool Init(THoeInitSettings * his);
	/**
	* Vraci pointer na device viz Direct3D help.
	* @return Pointer na device.
	*/
	static HOE_INLINE IDirect3DDevice8 * Device() 
	{ 
		assert(m_Dev);
		return m_Dev; 
	}
	/**
	* Zacatek sceny. viz Direct3D help.
	*/
	bool Begin();
	/**
	* Konec vykreslovani sceny. viz Direct3D help.
	*/
	void End();
	/**
	* Vykresleni objektu. Pocet trojuhelniku se urcuje podle velikosti bufferu.
	* @param stream Vrcholy
	* @param index Indexy
	*/
	static void DrawStdObject(class HoeStream * stream, class HoeIndex * index);
	/**
	* Vykresleni objektu s nastavenim velikosti bufferu.
	* @param stream Vrcholy
	* @param index Indexy
	* @param vert Pocet platnych vrcholu
	* @param ind Pocet indexu = pocet trojuhelniku * 3
	*/
	static void DrawStdObject(HoeStream * stream, HoeIndex * index, dword vert, dword ind);
	/**
	* Vykresleni car podle vrcholu.
	* @param stream Vrcholy
	* @param lineCount Pocet car
	*/
	static void DrawLineObject(class HoeStream * stream, int lineCount);
	/**
	* Vykresleni objektu tvoreneho body.
	* @param stream Body
	* @param vertCount Pocet bodu
	*/
	static void DrawPointObject(class HoeStream * stream, int vertCount);
	/** 
	* Nastaveni aktualni modelove matice.
	* @param m Matice
	*/
	static HOE_INLINE void SetMatrix(const HoeMath::MATRIX & m);
	/**
	* Zruseni Direct3D
	*/
	void Destroy();
};

/**
* Globalni funkce pro ziskani Direct3DDevice
*/
HOE_INLINE IDirect3DDevice8 * D3DDevice();

#include "ref_d3d8.inl"

#endif // _HOE_D3D8_