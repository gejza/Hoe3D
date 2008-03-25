

#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "../include/hoeinterfaces.h"
#include "object_inspector.h"
#include "physics.h"

Physics::Physics()
{
	SET_SHARED_PTR(physics);
	m_oi = NULL;
	Con_Print(T("Physics engine created v0.1!"));
}

Physics::~Physics()
{
	UNSET_SHARED_PTR(physics);
}


void Physics::SetPart(unsigned long partID)
{

}

XHoeObject * Physics::RayTrace(const vfloat * orig,const vfloat * dir,unsigned long flags)
{
	vfloat t = 0.f;
	
	XHoeObject * obj = NULL;
	XHoeObject * ret = NULL;
	
	/*const float dotAu = HoeMath::HoeDot(*VEC3(orig),*VEC3(dir));
	const float dotUU = HoeMath::HoeDot(*VEC3(dir),*VEC3(dir));

	m_oi->SetStreaming();
	while ((obj = m_oi->Object()) != NULL)
	{
		float max;
		float * pos = obj->GetPos(max);
		float tt = (HoeMath::HoeDot(*VEC3(pos),*VEC3(dir)) - dotAu) / dotUU;
		if (tt < 0)
			continue;

		if (ret && tt > t)
			continue;

		// compute leng
		HoeMath::Vector3 p = (*VEC3(orig)) + ((*VEC3(dir)) * tt);
		if (max < HoeMath::HoeMagnitude(p - (*VEC3(pos))))
			continue;

		ret = obj;
		t = tt;
	}*/

	return ret;
}





