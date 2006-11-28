
#ifndef _HOE_MATH_EXT_H_
#define _HOE_MATH_EXT_H_

#ifdef __cplusplus

#include "hoe_math.h"
#include "hoe_structures.h"

namespace HoeMath {

template<class TYPE> struct PolygonT
{
	HoeCore::List<TYPE> points;
};

struct Polygon2
{
	HoeCore::List<Vector2> points;
	bool LeftPoint(const Vector2 & look, Vector2 & point);
	bool RightPoint(const Vector2 & look, Vector2 & point);
};

struct Polygon2Ex : public Polygon2
{
	HoeMath::Vector2 min, max;
	Polygon2Ex();
	void ComputeMinMax();
};

} // namespace HoeMath

#endif // __cplusplus

#endif // _HOE_MATH_EXT_H_
