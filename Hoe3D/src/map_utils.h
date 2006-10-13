
#ifndef _HOE_OBJECT_H_
#define _HOE_OBJECT_H_

#include <hoe_math.h>

class HoeMapPart;

struct MapPosition
{
	//HoeMapPart * part;
	HoeMath::Vector3 xyz;
};

struct MapOrientation : public MapPosition
{
	HoeMath::Quat rot;

	// pridat orientaci
	void GetMatrix(HoeMath::Matrix *m);
	void Reset();
};

#endif // _HOE_OBJECT_H_
