
/**
   @file    ref_d3d9.h
   @date    Mar 2006
   @version 1.0
   @brief   Hlavni soubor deklarace DIRECT3D9.
*/

#ifndef _HOE_D3D9_
#define _HOE_D3D9_
#define _HOE_D3D_
#define _HOE_D3DA_

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>

#include "ref_base.h"

#define FVF_XYZ D3DFVF_XYZ
#define FVF_XYZRHW D3DFVF_XYZRHW
#define FVF_NORMAL D3DFVF_NORMAL
#define FVF_DIFFUSE D3DFVF_DIFFUSE
#define FVF_SPECULAR D3DFVF_SPECULAR
#define FVF_TEX1 D3DFVF_TEX1
#define FVF_TEX2 D3DFVF_TEX2


#define RESERVE_PAR ,NULL
typedef void** D3DLOCKTYPE;

typedef IDirect3DDevice9 * SysDevice;
typedef LPDIRECT3DTEXTURE9 SysTexture;
typedef IDirect3DStateBlock9 * SysStateBlock;
typedef IDirect3DIndexBuffer9 * SysIndexBuffer;

typedef IDirect3DVertexBuffer9 * SysVertexBuffer;

typedef D3DMATERIAL9 D3DMaterial;
typedef D3DVIEWPORT9 D3DViewport;
typedef D3DLIGHT9 D3DLight;

struct THoeInitSettings;

inline const D3DVECTOR & VECToD3D(const HoeMath::Vector3v &v)
{
	return *((D3DVECTOR*)&v);
}

/**
* @brief Trida s implementaci zakladnich vlastnosti <b>D3D9</b>
*/
class RefD3D9 : public RefBase
{
protected:
	HWND m_hWnd;  ///< Handle na okno
	uint m_Adapter; ///< Cislo adapteru
	IDirect3D9 * m_pD3D; ///< Ukazatel na DIRECT3D 
	static IDirect3DDevice9 * m_Dev; ///< Device
	D3DFORMAT m_AdapterFormat; ///< Format backbufferu
	D3DCAPS9 m_Caps; ///< caps
public:
	/**
	* Konstruktor
	*/
	RefD3D9();
	/**
	* Destruktor
	*/
	virtual ~RefD3D9() {}
	/**
	* Inicializace DIRECTD3
	* @param his vyplnena struktura THoeInitSettings
	* @return Pokud se zdari inicializace, vraci true, jinak false.
	*/
	bool Init(THoeInitSettings * his);
	/**
	* Vraci pointer na device viz DIRECT3D help.
	* @return Pointer na device.
	*/
	static HOE_INLINE IDirect3DDevice9 * Device() 
	{ 
		assert(m_Dev);
		return m_Dev; 
	}
	/**
	* Zacatek sceny. viz DIRECT3D help.
	*/
	bool Begin();
	/**
	* Konec vykreslovani sceny. viz DIRECT3D help.
	*/
	void End();
	/**
	* Vymazani bufferu
	* @param target Smaze target buffer
	* @param depth Smaze depth buffer
	*/
	void ClearBuffers(bool target, bool depth);
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
	* Vykresleni objektu s urcenim intervalu.
	* @param stream Vrcholy
	* @param index Indexy
	* @param vert Pocet platnych vrcholu
	* @param ind Pocet indexu = pocet trojuhelniku * 3
	*/
	static void DrawStdObjectFT(HoeStream * stream, HoeIndex * index, dword from, dword num);
	/**
	* Vykresleni indexu s prednastavenym streamem.
	* @param index Indexy
	* @param offset Od ktereho index uzacit
	* @param ind Pocet indexu = pocet trojuhelniku * 3
	*/
	static void DrawIndex(HoeIndex * index, dword offset, dword count);
	/**
	* Vykresleni fanu.
	* @param offset Od ktereho index uzacit
	* @param count Pocet indexu = pocet trojuhelniku + 2
	*/
	static void DrawFanObject(dword offset, dword count);
	/**
	* Vykresleni car podle vrcholu.
	* @param stream Vrcholy
	* @param lineCount Pocet car
	*/
	static void DrawLineObject(class HoeStream * stream, int lineCount);
	/**
	* Vykresleni bodu.
	* @param stream Body
	* @param vertCount Pocet bodu
	*/
	static void DrawPointObject(class HoeStream * stream, int vertCount);
	/** 
	* Nastaveni modelove matice.
	* @param m Matice
	*/
	static HOE_INLINE void SetMatrix(const HoeMath::Matrix4v & m);

	/**
	* Zjisti zda zarizeni podporuje format textury
	* @param TextureFormat format textury
	*/
	bool IsTextureFormatOk( enum HOEFORMAT TextureFormat);
	/**
	* Podporovana verze vertex shaderu
	* @return Verze shaderu jako hiword a loword
	*/
	word HOE_INLINE GetVertexShaderVersion()
	{
		return (word)(0xffff&m_Caps.VertexShaderVersion);
	}

	/**
	* Zruseni DIRECT3D
	*/
	void Destroy();
};

/**
* Globalni funkce pro ziskani DIRECT3DDevice
*/
HOE_INLINE IDirect3DDevice9 * D3DDevice();

void ListState();

#include "ref_d3d9.inl"

#endif // _HOE_D3D9_