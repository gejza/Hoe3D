
#include "StdAfx.h"
#include "shared.h"
#include "2d.h"
#include "../include/hoeinterfaces.h"
#include "scene_base.h"
#include "hoe.h"

HoeBaseScene::HoeBaseScene()
{
	m_2dcb = NULL;
}

void HoeBaseScene::Set2DCallback(XHoe2DCallback * cb)
{
	m_2dcb = cb;
}

int HoeBaseScene::RegisterObject(XHoeObject * obj, unsigned long flags)
{
	return -1;
}

void HoeBaseScene::UnregisterObject(XHoeObject * obj)
{
}

IHoeLight * HoeBaseScene::CreateLight(bool diRECT)
{
	return NULL;
}

IHoeParticleEmitor * HoeBaseScene::CreateParticleEmitor()
{
	return NULL;
}

void HoeBaseScene::Process(const double dtime)
{
}

void HoeBaseScene::Render(TRenderParameters * rp)
{
}

void HoeBaseScene::Paint2D()
{
	if (m_2dcb) m_2dcb->_Paint(Get2D());
}

IHoeScenePhysics * HoeBaseScene::GetScenePhysics()
{
	return NULL;
}

IHoeSceneEnv * HoeBaseScene::GetSceneEnv()
{
	return NULL;
}

IHoeCamera * HoeBaseScene::GetCamera()
{
	return NULL;
}

bool HoeBaseScene::IsActive()
{
	return (GetHoe()->GetActiveScene() == this);
}

void HoeBaseScene::Delete()
{
	delete this;
}

