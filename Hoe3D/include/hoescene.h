
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
		virtual void HOEAPI SetPos(vfloat * v, uint stride) = 0;
		virtual void HOEAPI SetTex(vfloat * v, uint stride) = 0;
		virtual void HOEAPI SetMaterial(IHoeMaterial * m) = 0;
	};
	class HeightMapSurface : public Base
	{
	public:
		/**
		* Nastavi pozici stredu mapy
		*/
		virtual void HOEAPI SetPosCenter( vfloat x, vfloat y, vfloat z) = 0;
		/**
		* Nastavi velikost mapy
		*/
		virtual void HOEAPI SetSize(vfloat sizeX, vfloat sizeY) = 0;
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
		virtual void HOEAPI SetHeightMap(int resX,int resY, vfloat *f) = 0;
		virtual void HOEAPI ShowBrush(bool show) = 0;
		virtual void HOEAPI SetBrush(vfloat x, vfloat y, vfloat radius, dword color) = 0;
		virtual void HOEAPI MoveHeight(vfloat x, vfloat y, vfloat radius, vfloat value) = 0;
		// Dump
		
	};
	/**
	* Model mrizky, s vice definovatelnymi povrchy
	*/
	class GridSurface : public Base
	{
	public:
		/**
		* Parametr jednoho policka
		*/
		struct TGridDesc
		{
			byte tex1; ///< index textury
			byte x1; ///< x-ove policko v texture
			byte y1; ///< y-ove policko v texture
			byte tex2; ///< index textury
			byte x2; ///< x-ove policko v texture
			byte y2; ///< y-ove policko v texture
		};
		/**
		* Nastavi pozici stredu mapy
		*/
		virtual void HOEAPI SetPosCenter( vfloat x, vfloat y, vfloat z) = 0;
		/**
		* Vytvori grid mapu
		* @param sizeX realna velikost na sirku
		* @param sizeY realna velikost na vysku
		* @param resX rozliseni sirky (pocet bodu mapy)
		* @param resY rozliseni vysky (pocet bodu mapy)
		*/
		virtual void HOEAPI Create(vfloat sizeX, vfloat sizeY, int resX,int resY) = 0;
		/**
		* Aktualizuje mapu
		*/
		virtual void HOEAPI Load() = 0;
		/**
		* Nastavi texturu
		* @param slot Na ktery slot nastavit texturu
		* @param texname Jmeno textury
		* @param width Pocet ctvercu na sirku
		* @param height Pocet ctvercu na vysku
		*/
		virtual void HOEAPI SetTexture(int slot, const tchar * texname, int width, int height) = 0;
		/**
		* Nastavi model
		* @param slot Na ktery slot nastavit model
		* @param modname Jmeno modelu
		*/		
		virtual void HOEAPI SetModel(int slot, const tchar * modname) = 0;
		/**
		* Ziskani parametru gridu. Muze byt i NULL
		* @param sizeX realna velikost na sirku
		* @param sizeY realna velikost na vysku
		* @param resX rozliseni sirky (pocet bodu mapy)
		* @param resY rozliseni vysky (pocet bodu mapy)
		*/
		virtual void HOEAPI GetDesc(vfloat *sizeX, vfloat *sizeY, uint *resX,uint *resY) = 0;
		/**
		* Nastavi parametry jedne mrize
		* @param x X-ova souradnice policka
		* @param y Y-ova souradnice policka
		* @param desc ukazatel na strukturu GridDesc
		*/
		virtual void HOEAPI SetGridDesc(int x, int y, TGridDesc * desc) = 0;
		/**
		* PRECTe parametry jedne mrize
		* @param x X-ova souradnice policka
		* @param y Y-ova souradnice policka
		* @param desc ukazatel na strukturu GridDesc
		*/
		virtual void HOEAPI GetGridDesc(int x, int y, TGridDesc * desc) = 0;
		/**
		* nastavi na zobrazeni dratoveho modelu
		* @param show Zda zobrazit wireframe
		*/
		virtual void HOEAPI ShowWireframe(bool show) = 0;
		/**
		* Ulozi objekt do streamu
		*/
		virtual void HOEAPI Dump(HoeCore::WriteStream * stream) = 0;
		/**
		* Nahraje objekt ze streamu
		*/
		virtual void HOEAPI LoadDump(HoeCore::ReadStream * stream) = 0;
		virtual void HOEAPI ReleaseData() = 0;

		/**
		* Nastavi skosenou rovinu
		* lt - levy horni
		* rt - pravy horni
		* lb - levy dolni
		* rb - pravy dolni
		*/
		virtual void HOEAPI SetGridPlane(int x, int y, vfloat height, vfloat lt = 0.f, vfloat rt = 0.f, vfloat lb = 0.f, vfloat rb = 0.f) = 0;
		virtual void HOEAPI SetGridModel(int x, int y, vfloat height, int modelid) = 0;
		virtual int HOEAPI GetGridModel(int x, int y) = 0;
		virtual void HOEAPI SetGridHeightmap(int x, int y, vfloat height, int resx, int resy, vfloat * h) = 0;
		virtual vfloat HOEAPI GetAvgHeight(const uint x, const uint y, vfloat *min, vfloat *max) = 0;
		// funkce na editovani
		virtual void HOEAPI MoveHeight(vfloat x, vfloat y, vfloat moveheight, vfloat radius) = 0;
	};
};

class IHoeScenePhysics : public IHoeElement
{
public:
	virtual vfloat HOEAPI GetHeight(vfloat x,vfloat y) = 0;
	virtual bool HOEAPI GetCamber(const vfloat x1,const vfloat x2,const vfloat y1,const vfloat y2, vfloat & min,vfloat &max) = 0;
	/** Ray tracer */
	virtual XHoeObject * HOEAPI Ray(const HoeMath::Vector3v & vPickRayDir, const HoeMath::Vector3v & vPickRayOrig) = 0;

};

class IHoeSceneEnv : public IHoeElement
{
public:
	/** funkce ktera vytvori polygon o n vrcholech*/
	virtual IHoeEnv::Poly * HOEAPI CreatePolygon(int numVec) = 0;
	virtual void HOEAPI RemovePolygon(int id) = 0;
	virtual IHoeEnv::HeightMapSurface * HOEAPI CreateHeightMapSurface() = 0;
	virtual IHoeEnv::GridSurface * HOEAPI CreateGridSurface() = 0;
	virtual IHoeMaterial * HOEAPI GetMaterial(const tchar * name) = 0;
};


class IHoeScene : public IHoeElement
{
public:
	virtual void HOEAPI Set2DCallback(XHoe2DCallback * cb) = 0;
	/** Zaregistruje objekt */
	virtual int HOEAPI RegisterObject(XHoeObject *, unsigned long flags = 0) = 0;
	/** Zaregistruje objekt */
	virtual void HOEAPI UnregisterObject(XHoeObject *) = 0;
	/** Vytvori particle emitor */
	virtual IHoeParticleEmitor * HOEAPI CreateParticleEmitor() = 0;
	/** Vytvori svetlo */
	virtual IHoeLight * HOEAPI CreateLight(bool diRECT) = 0;
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

