/**
   @file    QuadSurface.h
   @date    jun 2006
   @version 1.0
   @brief   Implementace terenu pomoci quad tree, resp. urovni bodu.
			Povrch implementovaný pomocí Quad závislostí pro triangulaci.
*/

#ifndef _QUADSURFACE_H_
#define _QUADSURFACE_H_

#include "../include/hoeinterfaces.h"
#include "hoe3d_math.h"
#include "hoe_index.h"
#include "hoe_stream.h"
#include "filesystem.h"
#include "camera.h"
#include "structures.h"

class QuadSurface : Surface {

private:
	void enableVertex(int x, int y);
	void enableAll();
	void disableVertex(int x, int y);
	int  getVertexLevel(int x, int y);
	int  getMaxVertexLevel();
	bool isEnabledVertex(int x, int y);	
	
	bool checkVertex(int x, int y);
	bool checkVertex(int x, int y, int level, int sx, int sy);
	void checkVertices();

	void createQuadrantList(int x0,int y0,int x1,int y1,int x2,int y2,int level);

	float getScreenError(HoeCamera* cam, float dx, float dy, float y);
	float getLOD(float distance);

    float lod;
	bool* enabledVertex;	
	int size;
	int sizeLevel;
	
public:
	QuadSurface();
	~QuadSurface();
	
	virtual void setLOD(float lod);
	
	//virtual void Create(float width, float height);
	void render(HoeCamera *cam);
};

/*
class HeightMapSurfaceSet : public IHoeEnv::HeightMapSurface
{
	HoeQuadTerrain * m_ter;
public:
	HeightMapSurfaceSet(HoeQuadTerrain * ter);
	virtual int HOEAPI GetID() { return 0; }
	virtual void HOEAPI Release() { delete this; };
	virtual void HOEAPI LoadHeight(float _distX,float _distY,int _sizeLevel, float *f);
};
*/

#endif