
#ifndef _HOE_OBJECT_H_
#define _HOE_OBJECT_H_

#include <hoe_math.h>

class HoeMapPart;

struct MapPosition
{
	//HoeMapPart * part;
	HoeMath::Vector3v xyz;
};

struct MapOrientation : public MapPosition
{
	HoeMath::Quat rot;

	// pridat orientaci
	void GetMatrix(HoeMath::Matrix4v *m);
	void Reset();
};

#endif // _HOE_OBJECT_H_
