
#ifndef _PHYSICS_H_
#define _PHYSICS_H_

class ObjectInspector;

// nejak naznacene kolize
// na to musi byt seznam polygonalnich veci
// v podstate zatim nijak nepropojene

class Physics : public IHoePhysics
{
	ObjectInspector * m_oi;
public:
	Physics();
	~Physics();
	virtual const tchar * GetName() { return T("Physics engine v1.0");}

	virtual void HOEAPI SetPart(unsigned long partID);
	virtual XHoeObject * HOEAPI RayTrace(const vfloat *,const vfloat *,unsigned long flags);
};

#endif // _PHYSICS_H_

