
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "texture_system.h"
#include "hoe_material.h"
#include "states.h"
#include "material_parser.h"

HoeMaterialColor::HoeMaterialColor()
{
	a = r = g = b = 1.f;
}

HoeMaterialColor::HoeMaterialColor(float _r, float _g, float _b, float _a)
{
	a = _a;r = _r; g = _g; b = _b;
}

///////////////////////////////////////////

HoeMaterial::HoeMaterial()
{
	m_tex = NULL;
	m_bump = NULL;
	m_lightreag = true;
#ifdef _HOE_D3D_
	ZeroMemory( &m_mtrl, sizeof(m_mtrl) );
#endif
	SetColor(~0, HoeMaterialColor(1,1,1,1));
}

inline dword F2DW( float f ) { return *((dword*)&f); }


void HoeMaterial::Setup(dword overcolor)
{
	// material
	

#ifdef _HOE_D3D_
	if (overcolor != 0xffffffff)
	{
		D3DMaterial mat = m_mtrl;
		HoeMaterialColor c; 
		c = overcolor;
		mat.Ambient = c;
		mat.Diffuse = c;
		mat.Specular = c;
		D3DDevice()->SetMaterial( &mat );
		// zapnout svetlo
	}
	else
		D3DDevice()->SetMaterial( &m_mtrl );
#endif
#ifdef _HOE_OPENGL_
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,m_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,m_diffuse);
	//glMaterialfv(GL_FRONT,GL_SPECULAR,m_specular);
	float em[4] = { 0.f,0.f,0.f,1.f };
	glMaterialfv( GL_FRONT, GL_EMISSION, em);
	glMaterialf( GL_FRONT, GL_SHININESS, 0);
#endif

	int tex = 0;
	if (m_tex)
	{
		GetStates()->EnableTexture();
		GetTextureSystem()->SetTexture(tex,m_tex);
#ifdef _HOE_D3D_
		D3DDevice()->SetTextureStageState( tex, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		D3DDevice()->SetTextureStageState( tex, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		D3DDevice()->SetTextureStageState( tex, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
#endif
#ifdef _HOE_OPENGL_
#endif
		tex++;
	}

	// bump
	if (m_bump)
	{
		GetTextureSystem()->SetTexture(tex,m_bump);
#ifdef _HOE_D3D_
		D3DDevice()->SetTextureStageState( tex, D3DTSS_TEXCOORDINDEX, 0 );
		D3DDevice()->SetTextureStageState( tex, D3DTSS_COLOROP,   D3DTOP_BUMPENVMAPLUMINANCE );		
		D3DDevice()->SetTextureStageState( tex, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		D3DDevice()->SetTextureStageState( tex, D3DTSS_COLORARG2, D3DTA_CURRENT );
		// Set the bump mapping Matrix.
		//
		// Note  These calls rely on the following inline shortcut function:
		D3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT00, F2DW(1.0f) );
		D3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT01, F2DW(0.0f) );
		D3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT10, F2DW(0.0f) );
		D3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVMAT11, F2DW(1.0f) );

		D3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVLSCALE, F2DW(1.5f) );
		D3DDevice()->SetTextureStageState( 1, D3DTSS_BUMPENVLOFFSET, F2DW(0.0f) );
#endif
#ifdef _HOE_OPENGL_
#endif
		tex++;
	}

#ifdef _HOE_D3D_
	D3DDevice()->SetTextureStageState( tex, D3DTSS_COLOROP,   D3DTOP_DISABLE );
#endif
#ifdef _HOE_OPENGL_
#endif

	if (tex==0)
		GetStates()->DisableTexture();
	//else
	//	GetStates()->EnableTexture();

	dword c = 0xffffffff;

#ifdef _HOE_D3D_
	D3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	D3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	D3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    D3DDevice()->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x000000b0);
    D3DDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE); 
    D3DDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
#endif
#ifdef _HOE_OPENGL_
	glAlphaFunc( GL_GREATER, 0.6f);// Nastavení alfa testingu
	glEnable(GL_ALPHA_TEST);// Zapne alfa testing
	/*!!!*/
	/*if (!m_lightreag)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
		*/
#endif
	
}

void HoeMaterial::SetColor(int type, const HoeMaterialColor & color)
{
#ifdef _HOE_D3D9_
	if (type & Ambient)
		m_mtrl.Ambient = color;
	if (type & Diffuse)
		m_mtrl.Diffuse = color;
	if (type & Specular)
		m_mtrl.Specular = color;
#endif
#ifdef _HOE_OPENGL_
	if (type & Ambient)
		m_ambient = color;
	if (type & Diffuse)
		m_diffuse = color;
	if (type & Specular)
		m_specular = color;
#endif
}

bool HoeMaterial::LoadFromFile(const char *path)
{
	if (!material_load(path))
	{
		Con_Print("Material file '%s' not found.",path);
		return false;
	}

	mtl_value val;
	while (material_parse(&val) > 0)
	{
		switch (val.type)
		{
		case MTLP_TEXTURE:
			m_tex = GetTextureSystem()->GetTexture(val.text);
			break;
		default:
			break;
		}
	}

	return true;
}


