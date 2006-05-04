
// jednotlive casti sceny
// nektere jsou mimo a sem vlozeny jako includy

#ifndef _SCENE_ENVIRONMENT_H_
#define _SCENE_ENVIRONMENT_H_

#include "../include/hoeinterfaces.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include "hoe_material.h"

#include "heightmap.h"

class HoePolygon
{
public:
	int id;
	HoeStream m_stream;
	int nump;
	HoeMaterial *mat;
	void Create(int n);
	void Render();
};

class HoePolygonSet : public IHoeEnv::Poly
{
	HoePolygon * poly;
public:
	HoePolygonSet(HoePolygon * p);
	virtual void HOEAPI SetPos(float * v, uint stride);
	virtual void HOEAPI SetTex(float * v, uint stride);
	virtual void HOEAPI SetMaterial(IHoeMaterial * m);
	virtual int HOEAPI GetID() { return poly->id; }
	virtual void HOEAPI Release() { delete this; };
};

#endif // _SCENE_ENVIRONMENT_H_


