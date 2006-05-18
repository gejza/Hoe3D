
#include "system.h"
#include "utils.h"
#include "ref.h"
#include "hoe_model.h"
#include "object_controller.h"
#include "shared.h"
#include "camera.h"
#include "scene.h"

#include "hoe.h"
#include "hoe3d_math.h"

ObjectController::ObjectController() : m_scale(1,1,1)
{
	object = NULL;
	flags = 0;
}

bool ObjectController::Create(XHoeObject * obj)
{
	object = obj;
	model = NULL;
	pos.Reset();
	obj->SetCtrl(this);
	return true;
}

class HoePaint3D : public IHoePaint3D
{
protected:
	const HoeMath::MATRIX * m_basematrix;
	HoeMath::MATRIX actmatrix;
public:
	HoePaint3D(const HoeMath::MATRIX * base);
	virtual void ResetPos();
	virtual void Move(float x, float y, float z);
	virtual void Scale(float x, float y, float z);
	virtual void RotateY(float angle);
	virtual void Paint(IHoeModel * model);
	virtual const char * GetName();
};

HoePaint3D::HoePaint3D(const HoeMath::MATRIX * base)
{
	m_basematrix = base;
	ResetPos();
}

void HoePaint3D::ResetPos()
{
	actmatrix.Identity();
}

const char * HoePaint3D::GetName()
{
	return "Hoe Advanced Painter";
}

void HoePaint3D::Move(float x, float y, float z)
{
	HoeMath::MATRIX m;
	m.Translate(x,y,z);
	actmatrix.Multiply(m);
}

void HoePaint3D::Scale(float x, float y, float z)
{
	HoeMath::MATRIX m;
	m.Scale(x,y,z);
	actmatrix.Multiply(m);
}


void HoePaint3D::RotateY(float angle)
{
	HoeMath::MATRIX m;
	m.RotationY(angle);
	actmatrix.Multiply(m);
}

void HoePaint3D::Paint(IHoeModel * model)
{
	actmatrix.Multiply(*this->m_basematrix);
	Ref::SetMatrix(actmatrix);
	const HoeModel * m = dynamic_cast<HoeModel*>(model);
	assert(m != NULL);
	//assert(!"old code");
	m->Render(NULL);
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
        model->Render(scene);

	// info
	if (flags & HOF_ADVSHOW)
	{
		HoePaint3D p(&m);
		this->object->AdvPaint(&p);
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

void ObjectController::SetFlags(unsigned long f)
{
	flags |= f;
}

void ObjectController::UnsetFlags(unsigned long f)
{
	flags &= ~f;
}





