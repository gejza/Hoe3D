
#include "StdAfx.h"
#include "ref.h"
#include "utils.h"
#include "camera.h"
#include "texture_system.h"
//#include "2d.h"
#include "hoe_format.h"
#include "hoe_texture.h"
#include "hoe_picture.h"


HoePicture::HoePicture()
{
	m_flags = 0;
	m_tex = NULL;
	//texture = t;
	//alpha = false;
	//alpharef = 0;
	//sx = sy = 1;
}

void HoePicture::CreateRegions(uint x,uint y)
{
	this->m_rX = x;
	this->m_rY = y;
	m_flags = m_flags | HOE_PICTURE_ARR;
}

void HoePicture::SetActiveRegion(uint u)
{
	m_active = u;
}

uint HoePicture::GetNumRegions() const
{
	return this->m_rX * this->m_rY;
}

void HoePicture::GetRect(THoeRect * rect) const
{
	const float res_x = 1.f / m_rX;
	const float res_y = 1.f / m_rY;

	int radek = m_active / m_rX;
	int sloupec = m_active % m_rX;

	rect->left = sloupec*res_x;
	rect->right = (sloupec+1)*res_x;
	rect->top = radek*res_y;
	rect->bottom = (radek+1)*res_y;
}

unsigned int HoePicture::GetWidth() const
{
	return m_tex->GetWidth();
}

unsigned int HoePicture::GetHeight() const
{
	return m_tex->GetHeight();
}

/*int HoePicture::GetNumRect()
{
	return sx * sy;
}

void HoePicture::Alpha(bool a,DWORD ref)
{
	//alpha = a;
	//alpharef = ref;
}

/*struct FontVertex
{
    float x, y, z, rhw; // The transformed position for the vertex.
    DWORD color;        // The vertex color.
	float tu, tv;
};

inline void InitVertex(FontVertex * pVert,float x,float y,float tu,float tv)
{
	pVert->x = x;
	pVert->y = y;
	pVert->z = 0.9f;
	pVert->rhw = 1.0f;
	pVert->color = 0x80ffffff;
	pVert->tv = tv;
	pVert->tu = tu;
}*/
/*#ifdef _HOE_OPENGL_

void HoePicture::Paint(float x1,float x2,float y1,float y2,int mer)
{
	g_state->ApplyState(c_state::HSB_2D);
	g_camera->Set2D(1.0f,1.0f);

	glLoadIdentity();
	tx::SetTexture(this->texture);
	glDisable(GL_DEPTH_TEST);// 
	glColor3f(1.f,1.f,1.f);// Zelená barva
	//glTranslatef(0.0f,0.0f,-2.0f);// Pøesun o jednotku do obrazovky
	//glRotatef(roll*360,0.0f,0.0f,1.0f);// Rotace na ose z

	glBegin(GL_QUADS);// Zaèátek kreslení obdélníkù
		
	//glTexCoord2f(((p%8)*0.125), ((p/8)*0.125)); glVertex2f(-1.1f,-1.1f);//, 0.0f);
	//glTexCoord2f(((p%8+1)*0.125), ((p/8)*0.125)); glVertex3f( 1.1f,-1.1f, 0.0f);
	//glTexCoord2f(((p%8+1)*0.125), ((p/8+1)*0.125)); glVertex3f( 1.1f, 1.1f, 0.0f);
	//glTexCoord2f(((p%8)*0.125), ((p/8+1)*0.125)); glVertex3f(-1.1f, 1.1f, 0.0f);

	const float res_x = 1.f / sx;
	const float res_y = 1.f / sy;

	int radek = mer / sx;
	int sloupec = mer % sx;

	glTexCoord2f(sloupec*res_x, radek*res_y); glVertex2f(	x1, y1);//, 0.0f);
	glTexCoord2f((sloupec+1)*res_x, radek*res_y); glVertex2f( x2, y1);
	glTexCoord2f((sloupec+1)*res_x, (radek+1)*res_y); glVertex2f( x2, y2);
	glTexCoord2f(sloupec*res_x, (radek+1)*res_y); glVertex2f( x1, y2);
	glEnd();// Konec kreslení

	

	glEnable(GL_DEPTH_TEST);// 
}

#endif // _HOE_OPENGL_

}; // namespace HOE2D*/



