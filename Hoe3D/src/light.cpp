
#include "system.h"
#include "ref.h"
#include "shared.h"
#include "utils.h"
#include "hoe_time.h"
#include "hoe3d_math.h"
#include "light.h"

HoeLight::HoeLight()
{
#ifdef _HOE_D3D_
	ZeroMemory( &light, sizeof(light) );
	light.Type = D3DLIGHT_POINT;

	light.Ambient.r = 0;
	light.Ambient.g = 0;
	light.Ambient.b = 0;
	light.Ambient.a = 1.0f;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;
	light.Attenuation0 = 1.f;

	light.Range = 1000;

#endif

	this->next = NULL;
}

void HoeLight::Set(int slot)
{
#ifdef _HOE_D3D_
	D3DDevice()->SetLight( slot, &light );
	D3DDevice()->LightEnable( slot, TRUE);
	D3DDevice()->SetRenderState( D3DRS_AMBIENT, 0x00000000 );

#endif
#ifdef _HOE_OPENGL_
	GLfloat LightAmbient[]	=	{ 0.0f, 0.0f, 0.0f, 0.0f };		//svetlo ktore osvetluje zo vsetkych stran rovnako (okolite svetlo)

	glLightModelfv ( GL_LIGHT_MODEL_AMBIENT, LightAmbient );	// nastavenie okoliteho (Ambient) svetla
	glLightfv (GL_LIGHT0+slot,GL_DIFFUSE, color.m );			// nastavenie difuzneho svetla
	glLightfv ( GL_LIGHT0+slot,GL_POSITION, pos.m  );		// pozicia difuzneho svetla
	glEnable  ( GL_LIGHT0+slot );									// aktivovanie svetla 0
#endif
}

void HoeLight::SetPosition(const float x, const float y, const float z)
{
#ifdef _HOE_D3D_
	light.Position.x = x; light.Position.y = y; light.Position.z = z;
#endif
#ifdef _HOE_OPENGL_
	pos.x = x; pos.y = y; pos.z = z; pos.w = 1.f;
#endif
}

void HoeLight::SetColor(const float r, const float g, const float b)
{
#ifdef _HOE_D3D_
	light.Diffuse.r = r; light.Diffuse.g = g; light.Diffuse.b = b;light.Diffuse.a = 1.f;
#endif
#ifdef _HOE_OPENGL_
	color.x = r; color.y = g; color.z = b;
#endif
}
