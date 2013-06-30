
#ifndef _HOE_OBJECT_INTERFACE_H_
#define _HOE_OBJECT_INTERFACE_H_

#include <HoeCore/hoe_types.h>

/*
* OBJECT
*
*/

class IHoeModel;
class IHoeParticleEmitor;

struct THoeSubObject
{
	enum Type {
		Object,
		Particle
	};
};

struct THoeSub_Model : public THoeSubObject
{
	IHoeModel * model;
	HoeMath::Matrix4v pos;
};

struct THoeSub_Particle : public THoeSubObject
{
	IHoeParticleEmitor * emitor;
	HoeMath::Vector3v pos;
};

struct THoeSub_Sound : public THoeSubObject
{
	IHoeParticleEmitor * emitor;
	HoeMath::Vector3v pos;
};

// flags
// zobrazi objekt
#define HOF_SHOW     0x01
// vola autoupdate pri kazdem pocitani snimku
#define HOF_UPDATE   0x02
// roztahnuty objekt
#define HOF_SCALED   0x08
// vykresluje jako dratovy model
#define HOF_WIRE	 0x20

/**
 * @brief Nastaveni objektu v enginu.  
 */
class IHoeObjectController : public IHoeInterface
{
public:
	/** Prirazeni modelu */
	virtual void HOEAPI SetModel(IHoeModel * model) = 0;

	/** Vrati aktualni model */
	virtual IHoeModel * HOEAPI GetModel() = 0;

	/** Nahrani modelu */
	virtual bool HOEAPI LoadModel(const tchar * cmd) = 0;

	/** Nastavi aktualni pozici objektu */
	virtual void HOEAPI SetPosition(const HoeMath::Vector3v &pos) = 0;

	/** vrati aktualni pozici objektu */
	virtual const HoeMath::Vector3v & HOEAPI GetPosition() const = 0;

	/** Nastavi oriantaci objektu */
	virtual void HOEAPI SetOrientation(const vfloat x, const vfloat y, const vfloat z, const vfloat angle) = 0;

	/** Vrati oriantaci objektu */
	//virtual void HOEAPI GetOrientation(float *x, float *y, float *z, float *angle) = 0;

	virtual void HOEAPI SetScale(const HoeMath::Vector3v & scale) = 0;

	virtual const HoeMath::Vector3v & HOEAPI GetScale() const = 0;

	/** nastaveni priznaku */
	virtual void HOEAPI SetFlags(unsigned long  flags) = 0;

	/** odnastaveni priznaku */
	virtual void HOEAPI UnsetFlags(unsigned long  flags) = 0;

	/** prilinkovani podobjektu */
	virtual void HOEAPI Link(THoeSubObject::Type type, THoeSubObject * obj) = 0;

	/** odlinkovani podobjektu */
	virtual void HOEAPI Unlink(THoeSubObject * obj) = 0;
	
	/** zobrazeni casu animace */
	virtual void HOEAPI SetAnimationTime(float t) = 0;

	virtual void HOEAPI SetOverColor(dword color) = 0;
};

/**
 * @brief Zaklad objektu. Objekt vytvari aplikace
 */
class XHoeObject
{
protected:
	IHoeObjectController * controler;
public:
	XHoeObject() { controler = NULL; }
	virtual ~XHoeObject() {}
	inline IHoeObjectController * GetCtrl() { assert(controler); return controler; }
	inline void SetCtrl(IHoeObjectController * ctrl) { controler = ctrl; };

	/** Prirazeni modelu */
	inline void SetModel(IHoeModel * model);

	/** Vrati aktualni model */
	inline IHoeModel * GetModel();

	/** Nahrani modelu */
	inline bool LoadModel(const tchar * cmd);
	
	/** Nastavi aktualni pozici objektu */
	inline void SetPosition(const vfloat x, const vfloat y, const vfloat z);

	/** Nastavi oriantaci objektu */
	inline void SetOrientation(const vfloat x, const vfloat y, const vfloat z, const vfloat angle);

	/** zobrazeni */
	inline void Show(bool show);

	inline void SetAnimationTime(float t);
};

/////////// HoeObject ///////////
void XHoeObject::SetModel(IHoeModel * model)
{
	GetCtrl()->SetModel(model);
}

IHoeModel * XHoeObject::GetModel()
{
	return GetCtrl()->GetModel();
}

bool XHoeObject::LoadModel(const tchar * cmd)
{
	return GetCtrl()->LoadModel(cmd);
}

void XHoeObject::SetPosition(const vfloat x, const vfloat y, const vfloat z)
{
	GetCtrl()->SetPosition(HoeMath::Vector3v(x,y,z));
}

void XHoeObject::SetOrientation(const vfloat x, const vfloat y, const vfloat z, const vfloat angle)
{
	GetCtrl()->SetOrientation(x,y,z,angle);
}

void XHoeObject::Show(bool show)
{
	if (show)
		GetCtrl()->SetFlags(HOF_SHOW);
	else
		GetCtrl()->UnsetFlags(HOF_SHOW);
}

void XHoeObject::SetAnimationTime(float t)
{
	GetCtrl()->SetAnimationTime(t);
}

#endif // _HOE_OBJECT_INTERFACE_H_

