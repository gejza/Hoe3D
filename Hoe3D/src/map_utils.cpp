
#include "StdAfx.h"
#include "map_utils.h"

void MapOrientation::Reset()
{
	xyz.Set(0,0,0);
	rot.Set(1,0,0,0);
}

void MapOrientation::GetMatrix(HoeMath::Matrix4v *m)
{
	HoeMath::Matrix4v t,r;
	t.Translate(xyz);
	rot.GetMatrix(&r);
	m->Multiply(r,t);
}
