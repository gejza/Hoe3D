
#include "system.h"
#include "utils.h"
#include "shared.h"
#include "ref.h"
#include "hoe2d.h"
#include "camera.h"
#include "states.h"
#include "light_system.h"
#include "filesystem.h"
#include "scene.h"
#include "../include/hfmt/hmap_file.h"
#include "hoe_model.h"
#include "texture_system.h"
#include "scene_env.h"

// 	m_2dcb = NULL;

HoeScene::HoeScene()
{
}

int HoeScene::RegisterObject(XHoeObject * obj, unsigned long flags)
{
	return m_objects.RegisterObject(obj,flags);
}

void HoeScene::UnregisterObject(int id)
{
	m_objects.UnregisterObject(id);
}

IHoeLight * HoeScene::CreateLight(bool direct)
{
	HoeLight * l = new HoeLight(direct);
	m_lights.AddLight(l);
	return l;
}

void HoeScene::Render()
{
	ObjectController * obj;
	GetHoeStates()->SetupModel();

	cam.SetupMatrices();
	m_lights.Setup();

	m_objects.SetStreaming(HOF_SHOW);
	while ((obj = m_objects.Object()) != NULL)
	{
		if (obj->GetFlags() & HOF_WIRE)
		{
			GetHoeStates()->StartWireframe();
			obj->Render(this);
			GetHoeStates()->EndWireframe();
		}
		else
			obj->Render(this);
	}

}

void HoeScene::Process(const double dtime)
{

	ObjectController * obj;
	m_objects.SetStreaming(HOF_UPDATE);
	while ((obj = m_objects.Object()) != NULL)
	{
		obj->GetObject()->Update(dtime);
	}
}

#define MAX_SIZE_HEADER (10*1024)


IHoeCamera * HoeScene::GetCamera()
{
	return &cam;
}

///////////////////////////////////////////////////////////////////
HoeGraphScene::HoeGraphScene()
{
	m_root = NULL;
	//this->m_terrain.LoadHeight(10.f,10.f,3,NULL);
}

void HoeGraphScene::Render()
{
	ObjectController * obj;
	GetHoeStates()->SetupModel();

	/*
	preproces objektu (orezani a zjisteni svetel)
	begin
	renderovai do textur
	render sceny
	postprocess
	end
	*/

	cam.SetupMatrices();
	m_lights.Setup();

	HoeMath::MATRIX m;
	m.Identity();
	Ref::SetMatrix(m);
	TSceneGroup * g = m_root;
	while (g)
	{
		switch (g->type)
		{
		case TSceneGroup::ePolygon:
			g->value.p->Render();
			break;
		//case TSceneGroup::eHeightMapSurface:
		//	g->value.ter->render(&cam);
		//	break;
		case TSceneGroup::eGridSurface:
			g->value.grid->Render();
			break;
		};
		g = g->next;
	}
	
	// objekty budou ulozene v parts
	m_objects.SetStreaming(HOF_SHOW);
	while ((obj = m_objects.Object()) != NULL)
	{
		if (obj->GetFlags() & HOF_WIRE)
		{
			GetHoeStates()->StartWireframe();
			obj->Render(this);
			GetHoeStates()->EndWireframe();
		}
		else
			obj->Render(this);
	}
	
}
//////////////////////////////////////////////////////////
TSceneGroup * HoeGraphScene::NewGroup()
{
	TSceneGroup ** g = &m_root;
	while (*g) g = &((*g)->next);
	*g = new TSceneGroup;
	(*g)->next = NULL;
	return *g;
}

IHoeEnv::Poly * HoeGraphScene::CreatePolygon(int numVec)
{
	// zde by se mel vytvorit polygon
	TSceneGroup * g = NewGroup();
	HoePolygon * p = new HoePolygon;
	p->Create(numVec);
	g->type = TSceneGroup::ePolygon;
	g->value.p = p;
	return new HoePolygonSet(p);
}

void HoeGraphScene::RemovePolygon(int id)
{
}

IHoeEnv::HeightMapSurface * HoeGraphScene::CreateHeightMapSurface()
{
    assert(!"nedodelano");
	TSceneGroup * g = NewGroup();
	//g->type = TSceneGroup::eHeightMapSurface;
	//g->value.ter = new HoeQuadTerrain();
	//return g->value.ter;
    return NULL;
}

IHoeEnv::GridSurface * HoeGraphScene::CreateGridSurface()
{
	TSceneGroup * g = NewGroup();
	g->type = TSceneGroup::eGridSurface;
	g->value.grid = new GridSurface();
	return g->value.grid;
}

IHoeMaterial * HoeGraphScene::GetMaterial(const char * name)
{
	HoeMaterial * m = new HoeMaterial;
	HoeTexture * t = GetTextureSystem()->GetTexture(name);
	m->SetTexture(t);
	return (IHoeMaterial*)m;
}

//////////////////////////////////////////////////////////

XHoeObject * HoeGraphScene::Ray(float * vPickRayDir, float * vPickRayOrig)
{
	// predpocitat d atd..
	XHoeObject * act = NULL;
	float actlength = 9999999.f;
	ObjectController * obj;
	HoeMath::VECTOR3 d(vPickRayDir);
	d.Normalize();
	const float o = -d.x*vPickRayOrig[0] -d.y*vPickRayOrig[1] -d.z*vPickRayOrig[2];
	// projit vsechny objekty
	m_objects.SetStreaming(HOF_SHOW);
	while ((obj = m_objects.Object()) != NULL)
	{
		register float bb; 
		if (obj->model)
            bb = obj->model->GetBound().ball * 0.6f;
		else
			continue;

		float l = 0.f;
		// phase 1
		// vypocitat t
		const float t = HoeMath::HoeDot(d,obj->GetVectorPosition()) + o; 
		// rovnou odstranit pokud nesplnuji t
		if (t + bb > actlength)
			continue;

		// pocitat x,y,z a odstranovat pokud nespnuji
		register const float x = d.x * t + vPickRayOrig[0] - obj->GetVectorPosition().x;
		l = x * x;
		if (bb * bb < l)
			continue;

		register const float y = d.y * t + vPickRayOrig[1] - obj->GetVectorPosition().y;
		l += y * y;
		if (bb * bb < l)
			continue;

		register const float z = d.z * t + vPickRayOrig[2] - obj->GetVectorPosition().z;
		l += z * z;
		if (bb * bb < l)
			continue;

		actlength = t - bb;
		act = obj->GetObject();
		// phase 2

	}

	return act;
}

float HoeGraphScene::GetHeight(float x,float y)
{
	//float distX, distY;

	//distX=m_terrain.getDistX();
	//distY=m_terrain.getDistY();

	//return m_terrain.heightMap.getHeightAt(int(x/distX),(int)(y/distY));	
	return 0.f;
}

bool HoeGraphScene::GetCamber(const float x1,const float x2,const float y1,const float y2, float & min,float &max)
{
	/*float distX, distY;
	int gridX1, gridY1, gridX2, gridY2;

	distX=m_terrain.getDistX();
	distY=m_terrain.getDistY();

	gridX1=x1/distX;	gridY1=y1/distY;
	gridX2=x2/distX;	gridY2=y2/distY;

*/		

	return false;
}


