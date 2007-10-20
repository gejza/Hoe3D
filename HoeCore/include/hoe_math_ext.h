
#ifndef _HOE_MATH_EXT_H_
#define _HOE_MATH_EXT_H_

#ifdef __cplusplus

#include "hoe_math.h"
#include "hoe_structures.h"

namespace HoeMath {

template<class TYPE> struct Polygon
{
	HoeCore::List<TYPE> points;
	bool LeftPoint(const Vector2f & look, Vector2f & point);
	bool RightPoint(const Vector2f & look, Vector2f & point);
};

template<class TYPE> struct PolygonEx : public Polygon<TYPE>
{
	TYPE min, max;
	PolygonEx();
	void ComputeMinMax();
};

} // namespace HoeMath

#endif // __cplusplus

#endif // _HOE_MATH_EXT_H_
