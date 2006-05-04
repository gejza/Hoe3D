
#ifndef _OBJECT_CONTROLLER_H_
#define _OBJECT_CONTROLLER_H_

#include "../include/hoeobject.h"
#include "map_utils.h"

class HoeModel;
class HoeCamera;

class ObjectController : public IHoeObjectController
{
	MapOrientation pos;

	dword flags;
public:
	XHoeObject * object;
	HoeModel * model;
	HoeMath::VECTOR3 m_scale;

	ObjectController();
	bool IsValid() { return (object != NULL); }
	bool Create(XHoeObject * obj);
	void Render(const HoeCamera * cam);
	//inline bool IsShow() { return flags & 0x1; }
	inline dword GetFlags() { return flags; }
	inline XHoeObject * GetObject() { return object; }
	inline const HoeMath::VECTOR3 & GetVectorPosition() { return pos.xyz; }

	virtual void HOEAPI SetModel(IHoeModel * model);
	virtual IHoeModel * HOEAPI GetModel();
	virtual bool HOEAPI LoadModel(const char * cmd);
	virtual void HOEAPI Unregister();

	virtual void HOEAPI SetPosition(const float x, const float y, const float z);
	virtual void HOEAPI SetOrientation(const float x, const float y, const float z, const float angle);
	virtual void HOEAPI GetPosition(float *x, float *y, float *z);
	virtual void HOEAPI GetOrientation(float *x, float *y, float *z, float *angle);
	virtual void HOEAPI SetScale(const float x, const float y, const float z);
	virtual void HOEAPI GetScale(float *x, float *y, float *z);


	virtual void HOEAPI SetFlags(unsigned long flags);
	virtual void HOEAPI UnsetFlags(unsigned long flags);
};

#endif // _OBJECT_CONTROLLER_H_

