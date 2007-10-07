
/**
   @file    ref_d3d9.h
   @date    Mar 2006
   @version 1.0
   @brief   Hlavni soubor deklarace DIRECT3D9.
*/

#ifndef _HOE_D3DM_
#define _HOE_D3DM_
#define _HOE_D3DA_

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

#define RESERVE_PAR
typedef void** D3DLOCKTYPE;

typedef IDirect3DMobileDevice * SysDevice;
typedef IDirect3DMobileTexture * SysTexture;
typedef IDirect3DMobileIndexBuffer * SysIndexBuffer;

typedef IDirect3DMobileVertexBuffer * SysVertexBuffer;

typedef D3DMMATERIAL D3DMaterial;
/*typedef D3DVIEWPORT9 D3DViewport;*/
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

	// utils
	static const char * GetErrorString(HRESULT hRes);
};

/**
* Globalni funkce pro ziskani DIRECT3DDevice
*/
HOE_INLINE IDirect3DMobileDevice * D3DDevice();

void ListState();

#define D3DVECTOR D3DVECTOR
#define D3DFORMAT D3DMFORMAT

#define D3DUSAGE_WRITEONLY		D3DMUSAGE_WRITEONLY
#define D3DUSAGE_DYNAMIC		D3DMUSAGE_DYNAMIC
#define D3DFMT_INDEX16			D3DMFMT_INDEX16
#define D3DPOOL_DEFAULT			D3DMPOOL_MANAGED

#define D3DRS_FILLMODE                  D3DMRS_FILLMODE                  
#define D3DRS_SHADEMODE                 D3DMRS_SHADEMODE                 
#define D3DRS_ZWRITEENABLE              D3DMRS_ZWRITEENABLE              
#define D3DRS_ALPHATESTENABLE           D3DMRS_ALPHATESTENABLE           
#define D3DRS_LASTPIXEL                 D3DMRS_LASTPIXEL                 
#define D3DRS_SRCBLEND                  D3DMRS_SRCBLEND                  
#define D3DRS_DESTBLEND                 D3DMRS_DESTBLEND                 
#define D3DRS_CULLMODE                  D3DMRS_CULLMODE                  
#define D3DRS_ZFUNC                     D3DMRS_ZFUNC                     
#define D3DRS_ALPHAREF                  D3DMRS_ALPHAREF                  
#define D3DRS_ALPHAFUNC                 D3DMRS_ALPHAFUNC                 
#define D3DRS_DITHERENABLE              D3DMRS_DITHERENABLE              
#define D3DRS_ALPHABLENDENABLE          D3DMRS_ALPHABLENDENABLE          
#define D3DRS_FOGENABLE                 D3DMRS_FOGENABLE                 
#define D3DRS_SPECULARENABLE            D3DMRS_SPECULARENABLE            
#define D3DRS_FOGCOLOR                  D3DMRS_FOGCOLOR                  
#define D3DRS_FOGTABLEMODE              D3DMRS_FOGTABLEMODE              
#define D3DRS_FOGSTART                  D3DMRS_FOGSTART                  
#define D3DRS_FOGEND                    D3DMRS_FOGEND                    
#define D3DRS_FOGDENSITY                D3DMRS_FOGDENSITY                
#define D3DRS_DEPTHBIAS                 D3DMRS_DEPTHBIAS                 
#define D3DRS_RANGEFOGENABLE            D3DMRS_RANGEFOGENABLE            
#define D3DRS_STENCILENABLE             D3DMRS_STENCILENABLE             
#define D3DRS_STENCILFAIL               D3DMRS_STENCILFAIL               
#define D3DRS_STENCILZFAIL              D3DMRS_STENCILZFAIL              
#define D3DRS_STENCILPASS               D3DMRS_STENCILPASS               
#define D3DRS_STENCILFUNC               D3DMRS_STENCILFUNC               
#define D3DRS_STENCILREF                D3DMRS_STENCILREF                
#define D3DRS_STENCILMASK               D3DMRS_STENCILMASK               
#define D3DRS_STENCILWRITEMASK          D3DMRS_STENCILWRITEMASK          
#define D3DRS_TEXTUREFACTOR             D3DMRS_TEXTUREFACTOR           #define D3DMRS_TEXTUREPERSPECTIVE        D3DRS_TEXTUREPERSPECTIVE        
#define D3DRS_WRAP0                     D3DMRS_WRAP0                     
#define D3DRS_WRAP1                     D3DMRS_WRAP1                     
#define D3DRS_WRAP2                     D3DMRS_WRAP2                     
#define D3DRS_WRAP3                     D3DMRS_WRAP3                     
#define D3DRS_CLIPPING                  D3DMRS_CLIPPING                  
#define D3DRS_LIGHTING                  D3DMRS_LIGHTING                  
#define D3DRS_AMBIENT                   D3DMRS_AMBIENT                   
#define D3DRS_FOGVERTEXMODE             D3DMRS_FOGVERTEXMODE             
#define D3DRS_COLORVERTEX               D3DMRS_COLORVERTEX               
#define D3DRS_LOCALVIEWER               D3DMRS_LOCALVIEWER               
#define D3DRS_NORMALIZENORMALS          D3DMRS_NORMALIZENORMALS          
#define D3DRS_DIFFUSEMATERIALSOURCE     D3DMRS_DIFFUSEMATERIALSOURCE     
#define D3DRS_SPECULARMATERIALSOURCE    D3DMRS_SPECULARMATERIALSOURCE    
#define D3DRS_AMBIENTMATERIALSOURCE     D3DMRS_AMBIENTMATERIALSOURCE     
#define D3DRS_COLORWRITEENABLE          D3DMRS_COLORWRITEENABLE          
#define D3DRS_BLENDOP                   D3DMRS_BLENDOP                   
#define D3DRS_ZENABLE                   D3DMRS_ZENABLE                   
#define D3DRS_SLOPESCALEDEPTHBIAS       D3DMRS_SLOPESCALEDEPTHBIAS       

#define D3DFILL_POINT					D3DMFILL_POINT
#define D3DFILL_WIREFRAME				D3DMFILL_WIREFRAME
#define D3DFILL_SOLID					D3DMFILL_SOLID


#include "ref_d3dmobile.inl"

#endif // _HOE_D3DM_