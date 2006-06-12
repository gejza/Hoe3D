
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "states.h"
#include "gridsurface.h"
#include "texture_system.h"

struct VecPDT
{
		HoeMath::VECTOR3 pos;
		dword color;
		HoeMath::VECTOR2 tex;
};

//////////////////////////////////////////////////////////
GridSurface::GridSurface() : m_vertices(true)
{
	m_worldpos.Identity();
	m_loaded = false;
	m_sizeX = m_sizeY = 10.f;
	m_mat.SetTexture(GetTextureSystem()->GetTexture("trava"));
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
			pv->tex = HoeMath::VECTOR2(x / (float)(hx-1)*5.f,y / (float)(hy-1)*5.f);
			// compute color
			if (l > 100.f)
				pv->color = 0xffffffff;
			else
			{
				byte c = byte(0xff * l/100.f);
				pv->color = (c << 8 | c) | 0xffff0000;
			}
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

	m_loaded = true;
}

void GridSurface::Render()
{
	if (m_loaded)
	{
		// wireframe
		Ref::SetMatrix(m_worldpos);
		GetHoeStates()->SetupMap();
		//GetHoeStates()->StartWireframe();
		m_mat.Setup();
		Ref::DrawStdObject(&m_vertices, &m_indices);
		// wireframe
		//GetHoeStates()->EndWireframe();
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


