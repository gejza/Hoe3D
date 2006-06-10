
#ifndef _HOE_SCENE_INTERFACE_H_
#define _HOE_SCENE_INTERFACE_H_

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
		virtual void HOEAPI GenerateHeight(float sizeX,float sizeY,int res) = 0;
		virtual void HOEAPI SetHeight(float sizeX,float sizeY,int res, float *f) = 0;
		virtual void HOEAPI ShowBrush(bool show) = 0;
		virtual void HOEAPI SetBrush(float x, float y, float radius, dword color) = 0;
		virtual void HOEAPI MoveHeight(float x, float y, float radius, float value) = 0;
		
	};
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


class IHoeScene : public IHoeElement
{
public:
	virtual void HOEAPI Set2DCallback(XHoe2DCallback * cb) = 0;
	/** Zaregistruje objekt */
	virtual int HOEAPI RegisterObject(XHoeObject *, unsigned long flags = 0) = 0;
	/** Vytvori particle emitor */
	virtual IHoeParticleEmitor * HOEAPI CreateParticleEmitor() = 0;
	/** Vytvori svetlo */
	virtual IHoeLight * HOEAPI CreateLight(bool direct) = 0;
	/** Fyzika */
	virtual IHoeScenePhysics * HOEAPI GetScenePhysics() = 0;

	/** Prostredi sceny */
	virtual IHoeSceneEnv * HOEAPI GetSceneEnv() = 0;

	/** Kamera */
	virtual IHoeCamera * HOEAPI GetCamera() = 0;
	/** smazani sceny */
	virtual void Delete() = 0;
};

#endif // _HOE_SCENE_INTERFACE_H_

