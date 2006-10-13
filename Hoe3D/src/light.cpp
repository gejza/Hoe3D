
#include "StdAfx.h"
#include "ref.h"
#include "shared.h"
#include "utils.h"
#include "hoe_time.h"
#include <hoe_math.h>
#include "light.h"

HoeLight::HoeLight(bool diRECT)
{
	m_diRECT = diRECT;
#ifdef _HOE_D3D_
	ZeroMemory( &light, sizeof(light) );
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Ambient.r = 0;
	light.Ambient.g = 0;
	light.Ambient.b = 0;
	light.Ambient.a = 1.0f;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;
	light.Attenuation0 = 1.f;

	float t = SysFloatTime();
	HoeMath::Vector3 vec(sinf(t),-1,cosf(t));
	vec.Normalize();
	light.Direction = VECToD3D(vec);
	light.Range = 50;

#endif

	this->next = NULL;
}

void HoeLight::Set(int slot)
{
#ifdef _HOE_D3D_
	float t = SysFloatTime();
	HoeMath::Vector3 vec(sinf(t),-1,cosf(t));
	vec.Normalize();
	light.Direction = VECToD3D(vec);

	D3DDevice()->SetLight( slot, &light );
	D3DDevice()->LightEnable( slot, TRUE);
	D3DDevice()->SetRenderState( D3DRS_AMBIENT, 0x00000000 );

#endif
#ifdef _HOE_OPENGL_
	GLfloat LightAmbient[]	=	{ 0.0f, 0.0f, 0.0f, 0.0f };		//svetlo ktore osvetluje zo vsetkych stran rovnako (okolite svetlo)

	glLightModelfv ( GL_LIGHT_MODEL_AMBIENT, LightAmbient );	// nastavenie okoliteho (Ambient) svetla
	glLightfv ( GL_LIGHT0+slot, GL_AMBIENT, LightAmbient );	// nastavenie 
	glLightfv (GL_LIGHT0+slot,GL_DIFFUSE, color.m );			// nastavenie difuzneho svetla
	glLightfv ( GL_LIGHT0+slot,GL_POSITION, pos.m  );		// pozicia difuzneho svetla
	glEnable  ( GL_LIGHT0+slot );									// aktivovanie svetla 0
#endif
}

void HoeLight::SetPosition(const HoeMath::Vector3 &a)
{
	HoeMath::Vector3 p = a;
	if (m_diRECT)
	{
		p.Normalize();
#ifdef _HOE_D3D_
		light.Direction = VECToD3D(p);
	}
	else
	{
		light.Position = VECToD3D(p);
#endif
	}
#ifdef _HOE_OPENGL_
	pos.x = p.x; pos.y = p.y; pos.z = p.z; pos.w = m_diRECT ? 0.f:1.f;
#endif
}

void HoeLight::SetColor(const float r, const float g, const float b)
{
#ifdef _HOE_D3D_
	light.Diffuse.r = r; light.Diffuse.g = g; light.Diffuse.b = b;light.Diffuse.a = 1.f;
#endif
#ifdef _HOE_OPENGL_
	color.x = r; color.y = g; color.z = b; color.w = 1.f;
#endif
}

const HoeMath::Vector4 HoeLight::GetPosition() const
{
#ifdef _HOE_D3D_
	return HoeMath::Vector4((float*)&light.Position);
#endif
#ifdef _HOE_OPENGL_
	return this->pos;
#endif
}

const HoeMath::Vector4 HoeLight::GetColor() const
{
#ifdef _HOE_D3D_
	return HoeMath::Vector4((float*)&light.Diffuse);
#endif
#ifdef _HOE_OPENGL_
	return this->color;
#endif

}

