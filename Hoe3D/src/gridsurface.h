
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

class GridTexture
{
public:
	HoeTexture * tex; ///< textura 
	size_t nx,ny; ///< velikost policek v texture
};

class GridSurface : public IHoeEnv::GridSurface
{
	HoeMath::MATRIX m_worldpos; ///< pozice stredu surface
	TGrid * m_grids; ///< ukazatel na pole gridu
	uint m_width, m_height; ///< velikost mrize
	float m_sizeX, m_sizeY; ///< realna velikost mrize
	GridTexture m_textures[32]; ///< povrchy
	size_t m_ntex; ///< pocet povrchu

	HoeStream m_vertices; ///< stream pro vertexy
	HoeIndex m_indices; ///< indexy
	bool m_loaded; ///< jestli jsou nahrany data
	bool m_wire; /// vykreslit v drataku

	HoeTexture * tex1,*tex2;
public:
	GridSurface();
	/**
	* Vytvori index a vertex streamy z gridu
	*/
	void Load();
	void Render();
	virtual void HOEAPI SetPosCenter( float x, float y, float z);
	virtual void HOEAPI SetSize(float sizeX, float sizeY);
	virtual void HOEAPI GenerateHeight(int resX,int resY);
	virtual void HOEAPI SetHeightMap(int resX,int resY, float *f);
	virtual void HOEAPI ShowBrush(bool show);
	virtual void HOEAPI SetBrush(float x, float y, float radius, dword color);
	virtual void HOEAPI MoveHeight(float x, float y, float radius, float value);
	virtual void HOEAPI ShowWireframe(bool show);
};

#endif // _HOE_GRIDSURFACE_H_
