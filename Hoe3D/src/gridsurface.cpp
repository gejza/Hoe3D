
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "states.h"
#include "gridsurface.h"
#include "texture_system.h"
#include "hoe_texture.h"

struct VecPDT
{
		HoeMath::VECTOR3 pos;
		dword color;
		HoeMath::VECTOR2 tex;
};

// D3DTOP_BLENDTEXTUREALPHA   current textura -- alpha current

//////////////////////////////////////////////////////////
GridSurface::GridSurface() : m_vertices(true)
{
	m_worldpos.Identity();
	m_loaded = false;
	m_sizeX = m_sizeY = 10.f;
	m_wire = false;
}

void GridSurface::Load()
{
	m_loaded = false;
	if (m_heights.isEmpty())
		return;
	int hx=m_heights.getSizeX();
	int hy=m_heights.getSizeY();
	if (!m_vertices.Create(hx*hy, "pdt", hx*hy*sizeof(VecPDT)))
		return;
	VecPDT * pv = (VecPDT*)m_vertices.Lock();

	for (int x=0;x < hx;x++)
		for (int y=0;y < hy;y++)
		{
			const float px = x*m_sizeX/hx-m_sizeX/2;
			const float py = y*m_sizeY/hy-m_sizeY/2;
			const float l = sqrtf((0-px)*(0-px)+(0-py)*(0-py));
			pv->pos = HoeMath::VECTOR3(px,m_heights.getHeightAt(x,y),py);
			pv->tex = HoeMath::VECTOR2(x / (float)(hx-1)*20.f,y / (float)(hy-1)*20.f);

			// compute color
			//if (l > 100.f)
				pv->color = 0xffffffff;
			//else
			//{
			//	byte c = byte(0xff * l/100.f);
			//	pv->color = (c << 8 | c) | 0xffff0000;
			//}
			pv++;
		}
	m_vertices.Unlock();
	if (!m_indices.Create((m_heights.getSizeX()-1)*(m_heights.getSizeY()-1)*6))
		return;
	word * i = m_indices.Lock();
#define POS(xx,yy) ((y+(yy))*hx + (x+(xx)))
	for (int x=0;x < hx-1;x++)
		for (int y=0;y < hy-1;y++)
		{
			*i++ = POS(0,0);
			*i++ = POS(1,0);
			*i++ = POS(1,1);
			*i++ = POS(0,0);
			*i++ = POS(1,1);
			*i++ = POS(0,1);
		}
#undef POS
	m_indices.Unlock();
	tex1 = GetTextureSystem()->GetTexture("trava");
	tex2 = GetTextureSystem()->GetTexture("strom_war3");
	m_loaded = true;
}

void GridSurface::Render()
{
	if (m_loaded)
	{
		// wireframe
		Ref::SetMatrix(m_worldpos);
		GetHoeStates()->SetupMap();
		if (m_wire)
			GetHoeStates()->StartWireframe();
		//m_mat.Setup();
#ifdef _HOE_D3D_
		D3DDevice()->SetTexture(0, tex1->GetTexture());
		D3DDevice()->SetTexture(1, tex2->GetTexture());
		D3DDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		D3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT); 
		D3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE); 
		D3DDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA); 
		D3DDevice()->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
		D3DDevice()->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
		D3DDevice()->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
#if 0
		D3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		D3DDevice()->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		D3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		D3DDevice()->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
#endif

#endif
#ifdef _HOE_OPENGL_
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex1->GetTexture());
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);

		// Here we turn on the COMBINE properties and increase our RGB
		// gamma for the detail texture. 2 seems to work just right.
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,	GL_INTERPOLATE_ARB);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_RGB_ARB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);

		// Bind the detail texture
		glBindTexture(GL_TEXTURE_2D, tex2->GetTexture());
		glDisable(GL_LIGHTING);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		checkgl("multitexture");
#endif

		Ref::DrawStdObject(&m_vertices, &m_indices);
		// wireframe
		if (m_wire)
			GetHoeStates()->EndWireframe();
	}
}

void HOEAPI GridSurface::SetPosCenter(float x, float y, float z)
{
	m_worldpos.Translate(x,z,y);
}

void HOEAPI GridSurface::SetSize(float sizeX, float sizeY)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	if (!m_heights.isEmpty())
		Load();
}

void HOEAPI GridSurface::GenerateHeight(int resX,int resY)
{
	m_heights.generatePlaneMap( resX, resY, 0.f);
	Load();
}

void HOEAPI GridSurface::SetHeightMap(int sizeX,int sizeY, float *f)
{
}

void HOEAPI GridSurface::ShowBrush(bool show)
{
}

void HOEAPI GridSurface::SetBrush(float x, float y, float radius, dword color)
{
}

void HOEAPI GridSurface::MoveHeight(float x, float y, float radius, float value)
{
	int hx=m_heights.getSizeX();
	int hy=m_heights.getSizeY();

	for (int mx=0;mx < hx;mx++)
		for (int my=0;my < hy;my++)
		{
			const float px = mx*m_sizeX/hx-m_sizeX/2;
			const float py = my*m_sizeY/hy-m_sizeY/2;
			const float l = sqrtf((x-px)*(x-px)+(y-py)*(y-py));
			if (l > radius)
				continue;
			m_heights.setHeightAt(mx,my, m_heights.getHeightAt(mx,my) + value * (radius-l)/radius);
		}
	Load();
}

void HOEAPI GridSurface::ShowWireframe(bool show)
{
	m_wire = show;
}

