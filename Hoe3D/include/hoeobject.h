
#ifndef _HOE_OBJECT_INTERFACE_H_
#define _HOE_OBJECT_INTERFACE_H_

/*
* OBJECT
*
*/

class IHoeModel;

// flags
// zobrazi objekt
#define HOF_SHOW     0x01
// vola autoupdate pri kazdem pocitani snimku
#define HOF_UPDATE   0x02
// roztahnuty objekt
#define HOF_SCALED   0x08
// pri vykresleni zavola jeste proceduru dodatecneho vykresleni
#define HOF_ADVSHOW  0x10

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
	virtual bool HOEAPI LoadModel(const char * cmd) = 0;

	/** Nastavi aktualni pozici objektu */
	virtual void HOEAPI SetPosition(const float x, const float y, const float z) = 0;

	/** vrati aktualni pozici objektu */
	virtual void HOEAPI GetPosition(float *x, float *y, float *z) = 0;

	/** Nastavi oriantaci objektu */
	virtual void HOEAPI SetOrientation(const float x, const float y, const float z, const float angle) = 0;

	/** Vrati oriantaci objektu */
	//virtual void HOEAPI GetOrientation(float *x, float *y, float *z, float *angle) = 0;

	virtual void HOEAPI SetScale(const float x, const float y, const float z) = 0;

	virtual void HOEAPI GetScale(float *x, float *y, float *z) = 0;

	/** odregistrovani objektu */
	virtual void HOEAPI Unregister() = 0;

	/** nastaveni priznaku */
	virtual void HOEAPI SetFlags(unsigned long  flags) = 0;

	/** odnastaveni priznaku */
	virtual void HOEAPI UnsetFlags(unsigned long  flags) = 0;

};

/**
 * @brief Zaklad objektu. Objekt vytvari aplikace
 */
class XHoeObject
{
protected:
	IHoeObjectController * controler;
public:
	inline IHoeObjectController * GetCtrl() { assert(controler); return controler; }
	inline void SetCtrl(IHoeObjectController * ctrl) { controler = ctrl; };

	/** Prirazeni modelu */
	inline void SetModel(IHoeModel * model);

	/** Vrati aktualni model */
	inline IHoeModel * GetModel();

	/** Nahrani modelu */
	inline bool LoadModel(const char * cmd);
	
	/** Nastavi aktualni pozici objektu */
	inline void SetPosition(const float x, const float y, const float z);

	/** Nastavi oriantaci objektu */
	inline void SetOrientation(const float x, const float y, const float z, const float angle);

	/** zobrazeni */
	inline void Show(bool show);

	/** autoupdate */
	virtual void Update(const double dtime) {};

	/** Vykresleni infa */
	virtual void AdvPaint(class IHoePaint3D * advpaint) {};
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

bool XHoeObject::LoadModel(const char * cmd)
{
	return GetCtrl()->LoadModel(cmd);
}

void XHoeObject::SetPosition(const float x, const float y, const float z)
{
	GetCtrl()->SetPosition(x,y,z);
}

void XHoeObject::SetOrientation(const float x, const float y, const float z, const float angle)
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


#endif // _HOE_OBJECT_INTERFACE_H_

