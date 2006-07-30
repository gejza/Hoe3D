
/**
   @file    gridsurface.h
   @date    Mar 2006
   @version 1.0
   @brief   Deklarace trid pro grid surface.
*/

#ifndef _HOE_GRIDSURFACE_H_
#define _HOE_GRIDSURFACE_H_

#include "../include/hoeinterfaces.h"
#include "hoe3d_math.h"
#include "hoe_index.h"
#include "hoe_stream.h"
#include "hoe_material.h"
#include "filesystem.h"
#include "camera.h"
#include "structures.h"
#include "heightmap.h"

struct TGridTexture
{
	HoeTexture *tex; ///< textura
	size_t nx,ny; ///< velikost policek v texture
};

struct TGridData : public IHoeEnv::GridSurface::TGridDesc
{
	enum Type
	{
		ePlane = 0,
		eModel,
		eHM,
		TypeReqdword = 0x7fffffff
	} type;
	union {
		struct {
			int modelid;
			float base_height;
		};
		struct {
			int resx;
			int resy;
			float * map_heights; //< v dumpu se pak uklada uplne na konci
		};
		float plane_heights[4];
	};
};
/*
musi se vymyslet jak se budou prirazovat vzory
k polickum, v zasade mohou byt 3 typy
1) placka (texturove souradnice jako pohled zezhora)
   - vyska
1.5 skosena placka
   - 4x vyska
2) podle vzoru (texturove souradnice ve vzoru)
   museji se nejak definovat vzory (pravdepodobne jako textury, nebo to budou modely, spis modely)
   - vzor
   - vyska
   - orientace
3) mala vyskova mapa (texturove souradnice zezhora)
   nastavitelne rozliseni
*/

// nejdriv zjistit nakolik objekt lezi v kamere
// pokud cely, preda se do kresleni cely
// pokud jen cast, proparsuje se dovnitr
// pokud zadny tak zahodit
// chce to seznam na kresleni


struct TGridSurfaceTreeItem
{
	TGridSurfaceTreeItem *q1,*q2,*q3,*q4; ///< podpovrchy
	///< rozmery a stred
	
	dword start; ///< zacatek trojuhelniku pro vykresleni
	dword end; ///< konec trojuhelniku pro vykresleni
	void DeleteSub();
};

struct TGridSurfaceType
{
	HoeTexture *tex1, *tex2; ///< textura 
	TGridSurfaceTreeItem * root; /// zacatek kresleni stromu
	TGridSurfaceType * next; ///< ukazatel na dalsi povrch
	/** Nastavi na vykresleni */
	void Setup();
};

// multistream - pracovni nazev, pak se presune
/**
* Univerzalni objekt, ktery obsahuje indexy a streamy
* je delany pro vetsi mnozstvi predem neznamych dat
* umi rozdelovat indexy a vertexy rovnormerne do streamu
* a pak renderovat jen nektere, je optimalizovan na renderovani dat v poradi ve kterem
* se zapisuji do multistreamu
* v ramci optimalizace neni zaruceno, ze nektera data se nevyrenderuji navic
*/
class MultiStream
{
protected:
	static const uint MaxVerticesSize = 65000;
	static const uint MaxIndicesSize = 65000;
	struct Mesh
	{
		HoeStream vertices; ///< stream pro vertexy
		HoeIndex indices; ///< indexy
		dword numvert; ///< pocet vertexu
		dword numind; ///< pocet indexu
	} m_mesh[30];
	uint m_nummesh; ///< pocet vytvorenych meshu
	HoeFVF m_fvf; ///< FVF
	size_t m_sn; ///< velikost jednoho vertexu
	byte * m_lv; ///< Zamcene vertexy
	word * m_il; ///< zamcene indexy
	byte * m_vbuffer; ///< ukazatel na buffer vertexy
	word * m_ibuffer; ///< ukazatel na buffer indexu
	dword m_actvertex; ///< 
	dword m_actindex;
	word m_vertexoffset;
	/**
	* Vytvori z bufferu Mesh
	*/
	bool CreateMeshFromBuffer();
public:
	/** Konstruktor */
	MultiStream();
	/** Destruktor */
	virtual ~MultiStream();
	/**
	* Zacatek vytvareni streamu
	* @param fvf Textove fvf
	* @param sn Velikost jednoho vertexu v bytech
	*/
	bool Begin(const char * fvf, size_t sn);
	/**
	* Konec vytvareni streamu
	*/
	void End();
	/** 
	* Vytvoreni novych vertexu a indexu
	* @param numvert Pocet dalsich vertexu
	* @param numind Pocet dalsich indexu
	* @return Ukazatel na vertexy
	*/
	byte * LockNewVertices(dword numvert, dword numind);
	/**
	* Prida dalsi index do streamu, k indexu se automaticky pricte 
	* posledni index predchazejici skupiny indexu
	* @param ind Index
	*/
	void AddIndex(word ind);
	/**
	* Vraceni zakodovane pozice, obsahuje zakodovane cislo meshe a pozici
	*/
	dword GetPos() const;
	/**
	* Renderovani casti multistreamu
	* @param from zakodovana pozice odkud se ma renderovat
	* @param to zakodovana pozice mista kam az se ma renderovat
	*/
	void Render(dword from, dword to);
};



