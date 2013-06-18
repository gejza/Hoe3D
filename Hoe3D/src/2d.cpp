
#include "StdAfx.h"
#include "ref.h"
#include "shared.h"
#include "utils.h"
#include "texture_system.h"
#include "config.h"
#include "camera.h"
#include "states.h"
#include "hoe_picture.h"
#include "2d.h"

#ifdef _HOE_D3DA_
struct HOE2D_VERTEXCT
{
	FLOAT x, y, z; // The transformed position for the vertex.
	DWORD color;        // The vertex color.
	FLOAT       tu, tv;   // The texture coordinates
};

struct HOE2D_VERTEXC
{
	FLOAT x, y, z; // The transformed position for the vertex.
	DWORD color;        // The vertex color.
};

#endif // _HOE_D3D9_


Hoe2D::Hoe2D()
{
	SET_SHARED_PTR(hoe2d);

	in_progress = false;
	m_maxX = 0.0f;
	m_maxY = 0.0f;
	m_multX = 1.f;
	m_multY = 1.f;

	Con_Print(T("Hoe 2d system created"));
}

Hoe2D::~Hoe2D()
{
	UNSET_SHARED_PTR(hoe2d);
}

void Hoe2D::Begin()
{
    in_progress = true;
#ifndef HOE2D
	HoeCamera::Setup2DMatrices(m_maxX,m_maxY);
	GetStates()->Setup2D();
#endif
}

void Hoe2D::End()
{
	in_progress = false;
}

///////////////////////////////////////

void Hoe2D::SetRect(const vfloat w,const vfloat h)
{
	m_maxX = w;
	m_maxY = h;
#ifndef HOE2D
	if (in_progress) //todo udelat kontrolu na nulove hodnoty
		HoeCamera::Setup2DMatrices(w,h);
#endif
	if (w > 0 && h > 0)
	{
		m_multX = (vfloat)GetRef()->GetWidth() / w;
		m_multY = (vfloat)GetRef()->GetHeight() / h;
	}
	else
	{
		m_multX = 1.f;
		m_multY = 1.f;
	}
}

void Hoe2D::PaintRect(const THoeRect* dest,unsigned long color,bool full)
{
	GetStates()->DisableTexture();
	if ((color & 0xff000000) == 0xff000000)
		GetStates()->Setup2D();
	else
		GetStates()->Setup2DAlpha();

#ifdef _HOE_OPENGL_
	const float ca = ((color & 0xff000000) >> 24) * (1/255.f);
	const float cr = ((color & 0xff0000) >> 16) * (1/255.f);
	const float cg = ((color & 0xff00) >> 8) * (1/255.f);
	const float cb = (color & 0xff) * (1/255.f);
	glColor4f(cr,cg,cb,ca);

	if (full)
	{
        glBegin(GL_QUADS);// Zaèátek kreslení obdélníkù
		glVertex2f( dest->left, dest->top);// Levý horní bod
		glVertex2f( dest->right, dest->top);// Pravý horní bod
		glVertex2f( dest->right, dest->bottom);// Pravý dolní bod
		glVertex2f( dest->left, dest->bottom);// Levý dolní bod
		glEnd();// Konec kreslení obdélníkù
	}
	else
	{
		glBegin(GL_LINES);
		glVertex2d( dest->left, dest->top);glVertex2d( dest->right, dest->top);
		glVertex2d( dest->right, dest->top);glVertex2d( dest->right, dest->bottom);
		glVertex2d( dest->right, dest->bottom);glVertex2d( dest->left, dest->bottom);
		glVertex2d( dest->left, dest->bottom);glVertex2d( dest->left, dest->top);
		glEnd();
	}
#endif // _HOE_OPENGL_
#ifdef _HOE_D3D_
	D3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	D3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	HOE2D_VERTEXC pv[] = 
	{
		{ dest->left , dest->top , 0.0f , color },
		{ dest->right , dest->top , 0.0f , color },
		{ dest->right, dest->bottom , 0.0f , color },
		{ dest->left, dest->bottom , 0.0f , color }
	};
	D3DDevice()->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	D3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,pv,sizeof(HOE2D_VERTEXC));
#endif // _HOE_D3D9_
#ifdef _HOE_D3DM_
	HRESULT hRes;
	IDirect3DMobileSurface* pbb;
	hRes = D3DDevice()->GetBackBuffer(0, D3DMBACKBUFFER_TYPE_MONO, &pbb);
	checkres(hRes, "IDirect3dMobileDevice::GetBackBuffer");
	RECT rect;
	const vfloat h = vfloat(GetRef()->GetWidth()) / this->m_maxX;
	const vfloat v = vfloat(GetRef()->GetHeight()) / this->m_maxY;
	rect.left = int(l * h);
	rect.right = int(r * h);
	rect.top = int(t * v);
	rect.bottom = int(b * v);
	hRes = D3DDevice()->ColorFill(pbb, &rect, color);
	checkres(hRes, "IDirect3dMobileDevice::ColorFill");
	pbb->Release();
