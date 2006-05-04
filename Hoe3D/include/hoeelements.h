
#ifndef _HOE_ELEMENTS_INTERFACE_H_
#define _HOE_ELEMENTS_INTERFACE_H_

class IHoeElement : public IHoeInterface
{
};

/////////////////////////////////////////////////////////

namespace IHoeEnv
{
	class Unknown
	{
	public:
		virtual int HOEAPI GetID() = 0;
		virtual void HOEAPI Release() = 0;
	};
	class Poly : public Unknown
	{
	public:
		virtual void HOEAPI SetPos(float * v, uint stride) = 0;
		virtual void HOEAPI SetTex(float * v, uint stride) = 0;
		virtual void HOEAPI SetMaterial(IHoeMaterial * m) = 0;
	};
	class HeightMapSurface : public Unknown
	{
	public:
		virtual void HOEAPI LoadHeight(float _distX,float _distY,int _sizeLevel, float *f) = 0;
	};
};

class IHoeCamera : public IHoeElement
{
public:
	virtual void HOEAPI Set(float * pos,float * look) = 0;
	virtual void HOEAPI Pick(const float x, const float y, float * vPickRayDir, float * vPickRayOrig) = 0;
	virtual void HOEAPI GetSize(int *w, int *h) const = 0;
};


class IHoeScenePhysics : public IHoeElement
{
public:
	virtual float HOEAPI GetHeight(float x,float y) = 0;
	virtual bool HOEAPI GetCamber(const float x1,const float x2,const float y1,const float y2, float & min,float &max) = 0;
	/** Ray tracer */
	virtual XHoeObject * HOEAPI Ray(float * vPickRayDir, float * vPickRayOrig) = 0;

};

class IHoeSceneEnv : public IHoeElement
{
public:
	/** funkce ktera vytvori polygon o n vrcholech*/
	virtual IHoeEnv::Poly * HOEAPI CreatePolygon(int numVec) = 0;
	virtual void HOEAPI RemovePolygon(int id) = 0;
	virtual IHoeEnv::HeightMapSurface * HOEAPI CreateHeightMapSurface() = 0;
	virtual IHoeMaterial * HOEAPI GetMaterial(const char * name) = 0;
};

/**
 * @brief Kurzor
 * @code
 * "cursor [resource]"
 * @endcode
 */
class iHoeCursor : public IHoeElement
{
public:

	/**
	 * Zobrazí kurzor 
	 */
	virtual void Show() = 0;

	/**
	 * Schová kurzor 
	 */
	virtual void Hide() = 0;
	/**
	 * Nastaví pozici kurzoru 
	 */
	virtual void SetPos(int x,int y) = 0;
	/** 
	 * Nahraje kurzor ze souboru 
	 */
	virtual bool LoadCursor(int px,int py,const char * lpCursorName) = 0;

	/**
	 * Propojí kurzor se vstupem myši.
	 */
	virtual bool AcquireInput() = 0;

	/**
	 * Odpojí kurzor od myši.
	 */
	virtual void UnacquireDevice() = 0;

	/**
	 * Nastavi funkci pro pøíjem kurzorových zpráv.
	 */
	//virtual void SetCallback(HoeInput::MsgCallback) = 0;
};

class IHoeParticleEmitor;
class IHoeLight
{
public:
	virtual void HOEAPI SetPosition(const float x, const float y, const float z) = 0;
	virtual void HOEAPI SetColor(const float r, const float g, const float b) = 0;
};

class IHoeScene : public IHoeElement
{
public:
	virtual void HOEAPI Set2DCallback(XHoe2DCallback * cb) = 0;
	/** Zaregistruje objekt */
	virtual int HOEAPI RegisterObject(XHoeObject *, unsigned long flags = 0) = 0;
	/** Vytvori particle emitor */
	virtual IHoeParticleEmitor * HOEAPI CreateParticleEmitor() = 0;
	/** Vytvori svetlo */
	virtual IHoeLight * HOEAPI CreateLight() = 0;
	/** Fyzika */
	virtual IHoeScenePhysics * HOEAPI GetScenePhysics() = 0;

	/** Prostredi sceny */
	virtual IHoeSceneEnv * HOEAPI GetSceneEnv() = 0;

	/** Kamera */
	virtual IHoeCamera * HOEAPI GetCamera() = 0;
	/** smazani sceny */
	virtual void Delete() = 0;
};

#endif // _HOE_ELEMENTS_INTERFACE_H_