class GridSurface : public IHoeEnv::GridSurface
{
	static const int MaxTextureSlots = 32;

	HoeMath::MATRIX m_worldpos; ///< pozice stredu surface

	TGridData * m_grids; ///< ukazatel na pole gridu
	uint m_width, m_height; ///< velikost mrize
	float m_sizeX, m_sizeY; ///< realna velikost mrize

	TGridTexture m_textures[MaxTextureSlots]; ///< sloty na textury povrchu

	TGridSurfaceType * m_gst_first; ///< ruzne povrchy

	MultiStream m_multi; ///< vertexova a indexova data
	bool m_loaded; ///< jestli jsou nahrany data
	bool m_wire; /// vykreslit v drataku

	/** funkce ktera vytvari podstromy
	*/
	TGridSurfaceTreeItem * CreateQuadTree(dword * gr, uint ngr, uint minx, uint maxx, uint miny, uint maxy);
	bool PlaneToMulti(float vx, float vy, const HoeMath::MATRIX & matrix, const TGridData & grid);
	bool ModelToMulti(const HoeMath::MATRIX & matrix, const TGridData & grid);
public:
	/** Konstruktor */
	GridSurface();
	/** Destruktor */
	virtual ~GridSurface();
	/**
	* Vytvori index a vertex streamy z gridu
	*/
	/** @see IHoeEnv::GridSurface::Load */
	virtual void HOEAPI Load();
	virtual void HOEAPI ReleaseData();
	void Unload();
	void Render();
	virtual void HOEAPI SetPosCenter( float x, float y, float z);
	/** @see IHoeEnv::GridSurface::Create */
	virtual void HOEAPI Create(float sizeX, float sizeY, int resX,int resY);
	/** @see IHoeEnv::GridSurface::SetTexture */
	virtual void HOEAPI SetTexture(int slot, const char * texname, int width, int height);
	/** @see IHoeEnv::GridSurface::GetDesc */
	virtual void HOEAPI GetDesc(float *sizeX, float *sizeY, uint *resX,uint *resY);
	/** @see IHoeEnv::GridSurface::SetGridDesc */
	virtual void HOEAPI SetGridDesc(int x, int y, TGridDesc * desc);
	/** @see IHoeEnv::GridSurface::GetGriddesc */
	virtual void HOEAPI GetGridDesc(int x, int y, TGridDesc * desc);
	/** @see IHoeEnv::GridSurface::SetGridModel */
	virtual void HOEAPI SetGridModel(int x, int y, float height, int modelid);
	/** @see IHoeEnv::GridSurface::SetGridPlane */
	virtual void HOEAPI SetGridPlane(int x, int y, float height, float lt = 0.f, float rt = 0.f, float lb = 0.f, float rb = 0.f);
	/** @see IHoeEnv::GridSurface::SetGridHeightmap */
	virtual void HOEAPI SetGridHeightmap(int x, int y, float height, int resx, int resy, float * h);

	// hejbani
	virtual void HOEAPI MoveHeight(float x, float y, float moveheight, float radius);

	bool GetHeight(const float x, const float y, float * height);

	//virtual void HOEAPI ShowBrush(bool show);
	//virtual void HOEAPI SetBrush(float x, float y, float radius, dword color);
	/** @see IHoeEnv::GridSurface::ShowWireframe */
	virtual void HOEAPI ShowWireframe(bool show);
	/** @see IHoeEnv::GridSurface::Dump */
	virtual void HOEAPI Dump(XHoeStreamWrite * stream);
	/** @see IHoeEnv::GridSurface::LoadDump */
	virtual void HOEAPI LoadDump(XHoeStreamRead * stream);
};

#endif // _HOE_GRIDSURFACE_H_
