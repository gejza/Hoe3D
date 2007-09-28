
/**
   @file    ref_d3d9.h
   @date    Mar 2006
   @version 1.0
   @brief   Hlavni soubor deklarace DIRECT3D9.
*/

#ifndef _HOE_D3DM_
#define _HOE_D3DM_
//#define _HOE_D3D_

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include <d3dm.h>

#include "ref_base.h"

#define FVF_XYZ			(1 << 0)
#define FVF_XYZRHW		(1 << 1) 
#define FVF_NORMAL		(1 << 2)
#define FVF_DIFFUSE		(1 << 3)
#define FVF_SPECULAR	(1 << 4)
#define FVF_TEX1		(1 << 5)
#define FVF_TEX2		(1 << 6)

#define RESERVE_PAR ,NULL
typedef void** D3DLOCKTYPE;

typedef IDirect3DMobileDevice * SysDevice;
typedef IDirect3DMobileTexture * SysTexture;
typedef IDirect3DMobileIndexBuffer * SysIndexBuffer;

typedef IDirect3DMobileVertexBuffer * SysVertexBuffer;

/*typedef D3DMATERIAL9 D3DMaterial;
typedef D3DVIEWPORT9 D3DViewport;*/
typedef D3DMLIGHT D3DLight;

struct THoeInitSettings;

inline const D3DMVECTOR & VECToD3D(const HoeMath::Vector3 &v)
{
	return *((D3DMVECTOR*)&v);
}

/**
* @brief Trida s implementaci zakladnich vlastnosti <b>D3D9</b>
*/
class RefD3DM : public RefBase
{
protected:
	HWND m_hWnd;  ///< Handle na okno
	uint m_Adapter; ///< Cislo adapteru
	IDirect3DMobile * m_pD3D; ///< Ukazatel na DIRECT3D 
	static IDirect3DMobileDevice * m_Dev; ///< Device
	D3DMFORMAT m_AdapterFormat; ///< Format backbufferu
	D3DMCAPS m_Caps; ///< caps
public:
	/**
	* Konstruktor
	*/
	RefD3DM();
	/**
	* Destruktor
	*/
	virtual ~RefD3DM() {}
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
	static HOE_INLINE IDirect3DMobileDevice * Device() 
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
	static HOE_INLINE void SetMatrix(const HoeMath::Matrix & m);

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
		return 0;
	}

	/**
	* Zruseni DIRECT3D
	*/
	void Destroy();
};

/**
* Globalni funkce pro ziskani DIRECT3DDevice
*/
HOE_INLINE IDirect3DMobileDevice * D3DDevice();

void ListState();

#define D3DVECTOR D3DVECTOR
#define D3DFORMAT D3DMFORMAT

//#include "ref_d3d9.inl"

#endif // _HOE_D3DM_