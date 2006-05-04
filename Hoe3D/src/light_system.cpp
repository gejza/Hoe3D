

#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "hoe3d_math.h"
#include "light_system.h"
#include "hoe_time.h"

LightSystem::LightSystem()
{
	m_lights = NULL;
}

LightSystem::~LightSystem()
{
}

void LightSystem::AddLight(HoeLight *l)
{
	HoeLight ** ll = &m_lights;
	while (*ll)
	{
		ll = &((*ll)->next);
	}
	*ll = l;
}

void LightSystem::Setup()
{
	int i=0;
	HoeLight * l = m_lights;
	while (l!=NULL)
	{
		l->Set(i);
		i++;
		l = l->next;
	}	
#ifdef _HOE_D3D_
	if (i)
		D3DDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	else
		D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
#endif
#ifdef _HOE_OPENGL_
	if (i)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
#endif
}
