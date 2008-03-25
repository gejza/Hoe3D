
#ifndef _HOE_SCENE_H_
#define _HOE_SCENE_H_

#include "object_inspector.h"
#include "scene_base.h"
#include "scene_graph.h"
#include "light_system.h"
#include "camera.h"

class HoeScene : public HoeBaseScene
{
protected:
	ObjectInspector m_objects;
	LightSystem m_lights;
	HoeCamera cam;
public:
	HoeScene();

	virtual int HOEAPI RegisterObject(XHoeObject *, unsigned long flags = 0);
	virtual void HOEAPI UnregisterObject(XHoeObject *);

	/** @see IHoeScene::CreateLight() */
	virtual IHoeLight * HOEAPI CreateLight(bool diRECT);
	//virtual IHoeScenePhysics * HOEAPI GetScenePhysics();
	const HoeCamera * GetCamera() const { return &cam; }
	const LightSystem * GetLS() const { return &m_lights; }
	virtual IHoeCamera * HOEAPI GetCamera();

	virtual void Process(const double dtime);
	virtual void Render(TRenderParameters * rp);

};

class HoeGraphScene : public HoeScene , public IHoeScenePhysics, public IHoeSceneEnv
{
protected:
	TSceneGroup * m_root;
	TSceneGroup * NewGroup();
public:
	HoeGraphScene();
	virtual void Render(TRenderParameters * rp);
	/** Metoda pro ziskani jmena objektu. */
	virtual const tchar * GetName() { return T("GraphScene");}
	virtual IHoeScenePhysics * HOEAPI GetScenePhysics() { return this; }
	virtual IHoeSceneEnv * HOEAPI GetSceneEnv() { return this; }

	// environment
	virtual IHoeEnv::Poly * HOEAPI CreatePolygon(int numVec);
	virtual void HOEAPI RemovePolygon(int id);
	virtual IHoeEnv::HeightMapSurface * HOEAPI CreateHeightMapSurface();
	virtual IHoeEnv::GridSurface * HOEAPI CreateGridSurface();
	virtual IHoeMaterial * HOEAPI GetMaterial(const tchar * name);

	// physicks
	//iHoeMap * HOEAPI LoadMap(const char * name, XMapLoaderCallback *);
	virtual XHoeObject * HOEAPI Ray(const HoeMath::Vector3v & vPickRayDir, const HoeMath::Vector3v & vPickRayOrig);
	virtual vfloat HOEAPI GetHeight(vfloat x,vfloat y);
	virtual bool HOEAPI GetCamber(const vfloat x1,const vfloat x2,const vfloat y1,const vfloat y2, vfloat & min,vfloat &max);

};

#endif // _HOE_SCENE_H_

