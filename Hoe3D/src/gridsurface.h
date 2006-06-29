
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

struct TGrid
{
	byte tex1; ///< index textury
	byte x1; ///< x-ove policko v texture
	byte y1; ///< y-ove policko v texture
	byte ori1; ///< orientace policka 0-3
	byte tex2; ///< index textury
	byte x2; ///< x-ove policko v texture
	byte y2; ///< y-ove policko v texture
	byte ori2; ///< orientace policka 0-3
};

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
	HoeStream m_vertices; ///< stream pro vertexy
	HoeIndex m_indices; ///< indexy
	size_t m_sn;
	byte * m_lv;
	word * m_il;
	word m_vert;
	dword m_pos;
public:
	byte * GetLockedVertices() { return m_lv; }
	void AddIndex(word ind)
	{
		*m_il = ind + m_vert;
		m_il++;
		m_pos++;
	}
	void SkipVertices(uint nv)
	{
		m_lv += nv * m_sn;
		m_vert += nv;
	}

	dword GetPos()
	{
		return m_pos;
	}
	bool Begin(uint vnum, uint inum, const char * fvf, size_t sn)
	{
		if (!m_vertices.Create(vnum, fvf, vnum*sn))
			return false;
		if (!m_indices.Create(inum))
			return false;
		m_sn = sn;
		m_lv = m_vertices.Lock();
		m_il = m_indices.Lock();

		m_pos = 0;
		m_vert = 0;
		return true;
	}
	void End()
	{
		// optimize
		m_vertices.Unlock();
		m_indices.Unlock();

	}
	void Render()
	{
		Ref::DrawStdObject(&m_vertices, &m_indices);
	}
};



class GridSurface : public IHoeEnv::GridSurface
{
	static const int MaxTextureSlots = 32;

	HoeMath::MATRIX m_worldpos; ///< pozice stredu surface

	TGrid * m_grids; ///< ukazatel na pole gridu
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
public:
	GridSurface();
	/**
	* Vytvori index a vertex streamy z gridu
	*/
	void Load();
	void Render();
	virtual void HOEAPI SetPosCenter( float x, float y, float z);
	/** @see IHoeEnv::GridSurface::Create */
	virtual void HOEAPI Create(float sizeX, float sizeY, int resX,int resY);
	/** @see IHoeEnv::GridSurface::SetTexture */
	virtual void HOEAPI SetTexture(int slot, const char * texname, int width, int height);
	virtual void HOEAPI ShowBrush(bool show);
	virtual void HOEAPI SetBrush(float x, float y, float radius, dword color);
	virtual void HOEAPI MoveHeight(float x, float y, float radius, float value);
	virtual void HOEAPI ShowWireframe(bool show);
	/** @see IHoeEnv::GridSurface::Dump */
	virtual void HOEAPI Dump();
	/** @see IHoeEnv::GridSurface::LoadDump */
	virtual void HOEAPI LoadDump();
};

#endif // _HOE_GRIDSURFACE_H_
