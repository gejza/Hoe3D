
#ifndef _HOE_OBJECT_H_
#define _HOE_OBJECT_H_

#include "hoe3d_math.h"

class HoeMapPart;

struct MapPosition
{
	//HoeMapPart * part;
	HoeMath::VECTOR3 xyz;
};

struct MapOrientation : public MapPosition
{
	HoeMath::QUAT rot;

	// pridat orientaci
	void GetMatrix(HoeMath::MATRIX *m);
	void Reset();
};

#endif // _HOE_OBJECT_H_
