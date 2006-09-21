
#include "system.h"
#include "utils.h"
#include "ref.h"
#include "hoe_model.h"
#include "object_controller.h"
#include "shared.h"
#include "camera.h"
#include "scene.h"
#include "hoe_time.h"

#include "hoe.h"
#include "hoe3d_math.h"
#include "particle_emitor.h"

ObjectController::ObjectController() : m_scale(1,1,1)
{
	object = NULL;
	flags = 0;
	m_frame = 0.f;
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

	HoeMath::MATRIX m;
	this->pos.GetMatrix(&m);
	if (flags & HOF_SCALED)
	{
		HoeMath::MATRIX t;
		t.Scale(m_scale);
		m.Multiply(t);
	}
	Ref::SetMatrix(m);

	if (model)
        model->Render(scene, m_frame);

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
					HoeMath::MATRIX a,b;
					if (sm.rotate)
					{
						b.Scale(sm.s_x,sm.s_y,sm.s_z);
						a.RotationY(SysFloatTime());
						a.Multiply(b);
					}
					else
					{
						a.Scale(sm.s_x,sm.s_y,sm.s_z);
					}
					b.Translate(sm.t_x,sm.t_y,sm.t_z);
					a.Multiply(b);
					a.Multiply(m);
					Ref::SetMatrix(a);
					if (sm.model)
						dynamic_cast<HoeModel*>(sm.model)->Render(scene, m_frame);
				}
				break;
			case THoeSubObject::Particle:
				{
					const THoeSub_Particle & sm = *reinterpret_cast<const THoeSub_Particle*>(p.ptr);
					HoeMath::MATRIX a;
					a.Translate(sm.t_x,sm.t_y,sm.t_z);
					a.Multiply(m);
					Ref::SetMatrix(a);
					if (sm.emitor)
						dynamic_cast<ParticleEmitor*>(sm.emitor)->Render();
				}
				break;
			};
		}
	}
	/*if (m_infocount > 0 && m_info)
	for (uint ii=0; ii < m_infocount;ii++)
	{
		if (m_info[ii])
		{
			HoeMath::MATRIX mt;
			mt.Translate(m_info[ii]->position[0],m_info[ii]->position[1],m_info[ii]->position[2]);
			HoeMath::MATRIX ms;
			ms.Scale(m_info[ii]->scale[0],m_info[ii]->scale[1],m_info[ii]->scale[2]);
			ms.Multiply(m);
			ms.Multiply(mt);
			Ref::SetMatrix(ms);
			assert(m_info[ii]->model);
			reinterpret_cast<HoeModel*>(m_info[ii]->model)->Render();
		}
	}*/
}

void ObjectController::Unregister()
{
	object = NULL;
}

void ObjectController::SetModel(IHoeModel * model_)
{
	model = static_cast<HoeModel*>(model_);
}

IHoeModel * ObjectController::GetModel()
{
	return model;
}

bool ObjectController::LoadModel(const char * cmd)
{
	model = (HoeModel*) GetHoe3D()->Create(cmd);

	return false;
}

void HOEAPI ObjectController::SetPosition(const float x, const float y, const float z)
{
	pos.xyz.Set(x,y,z);
}

void HOEAPI ObjectController::SetOrientation(const float x, const float y, const float z, const float angle)
{
	HoeMath::VECTOR3 vect(x,y,z);
	vect.Normalize();
	pos.rot.Create(vect,angle);
}

void ObjectController::GetPosition(float *x, float *y, float *z)
{
	if (x) *x = pos.xyz.x; 
	if (y) *y = pos.xyz.y; 
	if (z) *z = pos.xyz.z; 
}

void ObjectController::GetOrientation(float *x, float *y, float *z, float *angle)
{
	if (x) *x = pos.rot.x; 
	if (y) *y = pos.rot.y; 
	if (z) *z = pos.rot.z; 
	if (angle) *angle = pos.rot.w; 
}

void ObjectController::SetScale(const float x, const float y, const float z)
{
	m_scale.Set(x,y,z);
}

void ObjectController::GetScale(float *x, float *y, float *z)
{
	if (x) *x = m_scale.x;
	if (y) *y = m_scale.y;
	if (z) *z = m_scale.z;
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





