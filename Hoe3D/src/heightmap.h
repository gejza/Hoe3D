
/**
   @file    heightmap.h
   @date    Mar 2006
   @version 1.0
   @brief   Deklarace trid pro zobrazovani heightmapy.
*/

#ifndef _HOE_HEIGHTMAP_H_
#define _HOE_HEIGHTMAP_H_

#include "../include/hoeinterfaces.h"
#include <hoe_math.h>
#include "hoe_index.h"
#include "hoe_stream.h"
#include "filesystem.h"
#include "camera.h"
#include <hoe_structures.h>

/**
* Trida vyskove mapy.
*/
class Heightmap
{
private:
	static const int ADD=1;
	static const int SUB=2;
	static const int MUL=3;
	static const int DIV=4;
	
	void combine(float* heights, int x1, int y1, int x2, int y2, int operation);

	int sizeX;
	int sizeY;
	float* heights; ///< Ukazatel na data vyskove mapy

public:
	Heightmap();
	~Heightmap();
	
	void add(float* heights, int x1, int y1, int x2, int y2);
	void sub(float* heights, int x1, int y1, int x2, int y2);
	void mul(float* heights, int x1, int y1, int x2, int y2);
	void div(float* heights, int x1, int y1, int x2, int y2);
	void generateMap();
	int loadHeights(int sizeX, int sizeY, float *);
	int getSizeX();
	int getSizeY();
    void setSize(int sizeX, int sizeY);
    void setSizeX(int sizeX);
    void setSizeY(int sizeY);
	void setHeightAt(int x, int y, float height);	
	float getHeightAt(int x, int y);
	HoeMath::Vector3 getNormalAt(int x, int y);
    void resize(int sizeX, int sizeY);

};

#endif
