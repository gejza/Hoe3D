
/**
   @file    gridsurface.h
   @date    Mar 2006
   @version 1.0
   @brief   Deklarace trid pro grid surface.
*/

#ifndef _HOE_GRIDSURFACE_H_
#define _HOE_GRIDSURFACE_H_

#include "../include/hoeinterfaces.h"
#include "hoe3d_math.h"
#include "hoe_index.h"
#include "hoe_stream.h"
#include "hoe_material.h"
#include "filesystem.h"
#include "camera.h"
#include "structures.h"
#include "heightmap.h"

struct TGridData : public IHoeEnv::GridSurface::TGridDesc
{
	enum Type
	{
		ePlane = 0,
		eModel,
	} type;
	union {
		struct {
			int modelid;
			float base_height;
		};
		struct {
			int resx;
			int resy;
			float * map_heights; //< v dumpu se pak uklada uplne na konci
		};
		float plane_heights[4];
	};
};

struct TGridTexture
{
	HoeTexture *tex; ///< textura
	size_t nx,ny; ///< velikost policek v texture
};

class HoeModel;
struct TGridModel
{
	HoeModel * mod;
	float coigns[4];
	bool LoadModel(const char * name);
};

struct TGridSurfaceTreeItem
{
	TGridSurfaceTreeItem *q1,*q2,*q3,*q4; ///< podpovrchy
	///< rozmery a stred
	
	dword start; ///< zacatek trojuhelniku pro vykresleni
	dword end; ///< konec trojuhelniku pro vykresleni
	void DeleteSub();
};

struct TGridSurfaceType
{
	HoeTexture *tex1, *tex2; ///< textura 
	TGridSurfaceTreeItem * root; /// zacatek kresleni stromu
	TGridSurfaceType * next; ///< ukazatel na dalsi povrch
	/** Nastavi na vykresleni */
	void Setup();
};

// multistream - pracovni nazev, pak se presune
/**
* Univerzalni objekt, ktery obsahuje indexy a streamy
* je delany pro vetsi mnozstvi predem neznamych dat
* umi rozdelovat indexy a vertexy rovnormerne do streamu
* a pak renderovat jen nektere, je optimalizovan na renderovani dat v poradi ve kterem
* se zapisuji do multistreamu
* v ramci optimalizace neni zaruceno, ze nektera data se nevyrenderuji navic
*/
class MultiStream
{
protected:
	static const uint MaxVerticesSize = 65000;
	static const uint MaxIndicesSize = 65000;
	struct Mesh
	{
		HoeStream vertices; ///< stream pro vertexy
		HoeIndex indices; ///< indexy
		dword numvert; ///< pocet vertexu
		dword numind; ///< pocet indexu
	} m_mesh[30];
	uint m_nummesh; ///< pocet vytvorenych meshu
	HoeFVF m_fvf; ///< FVF
	size_t m_sn; ///< velikost jednoho vertexu
	byte * m_lv; ///< Zamcene vertexy
	word * m_il; ///< zamcene indexy
	byte * m_vbuffer; ///< ukazatel na buffer vertexy
	word * m_ibuffer; ///< ukazatel na buffer indexu
	dword m_actvertex; ///< 
	dword m_actindex;
	word m_vertexoffset;
	/**
	* Vytvori z bufferu Mesh
	*/
	bool CreateMeshFromBuffer();
public:
	/** Konstruktor */
	MultiStream();
	/** Destruktor */
	virtual ~MultiStream();
	/**
	* Zacatek vytvareni streamu
	* @param fvf Textove fvf
	* @param sn Velikost jednoho vertexu v bytech
	*/
	bool Begin(const char * fvf, size_t sn);
	/**
	* Konec vytvareni streamu
	*/
	void End();
	/** 
	* Vytvoreni novych vertexu a indexu
	* @param numvert Pocet dalsich vertexu
	* @param numind Pocet dalsich indexu
	* @return Ukazatel na vertexy
	*/
	byte * LockNewVertices(dword numvert, dword numind);
	/**
	* Prida dalsi index do streamu, k indexu se automaticky pricte 
	* posledni index predchazejici skupiny indexu
	* @param ind Index
	*/
	void AddIndex(word ind);
	/**
	* Vraceni zakodovane pozice, obsahuje zakodovane cislo meshe a pozici
	*/
	dword GetPos() const;
	/**
	* Renderovani casti multistreamu
	* @param from zakodovana pozice odkud se ma renderovat
	* @param to zakodovana pozice mista kam az se ma renderovat
	*/
	void Render(dword from, dword to);
};



class GridSurface : public IHoeEnv::GridSurface
{
	static const int MaxTextureSlots = 32;

	HoeMath::MATRIX m_worldpos; ///< pozice stredu surface

