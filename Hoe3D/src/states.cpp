
#include "StdAfx.h"
#include "shared.h"
#include "ref.h"
#include "utils.h"
#include "config.h"
#include "states.h"
#include "light_system.h"
#include "hoe_material.h"

//////////////////////////////////////////////////////

HoeStates::HoeStates()
{
	SET_SHARED_PTR(states);
	Con_Print("States system create");
	wireframe = false;
	GetExec()->Register("wireframe",HoeStates::c_setwireframe,NULL);

	// alphatest
	alphatest = false;
}

HoeStates::~HoeStates()
{
	UNSET_SHARED_PTR(states);
}

int HoeStates::c_setwireframe(int argc, const char * argv[], void * param)
{
	if (argc != 2)
		return 1;

	GetStates()->wireframe = atoi(argv[1]) != 0;
	return 0; 
}

void HoeStates::Reset()
{
#ifdef _HOE_D3D_
	/*float fFogStart = 50.f;
	float fFogEnd = 120.f;
    D3DDevice()->SetRenderState(D3DRS_FOGCOLOR, 0xaaaaaa);
    D3DDevice()->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);//DESTALPHA);
    D3DDevice()->SetRenderState(D3DRS_FOGSTART, *((DWORD*) (&fFogStart)));
	D3DDevice()->SetRenderState(D3DRS_FOGEND, *((DWORD*) (&fFogEnd)));
    D3DDevice()->SetRenderState(D3DRS_FOGENABLE, TRUE);*/
	D3DDevice()->SetRenderState(D3DRS_FILLMODE, wireframe ? D3DFILL_WIREFRAME:D3DFILL_SOLID);
#endif
#ifdef _HOE_D3D_

#endif
#ifdef _HOE_OPENGL_
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
#endif // _HOE_OPENGL_


}

void HoeStates::Setup2D()
{
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	D3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE ,FALSE);
	D3DDevice()->SetRenderState( D3DRS_ALPHATESTENABLE ,FALSE);
   	D3DDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
	D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
	D3DDevice()->SetVertexShader( NULL);
#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);// Funkce blendingu pro prùsvitnost založená na hodnotì alfa
	glDisable(GL_BLEND);// vypne blending
	glDisable(GL_ALPHA_TEST);// vypne
	if (GetRef()->ext.ARB_vertex_program)
		glDisable(GL_VERTEX_PROGRAM_ARB);
#endif // _HOE_OPENGL_
}

void HoeStates::Setup2DAlpha()
{
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	D3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE ,TRUE);
	D3DDevice()->SetRenderState( D3DRS_ALPHATESTENABLE ,FALSE);

    D3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    D3DDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	D3DDevice()->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
	if (this->texture)
        D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	else
		D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);

    //D3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);// Funkce blendingu pro prùsvitnost založená na hodnotì alfa
	glEnable(GL_BLEND);// Zapne blending
#endif // _HOE_OPENGL_
}

void HoeStates::Setup2DAlphaTest()
{
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	D3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE ,TRUE);
	D3DDevice()->SetRenderState( D3DRS_ALPHATESTENABLE ,FALSE);

    D3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    D3DDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	D3DDevice()->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
	if (this->texture)
        D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	else
		D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);

    //D3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);// vypne
#endif // _HOE_OPENGL_
}

void HoeStates::SetupModel()
{
#ifdef _HOE_D3D9_

	D3DDevice()->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
   	D3DDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );

	D3DDevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	D3DDevice()->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);

#endif // _HOE_D3D9_
#ifdef _HOE_D3D8_
	D3DDevice()->LightEnable( 0, TRUE);
	D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

	D3DDevice()->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
   	D3DDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );

	D3DDevice()->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	D3DDevice()->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);
#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	glEnable(GL_DEPTH_TEST);
	glColor4f(1,1,1,1);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
#endif // _HOE_OPENGL_

}