#endif // _HOE_D3D9_
#ifdef _HOE_DD_
	DDBLTFX fx;
	memset(&fx,0, sizeof(fx));
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = HoeRes::CompileColor(HOE_R5G6B5,color);
	RECT rect;
	rect.top = dest->top;
	rect.left = dest->left;
	rect.bottom = dest->bottom;
	rect.right = dest->right;
	HRESULT hRes = GetRef()->GetSurface()->Blt(&rect, NULL, NULL, DDBLT_COLORFILL,&fx);
	checkres(hRes, "Blt");
#endif

}

void Hoe2D::PaintLine(vfloat x1,vfloat y1,vfloat x2,vfloat y2,unsigned long c)
{
	vfloat r = ((c & 0xff0000) >> 16) / 255.f;
	vfloat g = ((c & 0xff00) >> 8) / 255.f;
	vfloat b = (c & 0xff) / 255.f;
#ifdef _HOE_OPENGL_
	glColor3f(r,g,b);
	glLineWidth(3.0f);// Šíøka èáry 1.0f
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);// Nastavení antialiasingu linek

	glBegin(GL_LINES);
	glVertex2d( x1, y1);// Levý horní bod
	glVertex2d( x2, y2);// Pravý dolní bod
	glEnd();
	//glEnable(GL_TEXTURE_2D);// Zapne mapování textur

#endif // _HOE_OPENGL_
#ifdef _HOE_D3D_
	D3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	D3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	HOE2D_VERTEXC pv[] = 
	{
		{ x1 , y1 , 0.0f , c },
		{ x2 , y2 , 0.0f , c },
	};
	D3DDevice()->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	D3DDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,pv,sizeof(HOE2D_VERTEXC));
#endif // _HOE_D3D9_
}

void Hoe2D::Blt(IHoePicture * pic, vfloat x, vfloat y, int rx)
{
	HoePicture *p = dynamic_cast<HoePicture *>(pic);
	THoeRect dest = { x,y, -1, -1 };
	THoeRect src;
	const vfloat sx = p->m_surf.GetWidth() / p->m_rX;
	const vfloat sy = p->m_surf.GetHeight() / p->m_rY;
	int ry = rx / p->m_rX;
	rx = rx % p->m_rX;
	src.left = rx * sx;
	src.right = src.left + sx;
	src.top = ry * sy;
	src.bottom = src.top + sy;
	Blt(pic, &dest, &src);
}

void Hoe2D::Blt(IHoePicture * pic,const THoeRect * dest,const THoeRect * src)
{
	// TODO
	/*
	// method?
	if (m_maxX > 0)
	{
		THoeRect dest2;
		MultRect(*dest, &dest2);
		::GetRef()->Blt(dynamic_cast<HoePicture *>(pic)->m_surf, &dest2, src, 0);
	}
	else
		::GetRef()->Blt(dynamic_cast<HoePicture *>(pic)->m_surf, dest, src, 0);
	*/
#ifndef HOE2D
	//TODO GetTextureSystem()->SetTexture(0,(reinterpret_cast<HoePicture *>(pic))->m_surf);
#endif
#ifdef _HOE_OPENGL_
	glColor4f(1.f,1.f,1.f,1.f);
	glBegin(GL_QUADS);// Zaèátek kreslení obdélníkù
	glTexCoord2f(src->left , src->top );glVertex2f( dest->left , dest->top );// Levý horní bod
	glTexCoord2f(src->right, src->top);glVertex2f( dest->right , dest->top);// Pravý horní bod
	glTexCoord2f(src->right , src->bottom);glVertex2f( dest->right, dest->bottom );// Pravý dolní bod
	glTexCoord2f(src->left, src->bottom);glVertex2f( dest->left, dest->bottom);// Levý dolní bod
	glEnd();// Konec kreslení obdélníkù
#endif // _HOE_OPENGL_
#ifdef _HOE_D3D_
	//D3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
 //   D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
 //   D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
 //   //D3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	HOE2D_VERTEXCT pv[] = 
	{
		{ dest->left , dest->top , 0.0f , 0xffffffff, src->left , src->top},
		{ dest->right , dest->top , 0.0f , 0xffffffff, src->right, src->top},
		{ dest->right, dest->bottom , 0.0f , 0xffffffff, src->right , src->bottom},
		{ dest->left, dest->bottom , 0.0f , 0xffffffff, src->left, src->bottom}
	};
	D3DDevice()->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	D3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,pv,sizeof(HOE2D_VERTEXCT));
#endif // _HOE_D3D9_

}

void Hoe2D::SetAlpha(bool set)
{
	if (set)
		GetStates()->Setup2DAlphaTest();
	//else
	//	GetHoeStates()->D
}






