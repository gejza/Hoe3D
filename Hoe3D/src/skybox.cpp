
#include "StdAfx.h"
#include "utils.h"
#include "ref.h"
#include "../include/hoeinterfaces.h"
#include "shared.h"
#include "resmgr.h"
#include "hoe_format.h"
#include <hoe_math.h>
#include "hmap_file.h"
#include "hoe_texture.h"
#include "texture_system.h"
#include "states.h"
#include "skybox.h"

bool SkyBox::Load( HoeFile *file)
{
	FileMap::MapSkybox skybox;
	if (!file->Read(&skybox,sizeof(FileMap::MapSkybox)))
		return false;

	// left
	m_sides[Left] = LoadSide(file, skybox.left);
	m_sides[Right] = LoadSide(file, skybox.right);
	m_sides[Top] = LoadSide(file, skybox.top);
	m_sides[Bottom] = LoadSide(file, skybox.bottom);
	m_sides[Front] = LoadSide(file, skybox.front);
	m_sides[Back] = LoadSide(file, skybox.back);

	return true;
}

HoeTexture * SkyBox::LoadSide(HoeFile *file, int size)
{
	char name[1024];
	if (size == 0)
		return NULL;
	file->Read(name,size);
	name[size] = '\0';
	return GetTextureSystem()->GetTexture(name);
}

const float p[] = { +100.0f, +100.0f, +100.0f, 1.0f, 0.0f,
						-100.0f, +100.0f, +100.0f,0.0f, 0.0f
						-100.0f, -100.0f, +100.0f,0.0f, 1.0f
						+100.0f, -100.0f, +100.0f,1.0f, 1.0f};

void SkyBox::Render(HoeCamera * cam)
{

	GetHoeStates()->SetupSkybox();

#ifdef _HOE_D3D9_

	if (this->m_sides[Back])
	{
		m_sides[Back]->Set();	// Bind the FRONT texture of the sky map to the FRONT side of the box

		D3DDevice()->SetFVF(D3DFVF_XYZ| D3DFVF_TEX1);
		if (D3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,1,p,sizeof(float) * 5))
			exit(0);
	}
#endif

#ifdef _HOE_OPENGL_

	if (this->m_sides[Front])
	{
		m_sides[Front]->Set();	// Bind the FRONT texture of the sky map to the FRONT side of the box

		// Start drawing the side as a QUAD
		glBegin(GL_TRIANGLE_FAN);	
			// Assign the texture coordinates and vertices for the FRONT Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f( -100.0f, +100.0f, -100.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( +100.0f, +100.0f, -100.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( +100.0f, -100.0f, -100.0f); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f( -100.0f, -100.0f, -100.0f);
		glEnd();
	}

	if (this->m_sides[Top])
	{
		m_sides[Top]->Set();	// Bind the LEFT texture of the sky map to the LEFT side of the box
		
		// Start drawing the side as a QUAD
		glBegin(GL_TRIANGLE_FAN);		
			
			glTexCoord2f(1.0f, 1.0f); glVertex3f( +100.0f, +100.0f, +100.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( -100.0f, +100.0f, +100.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( -100.0f, +100.0f, -100.0f); 
			glTexCoord2f(1.0f, 0.0f); glVertex3f( +100.0f, +100.0f, -100.0f);
	
		glEnd();
	}

	if (this->m_sides[Right])
	{
		m_sides[Right]->Set();

		// Start drawing the side as a QUAD
		glBegin(GL_TRIANGLE_FAN);		
			// Assign the texture coordinates and vertices for the LEFT Side
			glTexCoord2f(0.0f, 0.0f); glVertex3f( +100.0f, +100.0f, +100.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( +100.0f, +100.0f, -100.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( +100.0f, -100.0f, -100.0f); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f( +100.0f, -100.0f, +100.0f);
		glEnd();
	}

	if (this->m_sides[Left])
	{
		m_sides[Left]->Set();

		// Start drawing the side as a QUAD
		glBegin(GL_TRIANGLE_FAN);		
			// Assign the texture coordinates and vertices for the LEFT Side
			glTexCoord2f(0.0f, 0.0f); glVertex3f( -100.0f, +100.0f, -100.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( -100.0f, +100.0f, +100.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( -100.0f, -100.0f, +100.0f); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f( -100.0f, -100.0f, -100.0f);
		glEnd();
	}

	if (this->m_sides[Back])
	{
		m_sides[Back]->Set();	// Bind the FRONT texture of the sky map to the FRONT side of the box

		// Start drawing the side as a QUAD
		glBegin(GL_TRIANGLE_FAN);	
			// Assign the texture coordinates and vertices for the FRONT Side
			glTexCoord2f(1.0f, 0.0f); glVertex3f( +100.0f, +100.0f, +100.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( -100.0f, +100.0f, +100.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( -100.0f, -100.0f, +100.0f); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f( +100.0f, -100.0f, +100.0f);
		glEnd();
	}

#endif // _HOE_OPENGL_
}