void HoeStates::SetupMap()
{
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE ,FALSE);
	D3DDevice()->SetRenderState( D3DRS_ALPHATESTENABLE ,FALSE);

#ifdef _HOE_D3D9_
	D3DDevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	D3DDevice()->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
#endif
	//D3DDevice()->SetRenderState( D3DRS_CULLMODE,   D3DCULL_NONE );
   	D3DDevice()->SetRenderState( D3DRS_ZENABLE, TRUE ); 
	//D3DDevice()->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	D3DDevice()->SetTexture(0,NULL);
	D3DMaterial mat;
	memset(&mat,0,sizeof(mat));
	HoeMaterialColor c; 
	c = (dword)0xffffffff;
	mat.Ambient = c;
	mat.Diffuse = c;
	mat.Specular = c;
	D3DDevice()->SetMaterial( &mat );
#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
#endif // _HOE_OPENGL_

}

void HoeStates::SetupSkybox()
{

#ifdef _HOE_D3D9_
	//D3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE ,FALSE);
	D3DDevice()->SetRenderState( D3DRS_ALPHATESTENABLE ,FALSE);

	D3DDevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	D3DDevice()->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	D3DDevice()->SetRenderState( D3DRS_CULLMODE,   D3DCULL_NONE );
   	D3DDevice()->SetRenderState( D3DRS_ZENABLE, TRUE ); 
	D3DDevice()->SetRenderState(D3DRS_LIGHTING,FALSE);
	//D3DDevice()->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	//D3DDevice()->SetTextureStageState(0,
#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_

	/*if (sh)
	{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
	}
	else 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}*/

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
#endif // _HOE_OPENGL_

}

void HoeStates::SetupCursor()
{
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	D3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE ,TRUE);
	D3DDevice()->SetRenderState( D3DRS_ALPHATESTENABLE ,TRUE);

    D3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    D3DDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);

	D3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    //D3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);// Funkce blendingu pro prùsvitnost založená na hodnotì alfa
	glEnable(GL_BLEND);// Zapne blending
#endif // _HOE_OPENGL_

}

void HoeStates::SetupFont()
{
	EnableTexture();
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	D3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE ,TRUE);
	D3DDevice()->SetRenderState( D3DRS_ALPHATESTENABLE ,TRUE);

    D3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    D3DDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	D3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_CURRENT);
	D3DDevice()->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);

	D3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	D3DDevice()->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);

#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);// Funkce blendingu pro prùsvitnost založená na hodnotì alfa
	glEnable(GL_BLEND);// Zapne blending
#endif // _HOE_OPENGL_

}

void HoeStates::SetupFontBack()
{
	DisableTexture();
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	D3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE ,FALSE);
	D3DDevice()->SetRenderState( D3DRS_ALPHATESTENABLE ,FALSE);

	D3DDevice()->SetTexture(0,NULL);

#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);// Zapne blending
#endif // _HOE_OPENGL_
}

void HoeStates::EnableTexture()
{
	//if (this->texture == true)
	//	return;

#ifdef _HOE_D3D_
	//D3DDevice()->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT );
	D3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	glEnable(GL_TEXTURE_2D);
#endif // _HOE_OPENGL_

	this->texture = true;
}

void HoeStates::DisableTexture()
{
	//if (this->texture == false)
	//	return;

#ifdef _HOE_D3D_
	D3DDevice()->SetTexture(0,NULL);
	D3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	D3DDevice()->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD );
#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	glDisable(GL_TEXTURE_2D);
#endif // _HOE_OPENGL_

	this->texture = false;

}

void HoeStates::StartWireframe()
{
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
#endif
#ifdef _HOE_OPENGL_
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
#endif // _HOE_OPENGL_
}

void HoeStates::EndWireframe()
{
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState(D3DRS_FILLMODE, wireframe ? D3DFILL_WIREFRAME:D3DFILL_SOLID);
#endif
#ifdef _HOE_D3D_

#endif
#ifdef _HOE_OPENGL_
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
#endif // _HOE_OPENGL_
}


