
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

class GridSurface : public IHoeEnv::GridSurface
{
	HoeHeightMap m_heights;
	HoeMath::MATRIX m_worldpos;
	HoeStream m_vertices;
	HoeIndex m_indices;
	HoeMaterial m_mat;
	bool m_loaded;
	float m_sizeX;
	float m_sizeY;
	bool m_wire;
	HoeTexture * tex1, *tex2;
public:
	GridSurface();
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
