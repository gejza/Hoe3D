
/**
   @file    heightmap.h
   @date    Mar 2006
   @version 1.0
   @brief   Deklarace trid pro zobrazovani heightmapy.
*/

#ifndef _HOE_HEIGHTMAP_H_
#define _HOE_HEIGHTMAP_H_

#include "../include/hoeinterfaces.h"
#include "hoe3d_math.h"
#include "hoe_index.h"
#include "hoe_stream.h"
#include "hoe_material.h"
#include "filesystem.h"
#include "camera.h"
#include "structures.h"

/**
* Trida spravujici vyskovou mapu
*/
class HoeHeightMap
{
private:
	int sizeX; ///< Velikost na sirku
	int sizeY; ///< Velikost na vysku
	float* heights; ///< Ukazatel na data vyskove mapy

public:
	HoeHeightMap();
	~HoeHeightMap();
	
	void generateMap(int SizeX, int SizeY);	//	docasne, test
	int load(int sizeX, int sizeY, float *);
	int getSizeX();
	int getSizeY();
	void setHeightAt(int x, int y, float height);	
	float getHeightAt(int x, int y);
	HoeMath::VECTOR3 getNormalAt(int x, int y);

};

/**
* Trida vlastniho terenu
*/
class HoeQuadTerrain
{
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

	void createVertexList();
	void createIndexList();
	void createQuadrantList(int x0,int y0,int x1,int y1,int x2,int y2,int level);

	float getHeightAt(int x, int y);
	float getScreenError(float dx, float dy, float y);
	float getLOD(float distance);
	
	//FILE* fw;		//	delete - testing	

	HoeMath::VECTOR3 m_LastCamPos;

	bool* enabledVertex;
	HoeStream vertexStream;
	HoeIndex indexStream;
	HoeStream normalStream;
	HoeMaterial material;

	HoeHeightMap heightMap;

	float distX;
	float distY;
	long indexCounter;
	int size;
	int sizeLevel;
	bool loaded;
	
public:
	HoeQuadTerrain();
	~HoeQuadTerrain();

	HoeMath::VECTOR3 getNormalAt(int x, int y);
	float getDistX();
	float getDistY();
	int getSize();
	
	void setDistX(float DistX);
	void setDistY(float DistY);
	void setSizeLevel(int SizeLevel);

	virtual void setLOD(float _lod);
	
	virtual void increaseLOD(float increasement);
	virtual void decreaseLOD(float decreasement);
	virtual void loadHeight(float _distX,float _distY,int _sizeLevel, float *);
	//virtual void Create(float width, float height);
	void render(HoeCamera *cam);
	
	float lod;
};

class HeightMapSurfaceSet : public IHoeEnv::HeightMapSurface
{
	HoeQuadTerrain * m_ter;
public:
	HeightMapSurfaceSet(HoeQuadTerrain * ter);
	virtual int HOEAPI GetID() { return 0; }
	virtual void HOEAPI Release() { delete this; };
	virtual void HOEAPI LoadHeight(float _distX,float _distY,int _sizeLevel, float *f);
};

#endif // _HOE_HEIGHTMAP_H_




