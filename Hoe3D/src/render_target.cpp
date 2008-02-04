
#include "StdAfx.h"
#include "utils.h"
#include "shared.h"
#include "ref.h"
#include "render_target.h"
#include "hoe_texture.h"
#include "config.h"

HoeRenderTarget::HoeRenderTarget(Type type)
{
	m_type = type;
	m_tex = NULL;
	if (m_type == eToTexture)
	{
		dword w=256,h=256;
		HOEFORMAT f=HOE_R8G8B8;
		GetConfig()->CheckTexture(w,h,f);
		m_tex = new HoeRenderTexture();
		m_tex->Create(w,h,f);
#ifdef _HOE_D3D_
		m_rt = m_tex->GetSurface();
#endif

	}
}

void HoeRenderTarget::InitMain()
{
#ifdef _HOE_D3D8_
	 HRESULT hRef = D3DDevice()->GetRenderTarget(&m_rt);
	 checkres(hRef,"GetRenderTarget");
#endif
#ifdef _HOE_D3D9_
	 HRESULT hRef = D3DDevice()->GetRenderTarget(0,&m_rt);
	 checkres(hRef,"GetRenderTarget");
#endif
}

void HoeRenderTarget::Setup()
{
	// nastaveni view pointu atd
	// clear atd.
#ifdef _HOE_D3D8_
	//HRESULT hRes;
	//hRes = D3DDevice()->SetRenderTarget(m_rt,NULL);
	//checkres(hRes);
#endif
#ifdef _HOE_D3D9_
	HRESULT hRes;
	hRes = D3DDevice()->SetRenderTarget(0,m_rt);
	checkres(hRes,"SetRenderTarget");
#endif
#ifdef _HOE_OPENGL_
	switch (m_type)
	{
	case eMain:
		glViewport(0, 0, GetRef()->GetWidth(), GetRef()->GetHeight());// 
		break;
	case eToTexture:
		glViewport(0, 0, 128, 128); 
		break;
	};

#endif
	GetRef()->ClearBuffers(true,true);
}

void HoeRenderTarget::EndRender()
{
#ifdef _HOE_OPENGL_
	// kopirovani do textury, nebo dalsi veci
	// Zkopíruje viewport do textury (od 0, 0 do 128, 128, bez okraje)
	if (m_type == eToTexture)
	{
		glBindTexture(GL_TEXTURE_2D, m_tex->GetTexture());// Zvolí texturu
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 128, 128, 0);
		glEnable(GL_TEXTURE_2D);// 
	}
#endif
}


