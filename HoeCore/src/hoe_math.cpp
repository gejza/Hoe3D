
#include "StdAfx.h"
#include "../include/hoe_math_ext.h"

namespace HoeMath {

void MinMaxF(const float * src, uint count, uint stride, float * min, float * max)
{
	hoe_assert(count >= 1);
	register uint cnt = count-1;
	register float mn,mx;
	mx = mn = *src;
	ConstPointerAdd<const float*>(src, stride);
	while (cnt-- > 0)
	{
		if (*src > mx) mx = *src;
		if (*src < mn) mn = *src;
		ConstPointerAdd<const float*>(src, stride);
	}
	if (min) *min = mn;
	if (max) *max = mx;
}

void MinMaxV2(const Vector2f * src, uint count, uint stride, Vector2f * min, Vector2f * max)
{
	Vector2f x,n;
	MinMaxF(&(src->x), count, stride, &(n.x), &(x.x));
	MinMaxF(&(src->y), count, stride, &(n.y), &(x.y));
	if (min) *min = n;
	if (max) *max = x;
}

void MinMaxV3(const Vector3f * src, uint count, uint stride, Vector3f * min, Vector3f * max)
{
	Vector3f x,n;
	MinMaxF(&(src->x), count, stride, &(n.x), &(x.x));
	MinMaxF(&(src->y), count, stride, &(n.y), &(x.y));
	MinMaxF(&(src->z), count, stride, &(n.z), &(x.z));
	if (min) *min = n;
	if (max) *max = x;
}

template<> bool Polygon<Vector2v>::LeftPoint(const Vector2v & look, Vector2v & point)
{
	// najit nejlevejsi bod
	bool ret = false;
	Line2v line;
	line.SetXY(look, point);
	for (uint i=0;i < points.Count();i++)
	{
		if (line.Compute(points[i].x, points[i].y) <= 0.f)
			continue;
		//bod je pred, nastavit novou lajnu
		point = points[i];
		line.SetXY(look, point);
		ret = true;
	}
	return ret;
}

template<>
bool Polygon<Vector2v>::RightPoint(const Vector2v & look, Vector2v & point)
{
	// najit nejlevejsi bod
	bool ret = false;
	Line2v line;
	line.SetXY(look, point);
	for (uint i=0;i < points.Count();i++)
	{
		if (line.Compute(points[i].x, points[i].y) >= 0.f)
			continue;
		//bod je pred, nastavit novou lajnu
		point = points[i];
		line.SetXY(look, point);
		ret = true;
	}
	return ret;
}

template<>
PolygonEx<Vector2f>::PolygonEx()
{
	min.Set(0,0);
	max = min;
}

template<>
void PolygonEx<Vector2f>::ComputeMinMax()
{
	if (!points.IsEmpty())
	{
		HoeMath::MinMaxV2(&(points[0]), 
			points.Count(), sizeof(HoeMath::Vector2f), &min, &max);
	}
}

} // namespace HoeMath