	TGridData * m_grids; ///< ukazatel na pole gridu
	uint m_width, m_height; ///< velikost mrize
	float m_sizeX, m_sizeY; ///< realna velikost mrize
	HoeMath::VECTOR3 * m_normals; ///< predpocitane normaly

	TGridTexture m_textures[MaxTextureSlots]; ///< sloty na textury povrchu
	TGridModel m_models[MaxTextureSlots];///< sloty na modely

	TGridSurfaceType * m_gst_first; ///< ruzne povrchy

	MultiStream m_multi; ///< vertexova a indexova data
	bool m_loaded; ///< jestli jsou nahrany data
	bool m_wire; /// vykreslit v drataku

	/** funkce ktera vytvari podstromy
	*/
	TGridSurfaceTreeItem * CreateQuadTree(dword * gr, uint ngr, uint minx, uint maxx, uint miny, uint maxy);
	/** Nahrani roviny */
	bool PlaneToMulti(float vx, float vy, const HoeMath::MATRIX & matrix, const TGridData & grid, int x, int y);
	/** Nahrani modelu */
	bool ModelToMulti(const HoeMath::MATRIX & matrix, const TGridData & grid);
	/** Upravit roviny podle pripadnych modelu (nebo rovin). */
	void Opt_ProcessPlanes(uint fromx, uint tox, uint fromy, uint toy);
	/** Funkce ktera vraci vypoctenou vysku rohoveho bodu (zadava se spodni hranice)
	* Modely se prumeruji, roviny se prizpusobuji modelum, pripadne se prumeruji */
	float Opt_GetHeight(uint x, uint y);
	/** Funkce prepocita normaly */
	void BuildNormals();
	const HoeMath::VECTOR3 GetNormal(int x, int y, int roh);
public:
	/** Konstruktor */
	GridSurface();
	/** Destruktor */
	virtual ~GridSurface();
	/**
	* Vytvori index a vertex streamy z gridu
	*/
	/** @see IHoeEnv::GridSurface::Load */
	virtual void HOEAPI Load();
	virtual void HOEAPI ReleaseData();
	void Unload();
	void Render();
	virtual void HOEAPI SetPosCenter( float x, float y, float z);
	/** @see IHoeEnv::GridSurface::Create */
	virtual void HOEAPI Create(float sizeX, float sizeY, int resX,int resY);
	/** @see IHoeEnv::GridSurface::SetTexture */
	virtual void HOEAPI SetTexture(int slot, const char * texname, int width, int height);
	/** @see IHoeEnv::GridSurface::SetModel */
	virtual void HOEAPI SetModel(int slot, const char * modname);
	/** @see IHoeEnv::GridSurface::GetDesc */
	virtual void HOEAPI GetDesc(float *sizeX, float *sizeY, uint *resX,uint *resY);
	/** @see IHoeEnv::GridSurface::SetGridDesc */
	virtual void HOEAPI SetGridDesc(int x, int y, TGridDesc * desc);
	/** @see IHoeEnv::GridSurface::GetGriddesc */
	virtual void HOEAPI GetGridDesc(int x, int y, TGridDesc * desc);
	/** @see IHoeEnv::GridSurface::SetGridModel */
	virtual void HOEAPI SetGridModel(int x, int y, float height, int modelid);
	/** @see IHoeEnv::GridSurface::GetGridModel */
	virtual int HOEAPI GetGridModel(int x, int y);
	/** @see IHoeEnv::GridSurface::SetGridPlane */
	virtual void HOEAPI SetGridPlane(int x, int y, float height, float lt = 0.f, float rt = 0.f, float lb = 0.f, float rb = 0.f);
	/** @see IHoeEnv::GridSurface::SetGridHeightmap */
	virtual void HOEAPI SetGridHeightmap(int x, int y, float height, int resx, int resy, float * h);

	// hejbani
	virtual void HOEAPI MoveHeight(float x, float y, float moveheight, float radius);

	// funkce pro fyziku
	bool GetHeight(const float x, const float y, float * height);
	bool GetCamber(const float x1,const float x2,const float y1,const float y2, float & min,float &max);
	virtual float HOEAPI GetAvgHeight(const uint x, const uint y, float *min, float *max);

	//virtual void HOEAPI ShowBrush(bool show);
	//virtual void HOEAPI SetBrush(float x, float y, float radius, dword color);
	/** @see IHoeEnv::GridSurface::ShowWireframe */
	virtual void HOEAPI ShowWireframe(bool show);
	/** @see IHoeEnv::GridSurface::Dump */
	virtual void HOEAPI Dump(XHoeStreamWrite * stream);
	/** @see IHoeEnv::GridSurface::LoadDump */
	virtual void HOEAPI LoadDump(XHoeStreamRead * stream);
};

#endif // _HOE_GRIDSURFACE_H_
