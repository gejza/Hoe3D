

#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "resmgr.h"
#include "hoe_format.h"
#include "../include/hfmt/htex_file.h"
#include "texture_loader.h"
#include "texture_converter.h"
#include "hoe_texture.h"
#include "texture_system.h"


TextureSystem::TextureSystem()
{
	SET_SHARED_PTR(ts);
	m_log = NULL;
	Con_Print("Texture system created.");
}

TextureSystem::~TextureSystem()
{
	UNSET_SHARED_PTR(ts);
}

HoeTexture * TextureSystem::GetTexture(const char * name, HoeLog * log)
{
	m_log = log;
	LOG(m_log)->Log("Load texture %s",name);

	TextureLoader loader(m_log);
	HoeTexture * tex = new HoeTexture();
	tex->name = strdup(name);
	
	if (loader.Load(name))
	{
		//
		TextureConverter * conv = NULL;
		if (loader.GetHeader().format == HOE_JPEG)
			// jpeg conv
			conv = new TextureConverterJPG(&loader,m_log);
		else if (loader.GetHeader().format == HOE_PNG)
			conv = new TextureConverterPNG(&loader,m_log);
		else
			conv = new TextureConverterHX(&loader,m_log);

		if (!conv)
		{
			Con_Print("texture %s could not find codec.",name);
			return tex;
		}
		LOG(m_log)->Log("Run converter");
		if (conv->Run())
		{
			LOG(m_log)->Log("Create texture %dx%dx%s",conv->GetWidth(),conv->GetHeight(),HoeFormatString(conv->GetFormat()));
			if (!tex->Create(conv->GetWidth(),conv->GetHeight(),conv->GetFormat()))
			{
				Con_Print("Texture %s(%dx%dx%s) failed to create!",name,conv->GetWidth(),conv->GetHeight(),HoeFormatString(conv->GetFormat()));
				return NULL;
			}
#ifdef _HOE_OPENGL_
			tex->BindData(conv->Get(conv->GetWidth() * HoeFormatSize(conv->GetFormat()) / 8));
#endif // _HOE_OPENGL_
#ifdef _HOE_D3D_
			HoeTexture::LOCKRECT lr;
			if (tex->Lock(&lr))
			{
                conv->Get(lr.data,lr.pitch);
				tex->Unlock();
			}
#endif // _HOE_D3D9_
			Con_Print("create texture: %s",tex->name);
		}
		conv->Destroy();
	}
	else
	  Con_Print("texture %s not load.",name);
	m_log = NULL;
	return tex;
}

HoeTexture * TextureSystem::CreateTexture(dword width, dword height, dword * data)
{
	HoeTexture * tex = new HoeTexture();
	if (!tex->Create(width,height,HOE_B8G8R8X8))
		return NULL;
	HoeTexture::LOCKRECT lr;
	if (tex->Lock(&lr))
	{
		lr.data;
		lr.pitch;
		for (int y=0;y < height;y++)
		{
			dword * p = (dword*)(lr.data+(lr.pitch*y));
			for (int x=0;x < width;x++)
			{
				p[x] = *data++;
			}
		}
		tex->Unlock();
	}
	return tex;
}

void TextureSystem::SetTexture(int stage,const HoeTexture * t)
{
#ifdef _HOE_D3D_
	
	HRESULT hRes;
	if (t)
		hRes = D3DDevice()->SetTexture(stage,t->m_texture);
	else
		hRes = D3DDevice()->SetTexture(stage,NULL);

	if (FAILED(hRes))
		exit(0);
#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
	if (t)
		glBindTexture(GL_TEXTURE_2D,t->m_texture);
#endif // _HOE_OPENGL_
}
