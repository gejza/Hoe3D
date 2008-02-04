
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
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
        glBegin(GL_QUADS);// Za��tek kreslen� obd�ln�k�
		glVertex2f( l, t);// Lev� horn� bod
		glVertex2f( r, t);// Prav� horn� bod
		glVertex2f( r, b);// Prav� doln� bod
		glVertex2f( l, b);// Lev� doln� bod
		glEnd();// Konec kreslen� obd�ln�k�
	}
	else
	{
		glBegin(GL_LINES);
		glVertex2d( l, t);glVertex2d( r, t);
		glVertex2d( r, t);glVertex2d( r, b);
		glVertex2d( r, b);glVertex2d( l, b);
		glVertex2d( l, b);glVertex2d( l, t);
		glEnd();
	}
#endif // _HOE_OPENGL_
#ifdef _HOE_D3D_
	D3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	D3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	HOE2D_VERTEXC pv[] = 
	{
		{ l , t , 0.0f , color },
		{ r , t , 0.0f , color },
		{ r, b , 0.0f , color },
		{ l, b , 0.0f , color }
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
	fx.dwFillColor = color;
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
	glLineWidth(3.0f);// ���ka ��ry 1.0f
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);// Nastaven� antialiasingu linek

	glBegin(GL_LINES);
	glVertex2d( x1, y1);// Lev� horn� bod
	glVertex2d( x2, y2);// Prav� doln� bod
	glEnd();
	//glEnable(GL_TEXTURE_2D);// Zapne mapov�n� textur

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

void Hoe2D::Blt(IHoePicture * pic,const THoeRect * dest,const THoeRect * src)
{
	// method?

	::GetRef()->Blt(dynamic_cast<HoePicture *>(pic)->m_surf, dest, src, 0);
	//GetStates()->EnableTexture();
	//GetStates()->Setup2DAlphaTest();
#ifndef HOE2D
	GetTextureSystem()->SetTexture(0,reinterpret_cast<HoePicture *>(pic)->GetSource());
#endif
#ifdef _HOE_OPENGL_
	glColor4f(1.f,1.f,1.f,1.f);
	glBegin(GL_QUADS);// Za��tek kreslen� obd�ln�k�
	glTexCoord2f(src->left , src->top );glVertex2f( dest->left , dest->top );// Lev� horn� bod
	glTexCoord2f(src->right, src->top);glVertex2f( dest->right , dest->top);// Prav� horn� bod
	glTexCoord2f(src->right , src->bottom);glVertex2f( dest->right, dest->bottom );// Prav� doln� bod
	glTexCoord2f(src->left, src->bottom);glVertex2f( dest->left, dest->bottom);// Lev� doln� bod
	glEnd();// Konec kreslen� obd�ln�k�
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






