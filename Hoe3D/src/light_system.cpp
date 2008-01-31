

#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "light_system.h"
#include "hoe_time.h"
#include "config.h"

LightSystem::LightSystem()
{
	m_lights = NULL;
	m_active = 0;
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
	m_active = 0;
	while (l!=NULL)
	{
		if (i < GetConfig()->GetMaxLights())
		{
			l->Set(i);
			i++;
		}
		m_index[m_active++] = l;
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

uint LightSystem::GetNumActiveLights() const
{
	return m_active;
}

const HoeLight * LightSystem::GetActiveLight(uint i) const
{
	assert(i >= 0 && i < m_active);
	return m_index[i];
}
