
#include "StdAfx.h"
#include "utils.h"
#include "ref.h"
#include "hoe_model.h"
#include "object_controller.h"
#include "shared.h"
#include "camera.h"
#include "scene.h"
#include "hoe_time.h"

#include "hoe.h"
#include <hoe_math.h>
#include "particle_emitor.h"

ObjectController::ObjectController() : m_scale(1,1,1)
{
	object = NULL;
	flags = 0;
	m_frame = 0.f;
	m_overcolor = 0xffffffff;
}

bool ObjectController::Create(XHoeObject * obj)
{
	object = obj;
	model = NULL;
	pos.Reset();
	obj->SetCtrl(this);
	return true;
}

void ObjectController::Render(const HoeScene * scene)
{
	if (!model || !scene->GetCamera()->BoundInFlustrum(pos.xyz, model->GetBound()))
		return;

	HoeMath::Matrix4v m;
	this->pos.GetMatrix(&m);
	if (flags & HOF_SCALED)
	{
		HoeMath::Matrix4v t;
		t.Scale(m_scale);
		t.Multiply(m);
		Ref::SetMatrix(t);
	}
	else
		Ref::SetMatrix(m);

	if (model)
        model->Render(scene, m_frame, m_overcolor);

	// info
	if (m_adv.Count() > 0)
	{
		for (uint n=0;n < m_adv.Count();n++)
		{
			const TSubObjectPtr & p = m_adv.Get(n);
			switch (p.type)
			{
			case THoeSubObject::Object:
				{
					const THoeSub_Model & sm = *reinterpret_cast<const THoeSub_Model*>(p.ptr);
					HoeMath::Matrix4v a = sm.pos;
					a.Multiply(m);
					Ref::SetMatrix(a);
					if (sm.model)
						dynamic_cast<HoeModel*>(sm.model)->Render(scene, m_frame, m_overcolor);
				}
				break;
			case THoeSubObject::Particle:
				{
					const THoeSub_Particle & sm = *reinterpret_cast<const THoeSub_Particle*>(p.ptr);
					HoeMath::Matrix4v a;
					a.Translate(sm.pos);
					a.Multiply(m);
					Ref::SetMatrix(a);
					if (sm.emitor)
						dynamic_cast<ParticleEmitor*>(sm.emitor)->Render(scene->GetCamera());
				}
				break;
			};
		}
	}
}

void ObjectController::Unregister()
{
	// dfdsf
	object = NULL;
	m_adv.Delete();
	delete this;
}

void ObjectController::SetModel(IHoeModel * model_)
{
	model = static_cast<HoeModel*>(model_);
}

IHoeModel * ObjectController::GetModel()
{
	return model;
}

bool ObjectController::LoadModel(const tchar * cmd)
{
	model = (HoeModel*) GetHoe3D()->Create(cmd);

	return false;
}

void HOEAPI ObjectController::SetPosition(const HoeMath::Vector3v &p)
{
	pos.xyz = p;
}

void HOEAPI ObjectController::SetOrientation(const float x, const float y, const float z, const float angle)
{
	HoeMath::Vector3v vect(x,y,z);
	vect.Normalize();
	pos.rot.Create(vect,angle);
}

const HoeMath::Vector3v & ObjectController::GetPosition() const
{
	return pos.xyz; 
}

void ObjectController::GetOrientation(float *x, float *y, float *z, float *angle)
{
	if (x) *x = pos.rot.x; 
	if (y) *y = pos.rot.y; 
	if (z) *z = pos.rot.z; 
	if (angle) *angle = pos.rot.w; 
}

void ObjectController::SetScale(const HoeMath::Vector3v &scale)
{
	if (scale == HoeMath::Vector3v(1.f, 1.f, 1.f))
		UnsetFlags(HOF_SCALED);
	else
		SetFlags(HOF_SCALED);
	m_scale = scale;
}

const HoeMath::Vector3v & ObjectController::GetScale() const
{
	return m_scale;
}

void ObjectController::Link(THoeSubObject::Type type, THoeSubObject * obj)
{
	const TSubObjectPtr ptr = { type, obj };
	m_adv.Add(ptr);
}

void ObjectController::Unlink(THoeSubObject * obj)
{
	assert(!"not implemented.");
	//m_adv.Remove(obj);
}

void ObjectController::SetFlags(unsigned long f)
{
	flags |= f;
}

void ObjectController::UnsetFlags(unsigned long f)
{
	flags &= ~f;
}





