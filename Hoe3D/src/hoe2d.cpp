
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "texture_system.h"
#include "config.h"
#include "camera.h"
#include "states.h"
#include "hoe_picture.h"
#include "hoe2d.h"
#include "freetype.h"

#ifdef _HOE_D3D_
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

	freetype.Init();

	Con_Print("Hoe 2d system created");
}

Hoe2D::~Hoe2D()
{
	UNSET_SHARED_PTR(hoe2d);
}

void Hoe2D::Begin()
{
    in_progress = true;
	HoeCamera::Setup2DMatrices(m_maxX,m_maxY);
	GetStates()->Setup2D();


}

void Hoe2D::End()
{
	in_progress = false;
}

HoePicture * Hoe2D::CreatePicture(const tchar * name, HoeLog * log)
{
	HoePicture * ret = new HoePicture;
	if (name)
		ret->SetSource(GetTextureSystem()->GetTexture(name, log));
	return ret;
}

///////////////////////////////////////

void Hoe2D::SetRect(const float w,const float h)
{
	m_maxX = w;
	m_maxY = h;
	if (in_progress)
		HoeCamera::Setup2DMatrices(w,h);
}

void Hoe2D::PaintRect(const float l,const float r,const float t,const float b,unsigned long color,bool full)
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
		glVertex2f( l, t);// Levý horní bod
		glVertex2f( r, t);// Pravý horní bod
		glVertex2f( r, b);// Pravý dolní bod
		glVertex2f( l, b);// Levý dolní bod
		glEnd();// Konec kreslení obdélníkù
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

}

void Hoe2D::PaintLine(float x1,float y1,float x2,float y2,unsigned long c)
{
	float r = ((c & 0xff0000) >> 16) / 255.f;
	float g = ((c & 0xff00) >> 8) / 255.f;
	float b = (c & 0xff) / 255.f;
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

void Hoe2D::BltFast(const float l,const float r,const float t,const float b,IHoePicture * pic)
{
	THoeRect src = { 0.0f,0.0f,1.0f,1.0f };
//	reinterpret_cast<HoePicture*>(pic)->GetRect(&src);
	const THoeRect dest = { l,t,r,b };
	Blt(&dest,pic,&src);

}

void Hoe2D::Blt(const THoeRect * dest,IHoePicture * pic,const THoeRect * src)
{
	GetStates()->EnableTexture();
	GetStates()->Setup2DAlphaTest();

	GetTextureSystem()->SetTexture(0,reinterpret_cast<HoePicture *>(pic)->GetSource());
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

void Hoe2D::Blt(const THoeRect * dest,IHoePicture * pic)
{
	const THoeRect src = { 0.0f,0.0f,1.0f,1.0f };
	Blt(dest,pic,&src);
}

void Hoe2D::SetAlpha(bool set)
{
	if (set)
		GetStates()->Setup2DAlphaTest();
	//else
	//	GetHoeStates()->D
}






