
#include "StdAfx.h"
#include <hoe_math.h>
#include "map_utils.h"

void MapOrientation::Reset()
{
	xyz.Set(0,0,0);
	rot.Set(1,0,0,0);
}

void MapOrientation::GetMatrix(HoeMath::Matrix *m)
{
	HoeMath::Matrix t,r;
	t.Translate(xyz);
	rot.GetMatrix(&r);
	m->Multiply(r,t);
}
