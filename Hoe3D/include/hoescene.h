
#ifndef _HOE_SCENE_INTERFACE_H_
#define _HOE_SCENE_INTERFACE_H_

/////////////////////////////////////////////////////////

namespace IHoeEnv
{
	class Base
	{
	public:
	};
	class Poly : public Base
	{
	public:
		virtual void HOEAPI SetPos(float * v, uint stride) = 0;
		virtual void HOEAPI SetTex(float * v, uint stride) = 0;
		virtual void HOEAPI SetMaterial(IHoeMaterial * m) = 0;
	};
	class HeightMapSurface : public Base
	{
	public:
		/**
		* Nastavi pozici stredu mapy
		*/
		virtual void HOEAPI SetPosCenter( float x, float y, float z) = 0;
		/**
		* Nastavi velikost mapy
		*/
		virtual void HOEAPI SetSize(float sizeX, float sizeY) = 0;
		/**
		* Vygeneruje vyskovou mapu
		* @param resX rozliseni sirky (pocet bodu mapy)
		* @param resY rozliseni vysky (pocet bodu mapy)
		*/
		virtual void HOEAPI GenerateHeight(int resX,int resY) = 0;
		/**
		* Nastavi vyskovou mapu
		* @param resX rozliseni sirky (pocet bodu mapy)
		* @param resY rozliseni vysky (pocet bodu mapy)
		* @param f Jednotlive vysky mapy, ulozene po radcich (podle sizeX)
		*/
		virtual void HOEAPI SetHeightMap(int resX,int resY, float *f) = 0;
		virtual void HOEAPI ShowBrush(bool show) = 0;
		virtual void HOEAPI SetBrush(float x, float y, float radius, dword color) = 0;
		virtual void HOEAPI MoveHeight(float x, float y, float radius, float value) = 0;
		// Dump
		
	};
	class GridSurface : public Base
	{
	public:
		/**
		* Nastavi pozici stredu mapy
		*/
		virtual void HOEAPI SetPosCenter( float x, float y, float z) = 0;
		/**
		* Nastavi velikost mapy
		*/
		virtual void HOEAPI SetSize(float sizeX, float sizeY) = 0;
		/**
		* Vygeneruje vyskovou mapu
		* @param resX rozliseni sirky (pocet bodu mapy)
		* @param resY rozliseni vysky (pocet bodu mapy)
		*/
		virtual void HOEAPI GenerateHeight(int resX,int resY) = 0;
		/**
		* Nastavi vyskovou mapu
		* @param resX rozliseni sirky (pocet bodu mapy)
		* @param resY rozliseni vysky (pocet bodu mapy)
		* @param f Jednotlive vysky mapy, ulozene po radcich (podle sizeX)
		*/
		virtual void HOEAPI SetHeightMap(int resX,int resY, float *f) = 0;
		virtual void HOEAPI ShowBrush(bool show) = 0;
		virtual void HOEAPI SetBrush(float x, float y, float radius, dword color) = 0;
		virtual void HOEAPI MoveHeight(float x, float y, float radius, float value) = 0;
		virtual void HOEAPI ShowWireframe(bool show) = 0;
		
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
	virtual IHoeEnv::GridSurface * HOEAPI CreateGridSurface() = 0;
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

